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

    mygraph.RoundTrip("ORD");

    // auto start = high_resolution_clock::now();


    // mygraph.calcFrequency();

    // auto end = high_resolution_clock::now();
    // auto duration = duration_cast<milliseconds>(end - start);
    // cout << "Centrality spent: "<< duration.count() << " milliseconds" << endl;

    // Airport a(3830, "ORD", "Chicago", "United States", 41.9786, -87.9048);
    // Airport b(3877, "LAS", "Las Vegas", "United States", 36.08010101, -115.1520004);
    // Airport c(3731, "SAN", "San Diego", "United States", 32.7336006165,-117.190002441);
    // Airport d(3747, "MDW", "Chicago", "United States", 41.785999,-87.752403);
    // Airport e(3448, "BOS", "Boston","United States", 42.36429977,-71.00520325);
    // Airport f(3077, "HKG", "Hong Kong", "China", 22.308901,113.915001);
    // Airport g(3374, "SZX", "Shenzhen", "China", 22.639299392700195,113.81099700927734);
    // Airport h(507, "LHR", "London", "United Kingdom", 51.4706,-0.461941);
    
    // Graph graph;
    
    // vector<Airport> airports = {a, b, c, d, e, f, g, h};
    // for (Airport& airport : airports) {
    //     graph.insertAirport(airport);
    // }

    // graph.insertRoute("ORD", "LAS", 10);
    // graph.insertRoute("LAS", "ORD", 10);

    // graph.insertRoute("LAS", "SAN", 20);
    // graph.insertRoute("SAN", "LAS", 20);
    
    // graph.insertRoute("BOS", "SAN", 20);
    // graph.insertRoute("SAN", "BOS", 20);

    // graph.insertRoute("HKG", "SAN", 20);
    // graph.insertRoute("SAN", "HKG", 20);

    // graph.insertRoute("LHR", "LAS", 100);

    // graph.insertRoute("LHR", "SZX", 100);
    // graph.insertRoute("SZX", "LHR", 100);

    
    // graph.RoundTrip("ORD");

    return 0;
}