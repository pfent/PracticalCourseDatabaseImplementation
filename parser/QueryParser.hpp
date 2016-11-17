#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <exception>
#include <string>
#include <memory>
#include <sstream>
#include <unordered_map>
#include "Schema.hpp"

class QueryParserError : std::exception {
    std::string msg;
public:
    QueryParserError(const std::string& m) : msg(m) {}
    ~QueryParserError() throw() {}
    const char* what() const throw() {
        return msg.c_str();
    }
};

struct Query {
    using Projection = std::string;
    using Relation = std::string;
    using Selection = std::tuple<std::string, std::string>;
    using Predicate = std::tuple<std::string, std::string>;

    const Schema& schema;
    Query(const Schema& schema) : schema(schema){}
    std::vector<Projection> projections;
    std::vector<Relation> relations;
    std::vector<Predicate> selections;
    std::vector<Predicate> joinPredicates;

    std::string build();
    void verify(); // might throw exception
};

struct QueryParser {
    const Schema& schema;
    std::stringstream input;
    enum class State : unsigned {
        Init,
        Select, SelectList, SelectProjectionName,
        From, FromList, FromRelationName,
        Where, LeftPredicate, Equals, RightPredicate,
        And, Semicolon
    };
    State state;
    QueryParser(const Schema& schema, const std::string& input) : schema(schema), input(input), state(State::Init) {}
    std::unique_ptr<Query> parse();

private:
    void nextToken(unsigned line, const std::string& token, Query& q);
};

#endif // QUERYPARSER_H

