#include "../include/Algo.h"

void detDfs(int cur, int &curTime, vector<int> &enter, vector<int> &ret, vector<bool> &visited, vector<vector<int>> &adj,
            vector<int> &parents, map<string, Edge *> &edgesMap, vector<Edge *> &oneDetBridges) {
    stack<int> stack;
    stack.push(cur);
    visited[cur] = true;
    curTime = 0;
    enter[cur] = curTime;
    ret[cur] = curTime;
    while (!stack.empty()) {
        int node = stack.top();

        bool isAllVisited = true;
        for (int i = 0; i < adj[node].size(); ++i) {
            if (!visited[adj[node][i]]) {
                stack.push(adj[node][i]);
                parents[adj[node][i]] = node;
                visited[adj[node][i]] = true;
                curTime++;
                enter[adj[node][i]] = curTime;
                ret[adj[node][i]] = curTime;
                isAllVisited = false;
                break;
            } else if (visited[adj[node][i]] && parents[node] != adj[node][i] && parents[adj[node][i]] != node) {
                ret[node] = min(ret[node], enter[adj[node][i]]);
            }
        }
        /*
        for всех u смежных с v
          if (v, u) — обратное ребро
            ret[v] = min(ret[v], enter[u])
          if вершина u — белая
            dfs(u)
            ret[v] = min(ret[v], ret[u])
            if ret[u] > enter[v]
              ребро (v, u) — мост
        */
        if (isAllVisited) {
            stack.pop();
            if (!stack.empty()) {
                ret[stack.top()] = min(ret[stack.top()], ret[node]);
                if (ret[node] > enter[stack.top()]) {
                    Edge *edge = getEdge(stack.top(), node, edgesMap);
                    oneDetBridges.push_back(edge);
                }
            }
            //updateWeights(node, adj, edgesMap, parents);
            //stack.pop();
        }
    }

    /*curTime++;
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
     */
}

void dfs(int cur, vector<bool> &visited, vector<int> &parents, vector<vector<int>> &adj, map<string, Edge *> &edgesMap) {
    stack<int> stack;
    stack.push(cur);
    visited[cur] = true;
    while (!stack.empty()) {
        int node = stack.top();

        bool isAllVisited = true;
        for (int i = 0; i < adj[node].size(); ++i) {
            if (!visited[adj[node][i]]) {
                stack.push(adj[node][i]);
                parents[adj[node][i]] = node;
                visited[adj[node][i]] = true;
                isAllVisited = false;
                break;
            }
        }
        if (isAllVisited) {
            stack.pop();
            /*if (!stack.empty()) {
                parents[node] = stack.top();
            }*/
            updateWeights(node, adj, edgesMap, parents);
            //stack.pop();
        }
    }
    /*if (colors[cur] != 0) {
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
    updateWeights(cur, adj, edgesMap, parents);*/
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


double getDetDfsTime(int &curTime, vector<int> &enter, vector<int> &ret, vector<bool> &visited, vector<vector<int>> &adj,
                     vector<int> &parents, map<string, Edge *> &edgesMap, vector<Edge *> &oneDetBridges) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    for (int i = 0; i < visited.size(); i++) {
        detDfs(i, curTime, enter, ret, visited, adj, parents, edgesMap, oneDetBridges);
    }
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    executionTime = elapsed_seconds.count();
    cout << "det_dfs_time: " << executionTime << endl;
    return executionTime;
}

double getDfsTime(int &curTime, vector<int> &enter, vector<int> &ret, vector<bool> &visited, vector<vector<int>> &adj,
                  vector<int> &parents, map<string, Edge *> &edgesMap, vector<Edge *> &oneDetBridges) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    for (int i = 0; i < visited.size(); i++) {
        dfs(i, visited, parents, adj, edgesMap);
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

double getRadixSortTime(vector<Edge *> &v) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    radixSort(v);
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    executionTime = elapsed_seconds.count();
    cout << "radix_sort: " << executionTime << endl;
    return executionTime;
}