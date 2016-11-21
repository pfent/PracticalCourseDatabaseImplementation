#include <iostream>
#include <string>
#include <chrono>
#include <memory>
#include "Database.h"
#include "parser/Parser.hpp"
#include "parser/Schema.hpp"
#include "parser/QueryParser.hpp"
#include <unistd.h>
#include <dlfcn.h>
#include <stdlib.h>

using namespace std;

auto &db = Database::instance();
string headers[] = {"Database.h", "hashfunctions.h", "Types.h"};

static void loadDatabase(const string &location) {
    cout << "Initializing database… " << flush;
    const auto start = chrono::steady_clock::now();
    db.importDatabaseFromPath(location);
    const auto end = chrono::steady_clock::now();
    cout << "took " << chrono::duration<double, milli>(end - start).count() << "ms\n" << endl;
}

static void chdirToTmp() {
    char buffer[512];
    string tmpdir = "";
    {
        shared_ptr<FILE> pipe(popen("mktemp -d", "r"), pclose);
        if (!pipe) {
            perror("popen()");
            throw runtime_error("popen() failed!");
        }
        while (!feof(pipe.get())) {
            if (fgets(buffer, 512, pipe.get())) {
                tmpdir += buffer;
            }
        }
    }
    tmpdir = tmpdir.substr(0, tmpdir.size() - 1); // trailing newline. ugh
    cout << "working directory: " << tmpdir << endl;

    for (const auto &header : headers) {
        const auto src = ifstream("../" + header, ios::binary);
        auto dst = ofstream(tmpdir + "/" + header, ios::binary);
        dst << src.rdbuf();
    }

    if (chdir(tmpdir.c_str()) < 0) {
        perror("chdir()");
        throw runtime_error("chdir('" + tmpdir + "') failed!");
    }
}

static void writeToFile(string filename, string text) {
    ofstream filestream;
    filestream.open(filename);
    filestream << text;
    filestream.close();
}

static void compileSo(string filename, string soName) {
    const string compileCommand = "g++ -O3 -std=c++14 -flto -march=native -pipe " + filename + " -fPIC -shared -o " + soName;
    {
        char buffer[512];
        string errormsg = "";
        FILE *pipe(popen(compileCommand.c_str(), "r"));
        if (!pipe) {
            perror("popen()");
            throw runtime_error("popen() failed!");
        }
        while (!feof(pipe)) {
            if (fgets(buffer, 512, pipe)) {
                errormsg += buffer;
            }
        }
        if (pclose(pipe)) {
            throw runtime_error("Compilation failed:\n" + errormsg);
        }
    }
}

void executeSo(string filename, string functionname) {
    void *handle = dlopen(filename.c_str(), RTLD_NOW);
    if (!handle) {
        cerr << "dlopen() failed: " << dlerror() << endl;
        throw runtime_error("dlopen() returned NULL");
    }
    auto fn = reinterpret_cast<void (*)()>(dlsym(handle, functionname.c_str()));
    if (!fn) {
        cerr << "dlsym() failed: " << dlerror() << endl;
        throw runtime_error("dlsym() returned NULL");
    }

    // Call the function. Directly prints to stdout
    fn();

    if (dlclose(handle)) {
        cerr << "dlclose() failed: " << dlerror() << endl;
        throw runtime_error("dlclose() failed");
    }
}

int main() {
    loadDatabase("../tbls/");
    unique_ptr<Schema> schema = Parser("../schema.sql").parse();

    chdirToTmp(); // after initial loading, only write files in a temporary directory

    cout << "input a SQL query or finish execution by EOF (^D / Ctrl-D)" << endl;
    cout << "> ";
    for (string line; getline(cin, line); cout << "> ") {
        const auto startCompile = chrono::steady_clock::now();
        string result;
        try {
            auto query = QueryParser(*schema, line).parse();
            result = query->build();
        } catch (QueryParserError &ex) {
            cerr << ex.what() << endl;
            continue;
        }

        writeToFile("query.cpp", result);
        cout << "compiling… " << flush;
        compileSo("query.cpp", "query.so");
        const auto endCompile = chrono::steady_clock::now();
        cout << "took " << chrono::duration<double, milli>(endCompile - startCompile).count() << "ms\n" << endl;

        // Probably also adapt the compilation for main.cpp
        const auto startExecute = chrono::steady_clock::now();
        executeSo("./query.so", "dynamicQuery");
        const auto endExecute = chrono::steady_clock::now();
        cout << "took " << chrono::duration<double, milli>(endExecute - startExecute).count() << "ms\n" << endl;
    }

    return 0;
}







