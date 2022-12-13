#pragma once

#include "structures.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>

#include <map>
#include <filesystem>

/**
 * double distance, unsigned ID.
 * Used in calcPrevious. Distance placed in the front since operator> in
 * std::pair will compare the first element by defaut
 */
typedef std::pair<double, unsigned> disPair;

/**
 * unsigned frequency, unsigned airport ID
 * Used in frequencies_
 */
typedef std::pair<unsigned, unsigned> freqPair;

// These two file names will be used as output filename of writeFrequency
const std::string freq_filename = "../allFrequency.txt";

typedef std::unordered_map<std::pair<unsigned, unsigned>, bool, hash_pair> Cycle_Graph;
const std::string IATA_filename = "../allFrequency_IATA.txt";
const std::string invalid_filename = "INVALID";

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
     * Return all the airports in this graph as a std::vector of their IATAs.
     */
    std::vector<std::string> getAllAirports() const;
    
    /**
     * Given source and destination airports, if the two airports are directly connected, return the distance
     * Otherwise, return -1.
     * This is a slow version since it takes in string as parameter, which requires translation.
     * So this function should not be used in calcPrevious.
     * @param source, the IATA of the source airport
     * @param dest, the IATA of the destination airport
     * @returns the distance between two airports.
     */
    double getDistance(std::string source, std::string dest) const;

    /**
     * Inserts a new airport into the graph.
     * This function will overwrite if old stuff was there
     */
    void insertAirport(Airport& a);

    /**
     * Removes a given airport from the graph.
     * This function should also remove all the routes that are 
     * connected to this airport.
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
    bool empty() const { return airport_map_.empty(); }

    /**
     * Given the source and destination airports' IATA, this function returns a
     * std::vector of std::strings that represents all the airports in that path. We will
     * include both the source airport and the dest in this path.
     * 
     * Function will return an empty std::vector if either of source or dest is not 
     * included in graph or if dest is the same as source.
     * Function will return a std::vector of size one storing the IATA of destination airport 
     * if there is not a path from source to dest. For this case, an error message will also be displayed.
     * 
     * @param source source airport IATA
     * @param dest destination airport IATA
     * @returns a std::vector of IATA that represents the path
     */
    std::vector<std::string> shortestPath(std::string source, std::string dest) const;

    /**
     * Given the source airport, this function will use Dijkstra's algorithm to
     * calculate the shortest paths from source to all other airports. This is a
     * helper function for betweenness centrality algorithms.
     * Paths in the returned std::vector has no priority. Each path contains at least
     * two elements.
     * The function will return an empty std::vector if the given source airport 
     * is not inlucded in the graph.
     * @param source source airport IATA
     * @returns a std::vector of paths. We will include both the source and the dest in paths.
     */
    std::vector<std::vector<std::string>> allShortestPath(std::string source) const;


    /**
     * Given a starting airport IATA, this function return all the airports' IATA in BFS order.
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
     * Given airport IATA, return the frequency of this airport
     */
    unsigned getFrequency(std::string IATA) const;
    
    /*
     * This function deletes the frequency file where its path is stored in freq_filename
     */
    void clearFreqFile();

    /**
     * @param IATA the starting and ending airport of the trip
     * this function generates a possible travel plan that flies to 
    */
    void Eulerian_Cycle(std::string IATA);


    /**
     * compare which airport has larger frequency
    */
    static bool compareByFreq(const Airport &a, const Airport &b) {
      return a.frequency > b.frequency;
    }

    /*
    This function, given a starting position and a reference to a set, 
    runs the DFS traversal and updates the set where all the vertex DFS of 
    the initial vertex can gets to (including the initial vertex)
    */
    void DFS(unsigned ID, std::unordered_set<unsigned>& visited);

    /**
     * This function returns the round trip details given the source airport's IATA
    */
    void RoundTrip(std::string IATA);

  private:
    // true if frequency is calculated; false otherwise
    bool freq_updated = false;

    /**
     * key: airport's ID, value: its corresponding airport struct
     */
    std::unordered_map<unsigned, Airport> airport_map_;

    /**
     * Map used for mapping IATA to airport ID. 
     */
    std::unordered_map<std::string, unsigned> ID_map_;

    // storing frequencies, sorted from greatest to smallest
    std::set< freqPair, std::greater<freqPair> > frequencies_; 

    // Initially empty airport filename, non-empty after constructor
    std::string airport_filename_;

    // Initially empty airport filename, non-empty after constructor
    std::string route_filename_;

    // The following three functions does not check if the argument is valid or not. 
    std::string IDToIATA(unsigned ID) const {
      return airport_map_.find(ID)->second.IATA; 
    }
    
    const Airport& IDToAirport(unsigned ID) const {
      return (airport_map_.find(ID)->second); 
    }

    unsigned IATAToID(std::string IATA) const {
      return ID_map_.find(IATA)->second;
    }

    // remove all invalid airports
    void pruneAirports();

    /*
     * Return all the airports in this graph as a std::vector of their IDs.
     */
    std::vector<unsigned> getAllIDs() const;

    /**
     * The function takes in the ID of an airport 
     * and returns a map that stores info about adjacent airports
     */
    const std::unordered_map<unsigned, Route>& getAdjacentMap(unsigned ID) const;

    /**
     * Given source and destination airports, if the two airports are directly connected, return the distance
     * Otherwise, return -1.
     * @param source, the ID of the source airport
     * @param dest, the ID of the destination airport
     * @returns the distance between two airports.
     */
    double getDistance(unsigned source, unsigned dest) const;

    /**
     * Given the source airport, this function will use Dijkstra's algorithm to calculate 
     * the the parent of every airport. This can then be used for both shortest path functions
     * Not uses priority queue but uses std::set. Reverence: http://nmamano.com/blog/dijkstra/dijkstra.html
     * @param source source airport ID
     * @param previous a reference to the previous map. key: current ID, value: parent ID
     */
    void calcPrevious(unsigned source, std::unordered_map<unsigned, unsigned>& previous) const;

    /**
     * Given the source airport ID where the previous map was calculated, this function will 
     * add frequencies for all possible internal airports.
     * e.g undirected graph  A -- B -- C. Given source = C, B would be the internal airport. 
     */
    void freqHelper(unsigned source, std::unordered_map<unsigned, unsigned>& previous);

    /**
     * Helper function for calcFrequency. This function will:
     * (1) generate a file using freq_filename (defined as a constant in graph.h)
     * (2) also write the frequency into frequencies, a private variable of Graph
     * It will first write the input file names at the beginning of file. 
     * It will not overwrite the file if the file names already exists.
     */
    void writeFrequency();

    /**
     * Read from freq_filename and construct frequencies_.
     * Should only be called when freqExsists.
     */
    void readFrequency();

    /**
     * This function will return true when: 
     * (1) the file freq_file exsists. 
     * (2) the first two lines in that file matches the airport_filename and route_filename. 
     */
    bool freqExsists() const;

    /**
     * Prints error message with noticeable color
     * @param message - the error message that should be printed
     */
    void printError(std::string message) const;

    /**
     * Prints reminder message with noticeable color
     * @param message - the message that should be printed
     */
    void printReminder(std::string message) const;


    /**
     * Each trip can be deemed as round trip
     * if A->B exists, then B->A also exists 
     * @param IATA the starting airport of the trip
     * @return Generates a Strong connected airport graph which:
     */
    Cycle_Graph generateEulerianCycleGraph(std::string IATA);
  
    /**
     * helper function of generateEulerianCycleGraph
     */
    void generateEulerianCycleGraph(unsigned id, Cycle_Graph& G) const;

    /**
     * @param adj airport-id of the adjcent airport
     * @param des airport-id of the starting airport
     * @return when des->adj exists, True when adj->des also exists
     */
    bool isTwoWay(const unsigned adj, unsigned des) const;

    /**
    * Hierholzer's Algorithm
    * @param G  Cycle_Graph
    * @param start starting aiport's id
    * @return vectors of airports's id
    */
    std::vector<unsigned> cycleDFS(Cycle_Graph& G, unsigned start);
  
    /**
    * DFS for the fleury algorithm
    */
    void cycleDFS(Cycle_Graph& G, unsigned current_airport, std::vector<unsigned>& path, size_t num_total_path);
  
    /**
    * Helper function for Hieholzer's Algorithm
    */
    unsigned hasOtherPath(Cycle_Graph& G, unsigned from, unsigned not_to);

};
