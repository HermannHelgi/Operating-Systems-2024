#include "testlib.h"
#include "print.h"

int main()
{
    test_start("print.c");

    // Add some more test code here.
    print_line(42, "Hello World!");
    print_line(123123,"Heee\0");
    print_line(-123,"Ho");
    print_line(1844674407370955161,"Hooh");
    return test_end();
}
