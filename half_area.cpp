// Code for computation of the area of the half unit circle.
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double f(double x) {
    return sqrt(1 - x*x);
}

double area = 0.0;

int main() {
    int numPoints = 23;
    double stepSize = 2.0 / numPoints;

    double x = -1.0;
    for (int i = 0; i < numPoints; i++) {
        area = area + stepSize * f(x);
        x = x + stepSize;
    }

    std::cout << "The printed area is: " << area << "\n";

    return 0;
}