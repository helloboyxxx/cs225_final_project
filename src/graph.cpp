#include "graph.h"
#include "graph_generation.h"
#include "structures.h"
#include <cfloat>
#include <string>
#include <unordered_map>
#include <limits>
#include <queue>
#include <unordered_set>

using std::cout;
using std::endl;

/*
double distance, string IATA. Used in calcPrevious
Distance placed in the front since
operator> in std::pair will compare the first element by defaut */
typedef std::pair<double, string> disPair;


Graph::Graph(string airport_filename, string route_filename) {
  airport_map = Generator::readFromFile(airport_filename, route_filename);
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
  // Use these three lines when debugging. (Since it is easier to understand.)
  // I included the optimized version below. Uncomment to run. 
  if ( assertRouteExists(source, dest, __func__) == false ) { return -1; }
  const Airport& source_a =  airport_map.find(source)->second;  // source_a is the Airport struct of the given source
  return source_a.adjacent_airport.find(dest)->second.distance; // find the adjacent airport's route and return distance from it

  // // Optimized version bellow:
  // const auto& source_it = airport_map.find(source);   // iterator that points to the source in airport_map
  // // Check if the given sources exists
  // if (source_it == airport_map.end()) {
  //   cout << "The airport " << source << " is not included in the graph." << endl;
  //   return -1;
  // } else if (airport_map.find(dest) == airport_map.end()) {
  //   cout << "The airport " << dest << " is not included in the graph." << endl;
  //   return -1;
  // }

  // // iterator that points to the dest in the adjacent_airport. Reminder: value is a Route struct
  // const auto& dest_it = source_it->second.adjacent_airport.find(dest); 
  // if (dest_it == source_it->second.adjacent_airport.end()) {
  //   cout << "The route from " << source << " to " << dest << " is not included in the graph.";
  //   return -1;
  // } 

  // // Here we must have a route from source to dest
  // return dest_it->second.distance;
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
  return vector<string>();
}

vector<vector<string>> Graph::allShortestPath(string source) const {
  return vector<vector<string>>();
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
  for (auto dist : distances) {
    Q.push({dist.second, dist.first});    // {double distance, string IATA}
  }
  
  // Dijkstra starts
  while ( !Q.empty() ) {
    string cur_airport = Q.top().second;
    Q.pop();
    visited.insert(cur_airport);
    
    // Loop through the neighbors of this airport
    for (auto neighbor : getAdjacentAirports(cur_airport)) {
      if (visited.find(neighbor) != visited.end())
        continue;   // skip when visited this airport before
      
      double cur_distance = getDistance(cur_airport, neighbor);
      if (cur_distance + distances[cur_airport] < distances[neighbor]) {
        distances[neighbor] = cur_distance + distances[cur_airport];
        previous[neighbor] = cur_airport;
      }
    }
  }
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