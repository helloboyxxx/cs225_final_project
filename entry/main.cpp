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

// check data/smallCase.png for understanding this graph
void smallCase(Graph& graph) {
  // 3830,"Chicago O'Hare International Airport","Chicago","United States","ORD","KORD",41.9786,-87.9048,672,-6,"A","America/Chicago","airport","OurAirports"
  // 3877,"McCarran International Airport","Las Vegas","United States","LAS","KLAS",36.08010101,-115.1520004,2181,-8,"A","America/Los_Angeles","airport","OurAirports"
  // 3731,"San Diego International Airport","San Diego","United States","SAN","KSAN",32.7336006165,-117.190002441,17,-8,"A","America/Los_Angeles","airport","OurAirports"
  // 3747,"Chicago Midway International Airport","Chicago","United States","MDW","KMDW",41.785999,-87.752403,620,-6,"A","America/Chicago","airport","OurAirports"]
  // 3448,"General Edward Lawrence Logan International Airport","Boston","United States","BOS","KBOS",42.36429977,-71.00520325,20,-5,"A","America/New_York","airport","OurAirports"
  // 3077,"Hong Kong International Airport","Hong Kong","Hong Kong","HKG","VHHH",22.308901,113.915001,28,8,"U","Asia/Hong_Kong","airport","OurAirports"
  // 3374,"Shenzhen Bao'an International Airport","Shenzhen","China","SZX","ZGSZ",22.639299392700195,113.81099700927734,13,8,"U","Asia/Shanghai","airport","OurAirports"
  // 507,"London Heathrow Airport","London","United Kingdom","LHR","EGLL",51.4706,-0.461941,83,0,"E","Europe/London","airport","OurAirports"
  Airport a(3830, "A", "Chicago", "United States", 41.9786, -87.9048);
  Airport b(3877, "B", "Las Vegas", "United States", 36.08010101, -115.1520004);
  Airport c(3731, "C", "San Diego", "United States", 32.7336006165,-117.190002441);
  Airport d(3747, "D", "Chicago", "United States", 41.785999,-87.752403);
  Airport e(3448, "E", "Boston","United States", 42.36429977,-71.00520325);
  Airport f(3077, "F", "Hong Kong", "China", 22.308901,113.915001);
  Airport g(3374, "G", "Shenzhen", "China", 22.639299392700195,113.81099700927734);
  Airport h(507, "H", "London", "United Kingdom", 51.4706,-0.461941);

  vector<Airport> airports = {a, b, c, d, e, f, g, h};
  for (Airport& airport : airports) {
    graph.insertAirport(airport);
  }

  // Here I am using the graph in Dijkstra slides
  graph.insertRoute("A", "B", 10);  // a->b
  graph.insertRoute("A", "F", 7);   // a->f
  graph.insertRoute("B", "C", 7);   // b->c
  graph.insertRoute("B", "D", 5);   // b->d
  graph.insertRoute("D", "A", 3);   // d->a
  graph.insertRoute("C", "H", 4);   // c->h
  graph.insertRoute("H", "E", 3);   // h->e
  graph.insertRoute("H", "G", 5);   // h->g
  graph.insertRoute("E", "C", 6);   // e->c
  graph.insertRoute("F", "E", 5);   // f->e
  graph.insertRoute("F", "G", 4);   // f->g
  graph.insertRoute("G", "E", 2);   // g->e
}

void fiveUndirected(Graph& g) {
  Airport a(111, "A", "Chicago", "United States", 41.9786, -87.9048);
  Airport b(222, "B", "Las Vegas", "United States", 36.08010101, -115.1520004);
  Airport c(333, "C", "San Diego", "United States", 32.7336006165,-117.190002441);
  Airport d(444, "D", "Las Vegas", "United States", 36.08010101, -115.1520004);
  Airport e(555, "E", "San Diego", "United States", 32.7336006165,-117.190002441);

  g.insertAirport(a);
  g.insertAirport(b);
  g.insertAirport(c);
  g.insertAirport(d);
  g.insertAirport(e);


  g.insertRoute("A", "B", 10);
  g.insertRoute("A", "E", 10);
  g.insertRoute("B", "E", 10);
  g.insertRoute("B", "C", 11);
  g.insertRoute("C", "D", 10);
  g.insertRoute("E", "D", 10);
}

int main()
{
    // string airport_filename = "../data/airports.txt";
    // string route_filename = "../data/routes.txt";
    // Graph mygraph(airport_filename, route_filename);
    // string source = "ORD";
    // string dest = "SZX";

    // auto start = high_resolution_clock::now();
    // auto end = high_resolution_clock::now();
    // auto duration = duration_cast<milliseconds>(end - start);
    // cout << "Centrality spent: "<< duration.count() << " milliseconds" << endl;


    Graph g;
    // fiveUndirected(g);
    smallCase(g);
    g.calcFrequency();
    // for (const auto& airport : g.getAllAirports()) {
    //     cout << "STARTING FROM: " << airport << endl;
    //     for (const auto& path : g.allShortestPath(airport)) {
    //         print_vector(path);
    //     }
    //     cout << endl;
    // }

    



    

    return 0;
}