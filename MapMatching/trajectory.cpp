//
//  trajectory.cpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <fstream>
#include "trajectory.h"
#include "TR_SIM.hpp"
#include "trajectory_generated.hpp"
#include "Route.hpp"
#include "distance.hpp"

using namespace std;

int Deg(double rad){
    return (int)(rad / 3.1415926 * 180);
}

bool cmpEdgeDis(EdgeDis a, EdgeDis b){//用于对一个点附近的侯选边进行排序
    return a.dis < b.dis;
}

bool cmp1(EdgeDis a, EdgeDis b){//用于判断是否一条路的比较函数，90度为一个区间，从上顺时针由1到4
    return a.edge[0].beginLat < b.edge[0].beginLat;
}

bool cmp2(EdgeDis a, EdgeDis b){
    return a.edge[0].beginLng < b.edge[0].beginLng;
}

bool cmp3(EdgeDis a, EdgeDis b){
    return a.edge[0].beginLat > b.edge[0].beginLat;
}

bool cmp4(EdgeDis a, EdgeDis b){
    return a.edge[0].beginLng > b.edge[0].beginLng;
}
int Trajectory::convertPosToDirect(double lat1, double lng1, double lat2, double lng2){//得到一个有向的边与北方的夹角，0-360
    if(lat1 == lat2 && lng1 == lng2)
        return -1;//重合，非法数据
    double vecy = lng2 - lng1;
    double vecx = lat2 - lat1;
    int Rad = Deg( acos(vecx / sqrt(vecx * vecx + vecy * vecy) ) );
    if(vecy < 0)
        Rad = 360 - Rad;
    return Rad;
}


Edge Trajectory::convertDirect(Edge e, int direct){//如果边的方向和前进的方向不吻合，交换
    int Rad = Trajectory::convertPosToDirect(e.beginLat, e.beginLng, e.endLat, e.endLng);
    if(Rad == -1) return e;
    int lower = direct - 90 > -1 ? direct - 90 : direct + 270;
    int uper = direct + 90 < 360 ? direct + 90 : direct - 270;
    if(uper > lower){
        if(!(Rad > lower && Rad < uper)){
            swap(e.beginLat, e.endLat);
            swap(e.beginLng, e.endLng);
            swap(e.begin, e.end);
        }
    }else{
        if(Rad > uper && Rad < lower){
            swap(e.beginLat, e.endLat);
            swap(e.beginLng, e.endLng);
            swap(e.begin, e.end);
        }
    }
    return e;
}

Trajectory::Trajectory(){//构造函数
    tempRoute = NULL;
    tra.clear();
    T = 0;
    lenT = 0;
    mapSet = NULL;
}

Trajectory::Trajectory(Map* mapSet){
    tra.clear();
    this->mapSet = mapSet;
    T = 0;
    lenT = 0;
    tempRoute = NULL;
}

Trajectory::~Trajectory(){
    delete []tempRoute;
    //目前是在ver中delete
    //    std::list<Route*>::iterator s;
    //    for(s = candidateR.begin(); s != candidateR.end(); s++){
    //        delete *s;
    //        candidateR.erase(s);
    //    }
    candidateEdge.clear();
}

bool Trajectory::ifGridAndPointDisSmallerThanDmax(int rankP, int rankx, int ranky){//判断该网格与该点的最短距离是否小于Dmax
    double lat = LAT_BEGIN + rankx * LAT_PER_GRID;
    double lng = LNG_BEGIN + ranky * LNG_PER_GRID;
    int Case = 0;
    if(tra[rankP].pos.lat < lat)
        Case = 0;
    else if(tra[rankP].pos.lat > lat + LAT_PER_GRID)
        Case = 2;
    else if(tra[rankP].pos.lng < lng)
        Case = 3;
    else if(tra[rankP].pos.lng > lng + LNG_PER_GRID)
        Case = 1;
    else
        Case = 4;
    switch (Case) {
        case 0:
            return pointToSegDist(tra[rankP].pos.lat, tra[rankP].pos.lng, lat, lng, lat, lng + LNG_PER_GRID) < DMAX;
            break;
            
        case 1:
            return pointToSegDist(tra[rankP].pos.lat, tra[rankP].pos.lng, lat, lng + LNG_PER_GRID, lat + LAT_PER_GRID, lng + LNG_PER_GRID) < DMAX;
            break;
            
        case 2:
            return pointToSegDist(tra[rankP].pos.lat, tra[rankP].pos.lng, lat + LAT_PER_GRID, lng + LNG_PER_GRID, lat + LAT_PER_GRID, lng) < DMAX;
            break;
            
        case 3:
            return pointToSegDist(tra[rankP].pos.lat, tra[rankP].pos.lng, lat, lng, lat + LAT_PER_GRID, lng) < DMAX;
            break;
            
        default:
            return true;
            break;
    }
}

