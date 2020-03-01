#pragma once
#include <vector>
#include "igraph.h"


class Graph : public IGraph
{
public:
    virtual void AddNode(int selectedNodeId, int x, int y) override;
    virtual void AddLink(int nodeId1, int nodeId2) override;
    virtual void DeleteNodeById(int nodeId) override;
    virtual void AddLinkWeight(Edge link, int weight) override;
    virtual std::vector<Node>::iterator GetNodeById(int nodeId) override;
    virtual std::vector<Node>& GetNodes() override;
    virtual void ClearNodes() override;
private:
    std::vector<Node> nodes;
    int nextId = 1;
};

