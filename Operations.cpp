#include "Operations.h"

shared_ptr<Operation_t> makeOperation(const string & str){
    vector<StrToFuncMap_t> strToOperation = {strToSimpleOperation, strToMathFunction};
    for (const auto & sto : strToOperation) {
        if (sto.find(str) != sto.end()) {
            return sto.at(str)();
        }
    }
    throw invalid_argument("Unknown operation: " + str);
}

shared_ptr<Operation_t> makeUnaryOperation(const string & str){
    if (strToUnaryOperation.find(str) != strToUnaryOperation.end())
        return strToUnaryOperation.at(str)();
    else
        throw invalid_argument("Unknown unary operation: " + str);
}
