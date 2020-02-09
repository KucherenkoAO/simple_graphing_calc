#include <iostream>
#include <exception>
#include "Node.h"

Operation_t::Operation_t(const string & s, int p, size_t c)
    : name(s), priority(p), count_arg(c) { args.resize(count_arg); }

void Operation_t::get_arg(stack<shared_ptr<Node>> & s){
    if (s.size() < count_arg)
        throw runtime_error("too few arguments for operator " + name);
    size_t tmp = count_arg;
    do {
        args[tmp - 1] = s.top();
        s.pop();
    } while (--tmp);
}

void Operation_t::Print() const {
    std::cout << name << '(';
    for (size_t i = 0; i+1 < count_arg; ++i)
        std::cout << args[i]->strVal() << ',';
    std::cout << args[count_arg-1]->strVal() << ") = "
              << evalute() << endl;
}
