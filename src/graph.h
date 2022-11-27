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
    Graph() = default;

    // Given two file names, this constructor calls graph_generation to help generate the airport_map
    Graph(string airport_filename, string route_filename);


    /**
    * Return the adjacent airports of this airport
    * The function takes in the IATA of an airport 
    * and returns a vector of its adjacent airports' IATAs.
    * If the given IATA is not in the graph, we return an empty vector.
    * Assume that this is a connected graph.
    */
    vector<string> getAdjacentAirports(string IATA) const;

    // const vector<string> & getAdjacentTest(string IATA) const;
    void getAdjacentTest(string IATA, vector<string> airports ) const;

    /*
    Return all the airports in this graph as a vector of their IATAs.
    */
    vector<string> getAllAirports() const;
    

    /**
    Given source and destination airports, if the two airports are directly connected, return the distance
    Otherwise, return -1.
    @param source, the IATA of the source airport
    @param dest, the IATA of the destination airport
    @returns the distance between two airports.
    */
    double getDistance(string source, string dest) const;


    /**
    Inserts a new airport into the graph.
    This function will overwrite if old stuff was there
    */
    void insertAirport(Airport a);

    /**
    Removes a given airport from the graph.
    This function should also remove all the routes that are 
    connected to this airport.
    */
    void removeAirport(string IATA);


    /**
     * Inserts an route between two airports.
     * Hence, an error is not thrown when it fails to insert an route.
     * In this function we assume that both source and dest exists. 
     * We will assert this before proceeding the rest of our program. 
     * @param source - source airport IATA 
     * @param dest - destination airport IATA
     * @return whether inserting the route was successful. Return false
     * if the route already exist
     */
    bool insertRoute(string source, string dest, double distance);


    /**
     * Removes the route between two airports
     * @param source - source airport IATA
     * @param dest - destination airport IATA
     */
    void removeRoute(string source, string dest);


    // Return true if this grpah is empty. 
    bool empty() const { return airport_map.empty(); }

    /**
    Given the source and destination airports' IATA, this function returns a
    vector of strings that represents all the airports in that path. We will
    include both the source airport and the dest in this path.
    
    Function will return an empty vector if either of source or dest is not 
    included in graph or if dest is the same as source.

    @param source source airport IATA
    @param dest destination airport IATA
    @returns a vector of IATA that represents the path
    */
    vector<string> shortestPath(string source, string dest) const;

    /**
    Given the source airport, this function will use Dijkstra's algorithm to
    calculate the shortest paths from source to all other airports. This is a
    helper function for betweenness centrality algorithms.
    Paths in the returned vector has no priority. Each path contains at least
    two elements.
    The function will return an empty vector if the given source airport 
    is not inlucded in the graph.
    @param source source airport IATA
    @returns a vector of paths. We will include both the source and the dest in paths.
    */
    vector<vector<string>> allShortestPath(string source) const;


    /**
    Given a starting airport IATA, this function return all the airports' IATA in BFS order.
    */
    vector<string> BFS(string source) const;

    bool assertRouteExists(string source, string dest) const;

    bool assertAirportExists(string IATA) const;

  private:

    // remove all invalid airports
    void pruneAirports();

    /**
    Given the source airport, this function will use Dijkstra's algorithm to calculate 
    the the parent of every airport. This can then be used for both shortest path functions
    @param source source airport IATA
    @param previous a reference to the previous map. key: current IATA, value: parent IATA
    */
    void calcPrevious(string source, std::unordered_map<string, string>& previous) const;


    /**
    Given the destination IATA and the previous map, this function returns a shortest path from 
    source to dest. This is a helper function for both shortest path functions. 
    @param source source airport IATA
    @param dest destination airport IATA
    @param previous a reference to the previous map. key: current IATA, value: parent IATA
    */
    vector<string> calcPath(string source, string dest, std::unordered_map<string, string>& previous) const;

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
    bool assertRouteExists(string source, string dest, string functionName) const;


    /**
     * Returns whether a given airport exists in the graph
     * @param IATA - IATA of an airport
     * @param functionName - the name of the calling function to return
     */
    bool assertAirportExists(string IATA, string functionName) const;

    /**
     * Prints error message with noticeable color
     * @param message - the error message that should be printed
     */
    void printError(string message) const;

    /**
    * The function takes in the IATA of an airport 
    * and returns a map that stores info about adjacent airports
    */
    const std::unordered_map<std::string, Route>& getAdjacentMap(string IATA) const;

};