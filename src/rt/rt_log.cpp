//
//  rt_log.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#include "rt_log.hpp"

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>

// https://stackoverflow.com/a/32983646/5071723
int printTimestamp() {
    const int bufferSize = 29;
  char buffer[bufferSize];
  int millisec;
  struct tm* tm_info;
  struct timeval tv;

  gettimeofday(&tv, NULL);

  millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
  if (millisec>=1000) { // Allow for rounding up to nearest second
    millisec -=1000;
    tv.tv_sec++;
  }

  tm_info = localtime(&tv.tv_sec);

  strftime(buffer, bufferSize, "[%Y:%m:%d %H:%M:%S", tm_info);
  printf("%s.%03d] ", buffer, millisec);

  return 0;
}

void rt::log(const char *str, bool timestamp, bool newline) {
    if (timestamp) {
        printTimestamp();
    }
    
    std::cout << str;
    
    if (newline) {
        std::cout << std::endl;
    }
}

void rt::log(std::string str, bool timestamp, bool newline) {
    rt::log(str.c_str(), timestamp, newline);
}
