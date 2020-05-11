#pragma once
#include "Token.h"

/*
Class: Token_stream

Description: This class will be used as a buffer to hold a token.
It comes equipped with two public functions, one to get a token
from user input or the buffer. the other to put a token in the
buffer. two private variables will be needed by for member functions
to carry out their task. One is a Token that is the actual buffer,
the other is a bool that keeps track of whether or not the buffer
is full
*/

class Token_stream
{
public:
    Token_stream();
    Token get();
    void putback(Token t);
    void ignore(char c);
private:
    bool full;
    Token buffer;
};

