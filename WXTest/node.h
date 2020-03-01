#pragma once
#include <vector>

struct Node
{
    Node(int id, int x, int y);
    int nodeId;
    int X;
    int Y;
    std::vector<std::pair<int, int>> adjacentNodes;
};

