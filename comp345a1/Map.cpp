#include "Map.h"
//#include <utility>


// Implementations for Territory

// Constructor
Territory::Territory(std::string id, int x, int y, std::string continent, std::vector<std::string> adjacent)
        : id(new std::string(std::move(id))),
          x(new int(x)),
          y(new int(y)),
          continent(new std::string(std::move(continent))),
          armies(new int(0)),
          owner(new std::string("")),
          adjacent(new std::vector<std::string>(std::move(adjacent))) {}

// Destructor
Territory::~Territory() {
    delete id;
    delete x;
    delete y;
    delete continent;
    delete armies;
    delete owner;
    delete adjacent;

}

// Copy Constructor deep copy
Territory::Territory(const Territory &other) {
    // Deep copy for string pointers
    id = new std::string(*(other.id));
    continent = new std::string(*(other.continent));
    owner = new std::string(*(other.owner));

    // Deep copy for integer pointers
    x = new int(*(other.x));
    y = new int(*(other.y));
    armies = new int(*(other.armies));

    // Deep copy for the vector pointer
    adjacent = new std::vector<std::string>(*(other.adjacent));

    // Simple copy for non-pointer members
    visited = other.visited;
}

Territory &Territory::operator=(const Territory &other) {
    if (this != &other) {
        // 1. Delete old data.
        delete id;
        delete x;
        delete y;
        delete continent;
        delete armies;
        delete owner;
        delete adjacent;

        // 2. Deep copy new data.
        id = new std::string(*(other.id));
        continent = new std::string(*(other.continent));
        owner = new std::string(*(other.owner));

        x = new int(*(other.x));
        y = new int(*(other.y));
        armies = new int(*(other.armies));

        adjacent = new std::vector<std::string>(*(other.adjacent));

        // 3. Copy non-pointer members.
        visited = other.visited;
    }
    return *this;
}

std::string *Territory::getId() const {
    return id;
}

std::vector<Territory *> *Territory::getTerritories() {
    return territories;
}

int *Territory::getX() const {
    return x;
}

int *Territory::getY() const {
    return y;
}

std::string *Territory::getContinent() {
    return continent;
}

int *Territory::getArmies() const {
    return armies;
}

void *Territory::setArmies(int *armiesNum) {
    armies = armiesNum;
    return nullptr;
}

void *Territory::setOwner(std::string *ownerT) {
    owner = ownerT;
    return nullptr;
}


std::string *Territory::getOwner() {
    return owner;
}

std::vector<std::string> *Territory::getAdjacent() {
    return adjacent;
}

std::ostream &operator<<(std::ostream &os, const Territory &territory) {
    os << *(territory.id) << " (" << *(territory.armies) << " armies, owned by " << *(territory.owner) << ")";
    return os;
}

Territory::Territory() {}

// Implementations for Continent

// Constructor
Continent::Continent(std::string *n, const int *b) {
    // Allocate memory and initialize member variables
    name = new std::string(*n);
    bonus = new int(*b);
    territories = new std::vector<Territory *>();
}

// Copy constructor deep copy
Continent::Continent(const Continent &other) {
    // Deep copy the name
    name = new std::string(*(other.name));
    // Deep copy the bonus
    bonus = new int(*(other.bonus));
    // Deep copy the territories vector
    territories = new std::vector<Territory *>;
    for (const auto &territory: *(other.territories)) {
        territories->push_back(new Territory(*territory));  // Assuming Territory has a proper copy constructor.
    }
}

Continent &Continent::operator=(const Continent &other) {
    if (this != &other) {
        // Clean up existing data
        delete name;
        delete bonus;
        for (auto &territory: *territories) {
            delete territory;
        }
        delete territories;

        // Deep copy name
        name = new std::string(*(other.name));

        // Deep copy bonus
        bonus = new int(*(other.bonus));

        // Deep copy territories
        territories = new std::vector<Territory *>;
        for (const auto &territory: *(other.territories)) {
            territories->push_back(new Territory(*territory));  // Assuming Territory has a proper copy constructor.
        }
    }
    return *this;
}


void Continent::addTerritory(Territory *territory) {
    territories->push_back(territory);
}

std::string *Continent::getName() {
    return name;
}

int *Continent::getBonus() const {
    return bonus;
}

std::vector<Territory *> *Continent::getTerritories() {
    return territories;
}

std::ostream &operator<<(std::ostream &os, const Continent &continent) {
    os << continent.name;
    return os;
}

// Implementations for Map
Map::Map()
        : author(new std::string("")),
          warn(new std::string("")),
          image(new std::string("")),
          wrap(new std::string("")),
          scroll(new std::string("")),
          continents(new std::vector<Continent *>()),
          territories(new std::unordered_map<std::string, Territory *>()) {}

