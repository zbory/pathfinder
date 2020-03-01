#include "model.h"
#include "algorithms.h"
#include "graph.h"


Model::Model()
    :graph(new Graph())
{}


IGraph* Model::GetGraph()
{
    return graph;
}

std::vector<Edge> Model::GetBFSTree()
{
    return BFSTree;
}

void Model::RunBFS()
{
    BFSTree = Algorithms::BFSTree(startNodeId, graph);
}



void Model::SetStartNodeId(int nodeId)
{
    startNodeId = nodeId;
}

void Model::SetEndNodeId(int nodeId)
{
    endNodeId = nodeId;
}

void Model::SetSelectedNodeId(int nodeId)
{
    if (nodeId == selectedNodeId)
        selectedNodeId = 0;
    selectedNodeId = nodeId;
}

int Model::GetStartNodeId()
{
    return startNodeId;
}

int Model::GetEndNodeId()
{
    return endNodeId;
}

int Model::GetSelectedNodeId()
{
    return selectedNodeId;
}

void Model::Reset()
{
    selectedNodeId = 0;
    selectedEdge = { 0,0 };
    startNodeId = 0;
    endNodeId = 0;
    BFSTree.clear();
    graph->ClearNodes();
}

void Model::SetSelectedEdge(Edge edge)
{
    selectedEdge = edge;
}

Edge Model::GetSelectedEdge()
{
    return selectedEdge;
}