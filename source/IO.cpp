#include "../include/IO.h"

void writeOutputFile(double detDfsTime, double dfsTime, double stdSortTime, double bucketSortTime, int n, int all_n,
                     int prob) {
    ofstream myfile;
    myfile.open("result" + to_string(all_n) + "_" + to_string(prob) + ".csv", std::ios_base::app);
    myfile << detDfsTime << ";" <<
           dfsTime << ";" <<
           stdSortTime << ";" <<
           bucketSortTime << ";" <<
           //radixSortTime << ";" <<
           n << "\n";
    myfile.close();
}

void writeEdgesToFile(int n, map<string, Edge *> &edgesMap) {
    ofstream myfile;
    myfile.open("edges" + to_string(n) + ".csv");
    for (auto & it : edgesMap) {
        myfile << it.second->getVIndex() << ";" << it.second->getUIndex() << "\n";
    }
    myfile.close();
}

void writeBasicEdgesToFile(int n, map<string, Edge *> &edgesMap) {
    ofstream myfile;
    myfile.open("basic_edges" + to_string(n) + ".csv");
    for (auto & it : edgesMap) {
        if (it.second->isBasic()) {
            myfile << it.second->getVIndex() << ";" << it.second->getUIndex() << "\n";
        }
    }
    myfile.close();
}