// Copy Constructor for deep copy
Map::Map(const Map &other)
        : filename(new std::string(*(other.filename))),
          author(new std::string(*(other.author))),
          warn(new std::string(*(other.warn))),
          image(new std::string(*(other.image))),
          wrap(new std::string(*(other.wrap))),
          scroll(new std::string(*(other.scroll))) {

    // Assumes copy constructor Continent class
    for (const auto &continent: *other.continents) {
        this->continents->push_back(new Continent(*continent));
    }
    // Assumes copy constructor Territory class
    for (const auto &pair: *(other.territories)) {
        (*this->territories)[pair.first] = new Territory(*(pair.second));
    }
}

// Destructor to clean up dynamically allocated memory
Map::~Map() {
    // Deallocate memory for continents
    for (Continent *continent: *continents) {
        delete continent;
    }

    // Deallocate memory for territories
    for (auto &pair: *territories) {
        delete pair.second;
    }

    // Deallocate memory for other dynamically allocated members
    delete author;
    delete warn;
    delete image;
    delete wrap;
    delete scroll;
}

Map &Map::operator=(const Map &other) {
    if (this != &other) {
        // Clean up existing resources
        for (auto &continent: *continents) {
            delete continent;
        }
        for (auto &pair: *territories) {
            delete pair.second;
        }

        continents->clear();
        territories->clear();

        // Perform deep copy from other
        for (const auto &continent: *other.continents) {
            this->continents->push_back(new Continent(*continent)); // Assuming Continent has a proper copy constructor
        }
        for (const auto &pair: *other.territories) {
            (*this->territories)[pair.first] = new Territory(
                    *(pair.second)); // Assuming Territory has a proper copy constructor
        }
    }
    return *this;
}

void Map::addContinent(const std::string &name, int bonus) {
    // Check if continent with same name already exists
    for (const auto &continent: *continents) {
        if (*(continent->getName()) == name) {
            throw std::runtime_error("Continent already exists: " + name);
        }
    }

    // Create a new continent and add to vector
    auto *newContinent = new Continent(new std::string(name), new int(bonus));
    continents->push_back(newContinent);
}

void Map::addTerritory(const std::string &id, int x, int y, const std::string &continentName,
                       std::vector<std::string> adjacentTerritories) {
    auto *newTerritory = new Territory(id, x, y, continentName, std::move(adjacentTerritories));

    (*territories)[id] = newTerritory;

    for (Continent *continent: *continents) {
        if (*(continent->getName()) == continentName) {
            continent->addTerritory(newTerritory);
            break;
        }
    }
}

void Territory::removeTerritory(Territory *territory) {
    if (territory = nullptr) {
    } else {
        std::cout << *(territory->getId()) << " has no more armies so it is now removed from the game";
        auto it = std::find(territory->getTerritories()->begin(), territory->getTerritories()->end(), territory);
        if (it != territory->getTerritories()->end()) {
            territory->getTerritories()->erase(it);
        }
    }
}

void Map::addMapInfo(const std::string &key, const std::string &value) {
    if (key == "author") {
        *author = value;
    } else if (key == "warn") {
        *warn = value;
    } else if (key == "image") {
        *image = value;
    } else if (key == "wrap") {
        *wrap = value;
    } else if (key == "scroll") {
        *scroll = value;
    }
}

Territory *Map::getTerritoryById(const std::string &id) {
    return (*territories)[id];
}

std::unordered_map<std::string, std::string> Map::getMapInfo() {
    std::unordered_map<std::string, std::string> mapInfo;

    mapInfo["author"] = *author;
    mapInfo["warn"] = *warn;
    mapInfo["image"] = *image;
    mapInfo["wrap"] = *wrap;
    mapInfo["scroll"] = *scroll;

    return mapInfo;
}

std::vector<Continent *> *Map::getContinents() {
    return continents;
}

std::unordered_map<std::string, Territory *> *Map::getTerritories() {
    return territories;
}

bool Map::validate() {
    // Assume all territories are unvisited initially.
    for (auto &pair: *territories) {
        pair.second->visited = false;
    }

    // 1) Check if the map is a connected graph.
    dfs(territories->begin()->second); // Start DFS from the first territory.
    for (auto &pair: *territories) {
        if (!pair.second->visited) {
            return false; // If any territory is unvisited, the graph is not connected.
        }
    }

    // 2) Check if continents are connected subgraphs.
    for (auto &pair: *territories) {
        pair.second->visited = false;
    }
    for (auto &pair: *territories) {
        if (!pair.second->visited) {
            std::string continent = *(pair.second->getContinent());
            dfs(pair.second);
            // After DFS, all territories in the same continent should be visited.
            for (auto &p: *territories) {
                if (*(p.second->getContinent()) == continent && !p.second->visited) {
                    return false;
                }
            }
        }
    }

    // 3) Check if each territory belongs to one and only one continent.
    std::unordered_set<std::string> territorySet;

    // Iterate over each continent
    for (const auto &continent: *continents) {
        // Get each continent's territories
        const auto &territoriesInContinent = continent->getTerritories();

        // Insert returns a pair where the second element is a boolean
        // that is true if the insert took place (element is unique in set)
        for (const auto &territory: *territoriesInContinent) {
            if (!territorySet.insert(*(territory->getId())).second) {
                return false;
            }
        }
    }

    return true;
}

