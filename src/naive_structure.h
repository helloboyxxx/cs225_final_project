#include <unordered_map>
#include <string>

/**
 * This is a struct that holds all the information requried for an airport.
 * It also includes the information about the edge (maybe?).
*/
struct airport {
  std::string IATA;   // 3-letter IATA code

  // Storing the number of shortest path that passes through this airport
  unsigned frequency;
  /**
   * Key: IATA of adjacent airports, value: distance of this route
  */
  std::unordered_map< std::string, double > adjacent_airport;
};


/**
 * This should be the main entry of our data structure
 * Key: each airport's IATA, value: airport struct
*/
std::unordered_map<std::string, airport> airports;


// We need to implement all the function below:
/**
 * Given an IATA string of an airport, this function should add this airport to our graph
 * @param IATA a 3-letter string that represents an airport
*/
void insertVertex(std::string IATA);   // Expected complexity O(1)


/**
 * @param IATA_1 a 3-letter string that represents the source airport
 * @param IATA_2 a 3-letter string that represents the destination airport
*/
void insertEdge(std::string IATA_1, std::string IATA_2);


/**
 * Remove the airport and the adjacent routes
 * Maybe not neccessary?
 * @param IATA 3-letter string
*/
void removeVertex(std::string IATA);





/**
 * Question 1:
 * How do we know which airline to take given the source and destination airport?
 * Ex: From Chicago to Iceland, which airline(s) should I take to get there?
 * Do we want to add this feature in our program? 
 * 
 * Sam's suggestion for Q1: 
 * Maybe storing <distance, airline_ID> instead of simply storing distance?
 * 
 * 
 * Question 2:
 * Notice that we have only about 3321 airports in the route dataset. 
 * 
*/




