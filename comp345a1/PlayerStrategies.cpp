#include <iostream>
#include <random>
#include "PlayerStrategies.h"

bool isOwned(Territory *t, std::vector<Territory *> *territoriesOwned) {
    if (t == nullptr || territoriesOwned == nullptr) {
        return false; // Null check for safety
    }

    for (const Territory *territory: *territoriesOwned) {
        if (territory == t) {
            return true; // Territory is found in the vector
        }
    }

    return false; // Territory is not found
}

Player *findPlayerByName(string *name, GameEngine *engine) {
    Player *targetPlayer;
    for (Player *p: *engine->getPlayers()) {
        if (*(p->getName()) == *(name)) {
            targetPlayer = p;
        }
    }
    return targetPlayer;
}

/**
 * Human Player strat
 * @param player
 * @return
 */
vector<Territory *> HumanPlayerStrategy::toDefend(Player *player, Map *map) {
    vector<Territory *> *territoriesToDefend;
    territoriesToDefend = player->ownedTerritories;
    return *territoriesToDefend;
}

vector<Territory *> HumanPlayerStrategy::toAttack(Player *player, Map *map) {
    vector<Territory *> *territoriesToDefend;
    territoriesToDefend = player->getOwnedTerritories();
    vector<Territory *> territoriesToAttack;
    for (auto continent: *map->getContinents()) {
        for (Territory *targetTerritory: *continent->getTerritories()) {
            std::vector<std::string> *adjacentIds = targetTerritory->getAdjacent();

            // If Player isn't the owner, print the territory and its adjacent territories
            if (!player->playerTerritoryCheck(map, *(targetTerritory->getId()))) {
                //for all adjacents of this territory, check if player owns it
                for (const std::string &adjacentId: *adjacentIds) {
                    Territory *adjacentTerritory = map->getTerritoryById(adjacentId);
                    if (isOwned(adjacentTerritory, territoriesToDefend) &&
                        !isOwned(targetTerritory, &territoriesToAttack)) {
                        territoriesToAttack.push_back(targetTerritory);
                    }
                }
            }
        }
    }
    return territoriesToAttack;
}


void HumanPlayerStrategy::issueOrder(Map *map, Player *player, GameEngineLoop *engine) {
    cout << "---------------------------------------------------" << endl;
    cout << *(player->getName()) << " issuing an order using the [Human] Player Strategy" << endl;

    // Issue Deploy and Advance orders first
    player->issueDeployOrder(map, player);
    player->issueAdvanceOrder(map, player, engine);

    // Prompt player to select a card from their hand
    int cardSelected = player->chooseCard();

    // Play the card
    engine->playCard(cardSelected, map, player);
}

/**
 * Aggressive player strat
 * @param player
 * @return
 */
vector<Territory *> AggressivePlayerStrategy::toDefend(Player *player, Map *map) {
    vector<Territory *> territoriesToDefend;
    vector<Territory *> *territoriesOwned = player->getOwnedTerritories();

    // If the vector isn't empty
    if (!territoriesOwned->empty()) {
        // Initialize strongest territory to the first territory in the list
        Territory *strongestT = (*territoriesOwned)[0];
        // For each territory the player owns
        for (Territory *territory: *territoriesOwned) {
            // If that territory is stronger that the currently set strongest territory, update stongestT
            if (*(territory->getArmies()) > *(strongestT->getArmies())) {
                strongestT = territory;
                territoriesToDefend.clear(); // Clear the list as we found a stronger territory
                territoriesToDefend.push_back(territory); // Add the new strongest territory
            } else if (*(territory->getArmies()) == *(strongestT->getArmies())) {
                territoriesToDefend.push_back(territory); // Add territories with equal strength
            }
        }
    }
    return territoriesToDefend; // Returns territory with the most armies
}

