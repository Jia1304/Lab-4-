#include <iostream>
#include <iomanip>
using namespace std;

//function to print a real number with specified field width and precision
void print_real(float number, int fieldspace, int precision) {
    cout << fixed                      //use fixed-point notation
         << setw(fieldspace)           //set the field width
         << setprecision(precision)    //set the precision (number of decimal places)
         << number << endl;            //print the number and add a newline
}

//template function to update and scale values
template <typename T>
void update_scale(T& m1, T& m2, T m3 = 10) {
    m1 = (m1 + m2) * m3;               //update m1: (m1 + m2) * m3
    m2 = (m1 - m2 * 2) * m3;           //update m2: (new m1 - m2 * 2) * m3
}

int main() {
    //test print_real function
    print_real(123.456789, 10, 4);     //print 123.456789 with width 10 and 4 decimal places

    //test update_scale function
    float a = 5.5, b = 2.2;
    cout << "Before update_scale: a = " << a << ", b = " << b << endl;
    update_scale(a, b);                //call update_scale with default m3 value (10)
    cout << "After update_scale: a = " << a << ", b = " << b << endl;

    return 0;
}

