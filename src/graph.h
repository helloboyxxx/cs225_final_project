#pragma once

#include "structures.h"

#include <string>
#include <unordered_map>
#include <vector>

class Graph {
  public:

    // Default constructor that creates an empty graph
    Graph() = default;

    // Given two file names, this constructor calls graph_generation to help generate the airport_map
    Graph(std::string airport_filename, std::string route_filename);


    /**
    * Return the adjacent airports of this airport
    * The function takes in the IATA of an airport 
    * and returns a std::vector of its adjacent airports' IATAs.
    * If the given IATA is not in the graph, we return an empty std::vector.
    * Assume that this is a connected graph.
    */
    std::vector<std::string> getAdjacentAirports(std::string IATA) const;

    /*
    Return all the airports in this graph as a std::vector of their IATAs.
    */
    std::vector<std::string> getAllAirports() const;
    

    /**
    Given source and destination airports, if the two airports are directly connected, return the distance
    Otherwise, return -1.
    This is a slow version since it takes in string as parameter, which requires translation.
    So this function should not be used in calcPrevious.
    @param source, the IATA of the source airport
    @param dest, the IATA of the destination airport
    @returns the distance between two airports.
    */
    double getDistance(std::string source, std::string dest) const;


    /**
    Inserts a new airport into the graph.
    This function will overwrite if old stuff was there
    */
    void insertAirport(Airport& a);

    /**
    Removes a given airport from the graph.
    This function should also remove all the routes that are 
    connected to this airport.
    */
    void removeAirport(unsigned IATA);


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
    bool insertRoute(std::string source, std::string dest, double distance);


    /**
     * Removes the route between two airports
     * @param source - source airport IATA
     * @param dest - destination airport IATA
     */
    void removeRoute(unsigned source, unsigned dest);


    // Return true if this grpah is empty. 
    bool empty() const { return airport_map.empty(); }

    /**
    Given the source and destination airports' IATA, this function returns a
    std::vector of std::strings that represents all the airports in that path. We will
    include both the source airport and the dest in this path.
    
    Function will return an empty std::vector if either of source or dest is not 
    included in graph or if dest is the same as source.
    Function will return a std::vector of size one storing the IATA of destination airport 
    if there is not a path from source to dest.

    @param source source airport IATA
    @param dest destination airport IATA
    @returns a std::vector of IATA that represents the path
    */
    std::vector<std::string> shortestPath(std::string source, std::string dest) const;

    /**
    Given the source airport, this function will use Dijkstra's algorithm to
    calculate the shortest paths from source to all other airports. This is a
    helper function for betweenness centrality algorithms.
    Paths in the returned std::vector has no priority. Each path contains at least
    two elements.
    The function will return an empty std::vector if the given source airport 
    is not inlucded in the graph.
    @param source source airport IATA
    @returns a std::vector of paths. We will include both the source and the dest in paths.
    */
    std::vector<std::vector<std::string>> allShortestPath(std::string source) const;


    /**
    Given a starting airport IATA, this function return all the airports' IATA in BFS order.
    */
    std::vector<std::string> BFS(std::string source) const;

    bool assertRouteExists(std::string source, std::string dest) const;

    bool assertAirportExists(std::string IATA) const;
    
    /**
     * This function runs the betweenness centrality algorithm 
     * which runs through Dijkstra's algorithm for each airport in the airport_map 
     * and updates the frequency of each airport the times it appears to be in 
     * the shortest path of other two airports.
     * This function will also update the frequency_updated to true.
    */
    void calcFrequency();
    
    /**
     * This function returns the a boolean representing 
     * the status of the frequencies of airports in the graph.
     * True means that all frequencies has been updated (calcFrequency has been called), 
     * false otherwise.
    */
    bool getFrequencyUpdated() const { return frequency_updated; }

    std::pair<std::string, unsigned> getMostImportantAirport() const;
  private:
    // true if the frequencies of the airports is updated 
    bool frequency_updated = false;

    /**
    key: airport's ID, value: its corresponding airport struct
    */
    std::unordered_map<unsigned, Airport> airport_map;

    /**
    Map used for mapping IATA to airport ID. 
    */
    std::unordered_map<std::string, unsigned> ID_map;

    // The following two functions does not check if the argument is valid or not. 
    std::string IDToIATA(unsigned ID) const {
      return airport_map.find(ID)->second.IATA; 
    }

    unsigned IATAToID(std::string IATA) const {
      return ID_map.find(IATA)->second;
    }

    // remove all invalid airports
    void pruneAirports();

    /*
    Return all the airports in this graph as a std::vector of their IDs.
    */
    std::vector<unsigned> getAllIDs() const;

    /**
    Given the source airport, this function will use Dijkstra's algorithm to calculate 
    the the parent of every airport. This can then be used for both shortest path functions
    @param source source airport ID
    @param previous a reference to the previous map. key: current ID, value: parent ID
    */
    void calcPrevious(unsigned source, std::unordered_map<unsigned, unsigned>& previous) const;


    /**
    Given source and destination airports, if the two airports are directly connected, return the distance
    Otherwise, return -1.
    @param source, the ID of the source airport
    @param dest, the ID of the destination airport
    @returns the distance between two airports.
    */
    double getDistance(unsigned source, unsigned dest) const;


    /**
    Given the destination ID and the previous map, this function returns a shortest path from 
    source to dest. This is a helper function for both shortest path functions. 
    @param source source airport ID
    @param dest destination airport ID
    @param previous a reference to the previous map. key: current ID, value: parent ID
    */
    std::vector<unsigned> calcPath(unsigned source, unsigned dest, std::unordered_map<unsigned, unsigned>& previous) const;



    /**
     * Returns whether thee route from source to dest exists in the graph
     * @param source - one airport
     * @param dest - another airport
     * @param functionName - the name of the calling function to return
     */
    bool assertRouteExists(unsigned source, unsigned dest, std::string functionName) const;


    /**
     * Returns whether a given airport exists in the graph
     * @param ID - ID of an airport
     * @param functionName - the name of the calling function to return
     */
    bool assertAirportExists(unsigned ID, std::string functionName) const;

    /**
     * Prints error message with noticeable color
     * @param message - the error message that should be printed
     */
    void printError(std::string message) const;

    /**
    * The function takes in the ID of an airport 
    * and returns a map that stores info about adjacent airports
    */
    const std::unordered_map<unsigned, Route>& getAdjacentMap(unsigned ID) const;

};