vector<Territory *> AggressivePlayerStrategy::toAttack(Player *player, Map *map) {
    vector<Territory *> territoriesToDefend = toDefend(player, map);
    vector<Territory *> territoriesToAttack;

    // If the vector isn't empty
    if (!territoriesToDefend.empty()) {
        // For each territory in Territories-To-Defend List (Territories with the most armies)
        for (Territory *territory: territoriesToDefend) {
            std::vector<std::string> *adjTerritoriesList = territory->getAdjacent();
            // For each of that territory's adjacent territories
            for (const std::string &adjTerName: *adjTerritoriesList) {
                Territory *adjTerritory = map->getTerritoryById(adjTerName);
                // If the territory isn't owned by the player, add it to the Territories-To-Attack list
                if (*(adjTerritory->getOwner()) != *(player->getName())) {
                    territoriesToAttack.push_back(adjTerritory);
                }
            }
        }
    }
    return territoriesToAttack; // Returns all adjacent territories around the most armed territories
}


void AggressivePlayerStrategy::issueOrder(Map *map, Player *player, GameEngineLoop *engine) {
    cout << *(player->getName()) << " issuing an order using the [Aggressive] Player Strategy" << endl;
    vector<Territory *> territoriesToDefend = toDefend(player, map);
    vector<Territory *> territoriesToAttack = toAttack(player, map);

    // Initialize strongest territory with first territory in toDefend list
    Territory *strongestTerritory = (territoriesToDefend)[0];

    int *deployedArmiesNum = new int(*(player->getArmies()));

    // Set strongest territory to player's territory with the most armies
    for (Territory *t1: territoriesToDefend) {
        for (Territory *t2: territoriesToDefend) {
            if (*(t2->getArmies()) >= *(t1->getArmies())) {
                strongestTerritory = t2;
            }
        }
    }
    cout << *(player->getName()) << "'s strongest territory is " << *(strongestTerritory) << endl;

    cout << "Deploy order issued and added to order list" << endl;
    cout << "Deploying " << *(player->getArmies()) << " armies to " << *(strongestTerritory->getId()) << endl;
    auto deployOrder = new Deploy(player, deployedArmiesNum, strongestTerritory);
    player->getOrderList()->addOrder(deployOrder);

    //Sets the number of armies left in player's reinforcement pool to 0
    player->setArmies(new int(0));
    cout << *(player->getName()) << ", you now have 0 armies in your reinforcement pool" << endl;

    // Initialize enemy territory with the least armies with first territory in toAttack list
    Territory *leastArmyEnemy = (territoriesToAttack)[0];
    for (Territory *t1: territoriesToAttack) {
        for (Territory *t2: territoriesToAttack) {
            if (t2->getArmies() <= t1->getArmies()) {
                leastArmyEnemy = t2;
            }
        }
    }

    cout << "Advance order issued and added to order list" << endl;
    cout << *(player->getName()) << " will advance all armies from " << *(strongestTerritory->getId()) <<
         " to " << *(leastArmyEnemy->getId()) << endl;

    int *strongestTArmyNum = strongestTerritory->getArmies();
    auto advanceOrder = new Advance(player, strongestTArmyNum, strongestTerritory, leastArmyEnemy);
    player->getOrderList()->addOrder(advanceOrder);

    // If player attacked a neutral player, change neutral player to aggressive
    if (advanceOrder->validate()) {
        engine->neutralPlayerCheck(leastArmyEnemy);
    }

    //Play bomb card if player has one
    if (player->ownsCard(new string("Bomb"), player->CardsOwned)) {
        Territory *mostArmy = (territoriesToAttack)[0];
        for (Territory *t: territoriesToAttack) {
            if (t->getArmies() >= mostArmy->getArmies()) {
                mostArmy = t;
            }
        }
        if (mostArmy != nullptr && *(mostArmy->getOwner()) != *(player->getName())) {
            cout << "Bomb order issued and added to order list" << endl;
            cout << *(player->getName()) << " bombed " << *(mostArmy->getId()) << endl;
            auto bombOrder = new Bomb(player, mostArmy);
            player->getOrderList()->addOrder(bombOrder);
            // If player attacked a neutral player, change neutral player to aggressive
            if (bombOrder->validate()) {
                engine->neutralPlayerCheck(mostArmy);
            }
        }
    }
}

/**
 * Benevolent player strat
 * @param player
 * @return
 */
