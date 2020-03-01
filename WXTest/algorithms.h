#pragma once
#include <vector>
#include "igraph.h"

class Algorithms
{
public:
    static std::vector<Edge> BFSTree(int startNodeId, IGraph* graph);
    static std::vector<Edge> Dijkstra(int startNodeId, IGraph* graph);
    static std::vector<Edge> AStar(int startNodeId, IGraph* graph);
private:
    static void PrintRoute(std::vector<std::pair<int, int>> linkList);
};

