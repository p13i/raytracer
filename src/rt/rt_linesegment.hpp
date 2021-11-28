//
//  rt_linesegment.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/31/21.
//

#ifndef rt_linesegment_hpp
#define rt_linesegment_hpp

#include "rt_point.hpp"

namespace rt {

struct LineSegment {
    rt::Point a, b;
    
    LineSegment(): LineSegment(rt::Point(), rt::Point()) { }
    
    LineSegment(rt::Point a, rt::Point b): a(a), b(b) { }
    
    Point midpoint();
    
    bool operator == (LineSegment other);
    
    bool operator != (LineSegment other);
};

}

#endif /* rt_linesegment_hpp */
