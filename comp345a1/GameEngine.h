#ifndef COMP345A1_GAMEENGINE_H
#define COMP345A1_GAMEENGINE_H

#include <string>
#include <map>
#include "Map.h"
#include "Player.h"
#include "GameEngine.h"
#include "LoggingObserver.h"

using namespace std;

//The following enum allows to navigate the various game states of the diagram
enum GameState {
    START,
    MAP_LOADED,
    MAP_VALIDATED,
    PLAYERS_ADDED,
    ASSIGN_REINFORCEMENT,
    ISSUE_ORDERS,
    EXECUTE_ORDERS,
    WIN
};

class Player;

class GameEngine : public ILoggable, public LogSubject {
public:
    //Constructors + copy constructor, assignment operator and stream insertion operator
    GameEngine(); //Default constructor
    GameEngine(const GameEngine &game);//Copy constructor
    GameEngine &operator=(const GameEngine &game);//Assignment operator
    friend std::ostream &operator<<(std::ostream &os, const GameEngine &state); //Stream insertion operator
    virtual ~GameEngine();

    string stringToLog() override;

    // Enums with matching human friendly strings
    std::map<GameState, std::string> gameStateStrings = {
            {START,                "START"},
            {MAP_LOADED,           "MAP_LOADED"},
            {MAP_VALIDATED,        "MAP_VALIDATED"},
            {PLAYERS_ADDED,        "PLAYERS_ADDED"},
            {ASSIGN_REINFORCEMENT, "ASSIGN_REINFORCEMENT"},
            {ISSUE_ORDERS,         "ISSUE_ORDERS"},
            {EXECUTE_ORDERS,       "EXECUTE_ORDERS"},
            {WIN,                  "WIN"}
    };


    void transitionToNextState(const std::string &command);

    void printCurrentState();

    string getUserInput();

    void startupPhase();

    Map *getSelectedMap() const;

    void setSelectedMap(Map *sMap);

    void addMapToGameEngine(Map *gameMap) const;

    virtual std::vector<Territory *> *getAllTerritories() const;

    void setAllTerritories(std::vector<Territory *> *newTerritories);

    void distributeTerritories(Map *gameMap);

    void distribute_territories();

    virtual std::vector<Player *> *getPlayers() const;

    virtual void setPlayers(std::vector<Player *> *newPlayers);

    void setPlayerStrategyList(vector<string> *newPlayerStrategyList);

    vector<string> *getPlayerStrategyList() const;

    vector<string> *createPlayerStrategiesList(int numOfPlayers);

    void createAndAddPlayers(int numberOfPlayers);

    void addPlayer(Player *player);

    void assign_armies(std::vector<Player *> players);

    void draw_initial_hand(std::vector<Player *> players);

    void shufflePlayers(std::vector<Player *> &players);

    void setMapSelected(string input);

private:
    GameState *currentState; //using a pointer for current state
    Map *selectedMap;
    std::vector<Player *> *players;
    std::vector<Territory *> *allTerritories; //all territories
    vector<string> *playerStrategyList = new vector<string>;
};

class GameEngineLoop : public GameEngine {
public:
    GameEngineLoop();

    GameEngineLoop(const GameEngineLoop &other); // Copy constructor
    GameEngineLoop &operator=(const GameEngineLoop &other); // Assignment operator
    ~GameEngineLoop() override; // Destructor

    void mainGameLoop(GameEngine *gameEngine, Map *gameMap, int maxTurns);

    bool isGameOver(int maxTurns, int currentTurn);

    void reinforcementPhase(Map *gameMap);

    void issueOrdersPhase(Map *gameMap);

    void executeOrdersPhase();

    void playCard(int cardSelected, Map *map, Player *player);

    void drawCard(Player *player);

    static bool playerControlsContinent(Player *player, Continent *continent);

    void removePlayer(Player *player);

    void neutralPlayerCheck(Territory *territory);

    void updateOwnedTerritories();

    std::vector<Player *> *getPlayers() const;

    void setPlayers(std::vector<Player *> *newPlayers);

    static void keyToContinue();

    string gameAutoLoop(GameEngine *gameEngine, Map *map, int maxTurns);


private:
    std::vector<Player *> *players; //players list
    std::vector<Territory *> *allTerritories; //all territories
    string gameWinner;
};

#endif //COMP345A1_GAMEENGINE_H


