#pragma once
#include <fstream>
#include <map>
#include "Edge.h"

using namespace std;

void writeOutputFile(double detDfsTime, double dfsTime, double stdSortTime, double bucketSortTime, double radixSortTime, int n, int prob);

void writeEdgesToFile(int n, int prob, map<string, Edge *> &edgesMap);

void writeBasicEdgesToFile(int n, int prob, map<string, Edge *> &edgesMap);

void writeTwoEdgesToFile(int n, int prob, vector<Edge *> vec);

void writeEdgesToFile2(int n, int prob, vector<Edge*> vec);