// Suppress clang recursion warning
#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void Map::dfs(Territory *territory) {
    territory->visited = true;
    for (const std::string &id: *territory->getAdjacent()) {
        Territory *neighbor = getTerritoryById(id);
        if (neighbor && !neighbor->visited) {
            dfs(neighbor);
        }
    }
}

#pragma clang diagnostic pop


std::ostream &operator<<(std::ostream &os, const Map &map) {
    os << "Map Details:" << std::endl;

    // Iterating over the continents and printing their details
    for (const auto &continent: *map.continents) {
        // Assume Continent has getName and getBonus methods to get name and bonus
        os << "Continent: " << continent->getName()
           << ", Bonus: " << continent->getBonus() << std::endl;

        // Iterating over the territories in the continent and printing their details
        for (const auto &territory: *continent->getTerritories()) {
            // Assume Territory has getName and getArmies methods to get name and number of armies
            os << "\tTerritory: " << territory->getId()
               << ", Xcord: " << territory->getX() << ", Ycord: " << territory->getY() << ", Continent: "
               << territory->getContinent() << ", adjacent: ";
            for (const auto &adj: *territory->getAdjacent()) {
                os << adj << ", ";
            }
            os << std::endl;
        }
    }
    return os;
}

// Prints names of each continent and territory in map
void Map::printMap() {
    std::cout << "These are the continents and territories from the map." << std::endl;
    for (auto continent: *getContinents()) {
        std::cout << "--------------------------------------------" << std::endl;
        std::cout << "Continent " << *(continent->getName()) << " contains these territories: " << std::endl;
        for (auto territory: *continent->getTerritories()) {
            std::cout << *(territory->getId()) << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Map::territoryExist(std::string territoryT) {
    for (auto continent: *getContinents()) {
        for (auto territory: *continent->getTerritories()) {
            if (*(territory->getId()) == *(new std::string(territoryT))) {
                return true;
            }
        }
    }
    return false;
}

// Implementations for MapLoader

// Handle parsing Map Info
void MapLoader::handleMapInfo(std::istringstream &iss, Map *map) {
    std::string key, value;
    std::getline(iss, key, '=');
    std::getline(iss, value);
    map->addMapInfo(key, value);
}

// Handle parsing Continents
void MapLoader::handleContinents(std::istringstream &iss, Map *map) {
    std::string name;
    int value;
    std::getline(iss, name, '=');
    iss >> value;
    map->addContinent(name, value);
}

// Handle parsing Territories
void MapLoader::handleTerritories(std::istringstream &iss, Map *map) {
    std::string id;
    int x, y;
    std::string continent;
    std::getline(iss, id, ',');
    iss >> x;
    iss.ignore();
    iss >> y;
    iss.ignore();
    std::getline(iss, continent, ',');

    std::vector<std::string> adjacentTerritories;
    std::string adjacent;
    while (std::getline(iss, adjacent, ',')) {
        adjacentTerritories.push_back(adjacent);
    }
    map->addTerritory(id, x, y, continent, adjacentTerritories);
}

// Load Map function
Map *MapLoader::loadMap(const std::string &filename) {
    Map *map = new Map();
    std::ifstream file(filename);

    if (!file.is_open()) {
        delete map;
        throw std::runtime_error("Could not open file " + filename);
    }

    std::string line;
    bool readingContinents = false, readingTerritories = false, readingMapInfo = false;
    std::string author, warn, image, wrap, scroll;

    while (std::getline(file, line)) {
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        if (line.empty()) continue;

        if (line == "[Map]") {
            readingMapInfo = true;
            readingContinents = false;
            readingTerritories = false;
            continue;
        }
        if (line == "[Continents]") {
            readingMapInfo = false;
            readingContinents = true;
            readingTerritories = false;
            continue;
        } else if (line == "[Territories]") {
            readingMapInfo = false;
            readingContinents = false;
            readingTerritories = true;
            continue;
        }

        std::istringstream iss(line);
        if (readingMapInfo) {
            handleMapInfo(iss, map);
        }
        if (readingContinents) {
            handleContinents(iss, map);
        } else if (readingTerritories) {
            handleTerritories(iss, map);
        }
    }
    file.close();
    return map;
}
