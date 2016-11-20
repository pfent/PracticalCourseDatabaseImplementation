#include "QueryParser.hpp"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <stack>
#include "query/TableScan.h"
#include "query/Selection.h"
#include "query/HashJoin.h"
#include "query/Printer.h"

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
    auto q = make_unique<Query>(schema);

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
    q->verify();
    return move(q);
}

static bool isIdentifier(const string &str) {
    if (
            str == keyword::Select ||
            str == keyword::From ||
            str == keyword::Where ||
            str == keyword::And)
        return false;
    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyz_1234567890\"") == string::npos;
}

static bool isInt(const string &str) {
    return str.find_first_not_of("0123456789") == string::npos;
}

static bool isStringLiteral(const string &str) {
    return str.find('\'') == 0 && str.find('\'', 1) == (str.length() - 1);
}

static bool isConstant(const string &str) {
    return isInt(str) || isStringLiteral(str);
}

void QueryParser::nextToken(unsigned line, const std::string &token, Query &query) {
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
            throw QueryParserError("Expected 'EOL' / \\n, found '" + token + "'");
        case State::Init:
            if (tok == keyword::Select)
                state = State::Select;
            else
                throw QueryParserError("Expected 'SELECT', found '" + token + "'");
            break;
        case State::Select:
            if (isIdentifier(tok)) {
                query.projections.push_back(tok);
                state = State::SelectProjectionName;
            } else {
                throw QueryParserError("Expected projection attribute, found '" + token + "'");
            }
            break;
        case State::SelectProjectionName:
            if (tok.size() == 1 && tok[0] == literal::Comma) {
                state = State::SelectList;
            } else if (tok == keyword::From) {
                state = State::From;
            } else {
                throw QueryParserError("Expected ',' or 'FROM' found '" + token + "'");
            }
            break;
        case State::SelectList:
            if (isIdentifier(tok)) {
                query.projections.push_back(tok);
                state = State::SelectProjectionName;
            } else {
                throw QueryParserError("Expected projection attribute, found '" + token + "'");
            }
            break;
        case State::From:
            if (isIdentifier(tok)) {
                query.relations.push_back(tok);
                state = State::FromRelationName;
            } else {
                throw QueryParserError("Expected table name, found '" + token + "'");
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
                throw QueryParserError("Expected ',' or 'WHERE' found '" + token + "'");
            }
            break;
        case State::FromList:
            if (isIdentifier(tok)) {
                query.relations.push_back(tok);
                state = State::FromRelationName;
            } else {
                throw QueryParserError("Expected table name, found '" + token + "'");
            }
            break;
        case State::And: // fallthrough
        case State::Where:
            if (isIdentifier(tok)) {
                query.joinPredicates.push_back({tok, ""});
                state = State::LeftPredicate;
            } else {
                throw QueryParserError("Expected predicate attribute, found '" + token + "'");
            }
            break;
        case State::LeftPredicate:
            if (tok.size() == 1 && tok[0] == literal::Equals) {
                state = State::Equals;
            } else {
                throw QueryParserError("Expected '=', found '" + token + "'");
            }
            break;
        case State::Equals:
            if (isConstant(token)) {
                auto pred = query.joinPredicates.back();
                query.joinPredicates.pop_back();
                query.selections.push_back(pred);
                get<1>(query.selections.back()) = token;
                state = State::RightPredicate;
            } else if (isIdentifier(tok)) {
                get<1>(query.joinPredicates.back()) = tok;
                state = State::RightPredicate;
            } else {
                throw QueryParserError("Expected predicate attribute or constant, found '" + token + "'");
            }
            break;
        case State::RightPredicate:
            if (tok.size() == 1 && tok[0] == literal::Semicolon) {
                state = State::Semicolon;
            } else if (tok == keyword::And) {
                state = State::And;
            } else {
                throw QueryParserError("Expected 'AND' or ';', found '" + token + "'");
            }
            break;
    }
}

static std::vector<std::tuple<IU *, std::string>>
getSelectionsForRelation(std::vector<Query::PredicateString> &selections, unordered_map<std::string, IU *> &IULookup,
                         Operator *op) {
    std::vector<std::tuple<IU *, std::string>> conds;
    for (auto &selection : selections) {
        auto selectionIU = IULookup[get<0>(selection)];
        auto operatorIUs = op->getProduced();
        if (find(operatorIUs.begin(), operatorIUs.end(), selectionIU) != operatorIUs.end()) {
            conds.push_back({selectionIU, get<1>(selection)});
        }
    }
    for (auto &cond : conds) {
        selections.erase(remove_if(selections.begin(), selections.end(), [&](auto s) {
            return IULookup[get<0>(s)] == get<0>(cond) && get<1>(s) == get<1>(cond);
        }), selections.end());
    }
    return conds;
}

