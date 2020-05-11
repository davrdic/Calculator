#pragma once
class Token2
{
public:
    char kind;
    double value;
    Token2(char ch)
        :kind(ch), value(0) { }
    Token2(char ch, double val)
        :kind(ch), value(val) { }
};

