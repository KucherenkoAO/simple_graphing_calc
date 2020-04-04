#include <algorithm>
#include <iostream>
#include "RPN.h"

vector<string> parseExpr(const string & exp)
{
    if (exp.empty())
        return {};

    string expr = exp;
    auto it = remove_if(expr.begin(), expr.end(), [](char ch) {return isspace(ch);} );
    expr.erase(it, expr.end());

    auto endIt = expr.end();
    vector<string> rez;
    for (auto it = expr.begin(); it < endIt; ) {
        if (isdigit(*it)) {
            auto tmp = it + 1;
            while (tmp != endIt && (isdigit(*tmp) || '.' == *tmp))
                ++tmp;
            rez.push_back(string(it, tmp));
            it = tmp;
        }
        else if (isOneSymbolOperator(*it)) {
            rez.push_back(string(1, *it));
            ++it;
        }
        else {
            auto tmp = it + 1;
            while (tmp != endIt && !isOneSymbolOperator(*tmp))
                ++tmp;
            rez.push_back(string(it, tmp));
            it = tmp;
        }
    }
    return rez;
}

bool isOneSymbolOperator(const char op) {
    auto n = string("(),+-*/^!").find(op);
    return n != string::npos;
}

queue<shared_ptr<Node>> parseTokens(const vector<string> & tokens, double * X_ptr) {
    if (tokens.empty())
        return {};

    stack<shared_ptr<Node>> s;
    queue<shared_ptr<Node>> rpn;

    for (auto tokenIt = tokens.begin(); tokenIt != tokens.end(); ++tokenIt) {
        auto token = *tokenIt;
        if (isdigit(token[0]))
           rpn.push(make_shared<Numerical>(atof(token.c_str())));
        else if (token == ",")
            continue;
        else if (token == "(")
            s.push(make_shared<Bracket_t>(token));
        else if (token == ")") {
            while (!s.empty() && typeid(*s.top()) != typeid(Bracket_t)) {
                rpn.push(s.top());
                s.pop();
            }
            if (s.empty())
                throw invalid_argument("Bad expression: missed \'(\'");
            s.pop();
        }
        else if (X_ptr && token == "x")
            rpn.push(make_shared<Variable>(*X_ptr));
        else if (strToConstant.find(token) != strToConstant.end())
            rpn.push(make_shared<Numerical>(strToConstant.at(token)));
        else {
            shared_ptr<Operation_t> op;
            if  (   (token == "+" || token == "-") && ( tokenIt == tokens.begin() ||
                    (isOneSymbolOperator((tokenIt-1)->at(0)) && (tokenIt-1)->at(0) != '!') )
                )
            {
                op = makeUnaryOperation(token);
            }
            else
                op = makeOperation(token);

            while (!s.empty() && dynamic_pointer_cast<Operation_t>(s.top())->priority >= op->priority) {
                rpn.push(s.top());
                s.pop();
            }
            s.push(op);
        }
    }

    while (!s.empty()) {
        if (s.top()->strVal() == "(")
            throw invalid_argument("Bad expression: missed \')\'");
        rpn.push(s.top());
        s.pop();
    }
    return rpn;
}

double calcRPN(queue<shared_ptr<Node>> rpn) {
    if (rpn.empty())
        return 0;

    stack<shared_ptr<Node>> s;
    while (!rpn.empty()) {
        if (typeid(*rpn.front()) == typeid(Numerical) || typeid(*rpn.front()) == typeid(Variable)) {
            s.push(rpn.front());
        }
        else {
            shared_ptr<Operation_t> op = dynamic_pointer_cast<Operation_t>(rpn.front());
            op->get_arg(s);
            s.push(make_shared<Numerical>(op->evalute()));
        }
        rpn.pop();
    }
    if (s.size() != 1)
        throw runtime_error("Invalid expression: unused arguments left");
    return s.top()->evalute();
}


double calcExpr(const string & expr) {
    auto tokens = parseExpr(expr);
    auto rpn = parseTokens(tokens);
    return calcRPN(rpn);
}
