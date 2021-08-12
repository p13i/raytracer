//
//  rt_parser.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 8/5/21.
//

#ifndef rt_parser_hpp
#define rt_parser_hpp

#include "rt_raytracer.hpp"
#include "rt_geometry.hpp"

namespace rt { namespace parser {

rt::Geometry readGeometry(std::string filename);

} }

#endif /* rt_parser_hpp */
