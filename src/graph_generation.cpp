#include "graph_generation.h"
#include <math.h>

std::unordered_map<std::string, airport> Generator::readFromFile(std::string airport_filename, std::string route_filename) {
    std::fstream airport_file(airport_filename);
    std::fstream route_file(route_filename);
    if (!airport_file.is_open() || !route_file.is_open()) {
        throw std::runtime_error("invalid file");
    }
    //initialize the output map
    std::unordered_map<std::string, airport> map;

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
        airport ap(info[4], info[2], info[3], std::stod(info[7]), std::stod(info[6]), std::stod(info[8]));
        map[info[4]] = ap;
    }

    return map;
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