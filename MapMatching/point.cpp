//
//  point.cpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//
#include "point.h"
#include <stdio.h>
#include <cmath>

double DoublePos::getTwoPosDis(DoublePos a, DoublePos b){
    return sqrt((a.lat - b.lat) * (a.lat - b.lat) + (a.lng - b.lng) * (a.lng - b.lng));
}
