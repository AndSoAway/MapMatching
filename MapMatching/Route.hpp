//
//  Route.hpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#ifndef Route_hpp
#define Route_hpp

#include <stdio.h>

struct Route{
    int* rank;
    void init(int T){
        rank = new int[T];
    }
    void input(int* m, int T){
        for(int i = 0; i < T; i++)
            rank[i] = m[i];
    }
    Route(){}
    ~Route(){
        delete []rank;
    }
};

#endif /* Route_hpp */
