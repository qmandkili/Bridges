#pragma once
#include <chrono>
#include <vector>
#include <map>
#include <iostream>
#include "Edge.h"
#include "Sorts.h"
#include <algorithm>
#include <stack>

using namespace std;

void detDfs(int cur, int &curTime, vector<int> &enter, vector<int> &ret, vector<bool> &visited, vector<vector<int>> &adj,
            vector<int> &parents, map<string, Edge *> &edgesMap, vector<Edge *> &oneDetBridges);

void dfs(int cur, vector<bool> &visited, vector<int> &parents, vector<vector<int>> &adj, map<string, Edge *> &edgesMap);

bool isDetDfsEqualToDfs(vector<Edge *> &detDfs, vector<Edge *> &dfs);


double getDetDfsTime(int &curTime, vector<int> &enter, vector<int> &ret, vector<bool> &visited, vector<vector<int>> &adj,
                     vector<int> &parents, map<string, Edge *> &edgesMap, vector<Edge *> &oneDetBridges);

double getDfsTime(int &curTime, vector<int> &enter, vector<int> &ret, vector<bool> &visited, vector<vector<int>> &adj,
                  vector<int> &parents, map<string, Edge *> &edgesMap, vector<Edge *> &oneDetBridges);

double getStdSortTime(vector<Edge *> &v);

double getBucketSortTime(vector<Edge *> &v, int n, long long maxValue);

double getRadixSortTime(vector<Edge *> &v);