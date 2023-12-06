#include "Player.h"
#include "GameEngine.h"
#include <iostream>
#include <istream>
#include <random>

// GameEngineLoop constructor
GameEngineLoop::GameEngineLoop() :
        players(new std::vector<Player *>()),
        allTerritories(new std::vector<Territory *>()) {}


// GameEngineLoop Copy Constructor
GameEngineLoop::GameEngineLoop(const GameEngineLoop &other) :
        players(new std::vector<Player *>(*other.players)),
        allTerritories(new std::vector<Territory *>(*other.allTerritories)) {}


// GameEngineLoop Assignment operator
GameEngineLoop &GameEngineLoop::operator=(const GameEngineLoop &other) {
    return *this;
}

// GameEngineLoop Destructor
GameEngineLoop::~GameEngineLoop() {
    for (auto &player: *(players)) {
        delete player;
    }
    for (auto &territory: *(allTerritories)) {
        delete territory;
    }
}

// Adds a determined number of armies to each players reinforcement pool at the start of their turns
void GameEngineLoop::reinforcementPhase(Map *map) {
    for (auto &player: *players) {
        // Calculate armies based on territories owned
        cout << "\nCalculating " << *(player->getName()) << "'s armies based on territories owned:" << endl;
        int currentTerritoriesNum = (player->ownedTerritories)->size();
        int territoryArmies = currentTerritoriesNum / 3;
        cout << *(player->getName()) << " has " << (player->ownedTerritories)->size() <<
             " territories" << endl;
        territoryArmies = std::max(territoryArmies, 3); // Ensure minimum reinforcement

        // Calculate bonus armies for continent control
        int continentBonus = 0;
        for (auto &continent: *map->getContinents()) {
            if (playerControlsContinent(player, continent)) {
                continentBonus += *(continent->getBonus());
                cout << *(player->getName()) << " controls continent " << *(continent->getName()) << endl;
                cout << *(player->getName()) << "'s continent bonus is: " << continentBonus << endl;
            }
        }
        cout << *(player->getName()) << " has " << *(player->getArmies()) << " armies currently" << endl;
        // Assign total armies to player
        int totalReinforcement = territoryArmies + continentBonus;
        *(player->getArmies()) += totalReinforcement;
        cout << *(player->getName()) << " received " << totalReinforcement << " armies." << std::endl;
        cout << *(player->getName()) << " now has " << *(player->getArmies())
             << " armies in their reinforcement pool." << std::endl;
    }
}

// Issues the orders passed by each player
void GameEngineLoop::issueOrdersPhase(Map *map) {
    for (auto &player: *players) {
        cout << "-------------------------------------------" << endl;
        // Prompts player to draw a card
        drawCard(player);

        // Updates each players' owned territories list
        updateOwnedTerritories();

        // Issues order for player using their strategy
        player->getStrategy()->issueOrder(map, player, this);
        cout << endl;
    }
}

// Runs the orders execution for each player
void GameEngineLoop::executeOrdersPhase() {
    cout << "-------------------------------------------" << endl;
    // Iterate through each player and execute their orders
    for (auto &player: *(players)) {
        cout << "Executing orders for " << *(player->getName()) << endl;
        cout << "-------------------------------------------" << endl;
        for (int orderIndex = 1; orderIndex <= player->ordersList->getSize(); orderIndex++) {
            player->ordersList->getOrder(orderIndex)->execute();
        }
        cout << "-------------------------------------------" << endl;
        player->clearAllyList();
    }
}

// Interactive version of the main game loop
void GameEngineLoop::mainGameLoop(GameEngine *gameEngine, Map *map, int maxTurns) {

    // Copies game engine players and territories to game loop
    setPlayers(gameEngine->getPlayers());
    setAllTerritories(gameEngine->getAllTerritories());

    cout << "Starting game loop" << endl;
    int currentTurn = 0;

    while (!isGameOver(maxTurns, currentTurn)) {
        currentTurn++;
        cout << endl << "***Turn " << currentTurn << "***" << endl;

        cout << endl << "==========================================";
        cout << endl << "=  Entering Reinforcement Orders Phase   =";
        cout << endl << "==========================================" << endl;
        keyToContinue();
        reinforcementPhase(map);

        cout << endl << "==========================================";
        cout << endl << "=      Entering Issue Orders Phase       =";
        cout << endl << "==========================================" << endl;
        keyToContinue();
        issueOrdersPhase(map);

        cout << endl << "==========================================";
        cout << endl << "=         Entering Execute Phase         =";
        cout << endl << "==========================================" << endl;
        keyToContinue();
        executeOrdersPhase();
    }
}

