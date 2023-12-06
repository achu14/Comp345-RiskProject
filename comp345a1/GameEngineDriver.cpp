#include "GameEngine.h"
#include "GameEngineDriver.h"

#include <iostream>
#include <string>

using namespace std;


int testGameStates() {
    GameEngine game;
    std::string userCommand;

    //Ask user for input:
    cout << "Welcome to the game.";
    userCommand = game.getUserInput();
    while (userCommand != "end") {
        game.transitionToNextState(userCommand);
        game.printCurrentState();
        userCommand = game.getUserInput();
    }
    return 0;
}

void testStartupPhase() {

    GameEngine gameEngine;

    LogObserver *logObserver = new LogObserver();

    gameEngine.addObserver(logObserver);


    gameEngine.startupPhase();
}
