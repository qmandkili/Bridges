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

        Edge(int v, int u) : v(v), u(u), weight(0), w(0), used(false), updated(false) {}

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
        /*bool operator > (Edge & edge) const
        {
            return (w > edge.w);
        }*/
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
        int bi = (n-1)*edge->getW() / maxValue; // Index in bucket
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

void updateWeights(int cur, vector<vector<int>> &adj);

long long MAX_LONG_LONG = LLONG_MAX;
static bitset<64> MIN_VALUE;

    //static const int n = 1000;
    static vector<int> parents;

    static vector<bool> leaves;
    vector<int> colors;
    static vector<vector<Edge *>> edges;
    vector<vector<int>> adj;

    vector<int> enter;
    vector<int> ret;

    map<string, Edge *> edgesMap;

    static int curTime = 0;
    vector<Edge *> oneBridges;



    long long getRandomW() {
        return rand() % LLONG_MAX;
    }

Edge * getEdge(int v, int u) {
    map<string, Edge *>::iterator it;
    string key;
    if (v < u) {
        key = to_string(v) + "_" + to_string(u);
    } else {
        key = to_string(u) + "_" + to_string(v);
    }
    it = edgesMap.find(key);
    return it->second;
}

void dfs(int cur, vector<vector<int>> &adj, vector<bool> &was, int &count, vector<int> &color) {
    curTime++;
    enter[cur] = curTime;
    ret[cur] = curTime;
    if (color[cur] != 0) {
        return;
    }
    was[cur] = true;
    color[cur] = 1;
    count++;
    for (int i = 0; i < adj[cur].size(); i++)  {
        if (color[adj[cur][i]] == 0) {
            parents[adj[cur][i]] = cur;
            dfs(adj[cur][i], adj, was, count, color);
            ret[cur] = min(ret[cur], ret[adj[cur][i]]);
            if (ret[adj[cur][i]] > enter[cur]) {
                Edge* edge = getEdge(cur, adj[cur][i]);
                oneBridges.push_back(edge);
            }
        } else if (parents[cur] != adj[cur][i] && color[adj[cur][i]] == 1) {
            ret[cur] = min(ret[cur], enter[adj[cur][i]]);
        }
    }
    //TODO: добавить обновление флага updated (и used???) на все ребра, по которым не переходили из данной вершины ???????
    color[cur] = 2;
    updateWeights(cur, adj);
}

void updateWeights(int cur, vector<vector<int>> &adj) {
    if (adj[cur].size() < 2) {
        Edge* edge = getEdge(cur, adj[cur][0]);
        edge->setUpdated(true);
        edge->setW(0);
    } else if (adj[cur].size() == 2) {
        Edge *edge1 = getEdge(cur, adj[cur][0]);
        Edge *edge2 = getEdge(cur, adj[cur][1]);
        if (edge1->isUpdated()) {
            edge2->setW(edge1->getW());
            edge2->setUpdated(true);
        } else if (edge2->isUpdated()) {
            edge1->setW(edge2->getW());
            edge1->setUpdated(true);
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
        }
    }
}

int getCount(int start, vector<vector<int>> &adj, vector<int> &color) {
        vector<bool> was(adj.size(), false);
        int count = 0;
        dfs(start, adj, was, count, color);
        return count;
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
    leaves = vector<bool>(n, false);
    colors = vector<int>(n, 0);
    edges = vector<vector<Edge *>>(n, vector<Edge *>());
    adj = vector<vector<int>>(n, vector<int>());
    oneBridges = vector<Edge*>();

    enter = vector<int>(n, 0);
    ret = vector<int>(n, 0);
}



/*void reverse(int index) {
    if (edges[index].size() < 2) {
        return;
    }
    if (edges[index].size() == 2) {
        updateWeightForTwoAdjacentEdges(index);
        reverse(parents[index]);
    }
}*/

/*void updateWeightForTwoAdjacentEdges(int index) {
    Edge edge1 = edges[index][0];
    Edge edge2 = edges[index][1];
    string key1 = to_string(edge1.getVIndex()) + "_" + to_string(edge1.getUIndex());
    string key2 = to_string(edge2.getVIndex()) + "_" + to_string(edge2.getUIndex());
    map<string,Edge>::iterator it1;
    it1 = edgesMap.find(key1);
    map<string,Edge>::iterator it2;
    it2 = edgesMap.find(key2);
    if (it1->second.isUsed()) {
        it1->second.setWeight(MAX_LONG_LONG - it2->second.getWeight());
    } else {
        it2->second.setWeight(MAX_LONG_LONG - it1->second.getWeight());
    }
}*/

