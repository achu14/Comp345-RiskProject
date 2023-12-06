#include "GameEngine.h"
#include "CommandProcessing.h"
#include "PlayerStrategies.h"

#include <iostream>
#include <string>
#include <random>
#include <functional>

using namespace std;


//Default constructor
GameEngine::GameEngine() :
        currentState(new GameState(START)),
        players(new std::vector<Player *>()),
        allTerritories(new std::vector<Territory *>()) {


}

//Copy constructor
GameEngine::GameEngine(const GameEngine &game) :
        currentState(new GameState),
//    currentState = *(game.currentState),//copy the value of the enum corresponding to the currentState of game
        players(new std::vector<Player *>(*game.players)),
        allTerritories(new std::vector<Territory *>(*game.allTerritories)) {}


// Define assignment operator for GameState
GameEngine &GameEngine::operator=(const GameEngine &game) {
    if (this == &game) {
        return *this;
    }

    delete currentState;
    currentState = new GameState(*game.currentState);

    return *this;
}

// Define stream insertion operator for GameState
std::ostream &operator<<(std::ostream &os, const GameState &state) {
    switch (state) {
        case GameState::START:
            os << "START";
            break;
        case GameState::MAP_LOADED:
            os << "MAP_LOADED";
            break;
        case GameState::MAP_VALIDATED:
            os << "MAP_VALIDATED";
            break;
        case GameState::PLAYERS_ADDED:
            os << "PLAYERS_ADDED";
            break;
        case GameState::ASSIGN_REINFORCEMENT:
            os << "ASSIGN_REINFORCEMENT";
            break;
        case GameState::ISSUE_ORDERS:
            os << "ISSUE_ORDERS";
            break;
        case GameState::EXECUTE_ORDERS:
            os << "EXECUTE_ORDERS";
            break;
        case GameState::WIN:
            os << "WIN";
            break;
        default:
            os << "START";
    }
    return os;
}

GameEngine::~GameEngine() {
    for (auto &player: *(players)) {
        delete player;
    }
    for (auto &territory: *(allTerritories)) {
        delete territory;
    }
    delete currentState;//Memory de-allocation
}


void GameEngine::transitionToNextState(const std::string &command) {
    std::string loadmapPrefix = "loadmap";
    std::string playerPrefix = "addplayer";


    switch (*currentState) {
        case START:
            if (command.compare(0, loadmapPrefix.length(), loadmapPrefix) == 0) {
                *currentState = MAP_LOADED;

                size_t pos = command.find(' ');
                std::string mapName = command.substr(pos + 1);
                setMapSelected(mapName);

            } else {
                cout << "Invalid command. Please type a valid command:\tloadmap\n";
                //add a call for used input
            }
            break;
        case MAP_LOADED:
            if (command.compare(0, loadmapPrefix.length(), loadmapPrefix) == 0) {
                *currentState = MAP_LOADED;

                size_t pos = command.find(' ');
                std::string mapName = command.substr(pos + 1);
                setMapSelected(mapName);
            } else if (command == "validatemap") {
                if (getSelectedMap()->validate()
                        ) {
                    cout << "Map is valid! " << endl;
                    *currentState = MAP_VALIDATED;

                } else {
                    cout << "Map is not valid! " << endl;

                }
            } else {
                cout << "Invalid command. Please type a valid command:\tloadmap\tvalidatemap\n";
                //add a call for used input
            }
            break;
        case MAP_VALIDATED:
            if (command.compare(0, playerPrefix.length(), playerPrefix) == 0) {
                *currentState = PLAYERS_ADDED;

                string playerName;

                size_t pos = command.find(' ');
                if (pos != std::string::npos) {
                    // Extract the name after the space
                    playerName = command.substr(pos + 1);
                } else {
                    std::cout << "Invalid command format" << std::endl;
                }
                PlayerStrategy *human = new HumanPlayerStrategy;
                Player *newPlayer = new Player(new string(playerName));
                players->push_back(newPlayer);
            } else {
                cout << "Invalid command. Please type a valid command:\taddplayer\n";
            }
            break;
        case PLAYERS_ADDED:

            if (command == "assigncountries") {
                *currentState = ASSIGN_REINFORCEMENT;
            } else if (command.compare(0, playerPrefix.length(), playerPrefix) == 0) {
                *currentState = PLAYERS_ADDED;

                string playerName;

                size_t pos = command.find(' ');
                if (pos != std::string::npos) {
                    // Extract the name after the space
                    playerName = command.substr(pos + 1);
                } else {
                    std::cout << "Invalid command format" << std::endl;
                }
                Player *newPlayer = new Player(new string(playerName));
                players->push_back(newPlayer);

            } else {
                cout << "Invalid command. Please type a valid command:\tassigncountries\taddplayer\n";
            }
            break;
        case ASSIGN_REINFORCEMENT:
            if (command == "issueorder") {
                *currentState = ISSUE_ORDERS;
            } else {
                cout << "Invalid command. Please type a valid command:\tissueorder\n";
            }
            break;
        case ISSUE_ORDERS:
            if (command == "endissueorders") {
                *currentState = EXECUTE_ORDERS;
            } else if (command == "issueorder") {
                *currentState = ISSUE_ORDERS;
            } else {
                cout << "Invalid command. Please type a valid command:\tissueorder\tendissueorders\n";
            }
            break;

        case EXECUTE_ORDERS:
            if (command == "win") {
                *currentState = WIN;
            } else if (command == "execorder") {
                *currentState = EXECUTE_ORDERS;
            } else if (command == "endexecorders") {
                *currentState = ASSIGN_REINFORCEMENT;
            } else {
                cout << "Invalid command. Please type a valid command:\texecorder\tendexecorder\twin\n";
            }
            break;

        case WIN:
            if (command == "end") {
                cout << "You've won. Congratulations!";
                return;
            } else if (command == "play") {
                *currentState = START;
            } else {
                cout << "Invalid command. Please type a valid command:\tplay\tend\n";
            }
            break;
    }
    notifyObservers(this);

}

