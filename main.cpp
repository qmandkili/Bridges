#include <iostream> // header in standard library
#include <vector>
#include <list>
#include <string>
#include <iterator>
#include <stdlib.h>
#include <limits.h>
#include <bitset>
#include <map>
#include <random>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>

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

        int getVIndex() {
            return v;
        }

        int getUIndex() {
            return u;
        }

        bitset<64> getWeight() {
            return weight;
        }

        void setWeight(bitset<64> weight) {
            this->weight = weight;
        }

        long long getW() {
            return w;
        }

        void setW(long long w) {
            this->w = w;
        }

        bool isUsed() {
            return used;
        }

        void setUsed(bool used) {
            this->used = used;
        }

        bool isBasic() {
            return basic;
        }

        void setBasic(bool basic) {
            this->basic = basic;
        }

        bool isUpdated() {
            return updated;
        }

        void setUpdated(bool updated) {
            this->updated = updated;
        }
    };

// Radix sort comparator for 32-bit two's complement integers
class radix_sort
{
    const int bit; // bit position [0..31] to examine
public:
    radix_sort(int offset) : bit(offset) {} // constructor

    bool operator()(Edge* edge) const // function call operator
    {
        if (bit == 63) // sign bit
            return edge->getW() < 0; // negative int to left partition
        else
            return !(edge->getW() & (1 << bit)); // 0 bit to left partition
    }
    /*bool operator()(const pair<int,int>& p) {
        return p.first < _median;
    }*/
};

bool sortByW(Edge* A, Edge* B) {
    return (A->getW() < B->getW());
}

// Most significant digit radix sort (recursive)
/*void msd_radix_sort(Edge *first, Edge *last, int msb = 63)
{
    if (first != last && msb >= 0)
    {
        Edge *mid = std::partition(first, last, radix_sort(msb));
        msb--; // decrement most-significant-bit
        msd_radix_sort(first, mid, msb); // sort left partition
        msd_radix_sort(mid, last, msb); // sort right partition
    }
}*/

// Least significant digit radix sort
void lsd_radix_sort(vector<Edge*> edges)
{
    for (int lsb = 0; lsb < 64; ++lsb) // least-significant-bit
    {
        std::stable_partition(edges.begin(), edges.end(), radix_sort(lsb));
    }
}

// Function to sort arr[] of size n using bucket sort
void bucketSort(vector<Edge*> &arr, int n, long long maxValue) {
    // 1) Create n empty buckets
    vector<Edge*> b[n];

    // 2) Put array elements in different buckets
    for (int i=0; i<arr.size(); i++)
    {
        Edge* edge = arr[i];
        //TODO: исправить ошибку в подсчете индексов корзин (мб она пофиксилась правильной работой std сортировки???)
        int bi = (n-1)*edge->getW(); // Index in bucket
        if (maxValue != 0) {
            bi = bi/maxValue;
        }
        if (bi < 0) {
            bi = 0;
        }
        b[bi].push_back(arr[i]);
    }

    // 3) Sort individual buckets
    for (int i=0; i<n; i++)
        sort(b[i].begin(), b[i].end(), sortByW);

    // 4) Concatenate all buckets into arr[]
    int index = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < b[i].size(); j++)
            arr[index++] = b[i][j];
}

void updateWeights(int cur);

long long MAX_LONG_LONG = LLONG_MAX;
static bitset<64> MIN_VALUE;

    static vector<int> parents;

    vector<int> colors;
    vector<int> clusters;
    vector<vector<int>> adj;

    vector<int> enter;
    vector<int> ret;

    map<string, Edge *> edgesMap;

    static int curTime = 0;
    vector<Edge *> oneDetBridges;
    vector<Edge *> oneDfsBridges;



long long getRandomW() {
    return rand() % LLONG_MAX;
}

string getKey(int v, int u) {
    return v < u ? to_string(v) + "_" + to_string(u) : to_string(u) + "_" + to_string(v);
}

Edge * getEdge(int v, int u) {
    return edgesMap.find(getKey(v, u))->second;
}

void detDfs(int cur) {
    curTime++;
    enter[cur] = curTime;
    ret[cur] = curTime;
    if (colors[cur] != 0) {
        return;
    }
    colors[cur] = 1;
    for (int i = 0; i < adj[cur].size(); i++)  {
        if (colors[adj[cur][i]] == 0) {
            parents[adj[cur][i]] = cur;
            detDfs(adj[cur][i]);
            ret[cur] = min(ret[cur], ret[adj[cur][i]]);
            if (ret[adj[cur][i]] > enter[cur]) {
                Edge* edge = getEdge(cur, adj[cur][i]);
                oneDetBridges.push_back(edge);
            }
        } else if (parents[cur] != adj[cur][i] && colors[adj[cur][i]] == 1) {
            ret[cur] = min(ret[cur], enter[adj[cur][i]]);
        }
    }
    colors[cur] = 2;
}

