#include "graph.h"
#include "graph_generation.h"
#include "structures.h"

#include <algorithm>
#include <cfloat>
#include <filesystem>
#include <fstream>
#include <limits>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stack>

#include "progressbar.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

/******************** PUBLIC FUNCTIONS BELLOW ********************/

Graph::Graph(string airport_filename, string route_filename)
    : airport_filename_(airport_filename), route_filename_(route_filename) {
  
  // Calling Generator and construct airport_map_
  airport_map_.clear();
  airport_map_ = std::unordered_map<unsigned, Airport>();
  Generator::readFromFile(airport_filename, route_filename, airport_map_);
  pruneAirports();

  // Construct ID_map
  for (const auto& airport : airport_map_) {
    ID_map_.emplace(airport.second.IATA, airport.first);
  }
}


vector<string> Graph::getAdjacentAirports(string IATA) const {
  if (assertAirportExists(IATA) == false) {
    return vector<string>();
  }
  
  const Airport& A = airport_map_.find( IATAToID(IATA) )->second;    // A is the Airport struct of the given source

  vector<string> airports;
  for (auto &airport : A.adjacent_airport) {
    airports.push_back(IDToIATA(airport.first));
  }
  return airports;
}


vector<string> Graph::getAllAirports() const {
  vector<string> airports;
  for (auto &airport : airport_map_) {
    airports.push_back(airport.second.IATA);
  }
  return airports;
}


double Graph::getDistance(string source, string dest) const {
  if (!assertRouteExists(source, dest)) {
    return -1;
  }
  const auto& source_it = airport_map_.find(IATAToID(source));   // iterator that points to the source in airport_map
  // Check if the given sources exists
  if (source_it == airport_map_.end()) {
    cout << "The airport " << source << " is not included in the graph." << endl;
    return -1;
  } else if (airport_map_.find(IATAToID(dest)) == airport_map_.end()) {
    cout << "The airport " << dest << " is not included in the graph." << endl;
    return -1;
  }

  // iterator that points to the dest in the adjacent_airport. Reminder: value is a Route struct
  const auto& dest_it = source_it->second.adjacent_airport.find(IATAToID(dest)); 
  if (dest_it == source_it->second.adjacent_airport.end()) {
    cout << "The route from " << source << " to " << dest << " is not included in the graph.";
    return -1;
  } 

  // Here we must have a route from source to dest
  return dest_it->second.distance;
}


void Graph::insertAirport(Airport& a) {
  removeAirport(a.ID);    // overwrite old stuff
  airport_map_[a.ID] = a;  // Insert the airport we want
  ID_map_[a.IATA] = a.ID;
}


void Graph::removeAirport(unsigned ID) {
  if (airport_map_.find(ID) != airport_map_.end()) {
    airport_map_.erase(ID);  // erase the airport
    // run a loop and remove all the routes that points to this airport
    for (auto it = airport_map_.begin(); it != airport_map_.end(); ++it) {
      auto& adj_airport = it->second.adjacent_airport;
      if (adj_airport.find(ID) != adj_airport.end()) {
        adj_airport.erase(ID);    // Erase route
      }
    }
  }
}


bool Graph::insertRoute(std::string source, std::string dest, double distance) {
  if ( assertAirportExists(source) == false || 
    assertAirportExists(dest) == false )
  {
    printError("insertRoute error");
    return false;
  }
  airport_map_[IATAToID(source)].adjacent_airport[IATAToID(dest)] = Route(distance);
  return true;
}


void Graph::removeRoute(unsigned source, unsigned dest) {
  // check if both source and dest are included in the graph
  if (airport_map_.find(source) == airport_map_.end() ||
    airport_map_.find(dest) == airport_map_.end()) 
  { 
      return;
  }

  airport_map_[source].adjacent_airport.erase(dest);
}


vector<string> Graph::shortestPath(string source, string dest) const {
  // check if source and dest airports are included in the airport map
  if (ID_map_.find(source) == ID_map_.end() || ID_map_.find(dest) == ID_map_.end()) {
    return vector<string>();
  }
  unsigned source_id = IATAToID(source);
  unsigned dest_id = IATAToID(dest);

  //check if dest is not the same as source
  if (source == dest) 
    return vector<string>();

  vector<string> path;

  // create the previous map for finding the path using calcPrevious.
  std::unordered_map<unsigned, unsigned> previous;
  calcPrevious(source_id, previous);

  path.push_back(dest);
  unsigned prev = previous[dest_id];
  while (prev != 0) {
    path.push_back(IDToIATA(prev));
    prev = previous[prev];
  }
  if (path.size() == 1) {
    printError("There is not a path from " + source + " to " + dest + ".");
  }
  // reverse to change the order to (start with source and end with dest)
  std::reverse(path.begin(), path.end());

  return path;
}


