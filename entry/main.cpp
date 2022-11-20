#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

#include "graph.h"
#include "structures.h"
#include "graph_generation.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

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

    cout << "From ORD to KEF: " << endl;
    auto result = mygraph.shortestPath("ORD", "KEF");
    for (auto path_airport : result) {
        cout << path_airport << "";
    }
    cout << endl;
    return 0;
}