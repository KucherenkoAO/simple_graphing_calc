#pragma once
// Revers Polish Notation functions
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include "Operations.h"

std::vector<std::string> parseExpr(const std::string & expr);
bool isOneSymbolOperator(char c);
std::queue<std::shared_ptr<Node>> parseTokens(const std::vector<std::string> & tokens, double * X_ptr = nullptr);
double calcRPN(std::queue<std::shared_ptr<Node>> rpn);
double calcExpr(const std::string & expr);
