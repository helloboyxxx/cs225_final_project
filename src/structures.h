#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

const double kInvalidDouble = -114514;

/**
 * This is a struct that holds route information of each airport, including number of flights and distance of travel
*/
struct Route {
    Route() = default;
    Route(double dist) : num_flight(1), distance(dist) {}

    unsigned num_flight = 0;
    double distance = -1;
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
    Airport() = default;

    Airport(unsigned _ID, std::string _IATA, std::string _city, std::string _country, double _lo, double _la) : 
        ID(_ID), IATA(_IATA), city(_city), country(_country), longitude(_lo), latitude(_la) {}
    
    // airport id. 0 is an invalid id. 
    unsigned ID = 0;

    // Three letter string
    std::string IATA;

    // the city the airport belongs to
    std::string city;
    
    // the country the airport belongs to
    std::string country;

    //number of flights arriving 
    unsigned route_in = 0;
    //number of flights departing
    unsigned route_out = 0;

    double longitude = kInvalidDouble;
    double latitude = kInvalidDouble;

    // Storing the number of shortest path that passes through this airport
    unsigned frequency = 0;

    /**
    * Key: IATA of adjacent airports, value: distance of this route
    */
    std::unordered_map<unsigned, Route> adjacent_airport;
};

