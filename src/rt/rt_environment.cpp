//
//  rt_environment.cpp
//  raytracer
//
//  Created by Pramod Kotipalli on 8/6/21.
//

#include "rt_environment.hpp"

#include <stdio.h>

using namespace std;

vector<Polygon> mergePolygons();

vector<Polygon> rt::Environment::convexPolygons() {
    vector<rt::Point> allPoints;
    for (LineSegment edge : geometry.edges) {
        allPoints.push_back(edge.a);
        allPoints.push_back(edge.b);
    }
    
    vector<Polygon> polygons;
    for (rt::Point p1 : allPoints) {
        for (rt::Point p2 : allPoints) {

            Point intersectionPoint;
            LineSegment intersectedEdge;
            if (!geometry.intersection({p1, p2}, intersectionPoint, intersectedEdge)) {
                continue;
            }

            polygons.push_back(Polygon{vector<LineSegment>{{p1, p2}}});
        }
    }
    
//    vector<Polygon>::const_iterator first = polygons.begin() + 0;
//    vector<Polygon>::const_iterator last = polygons.begin() + 1;
//    polygons = {first, last};

    cout << "Edge count = " << polygons[0].edges.size() << endl;
    
    return {polygons[0]};
}
