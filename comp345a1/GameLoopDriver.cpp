#include <iostream>
#include <istream>
#include "GameEngine.h"

using namespace std;

void testMainGameLoop() {

    cout << "==========================================" << endl;
    cout << "=         Testing Main Game Loop         =" << endl;
    cout << "==========================================" << endl;

    // Initialize game engine
    auto gameEngine = new GameEngine();

    // Get map selection from user
    cout << endl << "Which map do you want to play on?" << endl;
    cout << "Your choices are [Europe, Australia, Canada]" << endl;
    string mapName;
    cin >> mapName;

    // Load the map
    gameEngine->setMapSelected(mapName);

    // Creates the game map from player selected map
    Map *gameMap = gameEngine->getSelectedMap();

    // Print contents of the map
    gameMap->printMap();

    // Set the number of game players
    int numOfPlayers;
    cout << "How many players do you want in the game?" << endl;
    cin >> numOfPlayers;

    // Get player strategies from user
    string playerStrategy;
    gameEngine->setPlayerStrategyList(gameEngine->createPlayerStrategiesList(numOfPlayers));

    // Create players and assign them their strategies
    gameEngine->createAndAddPlayers(numOfPlayers);

    cout << "Shuffling players to randomize turn order" << endl;
    gameEngine->shufflePlayers(*gameEngine->getPlayers());

    cout << "Assigning each territory a default of 50 armies" << endl;
    gameEngine->assign_armies(*gameEngine->getPlayers());

    cout << endl << "**Now dealing out each player's hand**" << endl;
    gameEngine->draw_initial_hand(*gameEngine->getPlayers());

    cout << "Adding each territory on the map to the gameEngine all territories list" << endl;
    gameEngine->addMapToGameEngine(gameMap);

    cout << "Distributing territories evenly to each player" << endl;
    gameEngine->distributeTerritories(gameMap);

    int maxTurns = 0;
    cout << endl << "What is the maximum number of turns for this game?" << endl;
    cin >> maxTurns;

    // Initialize game loop
    auto gameLoop = new GameEngineLoop();

    // Run the main game loop [with pauses between phases; game log printed to console]
//    gameLoop->mainGameLoop(gameEngine, gameMap, maxTurns);

    // Runs the entire game loop [automatically with no pauses; game log printed to console]
    // Saves winner of the game in the winner string
    string winner = gameLoop->gameAutoLoop(gameEngine, gameMap, maxTurns);
}
