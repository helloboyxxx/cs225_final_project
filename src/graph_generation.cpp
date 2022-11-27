#include "graph_generation.h"
#include "structures.h"
#include <fstream>
#include <ostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>

void Generator::readFromFile(std::string airport_filename, std::string route_filename, std::unordered_map<unsigned, Airport>& map) {
    std::fstream airport_file(airport_filename);
    std::fstream route_file(route_filename);
    if (!airport_file.is_open() || !route_file.is_open()) {
        throw std::runtime_error("invalid file");
    }

    // generate all vertex (airport)
    std::string line;
    while (std::getline(airport_file, line)) {
        std::vector<std::string> info;
        std::string each_info;
        std::stringstream ss(line);
        while (getline(ss, each_info, ',')) {
            info.push_back(each_info);
        }
        if (info.size() != 14) continue;

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
        Airport ap(
            std::stoul(info[0]),                        // ID
            info[4].substr(1, (info[4]).size() - 2),    // IATA
            info[2].substr(1, (info[2]).size() - 2),    // City
            info[3].substr(1, (info[3]).size() - 2),    // Country
            std::stod(info[7]), std::stod(info[6])      // latitude and longtitude
        );
        map[ std::stoul(info[0]) ] = ap;
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
        if (info[3] == "\\N") continue;
        if (info[5] == "\\N") continue;
        unsigned source_id = std::stoul(info[3]);
        unsigned dest_id = std::stoul(info[5]);
        if (map.find(source_id) == map.end()) continue;
        if (map.find(dest_id) == map.end()) continue;

        Airport& departure = map[source_id];
        Airport& destination = map[dest_id];
        
        //record 1 flight out (edge out)
        departure.route_out += 1;
        //record 1 flight in (edge in)
        destination.route_in += 1;

        //generate the route (edge)
        if (departure.adjacent_airport.find(dest_id) == departure.adjacent_airport.end()) {
            Route this_flight(calculateDistance(departure.latitude, departure.longitude, destination.latitude, destination.longitude));
            departure.adjacent_airport[dest_id] = this_flight;
        } else {
            departure.adjacent_airport[dest_id].num_flight += 1;
        }
    }
    auto it = map.begin();
    while (it != map.end()) {
        if (it->second.route_in == 0 && it->second.route_out == 0) {
            map.erase(it++);
        } else {
            ++it;
        }
    }
}

// void writeToFile(std::string filename, std::unordered_map<std::string, Airport> map) {
//     //create file
//     std::fstream file;
//     file.open(filename);
//     file << "-Country~City~Departure Airport  --Destination Airport~~~distance between \n" << std::endl;

//     for (auto it : map) {
//         Airport airport = it.second;
//         file << airport.country<<" - "<< airport.city<<" - "<<it.first<<std::endl;
//         for (auto des : airport.adjacent_airport) {
//             file<<"        "<<des.first<<" ~~~ "<<des.second.distance<<std::endl;
//         }
//     }
//     file.close();
// }


double calculateDistance(double lat1, double long1, double lat2, double long2) {
    double dlat1 = toRadians(lat1);
    double dlat2 = toRadians(lat2);

    double latDist = dlat2 - dlat1;
    double longDist = toRadians(long2 - long1);

    // Radius of the Earth in km
    double R = 6378.1;

    double distance = pow(sin(latDist / 2), 2) + cos(dlat1) * cos(dlat2) * pow(sin(longDist / 2), 2);
    distance = 2 * R * asin(sqrt(distance));

    return distance;
}

double toRadians(const double degree) {
    return degree * M_PI / 180;
}