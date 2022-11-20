#include "graph_generation.h"
#include "structures.h"
#include <cmath>
#include <fstream>
#include <ostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

void Generator::readFromFile(std::string airport_filename, std::string route_filename, std::unordered_map<std::string, Airport>& map) {
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
        // std::string data = "";
        // bool start = false;
        // for (std::string::iterator c = line.begin(); c != line.end(); c++) {
        //     if (*c == ',' && start == false) {
        //         if (data.empty()) continue;
        //         info.push_back(data);
        //         data.clear();
        //         continue;
        //     }
        //     if (data.empty() && *c == '"') {
        //         start = true;
        //         continue;
        //     }
        //     if (start && *c == '"') {
        //         start = false;
        //         info.push_back(data);
        //         data.clear();
        //         continue;
        //     }
        //     data += *c;
        // }

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
            info[4].substr(1, (info[4]).size() - 2),    
            info[2].substr(1, (info[2]).size() - 2), 
            info[3].substr(1, (info[3]).size() - 2),
            std::stod(info[7]), std::stod(info[6]), std::stod(info[8]));
        map[info[4].substr(1, (info[4]).size() - 2)] = ap;
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
    auto it = map.begin();
    while (it != map.end()) {
        if (it->second.route_in == 0 && it->second.route_out == 0) {
            map.erase(it++);
        } else {
            ++it;
        }
    }

}

void writeToFile(std::string filename, std::unordered_map<std::string, Airport> map) {
    //create file
    std::fstream file;
    file.open(filename);
    file << "-Country~City~Departure Airport  --Destination Airport~~~distance between \n" << std::endl;

    for (auto it : map) {
        Airport airport = it.second;
        file << airport.country<<" - "<< airport.city<<" - "<<it.first<<std::endl;
        for (auto des : airport.adjacent_airport) {
            file<<"        "<<des.first<<" ~~~ "<<des.second.distance<<std::endl;
        }
    }
    file.close();
}

double Generator::distance_between(Airport a, Airport b) {
    double d = pow(a.altitude - b.altitude, 2) + pow(a.longitude - b.longitude, 2) + pow(a.latitude - b.latitude, 2);
    return std::sqrt(d);
}

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