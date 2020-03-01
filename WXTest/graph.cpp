#include <algorithm>
#include "graph.h"

void Graph::AddNode(int selectedNodeId, int x, int y)
{
    nodes.push_back(Node(nextId, x, y));
    nextId++;
}

void Graph::AddLink(int nodeId1, int nodeId2) {
    int weight = 0;

    GetNodeById(nodeId1)->adjacentNodes.push_back(std::make_pair(nodeId2, weight));
    //UNDIRECTED
    GetNodeById(nodeId2)->adjacentNodes.push_back(std::make_pair(nodeId1, weight));
}

std::vector<Node>::iterator Graph::GetNodeById(int nodeId)
{
    return std::find_if(nodes.begin(), nodes.end(), [nodeId](auto node) {return node.nodeId == nodeId; });
    //Check if nodeId is not in nodes
}

std::vector<Node>& Graph::GetNodes()
{
    return nodes;
}

void Graph::ClearNodes()
{
    nextId = 1;
    nodes.clear();
}

void Graph::AddLinkWeight(Edge link, int weight)
{
    if (link.first != 0) {
        auto& linksOfNode1 = GetNodeById(link.first)->adjacentNodes;
        std::for_each(linksOfNode1.begin(), linksOfNode1.end(), [link, weight](std::pair<int, int>& edge) {
            if (edge.first == link.second)
            {
                edge.second = weight;
            }});
        //UNDIRECTED
        auto& linksOfNode2 = GetNodeById(link.second)->adjacentNodes;
        std::for_each(linksOfNode2.begin(), linksOfNode2.end(), [link, weight](std::pair<int, int>& reverse) {
            if (reverse.first == link.first)
            {
                reverse.second = weight;
            }});

    }
}

void Graph::DeleteNodeById(int nodeId)
{
    //std::find_if(nodes.begin(), nodes.end(), [nodeId](auto node) {return node.nodeId == nodeId; });
    //for (auto node : nodes) {
    //    node.adjacentNodes.erase(std::find_if(node.adjacentNodes.begin(), node.adjacentNodes.end(), [nodeId](int id) {return id == nodeId; }));
    //}
    //nodes.erase(nodes.begin()+1);
}
