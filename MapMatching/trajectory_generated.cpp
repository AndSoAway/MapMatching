//
//  trajectory_generated.cpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#include "trajectory_generated.hpp"
#include <string>
#include <fstream>

using namespace std;

GenerateTrajectory::GenerateTrajectory(){
    point.clear();
}

GenerateTrajectory::~GenerateTrajectory(){
    
}

void GenerateTrajectory::fileAddPos(std::string file){
    std::ifstream fin;
    fin.open(file.c_str());
    //freopen(file.c_str(), "r", stdin);
    double lat, lng;
    while(fin >> lat >> lng){
        PosWithRank m;
        DoublePos temp;
        temp.lat = lat, temp.lng = lng;
        m.rank = 0;//仅仅用于直接测试，此处没意义
        m.pos = temp;
        point.push_back(m);
    }
    fin.close();
    //    scanf("%lf %lf", &lat, &lng);
    //    fdopen(origin_stdin, "r");
}
