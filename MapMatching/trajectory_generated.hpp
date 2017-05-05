//
//  trajectory_generated.hpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#ifndef trajectory_generated_hpp
#define trajectory_generated_hpp

#include <stdio.h>
#include <vector>
#include "point.h"


struct PosWithRank{
    DoublePos pos;
    int rank;
};

class GenerateTrajectory{
    std::vector<PosWithRank> point;
public:
    GenerateTrajectory();
    ~GenerateTrajectory();
    void fileAddPos(std::string);
    friend class TR_SIM;
    friend class EarlyTermination;
    friend class CutTraj;
    friend class FilterTree;
};
#endif /* trajectory_generated_hpp */
