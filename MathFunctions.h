#pragma once

#include "Node.h"
#include <cmath>
#include <map>
#include <functional>

using StrToFuncMap_t = map<string, function<shared_ptr<Operation_t>()>>;


class Sin_t : public Operation_t {
public:
    Sin_t() : Operation_t("sin", 5, 1) {}
    double evalute() const override {
        return sin(args[0]->evalute());
    }
};

class Cos_t : public Operation_t {
public:
    Cos_t() : Operation_t("cos", 5, 1) {}
    double evalute() const override {
        return sin(args[0]->evalute());
    }
};


const StrToFuncMap_t strToMathFunction {
    {"sin", make_shared<Sin_t>},
    {"cos", make_shared<Cos_t>},
};
