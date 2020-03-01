#include <iostream>
#include <queue>
#include <map>
#include "algorithms.h"

std::vector<Edge> Algorithms::BFSTree(int startNodeId, IGraph* graph)
{
    std::vector<Edge> result;
    std::vector<int> visited;
    std::queue<int> traversal;

    if (startNodeId != 0) {
        visited.push_back(startNodeId);
        traversal.push(startNodeId);

        while (!traversal.empty()) {
            auto adjNodeIds = graph->GetNodeById(traversal.front())->adjacentNodes;
            for (auto id : adjNodeIds) {
                if (std::find(visited.begin(), visited.end(), id.first) == visited.end()) {
                    visited.push_back(id.first);
                    traversal.push(id.first);
                    result.push_back(std::make_pair(traversal.front(), id.first));
                }
            }
            traversal.pop();
        }
    }

    PrintRoute(result);

    return result;
}


std::vector<Edge> Dijkstra(int startNodeId, IGraph* graph)
{
    //using NodeID_PrevNode = std::pair<int, int>;

    return std::vector<Edge>();
    //std::vector<NodeID_PrevNode> visited;
    //std::map<int, int> traversal;

    //if (startNodeId != 0) {
    //    visited.push_back({ startNodeId, 0 });
    //    traversal.insert({ 0, startNodeId });

    //    while (!traversal.empty()) {
    //        auto adjNodes = graph->GetNodeById(traversal.begin()->second)->adjacentNodes;
    //        for (auto node : adjNodes) {
    //            if (std::find(visited.begin(), visited.end(), node.first) == visited.end()) {
    //                int new_cost = traversal.begin()->first + node.second;
    //                if(traversal.find(node.first) == traversal.end() || traversal[node.second] > new_cost)
    //                    traversal[]
    //            }
    //        }
    //        traversal.pop();
    //    }
    //}
}

void Algorithms::PrintRoute(std::vector<Edge> linkList)
{
    for (auto link : linkList) {
        std::cout << "node" << link.first << " -> " << "node" << link.second << std::endl;
    }
}