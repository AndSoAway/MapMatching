//
//  TR_SIM.cpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//
#include <cmath>
#include "TR_SIM.hpp"
#include "distance.hpp"
#include "trajectory.h"

double pointToSegDist(double x, double y, double x1, double y1, double x2, double y2)
{
    double cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1);
    if (cross <= 0) return distance2meter(x, y, x1, y1);
    
    double d2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    if (cross >= d2) return distance2meter(x, y, x2, y2);
    
    double r = cross / d2;
    double px = x1 + (x2 - x1) * r;
    double py = y1 + (y2 - y1) * r;
    return distance2meter(x, y, px, py);
}

//TR_SIM::TR_SIM(){
//    sim = simPart1 = simPart2 = -1;
//}

TR_SIM::TR_SIM(GenerateTrajectory& R, Trajectory& T) : route(R), T(T){
    sim = simPart1 = simPart2 = -INF;
    lenR = 0;
    beginT = 0, endT = (int)T.tra.size();
    //addRouteAndT(R, T);
}

TR_SIM::TR_SIM(GenerateTrajectory& R, Trajectory& T, int TBegin, int TEnd) : route(R), T(T){
    sim = simPart1 = simPart2 = -INF;
    lenR = 0;
    beginT = TBegin, endT = TEnd;
}

TR_SIM::~TR_SIM(){
    
}

inline double TR_SIM::minPointInTToAnEdgeInRoute(int rankP, int rankE){
    return pointToSegDist(T.tra[rankP].pos.lat, T.tra[rankP].pos.lng, route.point[rankE].pos.lat, route.point[rankE].pos.lng, route.point[rankE+1].pos.lat, route.point[rankE+1].pos.lng);
}

double TR_SIM::minPointInTToRoute(int rankP){
    double minDis = INF, temp = INF;
    int edgeSize = (int)route.point.size() - 1;
    for(int i = 0; i < edgeSize; i++){
        temp = minPointInTToAnEdgeInRoute(rankP, i);
        if(temp < minDis) minDis = temp;
    }
    return minDis;
}

double TR_SIM::getDiToj(int rankP){
    double minDis = minPointInTToRoute(rankP);
    return minDis <= DMAX ? minDis / DMAX : 1;
}

//void TR_SIM::addRouteAndT(GenerateTrajectory& G, Trajectory& T){
//    route = G;
//    this->T = T;
//}

void TR_SIM::part1(){
    double sumDij = 0;
    for(int i = beginT; i < endT; i++){
        sumDij += getDiToj(i);
    }
    simPart1 =  1 - sumDij / (endT - beginT);
}

void TR_SIM::part2(){
    double lenR, lenT;
    lenR = 0;
    lenT = T.lenT;
    int sizeR = (int)route.point.size();
    //    for(int i = beginT; i < endT - 1; i++){
    //        //lenT += DoublePos::getTwoPosDis(T.tra[i].pos, T.tra[i+1].pos);
    //        lenT += distance2meter(T.tra[i].pos.lat, T.tra[i].pos.lng, T.tra[i+1].pos.lat, T.tra[i+1].pos.lng);
    //    }
    for(int i = 0; i < sizeR - 1; i++){
        //lenR += DoublePos::getTwoPosDis(route.point[i], route.point[i+1]);
        lenR += distance2meter(route.point[i].pos.lat, route.point[i].pos.lng, route.point[i+1].pos.lat, route.point[i+1].pos.lng);
    }
    simPart2 = 1 - fabs(lenR - lenT) / lenT;
    this->lenR = lenR;
}

void TR_SIM::computeSim(){
    if(simPart1 == -INF) part1();
    if(simPart2 == -INF) part2();
    sim = simPart1 * simPart2;
}
