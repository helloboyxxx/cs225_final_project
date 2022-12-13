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
#include "help_test.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
// Test betweenness Centralities

TEST_CASE("three undirected", "[part2]") {
  Graph g;
  Airport a(3830, "ORD", "Chicago", "United States", 41.9786, -87.9048);
  Airport b(3877, "LAS", "Las Vegas", "United States", 36.08010101, -115.1520004);
  Airport c(3731, "SAN", "San Diego", "United States", 32.7336006165,-117.190002441);

  g.insertAirport(a);
  g.insertAirport(b);
  g.insertAirport(c);

  // a - b
  g.insertRoute("ORD", "LAS", 1);
  g.insertRoute("LAS", "ORD", 1);

  // b - c
  g.insertRoute("LAS", "SAN", 1);
  g.insertRoute("SAN", "LAS", 1);

  g.clearFreqFile();
  g.calcFrequency();
  REQUIRE(g.getFrequency("ORD") == 0);
  REQUIRE(g.getFrequency("LAS") == 2);
  REQUIRE(g.getFrequency("SAN") == 0);
}

TEST_CASE("four undirected", "[part2]") {
  Graph g;
  Airport a(1111, "A", "Chicago", "United States", 41.9786, -87.9048);
  Airport b(2222, "B", "Las Vegas", "United States", 36.08010101, -115.1520004);
  Airport c(3333, "C", "San Diego", "United States", 32.7336006165,-117.190002441);
  Airport d(4444, "D", "Las Vegas", "United States", 36.08010101, -115.1520004);

  g.insertAirport(a);
  g.insertAirport(b);
  g.insertAirport(c);
  g.insertAirport(d);

  // a - b
  g.insertRoute("A", "B", 1);
  g.insertRoute("B", "A", 1);

  // b - c
  g.insertRoute("B", "C", 1);
  g.insertRoute("C", "B", 1);
  
  // b - d
  g.insertRoute("B", "D", 1);
  g.insertRoute("D", "B", 1);
  
  g.clearFreqFile();
  g.calcFrequency();
  REQUIRE(g.getFrequency("A") == 0);
  REQUIRE(g.getFrequency("B") == 6);
  REQUIRE(g.getFrequency("C") == 0);
  REQUIRE(g.getFrequency("D") == 0);
}

TEST_CASE("five directed", "[part2]") {
  Graph g;
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


  g.calcFrequency();

  REQUIRE(g.getFrequency("A") == 0);
  REQUIRE(g.getFrequency("B") == 1);
  REQUIRE(g.getFrequency("C") == 0);
  REQUIRE(g.getFrequency("D") == 0);
  REQUIRE(g.getFrequency("E") == 2);
}


TEST_CASE("five undirected", "[part2]") {
  Graph g;
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

  // a - b
  g.insertRoute("A", "B", 1);
  g.insertRoute("B", "A", 1);

  // b - c
  g.insertRoute("B", "C", 1);
  g.insertRoute("C", "B", 1);
  
  // c - d
  g.insertRoute("C", "D", 1);
  g.insertRoute("D", "C", 1);

  // d - e
  g.insertRoute("D", "E", 1);
  g.insertRoute("E", "D", 1);

  // e - a
  g.insertRoute("E", "A", 1);
  g.insertRoute("A", "E", 1);

  g.clearFreqFile();
  g.calcFrequency();
  REQUIRE(g.getFrequency("A") == 2);
  REQUIRE(g.getFrequency("B") == 2);
  REQUIRE(g.getFrequency("C") == 2);
  REQUIRE(g.getFrequency("D") == 2);
  REQUIRE(g.getFrequency("E") == 2);
}



TEST_CASE("small case directed", "[part2]") {
  Graph g;
  smallCase(g);
  g.clearFreqFile();
  g.calcFrequency();
  REQUIRE(g.getFrequency("ORD") == 7);
  REQUIRE(g.getFrequency("LAS") == 5);
  REQUIRE(g.getFrequency("SAN") == 9);
  REQUIRE(g.getFrequency("MDW") == 2);
  REQUIRE(g.getFrequency("BOS") == 5);
  REQUIRE(g.getFrequency("HKG") == 4);
  REQUIRE(g.getFrequency("SZX") == 0);
  REQUIRE(g.getFrequency("LHR") == 5);
}
