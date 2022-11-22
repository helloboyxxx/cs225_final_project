#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <utility>
#include <vector>
#include <set>

#include "graph.h"
#include "structures.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
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
  Airport a("ORD", "Chicago", "United States", 41.9786, -87.9048, 672);
  Airport b("LAS", "Las Vegas", "United States", 36.08010101, -115.1520004, 2181);
  Airport c("SAN", "San Diego", "United States", 32.7336006165,-117.190002441,17);
  Airport d("MDW", "Chicago", "United States", 41.785999,-87.752403,620);
  Airport e("BOS", "Boston","United States", 42.36429977,-71.00520325,20);
  Airport f("HKG", "Hong Kong", "China", 22.308901,113.915001,28);
  Airport g("SZX", "Shenzhen", "China", 22.639299392700195,113.81099700927734,13);
  Airport h("LHR", "London", "United Kingdom", 51.4706,-0.461941,83);

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

TEST_CASE("Empty graph return proper messages") {
  Graph empty_graph;
  std::string source = "ORD";
  std::string dest = "LAS";
  
  REQUIRE( empty_graph.getAllAirports().empty() );
  REQUIRE(empty_graph.getAdjacentAirports(source).empty());
  REQUIRE(empty_graph.getDistance(source, dest) < 0);
}


TEST_CASE("Insert a single airport") {
  Graph g;
  Airport a("ORD", "Chicago", "United States", 41.9786, -87.9048, 672);
  g.insertAirport(a);
  REQUIRE(g.getAllAirports().size() == 1);
  REQUIRE(g.getAllAirports().at(0) == "ORD");
  REQUIRE(g.getAdjacentAirports("ORD").empty());
}

TEST_CASE("Insert and remove one airport") {
  Graph g;
  Airport a("ORD", "Chicago", "United States", 41.9786, -87.9048, 672);
  g.insertAirport(a);
  g.removeAirport("ORD");
  REQUIRE(g.getAllAirports().empty());
  REQUIRE(g.empty());
}

TEST_CASE("Insert two airports and one edge") {
  Graph g;
  Airport a("ORD", "Chicago", "United States", 41.9786, -87.9048, 672);
  Airport b("LAS", "Las Vegas", "United States", 36.08010101, -115.1520004, 2181);

  double distance = 1;  // formula for calculating distance is required
  REQUIRE(g.getAdjacentAirports("ORD").empty());    // Graph error since airport doesn't exist

  g.insertAirport(a);
  g.insertAirport(b);

  REQUIRE(g.insertRoute("ORD", "LAS", distance));   // Graph error since route doesn't exist

  REQUIRE(g.getAdjacentAirports("ORD").size() == 1);
  REQUIRE(g.getAdjacentAirports("ORD").at(0) == "LAS");
  REQUIRE(g.getAdjacentAirports("LAS").empty());
  REQUIRE(g.getAllAirports().size() == 2);
}

typedef std::pair<double, string> disPair;
TEST_CASE("Test Distance struct") {
  std::priority_queue<disPair, vector<disPair>, std::greater<disPair>> Q;
  
  Q.push({3, "DEN"});
  Q.push({1, "ORD"});
  Q.push({2, "LAS"});

  cout << Q.top().second << ": " << Q.top().first << endl;
  Q.pop();
  cout << Q.top().second << ": " << Q.top().first << endl;
  Q.pop();
  cout << Q.top().second << ": " << Q.top().first << endl;
  Q.pop();
}

TEST_CASE("BFS test 1") {
  Graph empty_graph;
  REQUIRE(empty_graph.BFS("ORD").empty());
}

TEST_CASE("BFS test 2") {
  Graph g;
  smallCase(g);
  std::vector<std::string> bfs_result = g.BFS("ORD");
  std::cout << "bfs_result: " << std::endl;
  for (string airport : bfs_result) {
    std::cout << airport << " ";
  }
  std::cout << std::endl;
}