vector<Territory *> BenevolentPlayerStrategy::toDefend(Player *player, Map *map) {
    vector<Territory *> territoriesToDefend;
    vector<Territory *> *territoriesOwned;
    territoriesOwned = player->getOwnedTerritories();

    // Initialize weakest to the first territory in the list
    Territory *weakestTerritory = (*territoriesOwned)[0];

    if (!territoriesOwned->empty()) {
        for (Territory *territory: *territoriesOwned) {
            if (*(territory->getArmies()) < *(weakestTerritory->getArmies())) {
                weakestTerritory = territory;
                territoriesToDefend.clear(); // Clear the list as we found a stronger territory
                territoriesToDefend.push_back(territory); // Add the new strongest territory
            }
        }
    }

    territoriesToDefend.push_back(weakestTerritory);

    return territoriesToDefend;//returns the weakest point to defend
}

// Benevolent player never attacks
vector<Territory *> BenevolentPlayerStrategy::toAttack(Player *player, Map *map) {
    vector<Territory *> territoriesToAttack = {};
    return territoriesToAttack;//returns empty vector
}

void BenevolentPlayerStrategy::issueOrder(Map *map, Player *player, GameEngineLoop *engine) {
    cout << *(player->getName()) << " issuing an order using the [Benevolent] Player Strategy" << endl;
    vector<Territory *> territoriesToDefend = toDefend(player, map);

    // Find the territory with the most armies
    Territory *mostArmyT = (territoriesToDefend)[0];
    for (Territory *t: *player->getOwnedTerritories()) {
        if (*(t->getArmies()) >= *(mostArmyT->getArmies())) {
            mostArmyT = t;
        }
    }

    cout << *(player->getName()) << "'s territory with the most armies is " << *(mostArmyT) << endl;

    // Find and set the territory with the least armies
    Territory *leastArmyT = territoriesToDefend[0];

    int *depNum = new int(*(player->getArmies()));

    // Create deploy order and add it to player order list
    auto deployOrder = new Deploy(player, depNum, leastArmyT);
    player->getOrderList()->addOrder(deployOrder);
    cout << "Deploy order issued and added to order list" << endl;
    cout << *(player->getName()) << " will deploy " << *(depNum) << " armies to " << *(leastArmyT->getId()) << endl;

    // Advance half of armies from owned territory with the most armies to the one with the least
    int *advNum = new int((*(mostArmyT->getArmies()) / 2));

    // Create advance order and add it to player order list
    auto advanceOrder = new Advance(player, advNum, mostArmyT, leastArmyT);
    player->getOrderList()->addOrder(advanceOrder);
    cout << "Advance order issued and added to order list" << endl;
    cout << *(player->getName()) << " advance " << *(advNum) << " armies from " << *(mostArmyT->getId()) << " to "
         << *(leastArmyT)
         << endl;

    // Play Airlift card if in player's hand
    if (player->ownsCard(new string("Airlift"), player->CardsOwned)) {

        //If player have airlift card, it automatically split half of the strongest point to the weakest
        int *airNum = new int((*(mostArmyT->getArmies()) / 2));

        // Create airlift order and add it to player order list
        auto airliftOrder = new Airlift(player, airNum, mostArmyT, leastArmyT);
        player->getOrderList()->addOrder(airliftOrder);
        cout << "Airlift order issued and added to order list" << endl;
        cout << *(player->getName()) << " airlift " << *(airNum) << " armies from " << *(mostArmyT->getId()) << " to "
             << *(leastArmyT)
             << endl;
    }

        // Negotiate with the player with the most armies [strongestPlayer] if Negotiate card is in hand
    else if (player->ownsCard(new string("Negotiate"), player->CardsOwned)) {

        // Initialize allPlayers vector and strongestPlayer
        vector<Player *> *allPlayers = engine->getPlayers();
        Player *strongestPlayer = (*allPlayers)[0];

        // Find the strongest player
        for (Player *p: *allPlayers) {
            if (*(p->getArmies()) > *(strongestPlayer->getArmies()) && *(p->getName()) != *(player->getName())) {
                strongestPlayer = p;
            }
        }

        // Create and add negotiate order to order list
        if (strongestPlayer->getName() != player->getName()) {
            cout << "Negotiate order issued and added to order list" << endl;
            cout << *(player->getName()) << " will negotiate with " << *(strongestPlayer->getName()) << endl;

            // Create negotiate order and add it to player order list
            auto negotiateOrder = new Negotiate(player, strongestPlayer);
            player->getOrderList()->addOrder(negotiateOrder);
        }
    }

        // Play Blockade card on weakest territory if in player's hand, territory transferred to a neutral player
    else if (player->ownsCard(new string("Blockade"), player->CardsOwned)) {
        if (*(leastArmyT->getOwner()) == *(player->getName())) {
            for (auto *p: *engine->getPlayers()) {
                if (*(p->getStrategyName()) == "Neutral") {
                    cout << "Blockade order issued and added to order list" << endl;
                    cout << *(p->getName()) << " will blockade " << *(leastArmyT->getId()) << endl;

                    // Create blockade order and add it to player order list
                    auto blockadeOrder = new Blockade(player, leastArmyT);
                    player->getOrderList()->addOrder(blockadeOrder);
                    break;
                }
            }
        }
    }
}