//This function prints out the name of the current state
void GameEngine::printCurrentState() {
    // Directly use the overloaded operator<< for GameState
    std::cout << "The current game state is " << *currentState << std::endl;
}

//This function gets the user input to have the command used to transition between states
std::string GameEngine::getUserInput() {
    cout << "Please enter a command:\t";
    std::string command;
    cin >> command;
    return command;
}

// Getters
vector<Player *> *GameEngine::getPlayers() const {
    return players;
}

vector<Territory *> *GameEngine::getAllTerritories() const {
    return allTerritories;
}

vector<string> *GameEngine::getPlayerStrategyList() const {
    return playerStrategyList;
}

void GameEngine::setPlayerStrategyList(vector<string> *newPlayerStrategyList) {
    playerStrategyList = newPlayerStrategyList;
}

void GameEngine::setAllTerritories(vector<Territory *> *newTerritories) {
    allTerritories = newTerritories;
}

// Setters
void GameEngine::setPlayers(vector<Player *> *newPlayers) {
    players = newPlayers;
}

void GameEngine::startupPhase() {
    GameEngine *gameEngine = this;
    // Print available maps for convenience
    cout << "Maps available: \n";


    std::vector<std::string> stringList = {"maps/Australia.map", "maps/Canada.map", "maps/Europe.map"};
    for (const std::string &mapFileName: stringList) {
        std::cout << mapFileName << std::endl;
    }
    CommandProcessor cp("start");
    LogObserver *logObserver = new LogObserver();

    cp.addObserver(logObserver);

    while (true) {
        Command command = cp.getCommand();

        // Split the command string and extract the command keyword
        std::vector<std::string> commandParts = splitString(command.getCommand());
        std::string commandKeyword = commandParts.empty() ? "" : commandParts[0];

        if (command.getCommand() == "exit" || command.getEffect() == "invalid command") {
            cout << "Exiting the game due to exit/invalid command entered \n";
            break;
        }

        if (commandKeyword == "tournament") {

            auto tournamentParameters = parseTournamentCommand(splitString(command.getCommand()));
            // Extract tournament parameters
            const auto &mapFiles = tournamentParameters.at("-M");
            const auto &playerStrategies = tournamentParameters.at("-P");
            int numberOfGames = std::stoi(tournamentParameters.at("-G").front());
            int maxNumberOfTurns = std::stoi(tournamentParameters.at("-D").front());


            for (const auto &param: tournamentParameters) {
                std::cout << param.first << ": ";
                for (const auto &value: param.second) {
                    std::cout << value << " ";
                }
                std::cout << std::endl;  // New line for each parameter
            }

            cout << "~~~~~~~~~~~~ Player count check ~~~~~~~~~ \n";
            notifyObservers(this, "~~~~~~~~~~~~ Player count check ~~~~~~~~~ \n");

            if (gameEngine->getPlayers()->size() < 2 || gameEngine->players->size() > 6) {
                cout << "Invalid number of players\n";
                break;
            }

            cout << "~~~~~~~~~~~~ Player strategy assignment ~~~~~~~~~ \n";
            notifyObservers(this, "~~~~~~~~~~~~ Player strategy assignment ~~~~~~~~~ \n");

            for (int i = 0; i < players->size(); ++i) {
                Player *playerInstance = (*players)[i];
                auto playerStrategy = playerStrategies[i];


                if (playerStrategy == "Cheater") {
                    playerInstance->setStrategy(new CheaterPlayerStrategy());
                } else if (playerStrategy == "Aggressive") {
                    playerInstance->setStrategy(new AggressivePlayerStrategy());
                } else if (playerStrategy == "Benevolent") {
                    playerInstance->setStrategy(new BenevolentPlayerStrategy());
                } else if (playerStrategy == "Neutral") {
                    playerInstance->setStrategy(new NeutralPlayerStrategy());
                } else {
                    cout << "Error in the strategies selected \n";
                    break;
                }
            }


            // For each map entered
            for (auto currentMap: mapFiles) {
                cout << "~~~~~~~~~~~~ Loading map ~~~~~~~~~ \n";
                notifyObservers(this, "~~~~~~~~~~~~ Loading map ~~~~~~~~~ \n");

                gameEngine->setMapSelected(currentMap);
                gameEngine->addMapToGameEngine(getSelectedMap());

                //gameEngine->distribute_territories();
                gameEngine->distributeTerritories(getSelectedMap());

                getSelectedMap()->printMap();

                cout << "Each player has " << gameEngine->players->front()->getOwnedTerritories()->size()
                     << " territories \n";


                for (int i = 0; i < numberOfGames; ++i) {

                    cout << "~~~~~~~~~~~~ Assigning army units to all players ~~~~~~~~~ \n";
                    notifyObservers(this, "~~~~~~~~~~~~ Assigning army units to all players ~~~~~~~~~ \n");

                    assign_armies(*gameEngine->players);

                    notifyObservers(this, "~~~~~~~~~~~~ Drawing cards for all players ~~~~~~~~~ \n");
                    cout << "~~~~~~~~~~~~ Drawing cards for all players ~~~~~~~~~ \n";
                    draw_initial_hand(*gameEngine->players);

                    notifyObservers(this, "~~~~~~~~~~~~ Shuffling player order randomly ~~~~~~~~~ \n");
                    cout << "~~~~~~~~~~~~ Shuffling player order randomly ~~~~~~~~~ \n";
                    gameEngine->shufflePlayers(*gameEngine->players);


                    auto gameLoop = new GameEngineLoop();
                    string winner = gameLoop->gameAutoLoop(gameEngine, gameEngine->getSelectedMap(), maxNumberOfTurns);
                    cout << winner << "won!";

                    int round = i + 1;
                    notifyObservers(this,
                                    "Map : " + currentMap + " Game #: " + to_string(round) + " Winner : " + winner);


                }


            }


            // Create a structure to store tournament results
            std::map<std::string, std::vector<std::string>> tournamentResults;


            break; // Exit after the tournament
        }

        if (commandKeyword == "gamestart") {
            // If gamestate is in the players_added phase attempt to check the requirements otherwise keep asking the user to progress
            GameState state = *gameEngine->currentState;
            if (state == PLAYERS_ADDED) {
                cout << "gamestart command entered \n ";


                cout << "~~~~~~~~~~~~ Player count check ~~~~~~~~~ \n";
                if (gameEngine->getPlayers()->size() < 2 || gameEngine->players->size() > 6) {
                    cout << "Invalid number of players\n";
                    break;
                }

                cout << "~~~~~~~~~~~~ Shuffling player order randomly ~~~~~~~~~ \n";
                gameEngine->shufflePlayers(*players);

                cout << "~~~~~~~~~~~~ Loading map ~~~~~~~~~ \n";
                // string mapName = "Europe";
                // gameEngine->setMapSelected(mapName);
                Map *gameMap = gameEngine->getSelectedMap();

                cout << "~~~~~~~~~~~~ Splitting territories ~~~~~~~~~ \n";
                gameEngine->distributeTerritories(gameMap);


                cout << "This map has : " << gameEngine->getAllTerritories()->size() << " territories \n";
                cout << "Each player has " << gameEngine->players->front()->getOwnedTerritories()->size()
                     << " territories \n";

                cout << "~~~~~~~~~~~~ Assigning army units ~~~~~~~~~ \n";
                assign_armies(*gameEngine->players);

                cout << "~~~~~~~~~~~~ Drawing cards ~~~~~~~~~ \n";
                draw_initial_hand(*gameEngine->players);

                cout
                        << "~~~~~~~~~~~~ Transition to 'play' phase (aka assignreinforcements since this is the start of the 'play' phase) ~~~~~~~~~ \n";
                gameEngine->transitionToNextState("assigncountries");


            } else {
                cout << "You cannot enter the gamestart phase from this state \n";
            }
        }

        gameEngine->transitionToNextState(command.getCommand());

    }

}

