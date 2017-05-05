//
//  main.cpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#include <iostream>
#include "trajectory.h"
#include "Route.hpp"
#include "RouteWithSima.hpp"
#include "map.hpp"
#include "trajectory.h"

using namespace std;

extern std::vector< std::vector<EdgeDis> > candidateEdge;

int main() {
    std::cout << "Hello, World!\n";
    Route route;
    
    RouteWithSima routeWithSima(&route);
    Map* map = new Map();
    map->fileAddNode("BJ.cnode");
    map->fileAddEdge("BJ.edge");
    
    
    Trajectory test(map);
    test.fileAddPos("testTraj.txt");
    test.generateCandidateEdge();
    int canSize = (int)candidateEdge.size();
    cout << canSize << endl;
    return 0;
}