#include "../include/Algo.h"

void detDfs(int cur, int &curTime, vector<int> &enter, vector<int> &ret, vector<int> &colors, vector<vector<int>> &adj,
            vector<int> &parents, map<string, Edge *> &edgesMap, vector<Edge *> &oneDetBridges) {
    curTime++;
    enter[cur] = curTime;
    ret[cur] = curTime;
    if (colors[cur] != 0) {
        return;
    }
    colors[cur] = 1;
    for (int i = 0; i < adj[cur].size(); i++) {
        if (colors[adj[cur][i]] == 0) {
            parents[adj[cur][i]] = cur;
            detDfs(adj[cur][i], curTime, enter, ret, colors, adj, parents, edgesMap, oneDetBridges);
            ret[cur] = min(ret[cur], ret[adj[cur][i]]);
            if (ret[adj[cur][i]] > enter[cur]) {
                Edge *edge = getEdge(cur, adj[cur][i], edgesMap);
                oneDetBridges.push_back(edge);
            }
        } else if (parents[cur] != adj[cur][i] && colors[adj[cur][i]] == 1) {
            ret[cur] = min(ret[cur], enter[adj[cur][i]]);
        }
    }
    colors[cur] = 2;
}

void dfs(int cur, vector<int> &colors, vector<int> &parents, vector<vector<int>> &adj, map<string, Edge *> &edgesMap) {
    if (colors[cur] != 0) {
        return;
    }
    colors[cur] = 1;
    for (int i = 0; i < adj[cur].size(); i++) {
        if (colors[adj[cur][i]] == 0) {
            parents[adj[cur][i]] = cur;
            dfs(adj[cur][i], colors, parents, adj, edgesMap);
        }
    }
    colors[cur] = 2;
    updateWeights(cur, adj, edgesMap, parents);
}

bool isDetDfsEqualToDfs(vector<Edge *> &detDfs, vector<Edge *> &dfs) {
    if (detDfs.size() != dfs.size()) {
        return false;
    } else if (detDfs.empty() && detDfs.empty()) {
        return true;
    } else {
        bool isEqual = false;
        for (int i = 0; i < detDfs.size(); i++) {
            bool contains = false;
            for (int j = 0; j < dfs.size(); j++) {
                if (detDfs[i] == dfs[j]) {
                    contains = true;
                    break;
                }
            }
            if (contains) {
                isEqual = true;
            } else break;
        }
        return isEqual;
    }
}


double getDetDfsTime(int &curTime, vector<int> &enter, vector<int> &ret, vector<int> &colors, vector<vector<int>> &adj,
                     vector<int> &parents, map<string, Edge *> &edgesMap, vector<Edge *> &oneDetBridges) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    for (int i = 0; i < colors.size(); i++) {
        detDfs(i, curTime, enter, ret, colors, adj, parents, edgesMap, oneDetBridges);
    }
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    executionTime = elapsed_seconds.count();
    cout << "det_dfs_time: " << executionTime << endl;
    return executionTime;
}

double getDfsTime(int &curTime, vector<int> &enter, vector<int> &ret, vector<int> &colors, vector<vector<int>> &adj,
                  vector<int> &parents, map<string, Edge *> &edgesMap, vector<Edge *> &oneDetBridges) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    for (int i = 0; i < colors.size(); i++) {
        dfs(i, colors, parents, adj, edgesMap);
    }
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    executionTime = elapsed_seconds.count();
    cout << "dfs_time: " << executionTime << endl;
    return executionTime;
}

double getStdSortTime(vector<Edge *> &v) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    std::sort(v.begin(), v.end(), sortByW);
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    executionTime = elapsed_seconds.count();
    cout << "std::sort: " << executionTime << endl;
    return executionTime;
}

double getBucketSortTime(vector<Edge *> &v, int n, long long maxValue) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    bucketSort(v, n + 1, maxValue);
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    executionTime = elapsed_seconds.count();
    cout << "bucket_sort: " << executionTime << endl;
    return executionTime;
}