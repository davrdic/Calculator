#pragma once
#include "Token2.h"

class Token_stream2
{
public:
    Token_stream2();
    Token2 get();
    void putback(Token2 t);
    void ignore(char c);
private:
    bool full;
    Token2 buffer;
};

