//
//  distance.hpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#ifndef distance_hpp
#define distance_hpp

#include <stdio.h>

#define PI 3.14159265
#define EARTH_RADIUS 6378.137

double rad(double d);

double distance2meter(double lat1, double lng1, double lat2, double lng2);

#endif /* distance_hpp */
