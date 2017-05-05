//
//  RouteWithSima.cpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#include "RouteWithSima.hpp"
#include "trajectory.h"
#include <vector>
extern std::vector< std::vector<EdgeDis> > candidateEdge;

RouteWithSima::RouteWithSima(Route* a){
    m = a;
    int TSize = (int)candidateEdge.size();
    double dijSum = 0;
    for(int i = 0; i < TSize; i++){
        dijSum += candidateEdge[i][a->rank[i]].dis;
    }
    simA = 1 - dijSum / TSize;
}
