//
//  rt_trace.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 8/1/21.
//

#ifndef rt_trace_hpp
#define rt_trace_hpp

#include "rt_vector.hpp"

namespace rt {

template <class T>
struct Trace {
    T data;
    float luminance;
    
    bool isTerminal;
    struct Trace<T> *next;
    
    Trace(T data, float luminance, bool isTerminal = false, Trace *next = nullptr):
        data(data), luminance(luminance), isTerminal(isTerminal), next(next) { }
};

}

#endif /* rt_trace_hpp */
