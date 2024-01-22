#include "parseint.h"
#include <stdbool.h>

/*
 * Returns the value of c or -1 on error
 */
int convertDecimalDigit(char c)
{
    int decimal = c -'0';

    if (decimal >= 0 && decimal < 10)
    {
        return decimal;
    }
    else
    {
        return -1;
    }
}

/*
 * Parses a non-negative integer, interpreted as octal when starting with 0,
 * decimal otherwise. Returns -1 on error.
 */
int parseInt(char *string)
{
    int singleChar;
    bool octal = false;
    bool notNull = false;
    int total = 0;
    int count = 0;

    while (string[count] != '\0')
    {
        singleChar = convertDecimalDigit(string[count]);
        if (count == 0 && singleChar == 0)
        {
            octal = true;
        }
        if (count == 0)
        {
            notNull = true;
        }
    	
        if (singleChar == -1)
        {
            return -1;
        }

        if (octal)
        {
            total *= 8;
        }
        else
        {
            total *= 10;
        }

        total += singleChar;
        count++;
    }

    if (notNull)
    {
        return total;
    }
    else
    {
        return -1;
    }
}
