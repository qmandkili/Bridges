#include "../include/Edge.h"
#include "../include/Sorts.h"


void bucketSort(vector<Edge *> &arr, int n, long long maxValue) {
    // 1) Create n empty buckets
    vector<Edge *> b[n];

    // 2) Put array elements in different buckets
    for (int i = 0; i < arr.size(); i++) {
        Edge *edge = arr[i];
        double edgeW = edge->getW() / (double)maxValue;
        // if edgeW is NaN
        if (edgeW != edgeW){
            edgeW = 0;
        }
        int bi = (n - 1) * edgeW; // Index in bucket
        b[bi].push_back(arr[i]);
    }

    // 3) Sort individual buckets
    for (int i = 0; i < n; i++)
        std::sort(b[i].begin(), b[i].end(), sortByW);

    // 4) Concatenate all buckets into arr[]
    int index = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < b[i].size(); j++)
            arr[index++] = b[i][j];
}

void radixSort(vector<Edge *> &arr) {
    vector<Edge *> helper(arr.size());

    int b = 64;
    int r = 8;

    vector<int> count(1 << r);
    vector<int> pref(1 << r);

    int groups = ceil(b / r);

    int mask = (1 << r) - 1;

    for (int c = 0, shift = 0;  c < groups; c++, shift += r) {

        fill(count.begin(), count.end(), 0);

        for (int i = 0; i < arr.size(); i++) {
            count[(arr[i]->getW() >> shift) & mask]++;
        }

        pref[0] = 0;
        for (int i = 1; i < count.size(); i++) {
            pref[i] = pref[i - 1] + count[i -1];
        }

        for (int i = 0; i < arr.size(); i++) {
            helper[pref[(arr[i]->getW() >> shift)&mask]++] = arr[i];
        }

        for (int i = 0; i < arr.size(); i++) {
            arr[i] = helper[i];
        }
    }
}