// Auto run version of the main game loop
string GameEngineLoop::gameAutoLoop(GameEngine *gameEngine, Map *map, int maxTurns) {

    // Copies game engine players and territories to game loop
    setPlayers(gameEngine->getPlayers());
    setAllTerritories(gameEngine->getAllTerritories());

    cout << "Starting game loop" << endl;
    int currentTurn = 0;

    while (!isGameOver(maxTurns, currentTurn)) {
        currentTurn++;
        cout << endl << "***Turn " << currentTurn << "***" << endl;

        cout << endl << "==========================================";
        cout << endl << "=  Entering Reinforcement Orders Phase   =";
        cout << endl << "==========================================" << endl;
        reinforcementPhase(map);

        cout << endl << "==========================================";
        cout << endl << "=      Entering Issue Orders Phase       =";
        cout << endl << "==========================================" << endl;
        issueOrdersPhase(map);
        updateOwnedTerritories();

        cout << endl << "==========================================";
        cout << endl << "=         Entering Execute Phase         =";
        cout << endl << "==========================================" << endl;
        executeOrdersPhase();
        updateOwnedTerritories();
    }
    return gameWinner;
}

// If max turns reached or someone wins, end the game, otherwise continue
bool GameEngineLoop::isGameOver(int maxTurns, int currentTurn) {
    Player *winner = nullptr;
    int activePlayers = getPlayers()->size();
    cout << "Current number of players in the game: " << activePlayers << endl;

    updateOwnedTerritories();

    //if player no longer has territories, they are declared the loser
    for (Player *player: *(players)) {
        if (player->ownedTerritories->empty()) {
            cout << *(player->getName()) << " has no more territories. You lose! :P" << endl;
            cout << *(player->getName()) << " is now removed from the game." << endl;
            removePlayer(player);
            activePlayers--;
            cout << "New number of players in the game: " << activePlayers << endl << endl;
        }
    }

    // Game is over if only one active player remains
    if (activePlayers == 1) {
        for (Player *player: *(getPlayers())) {
            cout << endl << "GAME OVER! The winner is " << *(player->getName()) << endl;
            cout << "Congrats " << *(player->getName()) << "!!" << endl;
            gameWinner = *(player->getName());
        }
        return true;
    }

    // If there are no players, end the game
    if (activePlayers == 0) {
        cout << endl << "Everyone lost so GAME OVER! " << endl;
        gameWinner = "Nobody";
        return true;
    }

    //If max turns reached, end the game
    if (maxTurns == currentTurn) {
        cout << endl << "*Max turns have been reached - GAME OVER*" << endl;

        for (auto *player1: *getPlayers()) {
            for (auto *player2: *getPlayers()) {
                if (player2->ownedTerritories->size() > player1->ownedTerritories->size()) {
                    winner = player2;
                }
            }
        }

        if (winner != nullptr) {
            cout << "Since " << *(winner->getName()) << " has the most territories, they win" << endl;
            gameWinner = *(winner->getName());
        } else {
            cout << "The game ends in a draw" << endl;
            gameWinner = "Draw";
        }
        return true;
    }

    // Game continues if each player owns territories
    cout << "Each player has territories so the game continues" << endl;
    return false;
}

void GameEngineLoop::updateOwnedTerritories() {
    // For each player
    for (auto *player: *(getPlayers())) {
        std::vector<Territory *> territoriesToAdd;
        // For each territory in the game
        for (auto territory: *getAllTerritories()) {
            // Check for null pointers
            if (territory->getOwner() && player->getName() && territory->getId()) {
                // If the territory is owned by the player
                if (*(territory->getOwner()) == *(player->getName())) {
                    // Check if it's already in their ownedTerritories list
                    bool found = false;
                    for (auto ownedTerritory: *player->ownedTerritories) {
                        if (*(territory->getId()) == *(ownedTerritory->getId())) {
                            found = true;
                            break;
                        }
                    }
                    // If not found, put in territories-to-add list
                    if (!found) {
                        territoriesToAdd.push_back(territory);
                    }
                }
            }
        }

        // Add all the territories missing from player's owned-territories list
        for (auto territory: territoriesToAdd) {
            player->ownedTerritories->push_back(territory);
        }
    }

    // For each player
    for (auto *player: *(getPlayers())) {
        std::vector<Territory *> territoriesToRemove;
        // For each territory in their owned-territories list
        for (auto territory: *player->ownedTerritories) {
            // Check for null pointers
            if (territory->getOwner() && player->getName()) {
                // If the territory is not owned by the player
                if (*(territory->getOwner()) != *(player->getName())) {
                    // Add it to the territories-to-remove list
                    territoriesToRemove.push_back(territory);
                }
            }
        }
        // Remove each territory owned by another player
        for (auto territory: territoriesToRemove) {
            player->ownedTerritories->erase(std::remove(player->ownedTerritories->begin(),
                                                        player->ownedTerritories->end(), territory),
                                            player->ownedTerritories->end());
        }
    }
}


