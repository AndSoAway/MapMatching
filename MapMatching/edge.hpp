//
//  edge.hpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#ifndef edge_hpp
#define edge_hpp

#include <stdio.h>

class Edge{
public:
    double beginLat, beginLng, endLat, endLng;
    int begin, end, rank, length;
public:
    Edge(){}
    ~Edge(){}
};
#endif /* edge_hpp */