vector<vector<string>> Graph::allShortestPath(string source) const {
  // check if source is included in the graph
  if (ID_map_.find(source) == ID_map_.end()) {
    return vector<vector<string>>();
  }
  unsigned source_id = IATAToID(source);
  
  vector<vector<string>> to_return;

  // create the previous map for finding the path using calcPrevious.
  std::unordered_map<unsigned, unsigned> previous;
  calcPrevious(source_id, previous);

  // Iterate through all airports to find its shortest path to source airport
  for (const auto& cur_airport : getAllIDs()) {
    // source won't have a path to itself in this case
    if (cur_airport == source_id)
      continue;
    
    // find path from cur_airport to source
    vector<string> path;
    path.push_back( IDToIATA(cur_airport) );
    unsigned prev = previous[cur_airport];

    while (prev != 0) {
      path.push_back(IDToIATA(prev));
      prev = previous[prev];
    }

    // reverse to change the order to (start with source and end with cur_airport)
    std::reverse(path.begin(), path.end());
    to_return.push_back(path);
  }
  
  return to_return;
}


vector<string> Graph::BFS(string source) const {
  if (assertAirportExists(source) == false ) {
    return vector<string>(); 
  }
  vector<string> output;
  std::unordered_set<unsigned> visited;
  std::queue<unsigned> Q;
  Q.push(IATAToID(source));
  visited.emplace(IATAToID(source));

  while( !Q.empty() ) {
    unsigned cur_airport = Q.front();
    Q.pop();
    output.push_back(IDToIATA(cur_airport));

    for (auto adj_pair : getAdjacentMap(cur_airport)) {
      unsigned a = adj_pair.first;
      // Not adding visited airport
      if (visited.find(a) == visited.end()) {
        Q.emplace(a);
        visited.emplace(a);
      }
    }
  }
  return output;
}


bool Graph::assertRouteExists(string source, string dest) const {
  if (assertAirportExists(source) == false ||
      assertAirportExists(dest) == false) {
        // Let the assertAirportExists report the error message
        return false;
  }
  const Airport& source_a = airport_map_.find(IATAToID(source))->second;   // source_a is the Airport struct of the given source
  if (source_a.adjacent_airport.find(IATAToID(dest)) == source_a.adjacent_airport.end()) {
    string message = "The route from " + source + " to " + dest +
                 " is not included in the graph.";
    printError(message);
    return false;
  }
  return true;
}


bool Graph::assertAirportExists(string IATA) const {
  if (ID_map_.find(IATA) == ID_map_.end()) {
    string message = "The airport " + IATA +
                      " is not included in the graph.";
    printError(message);
    return false;
  }
  return true;
}


void Graph::calcFrequency() {
  // if we have calculated freq before, read from file
  if (freq_updated) {
    return; 
  }
  else if (freqExsists()) {
    // Output message
    printReminder("from calcFrequency: ");
    cout << "Frequency has been calculated before by using \"";
    cout << airport_filename_ << "\" and \"";
    cout << route_filename_ << "\"." << endl; 
    cout << "Now reading from existing file. "; 
    cout << "Call \"clearFreqFile\" if you want to recalculate." << endl;
    
    // Read freq from file
    readFrequency();
    return;
  }

  // Using progressbar here for prettier output
  cout << "Calculating Betweeness Centrality: " << endl;
  progressbar bar(airport_map_.size());

  std::unordered_map<unsigned, unsigned> previous;
  // loop through each airport in the airport map
  for (const auto& airport : airport_map_) {
    // find the shortest path from 
    calcPrevious(airport.first, previous);  // will change previous
    freqHelper(airport.first, previous);    // Update frequency to graph based on previous
    bar.update();
  }
  cout << endl; // Progress bar ends
  writeFrequency();
  freq_updated = true;
}


unsigned Graph::getFrequency(std::string IATA) const {
  if (freq_updated == false) {
    throw std::runtime_error("freq not calculated");
  }
  return airport_map_.find(IATAToID(IATA))->second.frequency;
}


void Graph::clearFreqFile() {
  freq_updated = false;
  std::filesystem::remove(freq_filename);
  std::filesystem::remove(IATA_filename);
}

/******************** PRIVATE FUNCTIONS BELLOW ********************/

void Graph::pruneAirports() {
  for (const unsigned airport : getAllIDs()) {
    const auto& pair = airport_map_.find(airport);
    // Check route in and out 
    if (pair->second.route_in == 0 || pair->second.route_out == 0) {
      removeAirport(airport);
    }
  }
  for(unsigned airport: getAllIDs()) {
    if (IDToIATA(airport).size() != 3) {
      removeAirport(airport);
    }
  }
}