bool Trajectory::ifAllEdgeAroundPointIsTheSameRoad(int rank){//判断所有的边是否为一条路
    bool ans = true;
    int size = (int)candidateEdge[rank].size();
    std::vector<EdgeDis> temp = candidateEdge[rank];
    int Case = -1;
    if(tra[rank].direct > 315 || tra[rank].direct <= 45)
        Case = 1;
    else if(tra[rank].direct > 45 && tra[rank].direct <= 135)
        Case = 2;
    else if(tra[rank].direct > 135 && tra[rank].direct <= 225)
        Case = 3;
    else
        Case = 4;
    switch (Case) {//根据方向对点进行排序
        case 1:
            sort(temp.begin(), temp.begin() + temp.size(), cmp1);
            break;
        case 2:
            sort(temp.begin(), temp.begin() + temp.size(), cmp2);
            break;
        case 3:
            sort(temp.begin(), temp.begin() + temp.size(), cmp3);
            break;
        default:
            sort(temp.begin(), temp.begin() + temp.size(), cmp4);
            break;
    }
    
    for(int i = 0; i < size - 1; i++){
        if(temp[i].edge[0].end != temp[i + 1].edge[0].begin){
            ans = false;
            break;
        }
    }
    return ans;
}

void Trajectory::generateOnePointCandidateEdge(int rank){//生成一个点附近的候选集
    vector<IntPos> candidateGrid;
    candidateGrid.clear();
    IntPos thisGrid = mapSet->convertToIntPos(tra[rank].pos.lat, tra[rank].pos.lng);
    candidateGrid.push_back(thisGrid);
    int cnt = 1;
    bool canFound = true;
    while (canFound) {//逐次向外扩张
        canFound = false;
        for(int i = -cnt; i <= cnt; i++){//最左边一列
            if(ifGridAndPointDisSmallerThanDmax(rank, thisGrid.x - cnt, thisGrid.y + i)){
                canFound = true;
                IntPos temp; temp.x = thisGrid.x - cnt, temp.y = thisGrid.y + i;
                candidateGrid.push_back(temp);
            }
        }
        for(int i = -cnt; i <= cnt; i++){//最右边一列
            if(ifGridAndPointDisSmallerThanDmax(rank, thisGrid.x + cnt, thisGrid.y + i)){
                canFound = true;
                IntPos temp; temp.x = thisGrid.x + cnt, temp.y = thisGrid.y + i;
                candidateGrid.push_back(temp);
            }
        }
        for(int i = -cnt + 1; i < cnt; i++){//上
            if(ifGridAndPointDisSmallerThanDmax(rank, thisGrid.x + i, thisGrid.y + cnt)){
                canFound = true;
                IntPos temp; temp.x = thisGrid.x + i, temp.y = thisGrid.y + cnt;
                candidateGrid.push_back(temp);
            }
        }
        for(int i = -cnt + 1; i < cnt; i++){//下
            if(ifGridAndPointDisSmallerThanDmax(rank, thisGrid.x + i, thisGrid.y - cnt)){
                canFound = true;
                IntPos temp; temp.x = thisGrid.x + i, temp.y = thisGrid.y - cnt;
                candidateGrid.push_back(temp);
            }
        }
        cnt++;
    }
    //至此grid候选集生成完毕
    int size = (int)candidateGrid.size();//下面为求并集的部分
    unordered_map<int, int> mapEdge;
    for(int i = 0; i < size; i++){
        int sizeE = (int)mapSet->grid[candidateGrid[i].x][candidateGrid[i].y].edge.size();
        for(int j = 0; j < sizeE; j++){
            if(mapEdge.count(mapSet->grid[candidateGrid[i].x][candidateGrid[i].y].edge[j].rank) == 0){
                mapEdge[mapSet->grid[candidateGrid[i].x][candidateGrid[i].y].edge[j].rank] = 1;
                EdgeDis tempE;
                tempE.edge.push_back(convertDirect(mapSet->grid[candidateGrid[i].x][candidateGrid[i].y].edge[j], tra[rank].direct));
                tempE.dis = pointToSegDist(tra[rank].pos.lat, tra[rank].pos.lng, tempE.edge[0].beginLat, tempE.edge[0].beginLng, tempE.edge[0].endLat, tempE.edge[0].endLng);
                if(tempE.dis > DMAX)
                    tempE.dis = 1;
                else
                    tempE.dis = tempE.dis / DMAX;
                candidateEdge[rank].push_back(tempE);
            }
        }
    }
    int sizeC = (int)candidateEdge[rank].size();
    sort(candidateEdge[rank].begin(), candidateEdge[rank].begin() + sizeC, cmpEdgeDis);
    
    
    int finalSize = sizeC > S ? S : sizeC;
    oriCandidateEdgeNumber[rank] = finalSize;
    CandidateEdgeNumberSC[rank] = finalSize;
    
    if(ifAllEdgeAroundPointIsTheSameRoad(rank)){//如果满足可以按全部为一条路剪枝的话，便剪枝
        int candidateSize = (int)candidateEdge[rank].size();
        for(int i = 1; i < candidateSize; i++){
            candidateEdge[rank][0].edge.push_back(candidateEdge[rank][i].edge[0]);
        }
        for(int i = candidateSize - 1; i >= 1; i--){
            candidateEdge[rank].erase(candidateEdge[rank].begin() + i);
        }
        CandidateEdgeNumberSC[rank] = 1;
        return;
    }
    
    for(int i = sizeC - 1; i >= finalSize; i--){
        candidateEdge[rank].erase(candidateEdge[rank].begin() + i);
    }
}

