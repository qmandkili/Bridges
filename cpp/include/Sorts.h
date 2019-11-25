#pragma once
#include "Edge.h"
#include <vector>
#include <algorithm>

// Radix sort comparator for 32-bit two's complement integers
class radix_sort {
    const int bit; // bit position [0..31] to examine
public:
    radix_sort(int offset) : bit(offset) {} // constructor
    bool operator()(Edge *edge) const; // function call operator
};

void bucketSort(vector<Edge *> &arr, int n, long long maxValue);

static bool sortByW(Edge *A, Edge *B) {
    return (A->getW() < B->getW());
}


// Least significant digit radix sort
void lsd_radix_sort(vector<Edge *> &edges);