//
//  rt_parser.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 8/5/21.
//

#include "rt_parser.hpp"

#include "rt_linesegment.hpp"

#include <fstream>
#include <stdio.h>

rt::Geometry rt::parser::readGeometry(std::string filename) {
    std::ifstream filestream(filename.c_str());
    
    std::vector<rt::LineSegment> lineSegments;
    
    float x1, y1, x2, y2;
    while (filestream >> x1 >> y1 >> x2 >> y2) {
        lineSegments.push_back({{x1, y1}, {x2, y2}});
    }
    
    return {lineSegments};
}
