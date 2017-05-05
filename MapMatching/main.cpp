//
//  main.cpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#include <iostream>
#include <vector>

#include "trajectory.h"
#include "Route.hpp"
#include "RouteWithSima.hpp"
#include "map.hpp"
#include "trajectory.h"

using namespace std;

void testSingleCandidate(Map* map);

int main() {
    std::cout << "Hello, World!\n";
    Map* map = new Map();
    map->fileAddNode("BJ.cnode");
    map->fileAddEdge("BJ.edge");
    testSingleCandidate(map);
    return 0;
}

void testSingleCandidate(Map* map) {
    vector<string> trajectory_files;
    double useSingleCandidate = 0;
    double notSingleCandidate = 0;
    for (int i = 0; i < trajectory_files.size(); ++i) {
        Trajectory test(map);
        test.fileAddPos("testTraj.txt");
        test.generateCandidateEdge();
        useSingleCandidate += test.getCandidateEdgeNumberSC();
        notSingleCandidate += test.getOriCandidateNumber();
    }
    cout << "SC: " << useSingleCandidate << endl;
    cout << "Ori: " << notSingleCandidate << endl;
}
