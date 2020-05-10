#pragma once
/*
Class: Token

Description: This class will be used to hold user input as a
two part object. One for its kind, the other for its value.
It comes equipped with two initializers, one for characters
that have no value, the other for ones that do
*/

class Token {
public:
    char kind;
    double value;
    Token(char ch)
        :kind(ch), value(0) { }
    Token(char ch, double val)
        :kind(ch), value(val) { }
};