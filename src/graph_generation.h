#pragma once

#include "structures.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <cmath>

/*this is the class reading file and generate the graph
* and write results to files
*/

class Generator {
public:
    static std::unordered_map<std::string, Airport> readFromFile(std::string airport_filename, std::string route_filename);

private:
    static double distance_between(Airport a, Airport b);
};

/**
 * Helper function for graph generation. This function calculates 
 * the distance between to aiports given the latitudes and longitudes of the airports.
 * This calculation uses the Haversine formula for calculating distance between two points on a sphere.
 * @param lat1 - latitude of the first airport
 * @param long1 - longitude of the first airport
 * @param lat2 - latitude of the second airport
 * @param long2 - longitude of the second airport
 * @return the distance between the two airports in kilometers
*/
double calculateDistance(double lat1, double long1, double lat2, double long2);

/**
 * Helper function for calculateDistance function.
 * This function converts degree into radians.
 * @param degree - degree that is to be converted
 * @return the result of convertion
*/
double toRadians(const double degree);