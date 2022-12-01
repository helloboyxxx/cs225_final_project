#include "graph.h"
#include "graph_generation.h"
#include "structures.h"

#include <algorithm>
#include <cfloat>
#include <limits>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::endl;
using std::string;
using std::vector;

/*
double distance, unsigned ID. Used in calcPrevious
Distance placed in the front since
operator> in std::pair will compare the first element by defaut */
typedef std::pair<double, unsigned> disPair;


Graph::Graph(string airport_filename, string route_filename) {
  airport_map = std::unordered_map<unsigned, Airport>();
  Generator::readFromFile(airport_filename, route_filename, airport_map);
  pruneAirports();

  // Construct ID_map
  for (const auto& airport : airport_map) {
    ID_map.emplace(airport.second.IATA, airport.first);
  }
}


void Graph::pruneAirports() {
  for (const unsigned airport : getAllIDs()) {
    const auto& pair = airport_map.find(airport);
    if (pair->second.route_in == 0 || pair->second.route_out == 0) {
      removeAirport(airport);
    }
  }
}


vector<string> Graph::getAllAirports() const {
  vector<string> airports;
  for (auto &airport : airport_map) {
    airports.push_back(airport.second.IATA);
  }
  return airports;
}

std::vector<unsigned> Graph::getAllIDs() const {
  vector<unsigned> airports;
  for (auto &airport : airport_map) {
    airports.push_back(airport.first);
  }
  return airports;
}

vector<string> Graph::getAdjacentAirports(string IATA) const {
  if (assertAirportExists(IATA) == false) {
    return vector<string>();
  }
  
  const Airport& A = airport_map.find( IATAToID(IATA) )->second;    // A is the Airport struct of the given source

  vector<string> airports;
  for (auto &airport : A.adjacent_airport) {
    airports.push_back(IDToIATA(airport.first));
  }
  return airports;
}


const std::unordered_map<unsigned, Route>& Graph::getAdjacentMap(unsigned ID) const {
  // if (assertAirportExists(IATA, __func__) == false) {
  //   throw std::invalid_argument("Invalid argument in getAdjacentAirports");
  // }
  return airport_map.find(ID)->second.adjacent_airport;
}

double Graph::getDistance(string source, string dest) const {
  if (!assertRouteExists(source, dest)) {
    return -1;
  }
  const auto& source_it = airport_map.find(IATAToID(source));   // iterator that points to the source in airport_map
  // Check if the given sources exists
  if (source_it == airport_map.end()) {
    cout << "The airport " << source << " is not included in the graph." << endl;
    return -1;
  } else if (airport_map.find(IATAToID(dest)) == airport_map.end()) {
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

double Graph::getDistance(unsigned source, unsigned dest) const {
  const auto& source_it = airport_map.find(source);   // iterator that points to the source in airport_map
  // Check if the given sources exists
  if (source_it == airport_map.end()) {
    cout << "The airport " << source << " is not included in the graph." << endl;
    return -1;
  } else if (airport_map.find(dest) == airport_map.end()) {
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



void Graph::insertAirport(Airport& a) {
  removeAirport(a.ID);    // overwrite old stuff
  airport_map[a.ID] = a;  // Insert the airport we want
  ID_map[a.IATA] = a.ID;
}

void Graph::removeAirport(unsigned ID) {
  if (airport_map.find(ID) != airport_map.end()) {
    airport_map.erase(ID);  // erase the airport
    // run a loop and remove all the routes that points to this airport
    for (auto it = airport_map.begin(); it != airport_map.end(); ++it) {
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
  airport_map[IATAToID(source)].adjacent_airport[IATAToID(dest)] = Route(distance);
  return true;
}

void Graph::removeRoute(unsigned source, unsigned dest) {
  assertRouteExists(source, dest, __func__);
  airport_map[source].adjacent_airport.erase(dest);
}


vector<string> Graph::shortestPath(string source, string dest) const {
  // check if source and dest airports are included in the airport map
  if (ID_map.find(source) == ID_map.end() || ID_map.find(dest) == ID_map.end()) {
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
  // reverse to change the order to (start with source and end with dest)
  std::reverse(path.begin(), path.end());

  return path;
}

vector<vector<string>> Graph::allShortestPath(string source) const {
  // check if source is included in the graph
  if (ID_map.find(source) == ID_map.end()) {
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

// Not using priority queue but using set
// reference: http://nmamano.com/blog/dijkstra/dijkstra.html
void Graph::calcPrevious(unsigned source, std::unordered_map<unsigned, unsigned>& previous) const {
  previous.clear();
  // stores airport IATA that has been visited before
  std::unordered_set<unsigned> visited; 
  // key: current IATA, value: shortest total distance from source to current
  std::unordered_map<unsigned, double> distances;

  // Initialize both distance and previous
  for (const auto& airport : airport_map) {
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

bool Graph::assertRouteExists(unsigned source, unsigned dest, string functionName) const {
  if (assertAirportExists(source, functionName) == false ||
      assertAirportExists(dest, functionName) == false) {
        // Let the assertAirportExists report the error message
        return false;
  }
  // std::priority_queue<disPair, vector<disPair>, std::greater<disPair>> Q;
  const Airport& source_a = airport_map.find(source)->second;   // source_a is the Airport struct of the given source
  if (source_a.adjacent_airport.find(dest) == source_a.adjacent_airport.end()) {
    string message = "The route from " + IDToIATA(source) + " to " + IDToIATA(dest) +
                     " is not included in the graph. Called by " + functionName;
    printError(message);
    return false;
  }
  return true;
}

bool Graph::assertAirportExists(unsigned ID, string functionName) const {
  if (airport_map.find(ID) == airport_map.end()) {
    if (functionName != "") {
      string message = "The airport " + std::to_string(ID) +
                       " is not included in the graph. Called by " + functionName;
      printError(message);
    }
    return false;
  }
  return true;
}


bool Graph::assertRouteExists(string source, string dest) const {
  if (assertAirportExists(source) == false ||
      assertAirportExists(dest) == false) {
        // Let the assertAirportExists report the error message
        return false;
  }
  const Airport& source_a = airport_map.find(IATAToID(source))->second;   // source_a is the Airport struct of the given source
  if (source_a.adjacent_airport.find(IATAToID(dest)) == source_a.adjacent_airport.end()) {
    string message = "The route from " + source + " to " + dest +
                 " is not included in the graph.";
    printError(message);
    return false;
  }
  return true;
}

bool Graph::assertAirportExists(string IATA) const {
  if (ID_map.find(IATA) == ID_map.end()) {
    string message = "The airport " + IATA +
                      " is not included in the graph.";
    printError(message);
    return false;
  }
  return true;
}

void Graph::printError(string message) const
{
    std::cerr << "\033[1;31m[Graph Error]\033[0m " + message << endl;
}

void Graph::calcFrequency() {
  std::unordered_map<unsigned, unsigned> previous;

  // loop through each airport in the airport map
  for (auto& airport : airport_map) {
    // find the shortest path from 
    calcPrevious(airport.first, previous);
    // for each value stored in previous, increment the count of frequency of the corresponding 
    for (auto& p : previous) {
      if (p.second != 0) {
        airport_map[p.second].frequency += 1;
      }
    }
  }
  frequency_updated = true;
}