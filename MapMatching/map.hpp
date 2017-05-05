//
//  map.hpp
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#ifndef map_hpp
#define map_hpp

#include <stdio.h>
#include <vector>
#include "node.hpp"
#include "edge.hpp"
#include "constNum.hpp"
#include "point.h"

struct Grid{
    std::vector<Node> node;
    std::vector<Edge> edge;
};

class Map{
public:
    Grid** grid;
    Node node[MAX_NODE];
    Edge edge[MAX_EDGE];
    int gridX, gridY, nodeSum, edgeSum;
public:
    Map();
    ~Map();
    DoublePos convertToDoublePos(int x, int y);
    IntPos convertToIntPos(double lat, double lng);
    DoublePos convertToGridPos(double lat, double lng);
    std::vector<Edge> getAllEdgeInOneGrid(double lat, double lng);
    void addNode(double lat, double lng, Node& node);
    void addEdge(double lat, double lng, Edge& edge);
    void fileAddNode(std::string);
    void fileAddEdge(std::string);
};

#endif /* map_hpp */
