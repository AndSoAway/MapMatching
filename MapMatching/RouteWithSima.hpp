//
//  RouteWithSima.hpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#ifndef RouteWithSima_hpp
#define RouteWithSima_hpp

#include <stdio.h>
#include "Route.hpp"
#include "trajectory.h"

struct RouteWithSima{
    Route* m;
    double simA;
    RouteWithSima(Trajectory* traj, Route* a);
    ~RouteWithSima(){}
};
#endif /* RouteWithSima_hpp */