std::vector<unsigned> Graph::getAllIDs() const {
  vector<unsigned> airports;
  for (auto &airport : airport_map_) {
    airports.push_back(airport.first);
  }
  return airports;
}


const std::unordered_map<unsigned, Route>& Graph::getAdjacentMap(unsigned ID) const {
  return airport_map_.find(ID)->second.adjacent_airport;
}


double Graph::getDistance(unsigned source, unsigned dest) const {
  const auto& source_it = airport_map_.find(source);   // iterator that points to the source in airport_map
  // Check if the given sources exists
  if (source_it == airport_map_.end()) {
    cout << "The airport " << source << " is not included in the graph." << endl;
    return -1;
  } else if (airport_map_.find(dest) == airport_map_.end()) {
    cout << "The airport " << dest << " is not included in the graph." << endl;
    return -1;
  }

  // iterator that points to the dest in the adjacent_airport. Reminder: value is a Route struct
  const auto& dest_it = source_it->second.adjacent_airport.find(dest); 
  if (dest_it == source_it->second.adjacent_airport.end()) {
    cout << "The route from " << source << " to " << dest << " is not included in the graph.";
    return -1;
  } 

  // Here we must have a route from source to dest
  return dest_it->second.distance;
}


void Graph::calcPrevious(unsigned source, std::unordered_map<unsigned, unsigned>& previous) const {
  previous.clear();
  // stores airport IATA that has been visited before
  std::unordered_set<unsigned> visited; 
  // key: current IATA, value: shortest total distance from source to current
  std::unordered_map<unsigned, double> distances;

  // Initialize both distance and previous
  for (const auto& airport : airport_map_) {
    unsigned ID = airport.first;
    distances[ID] = DBL_MAX;    // create a disPair and insert into distance graph
    previous[ID] = 0;
  }
  // Set distance of source to zero
  distances[source] = 0;

  // Q stores future nodes
  std::set<disPair> Q;
  // Q.insert({0, source});
  Q.emplace( 0, source );
  
  // Dijkstra starts
  while ( !Q.empty() ) {                                                          // O(n)
    unsigned cur_airport = Q.begin()->second;                                       // O(1)
    Q.erase(Q.begin());

    // Skip visited cur_airport
    if (visited.find(cur_airport) != visited.end())
      continue;
      
    visited.emplace(cur_airport);
    // Loop through the neighbors of this airport
    for (const auto& adj_pair : getAdjacentMap(cur_airport)) {               // O(m) since we have at most m edges
      unsigned neighbor = adj_pair.first;
      if (visited.find(neighbor) != visited.end())
        continue;   // skip when visited this airport before
      
      double cur_distance = getDistance(cur_airport, neighbor);
      if (cur_distance + distances[cur_airport] < distances[neighbor]) {
        // Erase old distance
        Q.erase({distances[neighbor], neighbor});                                 // O(log_n) since std::set is self_balanced BST

        distances[neighbor] = cur_distance + distances[cur_airport];
        previous[neighbor] = cur_airport;
        
        // update the set with neighbor and its new distance.
        Q.emplace( distances[neighbor], neighbor );                               // O(log_n)
      }
    }
  }
}


void Graph::freqHelper(unsigned source, std::unordered_map<unsigned, unsigned>& previous) {
  for (const auto& cur_airport : getAllIDs()) {
    unsigned prev = previous[cur_airport];
    // Trace the path from destination to source airport
    // prev == 0 when no path exists; prev == source when path ends
    while (prev != 0 && prev != source) {
      airport_map_[prev].frequency += 1;    // Increment centrality score
      prev = previous[prev];                // keep tracking
    }
  }
}


void Graph::writeFrequency() {
  // Create frequencies
  frequencies_.clear();
  for (const auto& airport : airport_map_) {
    unsigned ID = airport.first;
    unsigned freq = airport.second.frequency;

    // put a freqPair in frequencies
    frequencies_.emplace(freq, ID);
  }


  // Write from frequencies to file
  std::ofstream id_file;
  std::ofstream IATA_file;
  id_file.open(freq_filename);
  IATA_file.open(IATA_filename);

  // Write filenames IF graph not constructed from files, write invalid_filename
  if (airport_filename_.empty() || route_filename_.empty()) {
    id_file << invalid_filename << "\n" << invalid_filename << endl;
    IATA_file << invalid_filename << "\n" << invalid_filename << endl;
  }
  else {
    id_file << airport_filename_ << "\n" << route_filename_ << endl;
    IATA_file << airport_filename_ << "\n" << route_filename_ << endl;
  }

  for (const freqPair& freq : frequencies_) {
    id_file << freq.second << "," << freq.first << endl;
    IATA_file << IDToIATA(freq.second) << "," << freq.first << endl;
  }
  id_file.close();
  IATA_file.close();
}


