//
//  point.h
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#ifndef point_h
#define point_h
struct DoublePos{
    double lat, lng;
    static double getTwoPosDis(DoublePos a, DoublePos b);
};


struct IntPos{
    int x, y;
};


#endif /* point_h */
