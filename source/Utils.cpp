#include "../include/Utils.h"

void removeFromAdj(int index, int removeIndex, vector<vector<int>> &adj) {
    for (int i = 0; i < adj[index].size(); i++) {
        if (adj[index][i] == removeIndex) {
            adj[index].erase(adj[index].begin() + i);
            return;
        }
    }
}

void removeOneBridge(int v, int u, vector<vector<int>> &adj) {
    removeFromAdj(v, u, adj);
    removeFromAdj(u, v, adj);
}

void addRemovedOneBridge(int v, int u, vector<vector<int>> &adj) {
    adj[v].push_back(u);
    adj[u].push_back(v);
}

void checkClusters(int cur, int clusterIndex, vector<vector<int>> &adj, vector<int> &colors, vector<int> &clusters) {
    if (adj[cur].size() == 0) {
        return;
    }
    if (colors[cur] != 0) {
        return;
    }
    clusters[cur] = clusterIndex;
    colors[cur] = 1;
    for (int i = 0; i < adj[cur].size(); i++) {
        if (colors[adj[cur][i]] == 0) {
            checkClusters(adj[cur][i], clusterIndex, adj, colors, clusters);
        }
    }
    colors[cur] = 2;
}

bool isEdgeBridge(int cur, int clusterIndex, int u, vector<vector<int>> &adj, vector<int> &colors, vector<int> &clusters) {
    checkClusters(cur, clusterIndex, adj, colors, clusters);
    return clusters[u] != clusterIndex;
}