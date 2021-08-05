//
//  rt_log.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#ifndef rt_log_hpp
#define rt_log_hpp

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>

#define LOG(x) rt::log(x)

namespace rt {

void log(const char *str, bool timestamp = true, bool newline = true);

void log(std::string str, bool timestamp = true, bool newline = true);

}

#endif /* rt_log_hpp */