Map *GameEngine::getSelectedMap() const {
    return selectedMap;
}

void GameEngine::setSelectedMap(Map *sMap) {
    GameEngine::selectedMap = sMap;
}

// Adds the game map to the game engines all territories list
void GameEngine::addMapToGameEngine(Map *gameMap) const {
    for (auto continent: *gameMap->getContinents()) {
        for (auto territory: *continent->getTerritories()) {
            getAllTerritories()->push_back(territory);
        }
    }
}

//Assigns 50 armies to each player
void GameEngine::assign_armies(std::vector<Player *> players) {
    for (const auto player: players) {
        player->setArmies(new int(50));
    }
}


// Distribute territories evenly to each player
void GameEngine::distributeTerritories(Map *gameMap) {
    size_t playerCount = getPlayers()->size();
    for (size_t i = 0; i < getAllTerritories()->size(); ++i) {
        // Round-robin assignment of territories to players
        Player *currentPlayer = (*(getPlayers()))[i % playerCount];
        Territory *currentTerritory = (*(getAllTerritories()))[i];
        currentTerritory->setOwner(currentPlayer->getName());
        currentPlayer->ownedTerritories->push_back(currentTerritory);
    }
}

// assigns in round-robin an even amount of territories per player
void GameEngine::distribute_territories() {

    int numOfPlayers = players->size();
    int territoriesPerPlayer = this->getSelectedMap()->getTerritories()->size() / numOfPlayers;
    int playerIndex = 0;
    int territoriesAssigned = 0;

    for (const auto &pair: *this->getSelectedMap()->getTerritories()) {
        if (territoriesAssigned < numOfPlayers * territoriesPerPlayer) {

            Player *currentPlayer = (*players)[playerIndex];
            currentPlayer->addTerritory(reinterpret_cast<Territory *const *>(pair.second));

            playerIndex = (playerIndex + 1) % numOfPlayers;
            territoriesAssigned++;
        } else {
            break;
        }
    }
}

