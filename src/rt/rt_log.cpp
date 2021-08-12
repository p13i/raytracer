//
//  rt_log.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_log.hpp"

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>

void rt::log(const char *str, bool timestamp, bool newline) {
    std::cout << str;
    
    if (newline) {
        std::cout << std::endl;
    }
}

void rt::log(std::string str, bool timestamp, bool newline) {
    rt::log(str.c_str(), timestamp, newline);
}
