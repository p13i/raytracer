#ifndef rt_str_hpp
#define rt_str_hpp

#include <stdio.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <memory>
#include <string>
#include <stdexcept>

using namespace std;

namespace rt {
    namespace str {

        // Reference https://stackoverflow.com/a/26221725/5071723
        template<typename ... Args>
        string format(const string &format, Args ... args) {
            int size_s = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
            if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
            auto size = static_cast<size_t>( size_s );
            auto buf = new char[size]();
            snprintf(buf, size, format.c_str(), args ...);
            return {buf, buf + size - 1}; // We don't want the '\0' inside
        }

        // Reference https://stackoverflow.com/a/26221725/5071723
        template<typename ... Args>
        string format(const char *format, Args ... args) {
            return rt::str::format(std::string(format), args...);
        }

    }
}

#endif /* rt_str_hpp */