// Removes player from players list
void GameEngineLoop::removePlayer(Player *player) {
    auto it = std::find(players->begin(), players->end(), player);
    if (it != players->end()) {
        players->erase(it);
    }
}

// Adds interactive pauses between phases of the game
void GameEngineLoop::keyToContinue() {
    string empty;
    cout << endl << "Enter any string to continue" << endl;
    std::cin >> empty;
}

// Checks if a player controls a given continent
bool GameEngineLoop::playerControlsContinent(Player *player, Continent *continent) {
    for (auto &territory: *(continent->getTerritories())) {
        if (find(player->ownedTerritories->begin(), player->ownedTerritories->end(),
                 territory) == player->ownedTerritories->end()) {
            // If any territory in the continent is not found in the player's owned territories, return false
            return false;
        }
    }
    // If all territories in the continent are owned by the player, return true
    return true;
}

////////////////////////////// Card Methods ////////////////////////////////

// Player plays a card they selected from their hand
void GameEngineLoop::playCard(int cardSelected, Map *map, Player *player) {

    auto *bombCard = new string("Bomb");
    auto *blockadeCard = new string("Blockade");
    auto *airliftCard = new string("Airlift");
    auto *negotiateCard = new string("Negotiate");

    switch (cardSelected) {
        case 1:
            if (player->ownsCard(bombCard, player->CardsOwned)) {
                cout << "Bomb card selected" << endl;
                player->removeCard(bombCard);
                keyToContinue();
                player->issueOrder("bomb", map, player, this);
            } else {
                cout << "**Invalid request: You don't have the Bomb card.**" << endl <<
                     "Choose a card that's in your hand this time." << endl;
                playCard(player->chooseCard(), map, player);
            }
            break;
        case 2:
            if (player->ownsCard(blockadeCard, player->CardsOwned)) {
                cout << "Blockade card selected" << endl;
                player->removeCard(blockadeCard);
                keyToContinue();
                player->issueOrder("blockade", map, player, this);
            } else {
                cout << "**Invalid request: You don't have the Blockade card.**" << endl <<
                     "Choose a card that's in your hand this time." << endl;
                playCard(player->chooseCard(), map, player);
                break;
                case 3:
                    if (player->ownsCard(airliftCard, player->CardsOwned)) {
                        cout << "AirLift card selected" << endl;
                        player->removeCard(airliftCard);
                        keyToContinue();
                        player->issueOrder("airlift", map, player, this);
                    } else {
                        cout << "**Invalid request: You don't have the AirLift card.**" << endl <<
                             "Choose a card that's in your hand this time." << endl;
                        playCard(player->chooseCard(), map, player);
                        break;
                        case 4:
                            if (player->ownsCard(negotiateCard, player->CardsOwned)) {
                                cout << "Negotiate card selected" << endl;
                                player->removeCard(negotiateCard);
                                keyToContinue();
                                player->issueOrder("negotiate", map, player, this);
                            } else {
                                cout << "**Invalid request: You don't have the Negotiate card.**" << endl <<
                                     "Choose a card that's in your hand this time." << endl;
                                playCard(player->chooseCard(), map, player);
                                break;
                            }
                    }
            }
    }
}

// Draws a card for a player and adds it to their hand
void GameEngineLoop::drawCard(Player *player) {
    cout << *(player->getName()) << " drew the " << *(player->giveCard())
         << " card" << endl;
}

/////////////////////// Getters and Setters ////////////////////////////////////

vector<Player *> *GameEngineLoop::getPlayers() const {
    return players;
}

void GameEngineLoop::setPlayers(vector<Player *> *newPlayers) {
    players = newPlayers;
}
