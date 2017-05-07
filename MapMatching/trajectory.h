//
//  trajectory.h
//  MapMatching
//
//  Created by 谢永青 on 17/5/4.
//  Copyright © 2017年 谢永青. All rights reserved.
//

#ifndef trajectory_h
#define trajectory_h
#include <vector>
#include <list>
#include "Route.hpp"
#include "point.h"
#include "edge.hpp"
#include "map.hpp"

struct TraPosNode{
    DoublePos pos;
    int direct;//由北顺时针旋转的角度
};

struct EdgeDis{
    std::vector<Edge> edge;//相同路名的边放在一个vector里面
    double dis;
};

class Trajectory{//这里是每个原始路径
    std::vector<std::vector<EdgeDis> > candidateEdge;//候选路径上的小边，如果里面的vector大小不为一，代表有多种选择
    std::list<Route*> candidateR;
    
    std::vector<int> oriCandidateEdgeNumber;//候选路径，对应每个点的侯选边数
    std::vector<int> CandidateEdgeNumberSC;//使用SingleCandidate得到的边数
    
    int T;
    int* tempRoute;
    std::vector<TraPosNode> tra;
    //std::vector<GenerateTrajectory> generateTraj;//最终生成的候选路径，通过相似度匹配选择
    Map* mapSet;
    double lenT;
public:
    Trajectory();
    Trajectory(Map*);
    ~Trajectory();
    
    std::vector<std::vector<EdgeDis>>& getCandidateEdge() { return candidateEdge; }
    std::list<Route*>& getCandidateR() {return candidateR; }
    
    void setMap(Map*);
    void addPos(DoublePos, int);
    void fileAddPos(std::string);//从文件中读入，格式为经纬度相隔开
    
    void generateOnePointCandidateEdge(int rank);//对于一个点生成候选的小边
    void generateCandidateEdge();//生成候选路径上的小边
    
    
    
    long long getCandidateEdgeNumberSC();
    long long getOriCandidateNumber();
    
    //void getGenerateTraj();//由候选路径的小边生成最终的候选路径
    void getCandidateR(int rank);//在基础策略之后生成候选集，即generateCandidateEdge之后执行
    bool ifGridAndPointDisSmallerThanDmax(int rankP, int rankx, int ranky);//判断该网格与该点的最短距离是否小于Dmax
    bool ifAllEdgeAroundPointIsTheSameRoad(int rank);//判断这个点附近的边是不是全是一个路径,通过首尾是否一致来判定
    int getTraSize(){return (int)tra.size();}
    Edge convertDirect(Edge, int);
    static int convertPosToDirect(double lat, double lng, double latEnd, double lngEnd);
    friend class TR_SIM;
    friend class FilterTree;
    friend class CutTraj;
};

#endif /* trajectory_h */
