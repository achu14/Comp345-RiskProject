#ifndef COMP345A1_PLAYER_H
#define COMP345A1_PLAYER_H

#pragma once

#include <list>
#include <iostream>
#include <vector>
#include "Orders.h"
#include "Map.h"
#include "PlayerStrategies.h"

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::vector;
using std::list;

class Order;

class OrdersList;

class PlayerStrategy;

class GameEngineLoop;

class Player {
public:
    Player(string *name);

    Player(const Player &other); // Copy constructor
    Player &operator=(const Player &other); // Assignment operator
    ~Player(); // Destructor
    friend std::ostream &operator<<(std::ostream &out, const Player &player); // Stream insertion operator

    static Player Neutral;
    vector<Territory *> *ownedTerritories;
    vector<std::string *> CardsOwned;
    vector<std::string> CardTypes{"Bomb", "Blockade", "Airlift", "Negotiate"};
    vector<string> allyList;
    OrdersList *ordersList;

    string *getName() const;

    int *getArmies() const;

    OrdersList *getOrderList() const;

    vector<Territory *> *getOwnedTerritories() const;

    PlayerStrategy *getStrategy() const;

    string *getStrategyName() const;

    void setStrategy(PlayerStrategy *strategy);

    void *setArmies(int *armiesNum);

    void setName(string *name);

    void setOwnedTerritories(vector<Territory *> *ownedTerritories);

    string *giveCard();

    void printHand();

    void dealHand();

    void addCard(string *card);

    void removeCard(string *card);

    bool ownsCard(string *card, vector<std::string *> deck);

    static int validateCard(int cardSelected);

    int chooseCard();

    void clearAllyList();

    void addTerritory(Territory *const *territory) const;

    bool playerTerritoryCheck(Map *map, const string &territoryName) const;

    void printPlayerTerritoryList();

    vector<Territory *> toDefend(Map *map) const;

    vector<Territory *> toAttack(Map *map);

    void changePlayerStrategy(PlayerStrategy *newStrategy);

    void issueOrder(const string &orderType, Map *map, Player *player, GameEngineLoop *engine);

    void issueDeployOrder(Map *gameMap, Player *player);

    void issueAdvanceOrder(Map *gameMap, Player *player, GameEngineLoop *gameEngine);

    void issueBombOrder(Map *gameMap, Player *player, GameEngineLoop *gameEngine);

    void issueBlockadeOrder(Map *gameMap, Player *player);

    void issueAirliftOrder(Map *gameMap, Player *player);

    void issueNegotiateOrder(Map *gameMap, Player *player, GameEngineLoop *gameEngine);

private:
    int *armies;
    string *name;
    PlayerStrategy *strategy;
};

#endif //COMP345A1_PLAYER_H