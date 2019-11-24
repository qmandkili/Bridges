#include "../include/Edge.h"

using namespace std;

long long MAX_LONG_LONG = LLONG_MAX;

int Edge::getVIndex() {
    return v;
}

int Edge::getUIndex() {
    return u;
}

long long Edge::getW() {
    return w;
}

void Edge::setW(long long w) {
    this->w = w;
}

bool Edge::isBasic() {
    return basic;
}

void Edge::setBasic(bool basic) {
    this->basic = basic;
}

bool Edge::isUpdated() {
    return updated;
}

void Edge::setUpdated(bool updated) {
    this->updated = updated;
}

string getKey(int v, int u) {
    return v < u ? to_string(v) + "_" + to_string(u) : to_string(u) + "_" + to_string(v);
}

Edge *getEdge(int v, int u, map<string, Edge *> &edgesMap) {
    return edgesMap.find(getKey(v, u))->second;
}

long long getRandomW() {
    return rand() % LLONG_MAX;
}

void updateWeights(int cur, vector<vector<int>> &adj, map<string, Edge *> &edgesMap, vector<int> &parents) {
    if (adj[cur].size() < 1) {
        return;
    }
    if (adj[cur].size() == 1) {
        Edge *edge = getEdge(cur, adj[cur][0], edgesMap);
        edge->setUpdated(true);
        edge->setBasic(true);
        edge->setW(0);
    } else if (adj[cur].size() == 2) {
        Edge *edge1 = getEdge(cur, adj[cur][0], edgesMap);
        Edge *edge2 = getEdge(cur, adj[cur][1], edgesMap);
        if (edge1->isUpdated()) {
            edge2->setW(edge1->getW());
            edge2->setUpdated(true);
            edge2->setBasic(true);
        } else if (edge2->isUpdated()) {
            edge1->setW(edge2->getW());
            edge1->setUpdated(true);
            edge1->setBasic(true);
        } else if (!(edge1->isUpdated() && edge2->isUpdated())) {
            long long tmpW = getRandomW();
            edge1->setW(tmpW);
            edge1->setUpdated(true);
            edge2->setW(tmpW);
            edge2->setUpdated(true);
        }
    } else {
        long long tmpW = 0;
        Edge *parentEdge = NULL;
        for (int i = 0; i < adj[cur].size(); i++) {
            Edge *tmpEdge = getEdge(cur, adj[cur][i], edgesMap);
            if (tmpEdge->isUpdated()) {
                tmpW = tmpW ^ tmpEdge->getW();
            } else if (!tmpEdge->isUpdated() &&
                       (tmpEdge->getVIndex() == parents[cur] || tmpEdge->getUIndex() == parents[cur])) {
                parentEdge = tmpEdge;
            } else if (!tmpEdge->isUpdated() && tmpEdge->getVIndex() != parents[cur] &&
                       tmpEdge->getUIndex() != parents[cur]) {
                tmpEdge->setW(getRandomW());
                tmpEdge->setUpdated(true);
                tmpW = tmpW ^ tmpEdge->getW();
            }
        }
        if (parentEdge) {
            parentEdge->setW(tmpW);
            parentEdge->setUpdated(true);
            parentEdge->setBasic(true);
        }
    }
}