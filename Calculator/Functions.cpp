#include "Functions.h"

/*
Function: calculate_factorial()

Description: This function takes a number, calculate its
factorial, and returns its number
*/

double Functions::calculate_factorial(int val)
{
    for (int i = val - 1; i > 0; i--)
        val *= i;
    return val;
}