TEST_CASE("Test Shortest path 1") {
  Graph g;
  smallCase(g);
  vector<string> shortest_path = {"ORD", "LAS"};
  REQUIRE(g.shortestPath("ORD", "LAS").size() == 2);
  REQUIRE(g.shortestPath("ORD", "LAS") == shortest_path);
}

TEST_CASE("Test Shortest path 2") {
  Graph g;
  smallCase(g);
  vector<string> shortest_path = {"LAS", "MDW", "ORD"};
  REQUIRE(g.shortestPath("LAS", "ORD").size() == 3);
  REQUIRE(g.shortestPath("LAS", "ORD") == shortest_path);
}

TEST_CASE("Test Shortest path 3") {
  Graph g;
  smallCase(g);
  vector<string> shortest_path = {"HKG", "BOS"};
  REQUIRE(g.shortestPath("HKG", "BOS").size() == 2);
  REQUIRE(g.shortestPath("HKG", "BOS") == shortest_path);
}

TEST_CASE("Test Shortest path 4") {
  Graph g;
  smallCase(g);
  vector<string> shortest_path = {"LAS", "SAN", "LHR", "BOS"};
  REQUIRE(g.shortestPath("LAS", "BOS").size() == 4);
  REQUIRE(g.shortestPath("LAS", "BOS") == shortest_path);
}

TEST_CASE("Test All shortest path") {
  Graph s;
  smallCase(s);
  string source = "ORD";

  vector<string> b = {source, "LAS"};
  vector<string> c = {source, "LAS", "SAN"};
  vector<string> d = {source, "LAS", "MDW"};
  vector<string> e = {source, "LAS", "SAN", "LHR", "BOS"};
  vector<string> f = {source, "HKG"};
  vector<string> g = {source, "HKG", "SZX"};
  vector<string> h = {source, "LAS", "SAN", "LHR"};

  vector<vector<string>> result = s.allShortestPath(source);
  REQUIRE(result.size() == 7);

  std::set<vector<string>> shortest_paths;
  for (vector<string> path: result) {
    shortest_paths.insert(path);
  }

  auto posB = shortest_paths.find(b);
  if (posB == shortest_paths.end()) {
    std::cout << "B's Not Present" << std::endl;
  }
  auto posC = shortest_paths.find(b);
  if (posC == shortest_paths.end()) {
    std::cout << "C's Not Present" << std::endl;
  }
  auto posD = shortest_paths.find(b);
  if (posD == shortest_paths.end()) {
    std::cout << "D's Not Present" << std::endl;
  }
  auto posE = shortest_paths.find(b);
  if (posE == shortest_paths.end()) {
    std::cout << "E's Not Present" << std::endl;
  }
  auto posF = shortest_paths.find(b);
  if (posF == shortest_paths.end()) {
    std::cout << "F's Not Present" << std::endl;
  }
  auto posG = shortest_paths.find(b);
  if (posG == shortest_paths.end()) {
    std::cout << "G's Not Present" << std::endl;
  }
  auto posH = shortest_paths.find(b);
  if (posH == shortest_paths.end()) {
    std::cout << "H's Not Present" << std::endl;
  }
}



// check data from airports.txt and routes.txt

TEST_CASE("Test Distance") {
  string airport_filename = "../data/airports.txt";
  string route_filename = "../data/routes.txt";
  Graph mygraph(airport_filename, route_filename);

  REQUIRE(mygraph.getDistance("ORD", "BOS") == 1392);
}

TEST_CASE("Test Shortest Path, all(direct)-1") {
  string airport_filename = "../data/airports.txt";
  string route_filename = "../data/routes.txt";
  Graph mygraph(airport_filename, route_filename);
  string source = "ORD";
  string dest = "JFK";

  REQUIRE(mygraph.assertAirportExists(source) == true);
  REQUIRE(mygraph.assertAirportExists(dest) == true);
  REQUIRE(mygraph.assertRouteExists(source, dest) == true);

  auto result = mygraph.shortestPath(source, dest);
  vector<string> shortest_path = {source, dest};
  REQUIRE(mygraph.shortestPath(source, dest).size() == 2);
  REQUIRE(mygraph.shortestPath(source, dest) == shortest_path);
}