int main() {
    srand(time(NULL));
    const int start = 0;

    //int data[] = { 170, 45, 75, -90, -802, 24, 2, 66 };

    //std::copy(data, data + 8, std::ostream_iterator<int>(std::cout, " "));

    int n_vertices = 11;
    for (int n = 8; n < n_vertices; n+=10) {
        initModel(n);

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

        int probability = 40;
        //generateGraph(n, probability);

        auto startTime = std::chrono::system_clock::now();
        int result = getCount(start, adj, colors);
        cout << result << endl;

// Some computation here
        auto endTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = endTime-startTime;
        cout << "elapsed_seconds: " << elapsed_seconds.count() << endl;

        vector<Edge *> v_std;
        vector<Edge *> v_radix;
        vector<Edge *> v_bucket;

// populate map somehow
//std::map<char,int>::iterator it;
        int count = 0;
        for(map<string, Edge *>::iterator it = edgesMap.begin(); it != edgesMap.end(); ++it ) {
            count++;
            if (it->second->isUpdated()) {
                v_std.push_back( it->second );
                //v_radix.push_back( it->second );
                v_bucket.push_back( it->second );
            }
        }
        int v_bucket_size = v_bucket.size();
        cout << "v_size: : " << v_bucket_size << endl;
        cout << "count: : " << count << endl;


//std::sort(v.begin(), v.end(), greater<Edge *>());
        auto startTimeStdSort = std::chrono::system_clock::now();
        std::sort(v_std.begin(), v_std.end(), sortByW);
        auto endTimeStdSort = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds_std_sort = endTimeStdSort-startTimeStdSort;
        cout << "std::sort: " << elapsed_seconds_std_sort.count() << endl;

        /*startTime = std::chrono::system_clock::now();
        lsd_radix_sort(v_radix);
        endTime = std::chrono::system_clock::now();
        elapsed_seconds = endTime-startTime;
        cout << "lsd_radix_sort: " << elapsed_seconds.count() << endl;
        vector<Edge *> radix_error;
        bool isFalse = false;
        for (int i = 1; i < v_radix.size(); i++) {
            if (v_radix[i - 1]->getW() > v_radix[i]->getW()) {
                isFalse = true;
                radix_error.push_back(v_radix[i]);
            }
        }*/

        auto startTimeBucketSort = std::chrono::system_clock::now();
        startTime = std::chrono::system_clock::now();
        Edge* maxEdge = v_std[v_std.size() - 1];
        int bucketSize = sizeof(v_bucket) / sizeof(long long);
        bucketSort(v_bucket, n + 1, v_std[v_std.size() - 1]->getW());
        auto endTimeBucketSort = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds_bucket_sort = endTimeBucketSort-startTimeBucketSort;
        cout << "bucketSort: " << elapsed_seconds_bucket_sort.count() << endl;
        vector<Edge *> bucket_error;
        bool isFalse = false;
        for (int i = 1; i < v_bucket.size(); i++) {
            if (v_bucket[i - 1]->getW() > v_bucket[i]->getW()) {
                isFalse = true;
                bucket_error.push_back(v_bucket[i]);
            }
        }

        ofstream myfile;
        myfile.open ("example.csv", std::ios_base::app);
        myfile << elapsed_seconds.count() << ";" <<
                elapsed_seconds_std_sort.count() << ";" <<
                elapsed_seconds_bucket_sort.count() << ";" <<
                n << "\n";
        myfile.close();
    }





    //std::chrono::duration<double> elapsed_seconds = endTime-startTime;
    //std::time_t end_time = std::chrono::system_clock::to_time_t(endTime);

    //std::cout << "finished computation at " << std::ctime(&end_time)
    //          << "elapsed time: " << elapsed_seconds.count() << "s\n";

//std::sort(v.begin(), v.end(), sortByW);

return 0;

    /*adj[0].push_back(2);
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
    edgesMap.insert(pair<string, Edge *>("5_7", &edge5_7));*/

    /*Edge edge0_3(0, 3);
    Edge edge1_0(1, 0);
    Edge edge1_2(1, 2);
    Edge edge2_3(2, 3);
    Edge edge3_1(3, 1);
    edges[0].push_back(&edge0_3);
    edges[0].push_back(&edge1_0);
    edges[1].push_back(&edge1_0);
    edges[1].push_back(&edge1_2);
    edges[1].push_back(&edge3_1);
    edges[2].push_back(&edge2_3);
    edges[2].push_back(&edge1_2);
    edges[3].push_back(&edge3_1);
    edges[3].push_back(&edge0_3);
    edges[3].push_back(&edge2_3);

    Edge edge0_1(0, 1);
    Edge edge1_3(1, 3);

    edgesMap.insert(pair<string, Edge *>("0_3", &edge0_3));
    edgesMap.insert(pair<string, Edge *>("0_1", &edge0_1));
    edgesMap.insert(pair<string, Edge *>("1_2", &edge1_2));
    edgesMap.insert(pair<string, Edge *>("2_3", &edge2_3));
    edgesMap.insert(pair<string, Edge *>("1_3", &edge1_3));*/


    //Edge* tmpEdge = getEdge(0, 1);

    //it = edgesMap.begin();
    /*
    edgesMap.insert(pair<string, Bridge::Edge>("0_3", edge0_3));
    edgesMap.insert(pair<string, Bridge::Edge>("1_0", edge1_0));
    edgesMap.insert(pair<string, Bridge::Edge>("1_2", edge1_2));
    edgesMap.insert(pair<string, Bridge::Edge>("2_3", edge2_3));
    edgesMap.insert(pair<string, Bridge::Edge>("3_1", edge3_1));
     */

    /*Edge *tmp = edges[2][0];
    cout << tmp->getVIndex() + "_" + tmp->getUIndex() << endl;
    cout << tmp->getWeight().to_ullong() << endl;*/

    //cout << isUsed[0] << endl;

    /*Edge edge5_6(5, 6);
    map<string, Edge *> tmpMap;
    tmpMap.insert(pair<string, Edge *>("5_6", &edge5_6));

    vector<Edge *> tmpEdges;
    tmpEdges.push_back(&edge5_6);
    Edge *tmpEdge = tmpEdges[0];
    tmpEdge->setUsed(true);

    bitset<4> tmp1(string("1110"));
    bitset<4> tmp2(string("0000"));
    //cout << (tmp1 ^ tmp2) << endl;

    long long t1 = 4;
    long long t2 = 1;
    //cout << (MAX_LONG_LONG ^ (MAX_LONG_LONG + 1)) << endl;

    //cout << random_bitset<64>( 0.25) << endl;
    bitset<64> testBitSet = random_bitset<64>(0.25);
    cout << testBitSet.to_ullong() << endl;*/
};