static std::vector<std::tuple<IU *, IU *>>
getJoinPredicatesForRelations(std::vector<Query::PredicateString> &joinPredicates,
                              unordered_map<std::string, IU *> &IULookup, Operator *lhs, Operator *rhs) {
    std::vector<std::tuple<IU *, IU*>> conds;
    for (auto &predicate : joinPredicates) {
        auto lhsIU = IULookup[get<0>(predicate)];
        auto rhsIU = IULookup[get<1>(predicate)];
        auto lhsOperatorIUs = lhs->getProduced();
        auto rhsOperatorIUs = rhs->getProduced();
        if (find(lhsOperatorIUs.begin(), lhsOperatorIUs.end(), lhsIU) != lhsOperatorIUs.end()
                && find(rhsOperatorIUs.begin(), rhsOperatorIUs.end(), rhsIU) != rhsOperatorIUs.end()) {
            conds.push_back({lhsIU, rhsIU});
        }
    }
    for (auto &cond : conds) {
        joinPredicates.erase(remove_if(joinPredicates.begin(), joinPredicates.end(), [&](auto p) {
            return IULookup[get<0>(p)] == get<0>(cond) && IULookup[get<1>(p)] == get<1>(cond);
        }), joinPredicates.end());
    }
    return conds;
}

string Query::build() {
    vector<TableScan> tablescans;
    unordered_map<std::string, IU *> IULookup;

    for (auto &relation : relations) {
        auto &rel = schema.find(relation);
        tablescans.push_back(TableScan(rel));
        for (auto &attr : rel.attributes) {
            IULookup[attr.name] = tablescans.back().getIU(attr.name);
        }
    }

    vector<unique_ptr<Operator>> operators;
    stack<Operator *> operatorStack;
    for (size_t i = tablescans.size(); i > 0; --i) { // reverse order, as the stack reverses the order again
        operatorStack.push(&tablescans[i -1]);
    }
    while (operatorStack.size() >= 2) {
        auto lhs = operatorStack.top();
        operatorStack.pop();
        auto rhs = operatorStack.top();
        operatorStack.pop();
        auto lhsSelectionConditions = getSelectionsForRelation(selections, IULookup, lhs);
        auto rhsSelectionConditions = getSelectionsForRelation(selections, IULookup, rhs);

        operators.push_back(make_unique<Selection>(*lhs, lhsSelectionConditions));
        Operator *lhsSelected = operators.back().get();
        operators.push_back(make_unique<Selection>(*rhs, rhsSelectionConditions));
        Operator *rhsSelected = operators.back().get();

        auto predicates = getJoinPredicatesForRelations(joinPredicates, IULookup, lhsSelected, rhsSelected);

        operators.push_back(make_unique<HashJoin>(*lhsSelected, *rhsSelected, predicates));
        operatorStack.push(operators.back().get());
    }

    auto projs = vector<IU*>();
    for(auto& projection : projections)  {
        projs.push_back(IULookup[projection]);
    }
    auto result = Printer(*operatorStack.top(), projs);
    return result.produce();

    /* In pseudo code:
     * for all remaining tablescans
     *      for all selections, where the LHS is in AlgebraTreeRoot.getProduced
     *          algebraTreeRoot = selection(algebraTreeRoot, selection)
     *      for all joinPredicates, where LHS is in AlgebraTreeRoot.getProduced
     *                              AND RHS is in current tablescan
     *          algebraTreeRoot = HashJoin(algebraTreeRoot, tablescan, joinPredicates)
     * result = Printer(algebraTreeRoot, projections)
     * return result.produce()
     */
}

void Query::verify() {
    std::unordered_map<std::string, std::string> attributeLookup;
    // verify table existence
    try {
        for (auto &relation : relations) {
            auto r = schema.find(relation); // throws, if table not found
            for (auto &attribute : r.attributes) {
                attributeLookup[attribute.name] = relation;
            }
        }
    } catch (runtime_error &e) {
        throw QueryParserError(string("In 'FROM': ") + e.what());
    }
    // verify projection attribute existence
    for (auto &projection : projections) {
        if (attributeLookup.find(projection) == attributeLookup.end()) {
            throw QueryParserError("In 'SELECT': No such attribute '" + projection + "'");
        }
    }
    // verify predicate existence (take care with constants!)
    for (auto &selection : selections) {
        if (attributeLookup.find(get<0>(selection)) == attributeLookup.end()) {
            throw QueryParserError("In 'WHERE': No such attribute '" + get<0>(selection) + "'");
        }
    }
    for (auto &predicate : joinPredicates) {
        if (attributeLookup.find(get<0>(predicate)) == attributeLookup.end()) {
            throw QueryParserError("In 'WHERE': No such attribute '" + get<0>(predicate) + "'");
        }
        if (attributeLookup.find(get<1>(predicate)) == attributeLookup.end()) {
            throw QueryParserError("In 'WHERE': No such attribute '" + get<1>(predicate) + "'");
        }
    }
}

