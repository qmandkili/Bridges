#include "../include/Edge.h"
#include "../include/Sorts.h"

bool radix_sort::operator()(Edge *edge) const // function call operator
{
    if (bit == 63) // sign bit
        return edge->getW() < 0; // negative int to left partition
    else
        return !(edge->getW() & (1 << bit)); // 0 bit to left partition
}

void bucketSort(vector<Edge *> &arr, int n, long long maxValue) {
    // 1) Create n empty buckets
    vector<Edge *> b[n];

    // 2) Put array elements in different buckets
    for (int i = 0; i < arr.size(); i++) {
        Edge *edge = arr[i];
        double edgeW = edge->getW() / (double) maxValue;
        // if edgeW is NaN
        if (edgeW != edgeW) {
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

void lsd_radix_sort(vector<Edge *> edges) {
    for (int lsb = 0; lsb < 64; ++lsb) // least-significant-bit
    {
        std::stable_partition(edges.begin(), edges.end(), radix_sort(lsb));
    }
}
