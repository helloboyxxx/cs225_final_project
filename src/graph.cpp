#include "graph.h"
#include "graph_generation.h"
#include "structures.h"
#include <cfloat>
#include <string>
#include <unordered_map>
#include <limits>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <chrono>

using std::cout;
using std::endl;
using namespace std::chrono;

/*
double distance, string IATA. Used in calcPrevious
Distance placed in the front since
operator> in std::pair will compare the first element by defaut */
typedef std::pair<double, string> disPair;


Graph::Graph(string airport_filename, string route_filename) {
  airport_map = std::unordered_map<std::string, Airport>();
  Generator::readFromFile(airport_filename, route_filename, airport_map);

}

vector<string> Graph::getAllAirports() const {
  vector<string> airports;
  for (auto &airport : airport_map) {
    airports.push_back(airport.first);
  }
  return airports;
}

vector<string> Graph::getAdjacentAirports(string IATA) const {
  if (assertAirportExists(IATA, __func__) == false) {
    return vector<string>();
  }

  const Airport& A = airport_map.find(IATA)->second;    // A is the Airport struct of the given source

  vector<string> airports;
  for (auto &airport : A.adjacent_airport) {
    airports.push_back(airport.first);
  }
  return airports;
}


double Graph::getDistance(string source, string dest) const {
  // // Use these three lines when debugging. (Since it is easier to understand.)
  // // I included the optimized version below. Uncomment to run. 
  // if ( assertRouteExists(source, dest, __func__) == false ) { return -1; }
  // const Airport& source_a =  airport_map.find(source)->second;  // source_a is the Airport struct of the given source
  // return source_a.adjacent_airport.find(dest)->second.distance; // find the adjacent airport's route and return distance from it


  // Optimized version bellow:
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


void Graph::insertAirport(Airport a) {
  removeAirport(a.IATA);    // overwrite old stuff
  airport_map[a.IATA] = a;  // Insert the airport we want
}

void Graph::removeAirport(string IATA) {
  if (airport_map.find(IATA) != airport_map.end()) {
    airport_map.erase(IATA);  // erase the airport
    // run a loop and remove all the routes that points to this airport
    for (auto it = airport_map.begin(); it != airport_map.end(); ++it) {
      auto& adj_airport = it->second.adjacent_airport;
      if (adj_airport.find(IATA) != adj_airport.end()) {
        adj_airport.erase(IATA);
      }
    }
  }
}


bool Graph::insertRoute(string source, string dest, double distance) {
  if ( assertAirportExists(source, __func__) == false || 
  assertAirportExists(dest, __func__) == false )
  {
    printError("insertRoute error");
    return false;
  }
  airport_map[source].adjacent_airport[dest] = Route(distance);
  return true;
}

void Graph::removeRoute(string source, string dest) {
  assertRouteExists(source, dest, __func__);
  airport_map[source].adjacent_airport.erase(dest);
}


vector<string> Graph::shortestPath(string source, string dest) const {
  // check if both source and dest is included in the graph
  if (!assertAirportExists(source, "shortestPath") || !assertAirportExists(dest, "shortestPath"))
    return vector<string>();

  //check if dest is not the same as source
  if (source == dest) 
    return vector<string>();

  vector<string> path;

  // create the previous map for finding the path using calcPrevious.
  std::unordered_map<string, string> previous;
  calcPrevious(source, previous);

  path.push_back(dest);
  string prev = previous[dest];
  while (prev != "") {
    path.push_back(prev);
    prev = previous[prev];
  }
  // reverse to change the order to (start with source and end with dest)
  std::reverse(path.begin(), path.end());

  return path;
}

vector<vector<string>> Graph::allShortestPath(string source) const {
  // check if source is included in the graph
  if (!assertAirportExists(source, "allShortestPath"))
    return vector<vector<string>>();


  vector<vector<string>> to_return;

  // create the previous map for finding the path using calcPrevious.
  std::unordered_map<string, string> previous;
  calcPrevious(source, previous);

  // Iterate through all airports to find its shortest path to source airport
  for (const auto& cur_airport : getAllAirports()) {
    // source won't have a path to itself in this case
    if (cur_airport == source)
      continue;
    
    // find path from cur_airport to source
    vector<string> path;
    path.push_back(cur_airport);
    string prev = previous[cur_airport];

    while (prev != "") {
      path.push_back(prev);
      prev = previous[prev];
    }

    // reverse to change the order to (start with source and end with cur_airport)
    std::reverse(path.begin(), path.end());
    to_return.push_back(path);
  }
  
  return to_return;
}

void Graph::calcPrevious(string source, std::unordered_map<string, string>& previous) const {
  previous.clear();
  // stores airport IATA that has been visited before
  std::unordered_set<string> visited; 
  // key: current IATA, value: shortest total distance from source to current
  std::unordered_map<string, double> distances;

  // Initialize both distance and previous
  for (const auto& airport : airport_map) {
    string IATA = airport.first;
    distances[IATA] = DBL_MAX;    // create a disPair and insert into distance graph
    previous[IATA] = "";
  }
  // Set distance of source to zero
  distances[source] = 0;

  // initialize priority_queue.
  std::priority_queue< disPair, vector<disPair>, std::greater<disPair> > Q;
  Q.push({0, source});
  
  // Dijkstra starts
  while ( !Q.empty() ) {
    string cur_airport = Q.top().second;
    Q.pop();
    if (visited.find(cur_airport) != visited.end())
      continue;
    visited.insert(cur_airport);
    
    // Loop through the neighbors of this airport
    for (auto neighbor : getAdjacentAirports(cur_airport)) {
      if (visited.find(neighbor) != visited.end())
        continue;   // skip when visited this airport before
      
      double cur_distance = getDistance(cur_airport, neighbor);
      if (cur_distance + distances[cur_airport] < distances[neighbor]) {
        distances[neighbor] = cur_distance + distances[cur_airport];
        previous[neighbor] = cur_airport;

        // update the priority queue with neighbor and its new distance.
        Q.push({distances[neighbor], neighbor});
      }
    }
  }
}

vector<string> Graph::BFS(string source) const {
  if (assertAirportExists(source, __func__) == false ) {
    return vector<string>(); 
  }
  vector<string> output;
  std::unordered_set<string> visited;
  std::queue<string> Q;
  Q.push(source);
  visited.insert(source);

  while( !Q.empty() ) {
    string cur_airport = Q.front();
    Q.pop();
    output.push_back(cur_airport);
    vector<string> adj = getAdjacentAirports(cur_airport);
    for (string a : adj) {
      // Not adding visited airport
      if (visited.find(a) == visited.end()) {
        Q.push(a);
        visited.insert(a);
      }
    }
  }
  return output;
}

bool Graph::assertRouteExists(string source, string dest, string functionName) const {
  if (assertAirportExists(source, functionName) == false ||
      assertAirportExists(dest, functionName) == false) {
        // Let the assertAirportExists report the error message
        return false;
  }
  std::priority_queue<disPair, vector<disPair>, std::greater<disPair>> Q;
  const Airport& source_a = airport_map.find(source)->second;   // source_a is the Airport struct of the given source
  if (source_a.adjacent_airport.find(dest) == source_a.adjacent_airport.end()) {
    string message = "The route from " + source + " to " + dest +
                 " is not included in the graph. Called by " + functionName;
    printError(message);
    return false;
  }
  return true;
}

bool Graph::assertAirportExists(string IATA, string functionName) const {
  if (airport_map.find(IATA) == airport_map.end()) {
    if (functionName != "") {
      string message = "The airport " + IATA +
                       " is not included in the graph. Called by " + functionName;
      printError(message);
    }
    return false;
  }
  return true;
}

void Graph::printError(string message) const
{
    std::cerr << "\033[1;31m[Graph Error]\033[0m " + message << endl;
}

bool Graph::assertRouteExists(string source, string dest) const {
  // if (assertAirportExists(source, functionName) == false ||
  //     assertAirportExists(dest, functionName) == false) {
  //       // Let the assertAirportExists report the error message
  //       return false;
  // }
  std::priority_queue<disPair, vector<disPair>, std::greater<disPair>> Q;
  const Airport& source_a = airport_map.find(source)->second;   // source_a is the Airport struct of the given source
  if (source_a.adjacent_airport.find(dest) == source_a.adjacent_airport.end()) {
    string message = "The route from " + source + " to " + dest +
                 " is not included in the graph.";
    printError(message);
    return false;
  }
  return true;
}

bool Graph::assertAirportExists(string IATA) const {
  if (airport_map.find(IATA) == airport_map.end()) {
    string message = "The airport " + IATA +
                      " is not included in the graph.";
    printError(message);
    return false;
  }
  return true;
}