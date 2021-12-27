#ifndef rt_linesegment_hpp
#define rt_linesegment_hpp

#include "rt_point.hpp"

namespace rt {

struct LineSegment {
    Point a, b;
    
    LineSegment(): LineSegment(rt::Point(), rt::Point()) { }
    
    LineSegment(Point a, Point b): a(a), b(b) { }

    bool operator == (LineSegment other);
    
    bool operator != (LineSegment other);
};

}

#endif // rt_linesegment_hpp
