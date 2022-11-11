#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "structures.h"


class Graph {
  public:

    // Default constructor that creates an empty graph
    Graph();

    // Given two file names, this constructor calls graph_generation to help generate the airport_map
    Graph(std::string airport_filename, std::string route_filename);


    /**
    * Return the adjacent airports of this airport
    * The function takes in the IATA of an airport 
    * and returns a vector of its adjacent airports' IATAs.
    * If the given IATA is not in the graph, we return an empty vector.
    * Assume that this is a connected graph.
    */
    std::vector<std::string> getAdjacentAirports(std::string IATA) const;

    /*
    Return all the airports in this graph as a vector of their IATAs.
    */
    std::vector<std::string> getAllAiports() const;
    

    /*
    Given source and destination airports, if the two airports are directly connected, return the distance
    Otherwise, return -1.
    @param source, the IATA of the source airport
    @param dest, the IATA of the destination airport
    @return the distance between two airports
    */
    double getDistance(std::string source, std::string dest) const;





  private:
  /*
  key: airport's IATA, value: its corresponding airport struct
  */
    std::unordered_map<std::string, airport> airport_map;


};