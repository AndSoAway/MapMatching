//
//  map.cpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#include "map.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

Map::Map(){
    gridX = (LAT_END - LAT_BEGIN) / LAT_PER_GRID * 1.05;
    gridY = (LNG_END - LNG_BEGIN) / LNG_PER_GRID * 1.05;
    nodeSum = edgeSum = 0;
    grid = new Grid*[gridX];
    for(int i = 0; i < gridX; i++)
        grid[i] = new Grid[gridY];
}

Map::~Map(){
    for(int i = 0; i < gridX; i++)
        delete [] grid[i];
    delete [] grid;
}

DoublePos Map::convertToDoublePos(int x, int y){//返回grid对应的x和y的左下角
    DoublePos answer;
    answer.lat = x * LAT_PER_GRID + LAT_BEGIN;
    answer.lng = y * LNG_PER_GRID + LNG_BEGIN;
    return answer;
}

IntPos Map::convertToIntPos(double lat, double lng){//对于一个坐标，返回是在那一个grid里
    IntPos answer;
    answer.x = (int)((lat - LAT_BEGIN) / LAT_PER_GRID);
    answer.y = (int)((lng - LNG_BEGIN) / LNG_PER_GRID);
    return answer;
}

void Map::addNode(double lat, double lng, Node& node){//将对应坐标的一个Node插入到格子里
    if(lat < LAT_BEGIN || lat > LAT_END || lng < LNG_BEGIN || lng > LNG_END) return;
    IntPos temp = convertToIntPos(lat, lng);
    grid[temp.x][temp.y].node.push_back(node);
}

void Map::addEdge(double lat, double lng, Edge& edge){//将某个坐标对应的格子插入到对应的格子里
    if(lat < LAT_BEGIN || lat > LAT_END || lng < LNG_BEGIN || lng > LNG_END) return;
    IntPos temp = convertToIntPos(lat, lng);
    grid[temp.x][temp.y].edge.push_back(edge);
}


void Map::fileAddEdge(string file){//从文件向map里添加边
    freopen(file.c_str(), "r", stdin);
    int begin, end, sum = 1;
    int length;
    scanf("%d %d", &begin, &end);
    while(scanf("%d %d %d", &begin, &end, &length) != EOF){
        double latBegin = node[begin].lat, latEnd = node[end].lat;
        double lngBegin = node[begin].lng, lngEnd = node[end].lng;
        if(latBegin > latEnd){//也需要交换begin和end
            swap(latBegin, latEnd);
            swap(lngBegin, lngEnd);
            swap(begin, end);
        }
        
        Edge edge;
        edge.begin = begin, edge.end = end, edge.beginLat = latBegin, edge.beginLng = lngBegin, edge.endLat = latEnd, edge.endLng = lngEnd,edge.length = length;
        edge.rank = sum;
        double latRise = (latEnd - latBegin) / 10.0;
        if(latEnd - latBegin < 1e-8 && latEnd - latBegin > -(1e-8)) {//如果是纬度不改变的也要考虑
            double lngRise = (lngEnd - lngBegin) / 10.0;
            IntPos now = convertToIntPos(latBegin, lngBegin);
            addEdge(latBegin, lngBegin, edge);
            for(int i = 1; i <= 10; i++){
                IntPos tmp = convertToIntPos(latBegin, lngBegin + lngRise * i);
                if(tmp.x == now.x && tmp.y == now.y)
                    continue;
                addEdge(latBegin, lngBegin + lngRise * i, edge);
                now = tmp;
            }
            this->edge[sum++] = edge;
            continue;
        }
        double lngRise = (lngEnd - lngBegin) / (latEnd - latBegin) * latRise;
        double nowLat = latBegin, nowLng = lngBegin;
        int nowLatInt = -1, nowLngInt = -1;
        
        for(int i = 0; i <= 10; i++){
            IntPos intPos = convertToIntPos(nowLat, nowLng);
            if(intPos.x == nowLatInt && intPos.y == nowLngInt){
                nowLat += latRise, nowLng += lngRise;
                continue;
            }
            addEdge(nowLat, nowLng, edge);
            nowLat += latRise, nowLng += lngRise;
            nowLatInt = intPos.x, nowLngInt = intPos.y;
        }
        this->edge[sum++] = edge;
    }
    edgeSum = sum - 1;
    fdopen(origin_stdin, "r");
}

void Map::fileAddNode(string file){//从文件添加点
    freopen(file.c_str(), "r", stdin);
    int rank;
    double lat, lng;
    while(scanf("%d %lf %lf", &rank, &lng, &lat) != EOF){
        ++nodeSum;
        Node node;
        node.rank = rank, node.lat = lat, node.lng = lng;
        this->node[rank] = node;
        addNode(lat, lng, node);
    }
    fdopen(origin_stdin, "r");
}
