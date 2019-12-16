#pragma once

#include <vector>
#include <map>
#include <random>
#include "Edge.h"

using namespace std;

void removeFromAdj(int index, int removeIndex, vector<vector<int>> &adj);
void removeOneBridge(int v, int u, vector<vector<int>> &adj);
void addRemovedOneBridge(int v, int u, vector<vector<int>> &adj);
void checkClusters(int cur, int clusterIndex, vector<vector<int>> &adj, vector<int> &colors, vector<int> &clusters);
bool isEdgeBridge(int cur, int clusterIndex, int u, vector<vector<int>> &adj, vector<int> &colors, vector<int> &clusters);

