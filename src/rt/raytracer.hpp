//
//  raytracer.hpp
//  raytracer
//
//  Created by Pramod Kotipalli on 7/29/21.
//

#ifndef raytracer_hpp
#define raytracer_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <cmath>

#include <memory>
#include <string>
#include <stdexcept>

// *****
// Macros
// *****

#define RT_TEST(case, code) rt::log(rt::str::format("➡️ %s", case)); code;
#define RT_ASSERT(cond, msg) if (!(cond)) { rt::log("❌ FAIL ", true, false); rt::log(msg, false, true); exit(1); } else { rt::log("✅ PASS ", true, false); rt::log(msg, false, true); }

#define RT_RAY_MARCH_TIME_STEP 0.01
#define RT_RAY_MARCH_TIME_LIMIT 1000.0

#define RT_INTERSECTION_THRESHOLD 0.1
#define RT_FEQUAL(x, y) rt::math::fequal(x, y)

#define RT_MAX_DEPTH 6

#define OUT

#define PRINTLN(x) std::cout << x << std::endl;


namespace rt {

namespace math {

bool fequal(float a, float b, float threshold = 0.01);

float clamp(float value, float lower = 0., float upper = 1.);

bool inrange(float a, float lower = 0., float upper = 1.);

};

struct Point {
    float x, y;
    
    Point(): Point(0, 0) { }
    
    Point(float x, float y): x(x), y(y) { }
    
    bool operator == (Point other);
    
    Point operator + (Point other) {
        return Point(x + other.x, y + other.y);
    }
    
    Point operator - (Point other) {
        return Point(x - other.x, y - other.y);
    }
    
    Point operator * (float scalar) {
        return Point(x * scalar, y * scalar);
    }
    
    Point operator / (float scalar) {
        return Point(x / scalar, y / scalar);
    }
    
    // Source: https://stackoverflow.com/a/41648840
    // Required for use in std::set
    bool operator < (Point other) const {
      return (x < other.x) || ((!(other.x < x)) && (y < other.y));
    }
    
    float magnitude() {
      return sqrt(x * x + y * y);
    }
};

struct LineSegment {
    Point a, b;
    
    LineSegment(): LineSegment(Point(), Point()) { }
    
    LineSegment(Point a, Point b): a(a), b(b) { }
};

struct Vector {
    Point origin, dest;  // from, to
    
    Vector(Point dest): origin({0, 0}), dest(dest) { }
    
    Vector(Point orig, Point dest): origin(orig), dest(dest) { }
    
    Point _direction() {
        return dest - origin;
    }
    
    Vector unit() {
        return {_direction() / magnitude()};
    }
    
    rt::LineSegment lineSegment() {
        return {origin, dest};
    }
    
    float magnitude() {
        auto dir = _direction();
        return sqrt(dir.x * dir.x + dir.y * dir.y);
    }
    
    Vector operator * (float scalar) {
        auto newDest = origin + (dest - origin) * scalar;
        return Vector(origin, newDest);
    }
    
    bool operator == (Vector other) {
        return origin == other.origin && dest == other.dest;
    }
    
    Vector operator - (Vector other) {
        return Vector(origin - other.origin, dest - other.dest);
    }
};

struct Ray {
    Point origin;
    Vector direction;
    
    Ray(): Ray({}, {}) { }
    
    Ray(Point origin, Point towards): Ray(origin, Vector(towards - origin).unit()) { }
    
    Ray(Point origin, Vector direction): origin(origin), direction(direction) { }
    
    Point operator () (float time) {
        Vector atTime = direction.unit() * time;
        return origin + (direction * time).dest;
    }
    
    bool intersects(Point p) {
        // p is only on this ray if these normals are equal:
        return Vector(p - origin).unit() == direction.unit();
    }
    
    static Ray from(Vector v) {
        return {v.origin, v.dest};
    }
};

struct Geometry {
    std::vector<LineSegment> edges;
    
    Geometry(std::vector<LineSegment> edges): edges(edges) { }
};

struct Environment {
    Geometry geometry;
    Environment(rt::Geometry geo): geometry(geo) { }
};

class RayTracer {
    
public:
    Environment environment;
    
    RayTracer(): RayTracer(rt::Environment(rt::Geometry({}))) { }
    
    RayTracer(Environment environment): environment(environment) {}
    
    std::vector<rt::Vector> trace(rt::Ray start, unsigned int depth = 1);
    
};

int main();

void log(const char *str, bool timestamp = true, bool newline = true);

void log(std::string str, bool timestamp = true, bool newline = true);

namespace geo {

float dot(const rt::Point& a, const rt::Point& b);

// computes the intersection point, indicating if there was one
bool intersection(LineSegment j, LineSegment k, OUT Point& intersectionPoint);

rt::Ray reflect(rt::Vector forwardTrace, LineSegment intersectedEdge);

rt::Vector normal(rt::LineSegment ls);

float dist(Point a, Point b);
}

namespace str {

// Reference https://stackoverflow.com/a/26221725/5071723
template<typename ... Args>
std::string format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    auto buf = new char[size]();
    std::snprintf( buf, size, format.c_str(), args ... );
    return std::string( buf, buf + size - 1 ); // We don't want the '\0' inside
}

// Reference https://stackoverflow.com/a/26221725/5071723
template<typename ... Args>
std::string format( const char *format, Args ... args )
{
    return rt::str::format(std::string(format), args...);
}

}

namespace linalg {

// | a b |
// | c d |
struct Matrix2x2 {
    float a, b, c, d;
    
    Matrix2x2(float a, float b, float c, float d): a(a), b(b), c(c), d(d) { }
    
    float determinant();
};

float angleBetween(rt::Vector a, rt::Vector b);
}

}


#endif /* raytracer_hpp */