// Add player to players list
void GameEngine::addPlayer(Player *player) {
    getPlayers()->push_back(player);
}

map<std::string, function<PlayerStrategy *()>> strategyMap = {
        {"Aggressive", []() { return new AggressivePlayerStrategy(); }},
        {"Benevolent", []() { return new BenevolentPlayerStrategy(); }},
        {"Neutral",    []() { return new NeutralPlayerStrategy(); }},
        {"Cheater",    []() { return new CheaterPlayerStrategy(); }},
        {"Human",      []() { return new HumanPlayerStrategy(); }}
};

vector<string> *GameEngine::createPlayerStrategiesList(int numOfPlayers) {
    int currentPlayerNum = 1;
    string playerStrategy;
    vector<string> *playerStrategyList = new vector<string>;
    // Get the strategy of each player
    for (int i = 0; i < numOfPlayers; i++) {
        cout << "What is the Strategy of of Player" << currentPlayerNum << endl;
        cout << "The options are: [Human, Aggressive, Cheater, Benevolent, Neutral]" << endl;
        cin >> playerStrategy;
        playerStrategyList->push_back(playerStrategy);
        currentPlayerNum++;
    }
    return playerStrategyList;
}

// Creates and adds the requested number of players to the game
void GameEngine::createAndAddPlayers(int numberOfPlayers) {
    cout << "\nCreating " << numberOfPlayers << " players." << endl;

    // Create and assign each player as strategy from the player strategy list
    for (int i = 0; i < numberOfPlayers; ++i) {
        string strategyName = (*getPlayerStrategyList())[i];

        // Find the strategy creation function in the map
        auto it = strategyMap.find(strategyName);
        if (it != strategyMap.end()) {
            // Create player strategy using the function pointer
            PlayerStrategy *strategy = it->second();
            // Create player name
            string *playerName = new string("Player" + to_string(i + 1) + " [" + strategyName + "]");
            // Create a new player
            Player *newPlayer = new Player(playerName);
            // Set the player's strategy
            newPlayer->setStrategy(strategy);
            // Set Player Strategy Name
            newPlayer->getStrategy()->name = new string(strategyName);
            // Add player to the game engine
            addPlayer(newPlayer);
        } else {
            throw std::invalid_argument("Invalid strategy name: " + strategyName);
        }
    }
    cout << "All players added to the game." << std::endl;
}

// Deals out 5 cards to each player
void GameEngine::draw_initial_hand(std::vector<Player *> players) {
    for (const auto player: players) {
        player->dealHand();
    }
}

// Shuffles the players vector so the game order is random
void GameEngine::shufflePlayers(std::vector<Player *> &players) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(players.begin(), players.end(), gen);
}


// Validates selected map against availability otherwise will set to australia
void GameEngine::setMapSelected(string mapName) {
    auto *gameMap = MapLoader::loadMap("maps/" + mapName + ".map");
    if (gameMap->validate()) {
        cout << endl << "Loading the " << mapName << " map" << endl;
    } else {
        std::cerr << "\nMap is not valid." << endl;
    }
    setSelectedMap(gameMap);
}

string GameEngine::stringToLog() {
    return "Current state: " + gameStateStrings[*currentState];
}





