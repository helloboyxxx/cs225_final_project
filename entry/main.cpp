#include "graph.h"

#include <iostream>
#include <string>
#include <vector>

using std::string;


// You can create your own simple graph here.
void simple_graph(Graph& g) {
  Airport a(111, "A", "Chicago", "United States", 41.9786, -87.9048);
  Airport b(222, "B", "Las Vegas", "United States", 36.08010101, -115.1520004);
  Airport c(333, "C", "San Diego", "United States", 32.7336006165,-117.190002441);
  Airport d(444, "D", "Las Vegas", "United States", 36.08010101, -115.1520004);
  Airport e(555, "E", "San Diego", "United States", 32.7336006165,-117.190002441);

  g.insertAirport(a);
  g.insertAirport(b);
  g.insertAirport(c);
  g.insertAirport(d);
  g.insertAirport(e);

  g.insertRoute("A", "B", 10);
  g.insertRoute("A", "E", 10);
  g.insertRoute("B", "E", 10);
  g.insertRoute("B", "C", 11);
  g.insertRoute("C", "D", 10);
  g.insertRoute("E", "D", 10);

  // You can also call removeAirport and removeRoute if needed.
  // Please notice that these two functions takes airport ID as parameters.
}

int main()
{
    // The datasets we are using come from https://openflights.org/data.html
    string airport_filename = "../data/airports.txt";
    string route_filename = "../data/routes.txt";

    // Create the graph from given datasets. This will automatically generate vertices and edges
    Graph mygraph(airport_filename, route_filename);

    // This will give you a vector of adjacent airports of the given airport
    mygraph.getAdjacentAirports("ORD");

    // This will return a vector of all the airports included the graph
    mygraph.getAllAirports();

    string source = "ORD";
    string dest = "SZX";

    // This will return the shortest path from source to dest as a vector of airports(IATA)if exists
    mygraph.shortestPath(source, dest);
    
    // This will return the shortest paths from source to all other airports. 
    // The results reads similar to shortestPath where each path is stored in the output as a vector
    mygraph.allShortestPath(source);

    // This calculates the distance between two aiports.
    mygraph.getDistance("ORD", "PEK");

    // calling this function when you don't want to read frequency from existing
    mygraph.clearFreqFile();

    // This calculates the centrality scores of each aiport in the graph
    mygraph.calcFrequency();

    // After running calcFrequency, this will return the centrality score of the given airport
    mygraph.getFrequency("ORD");

    return 0;
}