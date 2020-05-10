#include "Facilities.h"

class Variable
{
    //------------------------------------------------------------------------------
    // Creates a variable consisting of a string (which must start with an alpha character)
    // and a numeric value (which may be the result of an expression)

public:
    string name;
    double value;
    Variable(string n, double val)
        : name(n), value(val) {}
};