void dfs(int cur) {
    if (colors[cur] != 0) {
        return;
    }
    colors[cur] = 1;
    for (int i = 0; i < adj[cur].size(); i++)  {
        if (colors[adj[cur][i]] == 0) {
            parents[adj[cur][i]] = cur;
            dfs(adj[cur][i]);
        }
    }
    colors[cur] = 2;
    updateWeights(cur);
}

void updateWeights(int cur) {
    if (adj[cur].size() < 1) {
        return;
    }
    if (adj[cur].size() == 1) {
        Edge* edge = getEdge(cur, adj[cur][0]);
        edge->setUpdated(true);
        edge->setBasic(true);
        edge->setW(0);
    } else if (adj[cur].size() == 2) {
        Edge *edge1 = getEdge(cur, adj[cur][0]);
        Edge *edge2 = getEdge(cur, adj[cur][1]);
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
            Edge *tmpEdge = getEdge(cur, adj[cur][i]);
            if (tmpEdge->isUpdated()) {
                tmpW = tmpW ^ tmpEdge->getW();
            } else if (!tmpEdge->isUpdated() && (tmpEdge->getVIndex() == parents[cur] || tmpEdge->getUIndex() == parents[cur])) {
                parentEdge = tmpEdge;
            } else if (!tmpEdge->isUpdated() && tmpEdge->getVIndex() != parents[cur] && tmpEdge->getUIndex() != parents[cur]) {
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

bool isDetDfsEqualToDfs(vector<Edge*> &detDfs, vector<Edge*> &dfs) {
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

void checkClusters(int cur, int clusterIndex) {
    if (adj[cur].size() == 0) {
        return;
    }
    if (colors[cur] != 0) {
        return;
    }
    clusters[cur] = clusterIndex;
    colors[cur] = 1;
    for (int i = 0; i < adj[cur].size(); i++)  {
        if (colors[adj[cur][i]] == 0) {
            checkClusters(adj[cur][i], clusterIndex);
        }
    }
    colors[cur] = 2;
}

bool isEdgeBridge(int cur, int clusterIndex, int u) {
    checkClusters(cur, clusterIndex);
    return clusters[u] != clusterIndex ? true : false;
}

void removeFromAdj(int index, int removeIndex) {
    for (int i = 0; i < adj[index].size(); i++) {
        if (adj[index][i] == removeIndex) {
            adj[index].erase(adj[index].begin() + i);
            return;
        }
    }
}

void removeOneBridge(int v, int u) {
    removeFromAdj(v, u);
    removeFromAdj(u, v);
}

void addRemovedOneBridge(int v, int u) {
    adj[v].push_back(u);
    adj[u].push_back(v);
}

void generateGraph(int n, int probability) {
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            int rnd = rand() % 100 + 1;
            if(rnd <= probability){
                adj[i].push_back(j);
                adj[j].push_back(i);
                Edge* edge = new Edge(i, j);
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
    oneDetBridges = vector<Edge*>();
    oneDfsBridges = vector<Edge*>();

    enter = vector<int>(n, 0);
    ret = vector<int>(n, 0);
}

void resetModel(int n) {
    parents = vector<int>(n, 0);
    colors = vector<int>(n, 0);
    clusters = vector<int>(n, -1);
}

void resetOneBridges() {
    oneDetBridges = vector<Edge*>();
    oneDfsBridges = vector<Edge*>();
}

void initTestGraph1(bool isDetDfs);

void writeOutputFile(double detDfsTime, double dfsTime, double stdSortTime, double bucketSortTime, int n, int all_n, int prob);

void writeEdgesToFile(int n);

void writeBasicEdgesToFile(int n);

double getDetDfsTime(int startIndex);

double getDfsTime(int startIndex);

double getStdSortTime(vector<Edge *> &v);

double getBucketSortTime(vector<Edge *> &v, int n, long long maxValue);

int main() {
    srand(time(NULL));
    const int start = 0;

    int n_vertices = 101;
    for (int n = 10; n < n_vertices; n+=10) {
        edgesMap.clear();
        initModel(n);
        resetOneBridges();

        int probability = 20;
        generateGraph(n, probability);

        double detDfsTime = getDetDfsTime(start);

        resetModel(n);
        double dfsTime = getDfsTime(start);

        vector<Edge *> v_std;
        vector<Edge *> v_radix;
        vector<Edge *> v_bucket;

        int count = 0;
        for(map<string, Edge *>::iterator it = edgesMap.begin(); it != edgesMap.end(); ++it ) {
            count++;
            if (it->second->isUpdated()) {
                v_std.push_back( it->second );
                //v_radix.push_back( it->second );
                v_bucket.push_back( it->second );
                if (it->second->getW() == 0 && it->second->isBasic()) {
                    oneDfsBridges.push_back( it->second );
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
        for (Edge* oneEdge : oneDfsBridges) {
            removeOneBridge(oneEdge->getVIndex(), oneEdge->getUIndex());
            bool isBridge = isEdgeBridge(oneEdge->getVIndex(), oneEdge->getVIndex(), oneEdge->getUIndex());
            if (isBridge) {
                oneBridges.push_back(oneEdge);
            }
            resetModel(n);
            addRemovedOneBridge(oneEdge->getVIndex(), oneEdge->getUIndex());
        }

        bool twoBridgesFound = false;
        for (int i = 1; i < v_std.size(); i++) {
            Edge* edge1 = v_std[i - 1];
            Edge* edge2 = v_std[i];
            if (edge1->getW() == edge2->getW()) {
                removeOneBridge(edge1->getVIndex(), edge1->getUIndex());
                removeOneBridge(edge2->getVIndex(), edge2->getUIndex());

                bool firstEdgeCheck = isEdgeBridge(edge1->getVIndex(), edge1->getVIndex(), edge2->getUIndex());
                resetModel(n);
                bool secondEdgeCheck = isEdgeBridge(edge2->getVIndex(), edge2->getVIndex(), edge1->getUIndex());
                resetModel(n);

                addRemovedOneBridge(edge1->getVIndex(), edge1->getUIndex());
                addRemovedOneBridge(edge2->getVIndex(), edge2->getUIndex());
                if (firstEdgeCheck && secondEdgeCheck) {
                    twoBridgesFound = true;
                    break;
                }
            }
        }

        writeEdgesToFile(n);
        writeBasicEdgesToFile(n);
        writeOutputFile(detDfsTime, dfsTime, stdSortTime, bucketSortTime, n, n_vertices, probability);
    }

return 0;
}

void writeOutputFile(double detDfsTime, double dfsTime, double stdSortTime, double bucketSortTime, int n, int all_n, int prob) {
    ofstream myfile;
    myfile.open ("result" + to_string(all_n) + "_" + to_string(prob) + ".csv", std::ios_base::app);
    myfile << detDfsTime << ";" <<
           dfsTime << ";" <<
           stdSortTime << ";" <<
           bucketSortTime << ";" <<
           //radixSortTime << ";" <<
           n << "\n";
    myfile.close();
}

void writeEdgesToFile(int n) {
    ofstream myfile;
    myfile.open ("edges" + to_string(n) + ".csv");
    for(map<string, Edge *>::iterator it = edgesMap.begin(); it != edgesMap.end(); ++it ) {
        myfile << it->second->getVIndex() << ";" << it->second->getUIndex() << "\n";
    }
    myfile.close();
}

void writeBasicEdgesToFile(int n) {
    ofstream myfile;
    myfile.open ("basic_edges" + to_string(n) + ".csv");
    for(map<string, Edge *>::iterator it = edgesMap.begin(); it != edgesMap.end(); ++it ) {
        if (it->second->isBasic()) {
            myfile << it->second->getVIndex() << ";" << it->second->getUIndex() << "\n";
        }
    }
    myfile.close();
}

double getDetDfsTime(int startIndex) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    detDfs(startIndex);
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime-startTime;
    executionTime = elapsed_seconds.count();
    cout << "det_dfs_time: " << executionTime << endl;
    return executionTime;
}

double getDfsTime(int startIndex) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    dfs(startIndex);
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime-startTime;
    executionTime = elapsed_seconds.count();
    cout << "dfs_time: " << executionTime << endl;
    return executionTime;
}

double getStdSortTime(vector<Edge *> &v) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    std::sort(v.begin(), v.end(), sortByW);
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime-startTime;
    executionTime = elapsed_seconds.count();
    cout << "std::sort: " << executionTime << endl;
    return executionTime;
}

double getBucketSortTime(vector<Edge *> &v, int n, long long maxValue) {
    double executionTime = 0;
    auto startTime = std::chrono::system_clock::now();
    bucketSort(v, n + 1, maxValue);
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime-startTime;
    executionTime = elapsed_seconds.count();
    cout << "bucket_sort: " << executionTime << endl;
    return executionTime;
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