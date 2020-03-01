#pragma once
#include <vector>
#include "node.h"

using Edge = std::pair<int, int>;

class IGraph
{
public:
    virtual void AddNode(int selectedNodeId, int x, int y) = 0;
    virtual void AddLink(int nodeId1, int nodeId2) = 0;
    virtual void DeleteNodeById(int nodeId) = 0;
    virtual void AddLinkWeight(Edge link, int weight) = 0;
    virtual std::vector<Node>::iterator GetNodeById(int nodeId) = 0;
    virtual std::vector<Node>& GetNodes() = 0;
    virtual void ClearNodes() = 0;
};