#include "../include/Edge.h"
#include "../include/Sorts.h"

bool radix_sort::operator()(Edge* edge) const // function call operator
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
        //TODO: исправить ошибку в подсчете индексов корзин (мб она пофиксилась правильной работой std сортировки???)
        int bi = (n - 1) * edge->getW(); // Index in bucket
        if (maxValue != 0) {
            bi = bi / maxValue;
        }
        if (bi < 0) {
            bi = 0;
        }
        b[bi].push_back(arr[i]);
    }

    // 3) Sort individual buckets
    for (int i = 0; i < n; i++)
        sort(b[i].begin(), b[i].end(), sortByW);

    // 4) Concatenate all buckets into arr[]
    int index = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < b[i].size(); j++)
            arr[index++] = b[i][j];
}