TEST_CASE("Test Shortest Path, all(direct)-2") {
  string airport_filename = "../data/airports.txt";
  string route_filename = "../data/routes.txt";
  Graph mygraph(airport_filename, route_filename);
  string source = "HKG";
  string dest = "SFO";

  REQUIRE(mygraph.assertAirportExists(source) == true);
  REQUIRE(mygraph.assertAirportExists(dest) == true);
  REQUIRE(mygraph.assertRouteExists(source, dest) == true);

  auto result = mygraph.shortestPath(source, dest);
  vector<string> shortest_path = {source, dest};
  REQUIRE(mygraph.shortestPath(source, dest).size() == 2);
  REQUIRE(mygraph.shortestPath(source, dest) == shortest_path);
}

TEST_CASE("Test Shortest Path, all(indirect)-1") {
  string airport_filename = "../data/airports.txt";
  string route_filename = "../data/routes.txt";
  Graph mygraph(airport_filename, route_filename);
  string source = "STR";
  string dest = "YGW";

  REQUIRE(mygraph.assertAirportExists(source) == true);
  REQUIRE(mygraph.assertAirportExists(dest) == true);
  REQUIRE(mygraph.assertRouteExists(source, dest) == false);

  auto result = mygraph.shortestPath(source, dest);
  for (auto a: result) {
    cout << a << " ";
  }
}

TEST_CASE("Test Shortest Path, all(indirect)-2") {
  string airport_filename = "../data/airports.txt";
  string route_filename = "../data/routes.txt";
  Graph mygraph(airport_filename, route_filename);
  string source = "ORD";
  string dest = "ATH";

  REQUIRE(mygraph.assertAirportExists(source) == true);
  REQUIRE(mygraph.assertAirportExists(dest) == true);
  REQUIRE(mygraph.assertRouteExists(source, dest) == false);

  auto result = mygraph.shortestPath(source, dest);
  for (auto a: result) {
    cout << a << " ";
  }
}

TEST_CASE("Test All Shortest Path, all") {
  string airport_filename = "../data/airports.txt";
  string route_filename = "../data/routes.txt";
  Graph mygraph(airport_filename, route_filename);
  string source = "ORD";

  vector<vector<string>> result = mygraph.allShortestPath(source);
  std::set<vector<string>> shortest_paths;
  for (vector<string> path: result) {
    shortest_paths.insert(path);
  }

  vector<string> a = {source, "JFK"};
  auto posA = shortest_paths.find(a);
  if (posA == shortest_paths.end()) {
    std::cout << "ORD-JFK Not Present" << std::endl;
  }

  vector<string> b = {source, "LHR"};
  auto posB = shortest_paths.find(b);
  if (posB == shortest_paths.end()) {
    std::cout << "ORD-LHR Not Present" << std::endl;
  }

  vector<string> c = {source, "NRT", "CEB", "ZAM"};
  auto posC = shortest_paths.find(c);
  if (posC == shortest_paths.end()) {
    std::cout << "ORD-ZAM Not Present" << std::endl;
  }

  vector<string> d = {source, "PEK", "SZX"};
  auto posD = shortest_paths.find(d);
  if (posD == shortest_paths.end()) {
    std::cout << "ORD-SZX Not Present" << std::endl;
  }

  vector<string> e = {source, "ZRH", "ATH"};
  auto posE = shortest_paths.find(e);
  if (posE == shortest_paths.end()) {
    std::cout << "ORD-ATH Not Present" << std::endl;
  }
}

TEST_CASE("Test All Shortest Path size") {
  string airport_filename = "../data/airports.txt";
  string route_filename = "../data/routes.txt";
  Graph mygraph(airport_filename, route_filename);
  string source = "YYZ";
  unsigned short_length = mygraph.allShortestPath(source).size();
  unsigned bfs_length = mygraph.BFS(source).size();
  cout << "All airports num: " << mygraph.getAllAirports().size() << endl;
  REQUIRE(short_length == bfs_length);
}
