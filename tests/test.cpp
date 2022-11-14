#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

#include "graph.h"
#include "structures.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

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
  // 3830,"Chicago O'Hare International Airport","Chicago","United States","ORD","KORD",41.9786,-87.9048,672,-6,"A","America/Chicago","airport","OurAirports"
  Airport a("ORD", "Chicago", "United States", 41.9786, -87.9048, 672);
  g.insertAirport(a);
  REQUIRE(g.getAllAirports().size() == 1);
  REQUIRE(g.getAllAirports().at(0) == "ORD");
  REQUIRE(g.getAdjacentAirports("ORD").empty());
}

TEST_CASE("Insert and remove one airport") {
  Graph g;
  // 3830,"Chicago O'Hare International Airport","Chicago","United States","ORD","KORD",41.9786,-87.9048,672,-6,"A","America/Chicago","airport","OurAirports"
  Airport a("ORD", "Chicago", "United States", 41.9786, -87.9048, 672);
  g.insertAirport(a);
  g.removeAirport("ORD");
  REQUIRE(g.getAllAirports().empty());
  REQUIRE(g.empty());
}

TEST_CASE("Insert two airports and one edge") {
  Graph g;
  // 3830,"Chicago O'Hare International Airport","Chicago","United States","ORD","KORD",41.9786,-87.9048,672,-6,"A","America/Chicago","airport","OurAirports"
  // 3877,"McCarran International Airport","Las Vegas","United States","LAS","KLAS",36.08010101,-115.1520004,2181,-8,"A","America/Los_Angeles","airport","OurAirports"
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