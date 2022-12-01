#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <utility>
#include <vector>
#include <chrono>
#include <unordered_set>

#include "graph.h"
#include "structures.h"
#include "graph_generation.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

using namespace std::chrono;

void print_vector(vector<string> v) {
    for (auto& a : v) {
        cout << a << " ";
    }
    cout << endl;
}

int main()
{
    string airport_filename = "../data/airports.txt";
    string route_filename = "../data/routes.txt";
    Graph mygraph(airport_filename, route_filename);
    string source = "ORD";
    string dest = "SZX";

    auto start = high_resolution_clock::now();

    mygraph.calcFrequency();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Centrality spent: "<< duration.count() << " milliseconds" << endl;


    

    return 0;
}