#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <stack>
#include <memory>

using namespace std;

class Node {
public:
    virtual double evalute() const = 0;
    virtual string strVal() const = 0;
    virtual ~Node() {}
};

class Numerical : public Node {
private:
    double num;
public:
    Numerical(double n) : num(n) {}
    double evalute() const override { return num; }
    string strVal() const override { return to_string(num); }
};

class Operation_t : public Node {
public:
    const string name;
    const int priority;
    Operation_t(const string & s, int p, size_t c);
    string strVal() const override { return name; }
    void get_arg(stack<shared_ptr<Node>> & s);
    void Print() const;
protected:
    const size_t count_arg;
    vector<shared_ptr<Node>> args;
};

#endif // NODE_H
