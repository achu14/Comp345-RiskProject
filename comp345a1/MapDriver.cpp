#include "Map.h"
#include <iostream>
#include <vector>


// The testLoadMaps function.
void testLoadMaps(const std::vector<std::string> &filenames) {
    // Vector that stores valid map objects 
    std::vector<std::pair<Map *, std::string>> maps;

    // Load and validate maps
    for (const auto &filename: filenames) {
        Map *map = MapLoader::loadMap(filename);

        // 1) the map is a connected graph 
        // 2) continents are connected sub-graphs
        // 3) each country belongs to one and only one continent.
        if (map->validate()) {
            maps.emplace_back(map, filename);
        } else {
            std::cout << "The map contained in the file " + filename + " was not valid." << std::endl;
            delete map;
        }
    }

    if (maps.empty()) {
        std::cout << "No valid maps were provided" << std::endl;
        return;
    }

    // Display info on validated maps
    std::cout << "The following maps were valid: " << std::endl << std::endl;
    for (const auto &map: maps) {
        std::cout << "Name: " + map.second << std::endl << "\tAuthor: " + map.first->getMapInfo()["author"]
                  << std::endl;
        std::cout << "\tNumber of Continents: " + std::to_string(map.first->getContinents()->size()) << std::endl;
        std::cout << "\tNumber of Territories: " + std::to_string(map.first->getTerritories()->size()) << std::endl;
        std::cout << std::endl;

    }

    // Cleanup Stack
    for (const auto &mapPair: maps) {
        delete mapPair.first;  // delete the Map object
    }

}

