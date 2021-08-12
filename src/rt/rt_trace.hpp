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

struct Trace {
    Vector vec;
    float luminance;
    
    bool isTerminal;
    struct rt::Trace *next;
    
    Trace(Vector vec, float luminance, bool isTerminal = false, Trace *next = nullptr):
        vec(vec), luminance(luminance), isTerminal(isTerminal), next(next) { }
};

}

#endif /* rt_trace_hpp */
