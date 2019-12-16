#pragma once
#include <bitset>
#include <map>
#include <vector>
#include <limits.h>

using namespace std;

class Edge {

    int v;
    int u;
    long long w;
    bool basic;
    bool updated;

public:
    Edge(int v, int u) : v(v), u(u), w(0), updated(false), basic(false) {}

    int getVIndex();
    int getUIndex();

    long long getW();
    void setW(long long w);

    bool isBasic();

    void setBasic(bool basic);

    bool isUpdated();
    void setUpdated(bool updated);
};


long long getRandomW();
string getKey(int v, int u);
Edge *getEdge(int v, int u, map<string, Edge *> &edgesMap);
void updateWeights(int cur, vector<vector<int>> &adj, map<string, Edge *> &edgesMap, vector<int> &parents);