void Graph::readFrequency() {
  std::ifstream freq_file(freq_filename);

  if (freq_file.is_open() == false) {
    throw std::runtime_error("readFrequency cannot open file");
  }

  // Create frequencies_
  frequencies_.clear();

  string line; 
  // ignore two lines
  std::getline(freq_file, line);
  std::getline(freq_file, line);
  unsigned count = 0;
  while (std::getline(freq_file, line)) {
    ++count;
    std::vector<std::string> info;
    std::string each_info;
    std::stringstream ss(line);
    while (getline(ss, each_info, ',')) {
        info.push_back(each_info);
    }

    unsigned ID = std::stoul(info[0]);
    unsigned cur_freq = std::stoul(info[1]);

    // put a freqPair in frequencies
    frequencies_.emplace(cur_freq, ID);

    // change the current airport_map_
    airport_map_[ID].frequency = cur_freq;
  }
}


bool Graph::freqExsists() const {
  std::ifstream freq_file(freq_filename);

  // This file exists
  if (freq_file.is_open()) {
    string line; 

    // Check if the first line matches airport_filename_. 
    if (std::getline(freq_file, line)) {
      if (line != airport_filename_) return false;
    } else {
      return false;
    }

    // Check if the second line matches route_filename_.
    if (std::getline(freq_file, line)) {
      if (line != route_filename_) return false;
    } else {
      return false;
    }
    return true;
  }

  // freq_filename not exists. 
  return false;
}


Cycle_Graph Graph::generateEulerianCycleGraph(std::string IATA) {
  Cycle_Graph G;

  unsigned id = IATAToID(IATA);
  generateEulerianCycleGraph(id, G);
  return G;
}

void Graph::generateEulerianCycleGraph(unsigned id, Cycle_Graph& G) const {
  std::unordered_map<unsigned, Route> adjs = getAdjacentMap(id);

  for (const auto& adj : adjs) {
    if (G.find({id, adj.first}) != G.end()) continue;
    if (isTwoWay(adj.first, id)) {
      G[{id, adj.first}] = false;
      G[{adj.first, id}] = false;
      generateEulerianCycleGraph(adj.first, G);
    }
  }
}

bool Graph::isTwoWay(const unsigned adj, unsigned des) const {
  std::unordered_map<unsigned, Route> adjs = getAdjacentMap(adj);
  return (adjs.find(des) != adjs.end());
}


std::vector<unsigned> Graph::cycleDFS(Cycle_Graph& G, unsigned start) {
    std::vector<unsigned> path;
    path.push_back(start);
    cycleDFS(G, start, path, G.size());
    return path;
}

void Graph::cycleDFS(Cycle_Graph& G, unsigned current_airport, std::vector<unsigned>& path, size_t num_total_path) {
    for (auto& p : G) {
        //if this route has already been visited
        if (p.second == true) continue;
        //if this route does not start at current airport
        if (p.first.first != current_airport) continue;
        G[p.first] = true;
        path.push_back(p.first.second);
        // std::cout<<" "<<IDToIATA(path.back())<<" "<<std::endl;
        cycleDFS(G, p.first.second, path, num_total_path);
        break;
    }
    //after the loop, if path has size equal to num_total_path
    //all routes have been visited
    //return right away
    return;
    if (path.size() == num_total_path + 1) return;
    //if not, then there are unvisited routes
    unsigned not_to, from, to;
    while (true) {
      not_to = path.back();
      path.pop_back();
      from = path.back();
      G[{from, not_to}] = false;
      to = hasOtherPath(G, from, not_to);
      if (to != 0) break;
    }
    G[{from, to}] = true;
    path.push_back(to);
    cycleDFS(G, to, path, num_total_path);
}

unsigned Graph::hasOtherPath(Cycle_Graph& G, unsigned from, unsigned not_to) {
  for (auto& p : G) {
    if (p.second == true) continue;
    if (p.first.first != from) continue;
    if (p.first.second == not_to) continue;
    return p.first.second;
  }
  return 0;
}

void Graph::RoundTrip(std::string IATA) {
  Cycle_Graph c = generateEulerianCycleGraph(IATA);
  for (auto& p : c) {
    std::cout<< "From: " << IDToIATA(p.first.first) << " ~~~ To: " << IDToIATA(p.first.second) <<std::endl;
  }
  std::cout<< " "<<std::endl;
  std::vector<unsigned> path = cycleDFS(c, IATAToID(IATA));
  std::cout << "Start: ";
  for (auto& i : path) {
    std::cout<< IDToIATA(i) << " - ";
  }
  std::cout << "End" << std::endl;
}


void Graph::printError(string message) const {
    std::cerr << "\033[1;31m[Graph Error]\033[0m " + message << endl;
}


void Graph::printReminder(string message) const {
    std::cerr << "\033[1;32m[Reminder]\033[0m " + message << endl;
}