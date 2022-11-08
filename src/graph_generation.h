#pragma once

#include "structures.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <stdexcept>

/*this is the class reading file and generate the graph
* and write results to files
*/

class Generator {
    public:
    static std::unordered_map<std::string, airport> readFromFile(std::string airport_filename, std::string route_filename);
};