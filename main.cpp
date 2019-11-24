#include "include/Edge.h"
#include "include/Utils.h"
#include "include/IO.h"
#include "include/Algo.h"

using namespace std;

static int curTime = 0;

long long MAX_LONG_LONG = LLONG_MAX;

static bitset<64> MIN_VALUE;
static vector<int> parents;

vector<int> colors;
vector<int> clusters;
vector<vector<int>> adj;
vector<int> enter;
vector<int> ret;

vector<Edge *> oneDetBridges;
vector<Edge *> oneDfsBridges;

map<string, Edge *> edgesMap;

void generateGraph(int n, int probability) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int rnd = rand() % 100 + 1;
            if (rnd <= probability) {
                adj[i].push_back(j);
                adj[j].push_back(i);
                Edge *edge = new Edge(i, j);
                //Edge edge(i, j);
                string key = to_string(i) + "_" + to_string(j);
                edgesMap.insert(pair<string, Edge *>(key, edge));
            }
        }
    }
}

void initModel(int n) {
    parents = vector<int>(n, 0);
    colors = vector<int>(n, 0);
    clusters = vector<int>(n, -1);
    adj = vector<vector<int>>(n, vector<int>());
    oneDetBridges = vector<Edge *>();
    oneDfsBridges = vector<Edge *>();

    enter = vector<int>(n, 0);
    ret = vector<int>(n, 0);
}
void resetModel(int n) {
    parents = vector<int>(n, 0);
    colors = vector<int>(n, 0);
    clusters = vector<int>(n, -1);
}
void resetOneBridges() {
    oneDetBridges = vector<Edge *>();
    oneDfsBridges = vector<Edge *>();
}
void initTestGraph1(bool isDetDfs) {
    adj[0].push_back(2);
    adj[0].push_back(4);
    adj[0].push_back(6);
    adj[0].push_back(7);
    adj[1].push_back(2);
    adj[1].push_back(3);
    adj[1].push_back(7);
    adj[2].push_back(0);
    adj[2].push_back(1);
    adj[2].push_back(5);
    adj[3].push_back(1);
    adj[3].push_back(4);
    adj[4].push_back(0);
    adj[4].push_back(3);
    adj[5].push_back(2);
    adj[5].push_back(7);
    adj[6].push_back(0);
    adj[7].push_back(0);
    adj[7].push_back(1);
    adj[7].push_back(5);

    if (isDetDfs) {
        Edge edge0_2(0, 2);
        Edge edge0_4(0, 4);
        Edge edge0_6(0, 6);
        Edge edge0_7(0, 7);
        Edge edge1_2(1, 2);
        Edge edge1_3(1, 3);
        Edge edge1_7(1, 7);
        Edge edge2_5(2, 5);
        Edge edge3_4(3, 4);
        Edge edge5_7(5, 7);

        edgesMap.insert(pair<string, Edge *>("0_2", &edge0_2));
        edgesMap.insert(pair<string, Edge *>("0_4", &edge0_4));
        edgesMap.insert(pair<string, Edge *>("0_6", &edge0_6));
        edgesMap.insert(pair<string, Edge *>("0_7", &edge0_7));
        edgesMap.insert(pair<string, Edge *>("1_2", &edge1_2));
        edgesMap.insert(pair<string, Edge *>("1_3", &edge1_3));
        edgesMap.insert(pair<string, Edge *>("1_7", &edge1_7));
        edgesMap.insert(pair<string, Edge *>("2_5", &edge2_5));
        edgesMap.insert(pair<string, Edge *>("3_4", &edge3_4));
        edgesMap.insert(pair<string, Edge *>("5_7", &edge5_7));
    }
};


int main() {
    srand(time(NULL));
    const int start = 0;

    int n_vertices = 101;
    for (int n = 10; n < n_vertices; n += 10) {
        edgesMap.clear();
        initModel(n);
        resetOneBridges();

        int probability = 20;
        generateGraph(n, probability);

        double detDfsTime = getDetDfsTime(start, curTime, enter, ret, colors, adj, parents, edgesMap, oneDetBridges);

        resetModel(n);
        double dfsTime = getDfsTime(start, curTime, enter, ret, colors, adj, parents, edgesMap, oneDetBridges);

        vector<Edge *> v_std;
        vector<Edge *> v_radix;
        vector<Edge *> v_bucket;

        int count = 0;
        for (map<string, Edge *>::iterator it = edgesMap.begin(); it != edgesMap.end(); ++it) {
            count++;
            if (it->second->isUpdated()) {
                v_std.push_back(it->second);
                //v_radix.push_back( it->second );
                v_bucket.push_back(it->second);
                if (it->second->getW() == 0 && it->second->isBasic()) {
                    oneDfsBridges.push_back(it->second);
                }
            }
        }
        //int v_bucket_size = v_bucket.size();
        //cout << "v_size: : " << v_bucket_size << endl;
        cout << "count: : " << count << endl;

        double stdSortTime = getStdSortTime(v_std);
        double bucketSortTime = getBucketSortTime(v_bucket, n, v_std[v_std.size() - 1]->getW());

        bool isEqual = isDetDfsEqualToDfs(oneDfsBridges, oneDfsBridges);

        vector<Edge *> oneBridges = vector<Edge *>();
        resetModel(n);
        for (Edge *oneEdge : oneDfsBridges) {
            removeOneBridge(oneEdge->getVIndex(), oneEdge->getUIndex(), adj);
            bool isBridge = isEdgeBridge(oneEdge->getVIndex(), oneEdge->getVIndex(), oneEdge->getUIndex(), adj, colors, clusters);
            if (isBridge) {
                oneBridges.push_back(oneEdge);
            }
            resetModel(n);
            addRemovedOneBridge(oneEdge->getVIndex(), oneEdge->getUIndex(), adj);
        }

        bool twoBridgesFound = false;
        for (int i = 1; i < v_std.size(); i++) {
            Edge *edge1 = v_std[i - 1];
            Edge *edge2 = v_std[i];
            if (edge1->getW() == edge2->getW()) {
                removeOneBridge(edge1->getVIndex(), edge1->getUIndex(), adj);
                removeOneBridge(edge2->getVIndex(), edge2->getUIndex(), adj);

                bool firstEdgeCheck = isEdgeBridge(edge1->getVIndex(), edge1->getVIndex(), edge2->getUIndex(), adj, colors, clusters);
                resetModel(n);
                bool secondEdgeCheck = isEdgeBridge(edge2->getVIndex(), edge2->getVIndex(), edge1->getUIndex(), adj, colors, clusters);
                resetModel(n);

                addRemovedOneBridge(edge1->getVIndex(), edge1->getUIndex(), adj);
                addRemovedOneBridge(edge2->getVIndex(), edge2->getUIndex(), adj);
                if (firstEdgeCheck && secondEdgeCheck) {
                    twoBridgesFound = true;
                    break;
                }
            }
        }

        writeEdgesToFile(n, edgesMap);
        writeBasicEdgesToFile(n, edgesMap);
        writeOutputFile(detDfsTime, dfsTime, stdSortTime, bucketSortTime, n, n_vertices, probability);
    }

    return 0;
}