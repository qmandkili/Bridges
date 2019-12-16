#include "include/Edge.h"
#include "include/Utils.h"
#include "include/IO.h"
#include "include/Algo.h"

using namespace std;

static int curTime = 0;

static bitset<64> MIN_VALUE;
static vector<int> parents;

vector<int> colors;
vector<bool> visited;
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
                string key = to_string(i) + "_" + to_string(j);
                edgesMap.insert(pair<string, Edge *>(key, edge));
            }
        }
    }
}

void initModel(int n) {
    parents = vector<int>(n, 0);
    colors = vector<int>(n, 0);
    visited = vector<bool>(n, false);
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
    visited = vector<bool>(n, false);
    clusters = vector<int>(n, -1);
}
void resetOneBridges() {
    oneDetBridges = vector<Edge *>();
    oneDfsBridges = vector<Edge *>();
}

int main(int argc, char **argv) {
    srand(time(NULL));
    const int start = 0;

    int n_vertices = atoi(argv[1]);
    int probability = atoi(argv[2]);

    int c = 2;

    for (int n = 10; n < n_vertices + 1; n += 10) {
        edgesMap.clear();
        initModel(n);
        resetOneBridges();

        generateGraph(n, probability);

        cout << "n: " << n << endl;

        double detDfsTime = getDetDfsTime(curTime, enter, ret, visited, adj, parents, edgesMap, oneDetBridges);
        resetModel(n);
        double dfsTime = getDfsTime(curTime, enter, ret, visited, adj, parents, edgesMap, oneDetBridges);

        vector<Edge *> v_std;
        vector<Edge *> v_radix;
        vector<Edge *> v_bucket;

        int count = 0;
        for (map<string, Edge *>::iterator it = edgesMap.begin(); it != edgesMap.end(); ++it) {
            count++;
            if (it->second->isUpdated()) {
                v_std.push_back(it->second);
                v_radix.push_back( it->second );
                v_bucket.push_back(it->second);
                if (it->second->getW() == 0 && it->second->isBasic()) {
                    oneDfsBridges.push_back(it->second);
                }
            }
        }
        cout << "count: " << count << endl;
        double stdSortTime = getStdSortTime(v_std);
        double radixSortTime = getRadixSortTime(v_radix);
        double bucketSortTime = getBucketSortTime(v_bucket, n, v_std[v_std.size() - 1]->getW());


        writeEdgesToFile2(n, probability, oneDetBridges);

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

        vector<Edge *> twoBridges = vector<Edge *>();
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
                    twoBridges.push_back(edge1);
                    twoBridges.push_back(edge2);
                    break;
                }
            }
        }

        writeTwoEdgesToFile(n, probability, twoBridges);
        writeEdgesToFile(n, probability, edgesMap);
        writeBasicEdgesToFile(n, probability, edgesMap);
        writeOutputFile(detDfsTime, dfsTime, stdSortTime, bucketSortTime, radixSortTime, n, probability);

        cout << "______" << endl;
    }

    return 0;
}