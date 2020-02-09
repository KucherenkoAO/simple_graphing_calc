#pragma once

#include "Node.h"
#include "MathFunctions.h"
#include <cmath>
#include <map>
#include <functional>

shared_ptr<Operation_t> makeOperation(const string & str);
shared_ptr<Operation_t> makeUnaryOperation(const string & str);

class Bracket_t : public Operation_t {
public:
    Bracket_t(const string & s) : Operation_t(s, 0, 0) {}
    string strVal() const override { return name; }
    double evalute() const override { return 0; }
};

class Sum_t : public Operation_t {
public:
    Sum_t() : Operation_t("+", 2, 2) {}
    double evalute() const override {
        return args[0]->evalute() + args[1]->evalute();
    }
};

class Dif_t : public Operation_t {
public:
    Dif_t() : Operation_t("-", 2, 2) {}
    double evalute() const override {
        return args[0]->evalute() - args[1]->evalute();
    }
};

class Mult_t : public Operation_t {
public:
    Mult_t() : Operation_t("*", 3, 2) {}
    double evalute() const override {
        return args[0]->evalute() * args[1]->evalute();
    }
};

class Div_t : public Operation_t {
public:
    Div_t() : Operation_t("/", 3, 2) {}
    double evalute() const override {
        return args[0]->evalute() / args[1]->evalute();
    }
};

class Pow_t : public Operation_t {
public:
    Pow_t() : Operation_t("^", 4, 2) {}
    double evalute() const override {
        return pow(args[0]->evalute(), args[1]->evalute());
    }
};

class Negative_t : public Operation_t {
public:
    Negative_t() : Operation_t("unary(-)", 5, 1) {}
    double evalute() const override {
        return -(args[0]->evalute());
    }
};

class Positive_t : public Operation_t {
public:
    Positive_t() : Operation_t("unary(+)", 5, 1) {}
    double evalute() const override {
        return args[0]->evalute();
    }
};

class Factorial_t : public Operation_t {
public:
    Factorial_t() : Operation_t("!", 6, 1) {}
    double evalute() const override {
        double tmp = args[0]->evalute();
        int n = static_cast<int>(tmp);
        if (abs(tmp - n) > 0.0001)
            throw invalid_argument("get float number for factorial: " + to_string(tmp));
        else if (n < 0)
            throw invalid_argument("get negative number for factorial: " + to_string(n));
        else {
            int f = 1;
            for (int i = 1; i <= n; ++i)
                f *= i;
            return f;
        }
    }
};


const StrToFuncMap_t strToUnaryOperation {
    {"-", make_shared<Negative_t>},
    {"+", make_shared<Positive_t>},
};

const StrToFuncMap_t strToSimpleOperation {
    {"+", make_shared<Sum_t>},
    {"-", make_shared<Dif_t>},
    {"*", make_shared<Mult_t>},
    {"/", make_shared<Div_t>},
    {"^", make_shared<Pow_t>},
    {"!", make_shared<Factorial_t>},
};

const map<string, double> strToConstant {
    {"Pi", M_PI},
    {"e", M_E},
};