/**
 * Neutral player strat
 * @param player
 * @return
 */
vector<Territory *> NeutralPlayerStrategy::toDefend(Player *player, Map *map) {
    return vector<Territory *>(); // Returns an empty list
}

vector<Territory *> NeutralPlayerStrategy::toAttack(Player *player, Map *map) {
    return vector<Territory *>(); // Returns an empty list
}

// Neutral player does nothing until attacked
void NeutralPlayerStrategy::issueOrder(Map *map, Player *player, GameEngineLoop *engine) {
    cout << *(player->getName()) << " is a [Neutral] Player so no orders issued by them until attacked" << endl;

}

// If Neutral player was attacked, change their strategy to aggressive
void GameEngineLoop::neutralPlayerCheck(Territory *territory) {
    PlayerStrategy *neutral = new NeutralPlayerStrategy();

    // Loop through each player in the game
    for (auto *player: *getPlayers()) {
        // Check each player for neutral strategy
        if (player->getStrategy() == neutral) {
            // If the neutral player owns attacked territory, switch to aggressive
            if (*(player->getName()) == *(territory->getOwner())) {
                cout << *(player->getName()) << " was attacked so now they are an [aggressive] player"
                     << endl;
                player->setStrategy(new AggressivePlayerStrategy);
                player->getStrategy()->name = new string("Neutral");
            }
        }
    }
}

/**
 * Cheater player strat
 * @param player
 * @return
 */
vector<Territory *> CheaterPlayerStrategy::toDefend(Player *player, Map *map) {
    vector<Territory *> *territoriesToDefend;
    territoriesToDefend = player->ownedTerritories;
    return *territoriesToDefend;
}

vector<Territory *> CheaterPlayerStrategy::toAttack(Player *player, Map *map) {
    vector<Territory *> *territoriesToDefend;
    territoriesToDefend = player->ownedTerritories;
    vector<Territory *> territoriesToAttack;
    for (auto continent: *map->getContinents()) {
        for (Territory *targetTerritory: *continent->getTerritories()) {
            std::vector<std::string> *adjacentIds = targetTerritory->getAdjacent();

            // If Player isn't the owner, print the territory and its adjacent territories
            if (!player->playerTerritoryCheck(map, *(targetTerritory->getId()))) {
                //for all adjacents of this territory, check if player owns it
                for (const std::string &adjacentId: *adjacentIds) {
                    Territory *adjacentTerritory = map->getTerritoryById(adjacentId);
                    if (isOwned(adjacentTerritory, territoriesToDefend) &&
                        !isOwned(targetTerritory, &territoriesToAttack)) {
                        territoriesToAttack.push_back(targetTerritory);
                    }
                }
            }
        }
    }
    return territoriesToAttack;
}

void CheaterPlayerStrategy::issueOrder(Map *map, Player *player, GameEngineLoop *engine) {
    cout << *(player->getName()) << " issuing an order using the [Cheater] Player Strategy" << endl;

    // Automatically conquers all territory nearby
    cout << "Cheater player now conquering all neighboring territories" << endl;
    for (Territory *t: toAttack(player, map)) {
        cout << *(player->getName()) << " conquered " << *(t->getId()) << endl;
        t->setOwner(player->getName());
        player->ownedTerritories->push_back(t);
    }
}