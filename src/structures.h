#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

const double InvalidDouble = -114514;

/**
 * This is a struct that holds route information of each airport, including number of flights and distance of travel
*/
struct Route {
    Route() : num_flight(0), distance(-1) { }
    Route(double dist) : num_flight(1), distance(dist) { }
    unsigned num_flight;
    double distance;
};

/**
 * This is a struct that holds all the information requried for an airport.
 * It also includes the information about the edge (maybe?).
*/
struct Airport {
    /**
    Default constructor will use initialize this struct with some invalid values:
    IATA, city, country will be empty string.
    latitude, longitude, altitude all have special value -114514.
    route_in, route_out, frequency = 0 by default
    */
    Airport()
        : IATA(""), city(""), country(""), longitude(InvalidDouble),
          latitude(InvalidDouble), altitude(InvalidDouble), frequency(0) {}

    Airport(std::string _IATA, std::string _city, std::string _country, double _lo, double _la, double _al) : 
        IATA(_IATA), city(_city), country(_country), longitude(_lo), latitude(_la), altitude(_al), frequency(0) {  }
    
    std::string IATA;

    // the city the airport belongs to
    std::string city;
    
    // the country the airport belongs to
    std::string country;

    //number of flights arriving 
    unsigned route_in = 0;
    //number of flights departing
    unsigned route_out = 0;

    double longitude;
    double latitude;
    double altitude;    

    // Storing the number of shortest path that passes through this airport
    unsigned frequency;

    /**
    * Key: IATA of adjacent airports, value: distance of this route
    */
    std::unordered_map<std::string, Route> adjacent_airport;
};