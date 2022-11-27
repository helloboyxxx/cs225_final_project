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
    string dest = "SXX";


    // Find unconnected component
    std::unordered_set<string> visited;
    for (string& airport : mygraph.BFS(source)) {
        visited.emplace(airport);
    }

    std::vector<string> invalid_airports;

    for (string& airport : mygraph.getAllAirports()) {
        if (visited.find(airport) == visited.end()) {
            cout << "Invalid airport: " << airport << endl;
            invalid_airports.push_back(airport);
        }
    }

    // Check invalid airport to ORD
    for (const string& airport : invalid_airports) {
        if (mygraph.shortestPath(airport, "ORD").size() != 1) {
            cout << airport <<" to ORD: ";
            for (string& airport : mygraph.shortestPath(airport, "ORD")) {
                cout << airport << " ";
            }
            cout << endl;
        }
    }

    // cout << "ORD to UVE" << endl; 
    // for (string& airport : mygraph.shortestPath(source, dest)) {
    //     cout << airport << " ";
    // }
    // cout << endl;


    unsigned short_length = mygraph.allShortestPath("UVE").size();
    unsigned bfs_length = mygraph.BFS("UVE").size();
    cout << "All airports num: " << mygraph.getAllAirports().size() << endl;
    cout << "short_length: " << short_length << endl;
    cout << "bfs_length: " << bfs_length << endl;

    // cout << "UVE to ORD: ";
    // for (string& airport : mygraph.shortestPath("UVE", "ORD")) {
    //     cout << airport << " ";
    // }
    // cout << endl;

    // for (const string& airport : mygraph.BFS("UVE")) {
    //     cout << airport << " ";
    // }
    // cout << endl;

    // for (const string& airport : mygraph.BFS("MPA")) {
    //     cout << airport << " ";
    // }
    // cout << endl;


    // cout << "starting from SZX: " << endl;
    // auto start = high_resolution_clock::now();
    // for (unsigned i = 0; i < 100; ++i) {
    //     auto result = mygraph.shortestPath("SZX", "LAX");
    // }
    // // for (auto path_airport : result) {
    // //     cout << path_airport << " ";
    // // }
    // cout << endl;
    // auto end = high_resolution_clock::now();
    // auto duration = duration_cast<milliseconds>(end - start);
    // cout << "shortestPaths spent: "<< duration.count() << " milliseconds" << endl;

    return 0;
}