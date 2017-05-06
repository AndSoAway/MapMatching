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
    cout << "Hello, World!\n";
    Map* map = new Map();
    map->fileAddNode("BJ.cnode");
    map->fileAddEdge("BJ.edge");
    cout << "End Insert Map" << endl;
    testSingleCandidate(map);
    return 0;
}

void testSingleCandidate(Map* map) {
    int fileNumber = 100000;
    double useSingleCandidate = 0;
    double notSingleCandidate = 0;
    double maxPoint = -1;
    double minPoint = 10000;
    double totalPoint = 0;
    double averagePoint = 0;
    for (int i = 1; i <= fileNumber; ++i) {
        string filePath = "./trajfile/" + to_string(i) + ".csv";
        Trajectory test(map);
        test.fileAddPos(filePath.c_str());
        int pointSize = test.getTraSize();
        maxPoint = maxPoint < pointSize ? pointSize : maxPoint;
        minPoint = minPoint > pointSize ? pointSize : minPoint;
        totalPoint += pointSize;
     //   cout << "Begin Generate" << endl;
        test.generateCandidateEdge();
        useSingleCandidate += test.getCandidateEdgeNumberSC();
        notSingleCandidate += test.getOriCandidateNumber();
        cout << useSingleCandidate << ", " << notSingleCandidate << endl;
    }
    averagePoint = totalPoint / fileNumber;
    cout << "SC: " << useSingleCandidate << endl;
    cout << "Ori: " << notSingleCandidate << endl;
    cout << "MaxPoint: " << maxPoint << "MinPoint: " << minPoint << "averagePoint: " << averagePoint << endl;
}
