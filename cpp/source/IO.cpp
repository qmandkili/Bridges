#include "../include/IO.h"

void writeOutputFile(double detDfsTime, double dfsTime, double stdSortTime, double bucketSortTime, int n, int prob) {
    ofstream myfile;
    myfile.open("result_" + to_string(prob) + ".csv", std::ios_base::app);
    myfile << detDfsTime << ";" <<
           dfsTime << ";" <<
           stdSortTime << ";" <<
           bucketSortTime << ";" <<
           //radixSortTime << ";" <<
           n << "\n";
    myfile.close();
}

void writeEdgesToFile(int n, int prob, map<string, Edge *> &edgesMap) {
    ofstream myfile;
    myfile.open("edges_" + to_string(n) + "_" + to_string(prob) + ".csv");
    for (auto & it : edgesMap) {
        myfile << it.second->getVIndex() << ";" << it.second->getUIndex() << "\n";
    }
    myfile.close();
}

void writeBasicEdgesToFile(int n, int prob, map<string, Edge *> &edgesMap) {
    ofstream myfile;
    myfile.open("basic_edges_" + to_string(n) + "_" + to_string(prob) + ".csv");
    for (auto & it : edgesMap) {
        if (it.second->isBasic()) {
            myfile << it.second->getVIndex() << ";" << it.second->getUIndex() << "\n";
        }
    }
    myfile.close();
}

void writeTwoEdgesToFile(int n, int prob, vector<Edge *> vec) {
    ofstream myfile;
    myfile.open ("two_bridges_" + to_string(n) + "_" + to_string(prob) + ".csv");
    for (auto & it : vec) {
        myfile << it->getVIndex() << ";" << it->getUIndex() << "\n";
    }
    myfile.close();
}

void writeEdgesToFile2(int n, int prob, vector<Edge*> vec) {
    ofstream myfile;
    myfile.open("one_bridges_" + to_string(n) + "_" + to_string(prob) + ".csv");
    for (auto & it : vec) {
        myfile << it->getVIndex() << ";" << it->getUIndex() << "\n";
    }
    myfile.close();
}
