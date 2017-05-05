//
//  constNum.hpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#ifndef constNum_hpp
#define constNum_hpp

#include <stdio.h>
#include <unistd.h>

const double LAT_PER_GRID = 0.00091;//约为100米
const double LNG_PER_GRID = 0.00091;
const double LAT_BEGIN = 39.5;
const double LAT_END = 40.3;
const double LNG_BEGIN = 116;
const double LNG_END = 116.8;
const double DMAX = 100;//三百米
const double INF = 1000000;
const int MAX_NODE = 340000;
const int MAX_EDGE = 450000;
const int S = 5;
const int ALPHA = 60;
const int origin_stdin = dup(fileno(stdin));
const int origin_stdout = dup(fileno(stdout));
#endif /* constNum_hpp */
