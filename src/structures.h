#pragma once

#include <unordered_map>
#include <string>
#include <iostream>


/**
 * This is a struct that holds route information of each airport, including number of flights and distance of travel
*/
struct Route {
    Route() {}
    Route(double distance) {
        num_flight = 1;
        this->distance = distance;
    }
    unsigned num_flight = 0;
    double distance;
};

/**
 * This is a struct that holds all the information requried for an airport.
 * It also includes the information about the edge (maybe?).
*/
struct Airport {
    // 3-letter IATA code
    Airport() {}
    Airport(std::string _IATA, std::string _city, std::string _country, double _lo, double _la, double _al) {
        IATA = _IATA;
        city = _city;
        country = _country;
        longitude = _lo;
        latitude = _la;
        altitude = _al;
    }
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
    std::unordered_map< std::string, Route> adjacent_airport;
};