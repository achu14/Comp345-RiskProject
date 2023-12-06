#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <utility>


class Territory {
public:
    Territory();

// Constructor
    Territory(std::string id, int x, int y, std::string continent, std::vector<std::string> adjacent);

    // Destructor
    ~Territory();

    // Copy Constructor
    Territory(const Territory &other);

    // Overload assignment operator
    Territory &operator=(const Territory &other);

    // Allow access of private members
    friend std::ostream &operator<<(std::ostream &os, const Territory &territory);

    // Getter functions
    std::string *getId() const ;

    int *getX() const;

    int *getY() const;

    std::string *getContinent();

    int *getArmies() const;

    void *setArmies(int *armies);

    std::string *getOwner();

    std::vector<std::string> *getAdjacent();

    // Validation
    bool visited = false;

    // Removes a territory
    void removeTerritory(Territory *territory);

// Sets owner
    void *setOwner(std::string *ownerT);

private:
    // Name of territory
    std::string *id;
    int *x;
    int *y;
    std::string *continent;
    // Number of armies present in the territory
    int *armies;
    // Player who owns the territory
    std::string *owner;
    // Vector that stores pointers to adjacent territories
    std::vector<std::string> *adjacent;

    // Vector that stores pointers to territories vector
    std::vector<Territory *> *territories{};

    // Vector that gets territories
    std::vector<Territory *> *getTerritories();

};

// Function definition for stream insertion operator
std::ostream &operator<<(std::ostream &os, const Territory &territory);

class Continent {
public:
    // Constructor
    Continent(std::string *name, const int *bonus);

    // Copy Constructor
    Continent(const Continent &other);

    // Overload assignment operator
    Continent &operator=(const Continent &other);

    // Allow access of private members
    friend std::ostream &operator<<(std::ostream &os, const Continent &continent);

    friend class Map;

    // other member functions
    void addTerritory(Territory *territory);

    // Getter functions
    std::string *getName();

    int *getBonus() const;

    std::vector<Territory *> *getTerritories();

private:
    // Name of Continent
    std::string *name;
    // Bonus total control
    int *bonus;
    // Vector that stores pointers to territories that the continent contains
    std::vector<Territory *> *territories{};
};

// Function definition for stream insertion operator
std::ostream &operator<<(std::ostream &os, const Continent &continent);

class Map {
public:
    // Constructor
    Map();

    // Destructor
    ~Map();

    // Copy Constructor
    Map(const Map &other);

    // Overload assignment operator
    Map &operator=(const Map &other);

    // Allow access of private members
    friend std::ostream &operator<<(std::ostream &os, const Map &map);

    // other member functions
    void addContinent(const std::string &name, int value);

    void addTerritory(const std::string &id, int x, int y, const std::string &continent,
                      std::vector<std::string> adjacentTerritories);

    void addMapInfo(const std::string &key, const std::string &value);

    void printMap();

    bool territoryExist(std::string territory);

    // Getter functions
    Territory *getTerritoryById(const std::string &id);

    std::unordered_map<std::string, std::string> getMapInfo();

    std::vector<Continent *> *getContinents();

    std::unordered_map<std::string, Territory *> *getTerritories();

    // Validation
    bool validate();


private:
    // Map Info
    std::string *filename{};
    std::string *author{};
    std::string *warn{};
    std::string *image{};
    std::string *wrap{};
    std::string *scroll{};

    // Each Continent object pointer in the vector represents a continent in the map
    std::vector<Continent *> *continents{};
    // It is an unordered map where the key is the name of the territory
    // and the value is a pointer to the corresponding Territory object
    std::unordered_map<std::string, Territory *> *territories{};

    void dfs(Territory *territory);

};

std::ostream &operator<<(std::ostream &os, const Map &map);

class MapLoader {
public:
    static void handleMapInfo(std::istringstream &iss, Map *map);

    static void handleContinents(std::istringstream &iss, Map *map);

    static void handleTerritories(std::istringstream &iss, Map *map);

    static Map *loadMap(const std::string &filename);
};
