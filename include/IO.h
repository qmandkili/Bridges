#pragma once
#include <fstream>
#include <map>
#include "Edge.h"

using namespace std;

void writeOutputFile(double detDfsTime, double dfsTime, double stdSortTime, double bucketSortTime, int n, int all_n,
                     int prob);

void writeEdgesToFile(int n, map<string, Edge *> &edgesMap);
void writeBasicEdgesToFile(int n, map<string, Edge *> &edgesMap);
