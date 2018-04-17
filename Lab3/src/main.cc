/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <iostream>

#include "circle.h"
#include "polygon.h"

int main(int argc, char *argv[]) 
{
    int ununsed; // to fail warning test, remove

    Circle inner = Circle(Point2D(0.0,0.0), 4.0);
    Circle outer = Circle(Point2D(0.0,0.0), 8.0); 

    std::cout << "Circle-in-Circle: ";
    if (inner.containedWithin(outer)) 
        std::cout << "PASS\n";
    else 
        std::cout << "FAIL\n";
}
