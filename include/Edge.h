#pragma once
#include <bitset>
#include <map>
#include <vector>

using namespace std;

class Edge {

    int v;
    int u;
    bitset<64> weight;
    long long w;
    bool used;
    bool basic;
    bool updated;

public:
    Edge(int v, int u) : v(v), u(u), weight(0), w(0), used(false), updated(false), basic(false) {}

    int getVIndex();
    int getUIndex();

    bitset<64> getWeight();
    void setWeight(bitset<64> weight);

    long long getW();
    void setW(long long w);

    bool isUsed();
    void setUsed(bool used);

    bool isBasic();

    void setBasic(bool basic);

    bool isUpdated();
    void setUpdated(bool updated);
};


long long getRandomW();
string getKey(int v, int u);
Edge *getEdge(int v, int u, map<string, Edge *> &edgesMap);
void updateWeights(int cur, vector<vector<int>> &adj, map<string, Edge *> &edgesMap, vector<int> &parents);