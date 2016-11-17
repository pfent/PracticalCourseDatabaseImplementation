#include "QueryParser.hpp"

#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

namespace keyword {
const string Select = "select";
const string From = "from";
const string Where = "where";
const string And = "and";
}

namespace literal {
const char Comma = ',';
const char Semicolon = ';';
const char Equals = '=';
}

unique_ptr<Query> QueryParser::parse() {
    string token;
    unsigned line = 1;
    auto q = make_unique<Query>();

    while (input >> token) {
        string::size_type pos;
        string::size_type prevPos = 0;

        while ((pos = token.find_first_of(",;", prevPos)) != string::npos) {
            nextToken(line, token.substr(prevPos, pos - prevPos), *q);
            nextToken(line, token.substr(pos, 1), *q);
            prevPos = pos + 1;
        }
        nextToken(line, token.substr(prevPos), *q);
        if (token.find("\n") != string::npos)
            ++line;
    }
    q->verify(schema);
    return move(q);
}

static bool isIdentifier(const string& str) {
    if (
        str == keyword::Select ||
        str == keyword::From ||
        str == keyword::Where ||
        str == keyword::And)
        return false;
    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyz_1234567890\"") == string::npos;
}

static bool isInt(const string& str) {
    return str.find_first_not_of("0123456789") == string::npos;
}

static bool isStringLiteral(const string& str) {
    return str.find('\'') == 0 && str.find('\'', 1) == (str.length() - 1);
}

static bool isConstant(const string& str) {
    return isInt(str) || isStringLiteral(str);
}

void QueryParser::nextToken(unsigned line, const std::string& token, Query& query) {
    if (getenv("DEBUG")) {
        cerr << line << ": " << token << endl;
    }
    if (token.empty()) {
        return;
    }
    std::string tok;
    std::transform(token.begin(), token.end(), std::back_inserter(tok), ::tolower);
    switch (state) {
    case State::Semicolon:
        throw QueryParserError(line, "Expected 'EOL' / \\n, found '" + token + "'");
    case State::Init:
        if (tok == keyword::Select)
            state = State::Select;
        else
            throw QueryParserError(line, "Expected 'SELECT', found '" + token + "'");
        break;
    case State::Select:
        if (isIdentifier(tok)) {
            query.projections.push_back(tok);
            state = State::SelectProjectionName;
        } else {
            throw QueryParserError(line, "Expected projection attribute, found '" + token + "'");
        }
        break;
    case State::SelectProjectionName:
        if (tok.size() == 1 && tok[0] == literal::Comma) {
            state = State::SelectList;
        } else if (tok == keyword::From) {
            state = State::From;
        } else {
            throw QueryParserError(line, "Expected ',' or 'FROM' found '" + token + "'");
        }
        break;
    case State::SelectList:
        if (isIdentifier(tok)) {
            query.projections.push_back(tok);
            state = State::SelectProjectionName;
        } else {
            throw QueryParserError(line, "Expected projection attribute, found '" + token + "'");
        }
        break;
    case State::From:
        if (isIdentifier(tok)) {
            query.relations.push_back(tok);
            state = State::FromRelationName;
        } else {
            throw QueryParserError(line, "Expected table name, found '" + token + "'");
        }
        break;
    case State::FromRelationName:
        if (tok.size() == 1 && tok[0] == literal::Semicolon) {
            state = State::Semicolon;
        } else if (tok.size() == 1 && tok[0] == literal::Comma) {
            state = State::FromList;
        } else if (tok == keyword::Where) {
            state = State::Where;
        } else {
            throw QueryParserError(line, "Expected ',' or 'WHERE' found '" + token + "'");
        }
        break;
    case State::FromList:
        if (isIdentifier(tok)) {
            query.relations.push_back(tok);
            state = State::FromRelationName;
        } else {
            throw QueryParserError(line, "Expected table name, found '" + token + "'");
        }
        break;
    case State::And: // fallthrough
    case State::Where:
        if (isIdentifier(tok)) {
            query.predicates.push_back({tok, ""});
            state = State::LeftPredicate;
        } else {
            throw QueryParserError(line, "Expected predicate attribute, found '" + token + "'");
        }
        break;
    case State::LeftPredicate:
        if (tok.size() == 1 && tok[0] == literal::Equals) {
            state = State::Equals;
        } else {
            throw QueryParserError(line, "Expected '=', found '" + token + "'");
        }
        break;
    case State::Equals:
        if (isIdentifier(tok) || isConstant(tok)) {
            get<1>(query.predicates.back()) = tok;
            state = State::RightPredicate;
        } else {
            throw QueryParserError(line, "Expected predicate attribute or constant, found '" + token + "'");
        }
        break;
    case State::RightPredicate:
        if (tok.size() == 1 && tok[0] == literal::Semicolon) {
            state = State::Semicolon;
        } else if (tok == keyword::And) {
            state = State::And;
        } else {
            throw QueryParserError(line, "Expected 'AND' or ';', found '" + token + "'");
        }
        break;
    default:
        throw;
    }
}

string Query::build() {
    stringstream res;
    res << "SELECT ";
    for (auto& projection : projections) {
        res << projection << " ";
    }
    res << "\nFROM ";
    for (auto& relation : relations) {
        res << relation << " ";
    }
    if (predicates.size() > 0) {
        res << "\nWHERE";
        for (auto& predicate : predicates) {
            res << get<0>(predicate) << " = " << get<1>(predicate) << "\n";
        }
    }
    return res.str();
}

void Query::verify(const Schema& schema) {
    (void) schema;
    // TODO verify table existance
    // TODO verify projection attribute existance
    // TODO verify predicate existance (take care with constants!)
}

