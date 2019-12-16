#pragma once
#include "Edge.h"
#include <vector>
#include <algorithm>
#include <cmath>

void bucketSort(vector<Edge *> &arr, int n, long long maxValue);

static bool sortByW(Edge *A, Edge *B) {
    return (A->getW() < B->getW());
}

void radixSort(vector<Edge *> &arr);