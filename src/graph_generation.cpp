#include "graph_generation.h"
#include "structures.h"
#include <cmath>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

std::unordered_map<std::string, Airport> Generator::readFromFile(std::string airport_filename, std::string route_filename) {
    std::fstream airport_file(airport_filename);
    std::fstream route_file(route_filename);
    if (!airport_file.is_open() || !route_file.is_open()) {
        throw std::runtime_error("invalid file");
    }
    //initialize the output map
    std::unordered_map<std::string, Airport> map;

    // generate all vertex (airport)
    std::string line;
    while (std::getline(airport_file, line)) {
        std::vector<std::string> info;
        std::string each_info;
        std::stringstream ss(line);
        while (getline(ss, each_info, ',')) {
            info.push_back(each_info);
        }
        // each info contains the following info in order:
        // 0: Airport ID 
        // 1: Name 
        // 2: City
        // 3: Country
        // 4: IATA
        // 5: ICAO
        // 6: Latitude
        // 7: Longitude
        // 8: Altitude
        // 9: Timezone
        // 10: DST
        // 11: Tz database time zone
        // 12: Type
        // 13: Source
        Airport ap(info[4], info[2], info[3], std::stod(info[7]), std::stod(info[6]), std::stod(info[8]));
        map[info[4]] = ap;
    }

    // generate all edge (route)
    while (std::getline(route_file, line)) {
        std::vector<std::string> info;
        std::string each_info;
        std::stringstream ss(line);
        while(getline(ss, each_info, ',')) {
            info.push_back(each_info);
        }
        // each info contains the following info in order:
        // 0: Airline
        // 1: Airline ID
        // 2: Source Airport    (departure)
        // 3: Source Airport ID
        // 4: Destination Airport
        // 5: Destination Airport ID
        // 6: Codeshare
        // 7: Stops
        // 8: Equipment

        //skip if this is NOT non-stop flight
        if (info[7] != "0") continue;
        //skip if this is NOT an recorded airport   either departure or destination
        if (map.find(info[2]) == map.end()) continue;
        if (map.find(info[4]) == map.end()) continue;

        Airport& departure = map[info[2]];
        Airport& destination = map[info[4]];
        
        //record 1 flight out (edge out)
        departure.route_out += 1;
        //record 1 flight in (edge in)
        destination.route_in += 1;

        //generate the route (edge)
        if (departure.adjacent_airport.find(info[4]) == departure.adjacent_airport.end()) {
            Route this_flight(distance_between(departure, destination));
            departure.adjacent_airport[info[4]] = this_flight;
        } else {
            departure.adjacent_airport[info[4]].num_flight += 1;
        }
    }

    //@TODO
    //erase airports (vertex) that has no flight in and out

    //@TODO
    //update frequency for each airport (centrality)

    //@TODO
    //Based on frequency, choose edges that would produce Eulerian Path

    return map;
}

double Generator::distance_between(Airport a, Airport b) {
    double d = pow(a.altitude - b.altitude, 2) + pow(a.longitude - b.longitude, 2) + pow(a.latitude - b.latitude, 2);
    return std::sqrt(d);
}