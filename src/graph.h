#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "structures.h"

using std::string;
using std::vector;

class Graph {
  public:

    // Default constructor that creates an empty graph
    Graph();

    // Given two file names, this constructor calls graph_generation to help generate the airport_map
    Graph(string airport_filename, string route_filename);


    /**
    * Return the adjacent airports of this airport
    * The function takes in the IATA of an airport 
    * and returns a vector of its adjacent airports' IATAs.
    * If the given IATA is not in the graph, we return an empty vector.
    * Assume that this is a connected graph.
    */
    vector<string> getAdjacentAirports(string& IATA) const;

    /*
    Return all the airports in this graph as a vector of their IATAs.
    */
    vector<string> getAllAiports() const;
    

    /**
    Given source and destination airports, if the two airports are directly connected, return the distance
    Otherwise, return -1.
    @param source, the IATA of the source airport
    @param dest, the IATA of the destination airport
    @returns the distance between two airports.
    */
    double getDistance(string& source, string& dest) const;



  private:
    /*
    key: airport's IATA, value: its corresponding airport struct
    */
    std::unordered_map<string, Airport> airport_map;

    /**
     * Returns whether thee route from source to dest exists in the graph
     * @param source - one airport
     * @param dest - another airport
     * @param functionName - the name of the calling function to return
     */
    bool assertRouteExists(string& source, string& dest, string functionName) const;


    /**
     * Returns whether a given airport exists in the graph
     * @param IATA - IATA of an airport
     * @param functionName - the name of the calling function to return
     */
    bool assertAirportExists(string& IATA, string functionName) const;

};