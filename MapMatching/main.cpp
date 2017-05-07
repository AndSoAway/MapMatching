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
    int fileNumber = 1000;
    long long useSingleCandidate = 0;
    long long notSingleCandidate = 0;
    double maxPoint = -1;
    double minPoint = 1000;
    double totalPoint = 0;
    double averagePoint = 0;
    int validNumber = 0;
    for (int i = 1; validNumber <= fileNumber; ++i) {
        string filePath = "./trajfile/" + to_string(i) + ".csv";
        Trajectory test(map);
        test.fileAddPos(filePath.c_str());
        int pointSize = test.getTraSize();
        if (pointSize >= 50)
            continue;
        validNumber++;
        maxPoint = maxPoint < pointSize ? pointSize : maxPoint;
        minPoint = minPoint > pointSize ? pointSize : minPoint;
        totalPoint += pointSize;
     //  cout << "Begin Generate" << endl;
        test.generateCandidateEdge();
        long long cur_useSC = test.getCandidateEdgeNumberSC();
        long long cur_notSC = test.getOriCandidateNumber();
        cout << pointSize << " " << cur_useSC << " " << cur_notSC << endl;
        useSingleCandidate += cur_useSC;
        notSingleCandidate += cur_notSC;
     //   cout << useSingleCandidate << ", " << notSingleCandidate << endl;
    }
    averagePoint = totalPoint / fileNumber;
    cout << "SC: " << useSingleCandidate << endl;
    cout << "Ori: " << notSingleCandidate << endl;
    cout << "MaxPoint: " << maxPoint << "MinPoint: " << minPoint << "averagePoint: " << averagePoint << endl;
}
