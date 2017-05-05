//
//  TR_SIM.hpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#ifndef TR_SIM_hpp
#define TR_SIM_hpp

#include <stdio.h>
#include "trajectory.h"
#include "trajectory_generated.hpp"

double pointToSegDist(double x, double y, double x1, double y1, double x2, double y2);

class TR_SIM{
    double sim;
    double simPart1;
    double simPart2;
    double lenR;
    int beginT, endT;
    GenerateTrajectory& route;
    Trajectory& T;
public:
    //TR_SIM();
    TR_SIM(GenerateTrajectory& R, Trajectory& T);
    TR_SIM(GenerateTrajectory& R, Trajectory& T, int TBegin, int TEnd);
    ~TR_SIM();
    //void addRouteAndT(GenerateTrajectory& R, Trajectary& T);
    void part1();
    void part2();
    void computeSim();
    void setPart1(double x){simPart2 = x;}
    inline double minPointInTToAnEdgeInRoute(int rankP, int rankE);//T中的一个点到route中的一个边的最小距离
    double minPointInTToRoute(int rankP);//T中的一个点到route的最小距离
    double getDiToj(int rankP);//返回di->j
    double getSim(){return sim;}
    double getSimPart1(){return simPart1;}
    double getSimPart2(){return simPart2;}
    double getLenR(){return lenR;}
};

#endif /* TR_SIM_hpp */
