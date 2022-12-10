#include "help_test.h"

void smallCase(Graph& graph) {
  // 3830,"Chicago O'Hare International Airport","Chicago","United States","ORD","KORD",41.9786,-87.9048,672,-6,"A","America/Chicago","airport","OurAirports"
  // 3877,"McCarran International Airport","Las Vegas","United States","LAS","KLAS",36.08010101,-115.1520004,2181,-8,"A","America/Los_Angeles","airport","OurAirports"
  // 3731,"San Diego International Airport","San Diego","United States","SAN","KSAN",32.7336006165,-117.190002441,17,-8,"A","America/Los_Angeles","airport","OurAirports"
  // 3747,"Chicago Midway International Airport","Chicago","United States","MDW","KMDW",41.785999,-87.752403,620,-6,"A","America/Chicago","airport","OurAirports"]
  // 3448,"General Edward Lawrence Logan International Airport","Boston","United States","BOS","KBOS",42.36429977,-71.00520325,20,-5,"A","America/New_York","airport","OurAirports"
  // 3077,"Hong Kong International Airport","Hong Kong","Hong Kong","HKG","VHHH",22.308901,113.915001,28,8,"U","Asia/Hong_Kong","airport","OurAirports"
  // 3374,"Shenzhen Bao'an International Airport","Shenzhen","China","SZX","ZGSZ",22.639299392700195,113.81099700927734,13,8,"U","Asia/Shanghai","airport","OurAirports"
  // 507,"London Heathrow Airport","London","United Kingdom","LHR","EGLL",51.4706,-0.461941,83,0,"E","Europe/London","airport","OurAirports"
  Airport a(3830, "ORD", "Chicago", "United States", 41.9786, -87.9048);
  Airport b(3877, "LAS", "Las Vegas", "United States", 36.08010101, -115.1520004);
  Airport c(3731, "SAN", "San Diego", "United States", 32.7336006165,-117.190002441);
  Airport d(3747, "MDW", "Chicago", "United States", 41.785999,-87.752403);
  Airport e(3448, "BOS", "Boston","United States", 42.36429977,-71.00520325);
  Airport f(3077, "HKG", "Hong Kong", "China", 22.308901,113.915001);
  Airport g(3374, "SZX", "Shenzhen", "China", 22.639299392700195,113.81099700927734);
  Airport h(507, "LHR", "London", "United Kingdom", 51.4706,-0.461941);

  vector<Airport> airports = {a, b, c, d, e, f, g, h};
  for (Airport& airport : airports) {
    graph.insertAirport(airport);
  }

  // Here I am using the graph in Dijkstra slides
  graph.insertRoute("ORD", "LAS", 10);  // a->b
  graph.insertRoute("ORD", "HKG", 7);   // a->f
  graph.insertRoute("LAS", "SAN", 7);   // b->c
  graph.insertRoute("LAS", "MDW", 5);   // b->d
  graph.insertRoute("MDW", "ORD", 3);   // d->a
  graph.insertRoute("SAN", "LHR", 4);   // c->h
  graph.insertRoute("LHR", "BOS", 3);   // h->e
  graph.insertRoute("LHR", "SZX", 5);   // h->g
  graph.insertRoute("BOS", "SAN", 6);   // e->c
  graph.insertRoute("HKG", "BOS", 5);   // f->e
  graph.insertRoute("HKG", "SZX", 4);   // f->g
  graph.insertRoute("SZX", "BOS", 2);   // g->e
}