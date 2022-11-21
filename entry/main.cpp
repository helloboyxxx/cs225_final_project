#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <utility>
#include <vector>
#include <chrono>

#include "graph.h"
#include "structures.h"
#include "graph_generation.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

using namespace std::chrono;

int main()
{
    string airport_filename = "../data/airports.txt";
    string route_filename = "../data/routes.txt";
    Graph mygraph(airport_filename, route_filename);
    vector<string> ord_adj = mygraph.getAdjacentAirports("ORD");
    // cout << "Adjacent of ORD: " << endl;
    // for (auto airport : ord_adj) {
    //     cout << airport << " ";
    // }
    // cout << endl;

    // cout << "airport map size: ";
    // cout << mygraph.getAllAirports().size() << endl;
    
    cout << "starting from SZX: " << endl;
    auto start = high_resolution_clock::now();
    mygraph.shortestPath("SZX", "LAX");

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "shortestPaths spent: "<< duration.count() << " milliseconds" << endl;


    start = high_resolution_clock::now();
    for (unsigned i = 0; i < 1000; ++i) {
        vector<string> airports;
        mygraph.getAdjacentTest("SZX", airports);
    }
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "1000 getAdjacentTest spent: "<< duration.count() << " milliseconds" << endl;
    

    // start = high_resolution_clock::now();
    // for (unsigned i = 0; i < 100000; ++i) {
    //     mygraph.getDistance("SZX", "LAX");
    // }
    // end = high_resolution_clock::now();
    // duration = duration_cast<milliseconds>(end - start);
    // cout << "100000 getDistance spent: "<< duration.count() << " milliseconds" << endl;

    return 0;
}