#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>

#include "graph.h"


TEST_CASE("Empty graph return proper messages") {
  Graph empty_graph;
  std::string source = "ORD";
  std::string dest = "LAS";
  
  REQUIRE( empty_graph.getAllAiports().empty() );
  REQUIRE(empty_graph.getAdjacentAirports(source).empty());
  REQUIRE(empty_graph.getDistance(source, dest) < 0);
}