int Trajectory::getCandidateEdgeNumberSC() {
    double routeSize = 1;
    for (int i = 0; i < CandidateEdgeNumberSC.size(); ++i) {
        routeSize *= CandidateEdgeNumberSC[i];
    }
    return routeSize;
}

int Trajectory::getOriCandidateNumber() {
    double routeSize = 1;
    for (int i = 0; i < oriCandidateEdgeNumber.size(); ++i) {
        routeSize *= oriCandidateEdgeNumber[i];
    }
    return routeSize;
}

void Trajectory::generateCandidateEdge(){//生成全部点的候选边
    int size = (int)tra.size();
    for(int i = 0; i < size; i++)
        generateOnePointCandidateEdge(i);
    //getCandidateR(0);    //测试时先注释掉
}

void Trajectory::getCandidateR(int rank){//生成一个候选集
    if(rank == T){
        Route* tmp = new Route;
        tmp->init(T);
        tmp->input(tempRoute, T);
        candidateR.push_back(tmp);
    }
    int size = (int)candidateEdge[rank].size();
    for(int i = 0; i < size; i++){
        tempRoute[rank] = i;
        getCandidateR(rank+1);
    }
}


void Trajectory::setMap(Map* map){
    this->mapSet = map;
}

void Trajectory::addPos(DoublePos pos, int direct){
    TraPosNode temp;
    temp.pos = pos;
    temp.direct = direct;
    tra.push_back(temp);
    vector<EdgeDis> tmp;
    candidateEdge.push_back(tmp);
}

void Trajectory::fileAddPos(string file){//从文件中添加
    FILE* fs = fopen(file.c_str(), "rb");
    printf("open file %s\n", file.c_str());
    double lat, lng;
    double direct;
    char line[100];
    fgets(line, 100, fs);
    while (sscanf(line, "%lf %lf %lf", &lat, &lng, &direct) > 0) {
        DoublePos temp;
        temp.lat = lat, temp.lng = lng;
        addPos(temp, direct);
        fgets(line, 100, fs);
    }
    fclose(fs);
    cout << "stop addPos" << endl;
    T = (int)tra.size();
    tempRoute = new int[T];
    for(int i = 0; i < T - 1; i++){
        lenT += distance2meter(tra[i].pos.lat, tra[i].pos.lng, tra[i + 1].pos.lat, tra[i + 1].pos.lng);
    }
    oriCandidateEdgeNumber = std::vector<int>(T, 0);
    CandidateEdgeNumberSC = std::vector<int>(T, 0);
    //    scanf("%lf %lf", &lat, &lng);
    //    fdopen(origin_stdin, "r");
}
