#pragma once
#include <string>
#include "igraph.h"


class Model
{
public:
    Model();
    IGraph* GetGraph();
    std::vector<Edge> GetBFSTree();
    void RunBFS();


    void SetStartNodeId(int nodeId);
    void SetSelectedEdge(Edge edge);
    void SetEndNodeId(int nodeId);
    void SetSelectedNodeId(int nodeId);
    int GetStartNodeId();
    int GetEndNodeId();
    int GetSelectedNodeId();
    Edge GetSelectedEdge();
    void Reset();

private:
    IGraph* graph;
    std::vector<Edge> BFSTree;
    int startNodeId = 0;
    int endNodeId = 0;
    int selectedNodeId = 0;
    Edge selectedEdge = { 0,0 };

};

