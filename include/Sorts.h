#pragma once
#include "Edge.h"
#include <vector>

// Radix sort comparator for 32-bit two's complement integers
class radix_sort {
    const int bit; // bit position [0..31] to examine
public:
    radix_sort(int offset) : bit(offset) {} // constructor
    bool operator()(Edge *edge) const; // function call operator

    /*bool operator()(const pair<int,int>& p) {
        return p.first < _median;
    }*/
};

void bucketSort(vector<Edge *> &arr, int n, long long maxValue);

static bool sortByW(Edge *A, Edge *B) {
    return (A->getW() < B->getW());
}


// Least significant digit radix sort
//void lsd_radix_sort(vector<Edge *> edges) {
//    for (int lsb = 0; lsb < 64; ++lsb) // least-significant-bit
//    {
//        std::stable_partition(edges.begin(), edges.end(), radix_sort(lsb));
//    }
//}

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

// Function to sort arr[] of size n using bucket sort