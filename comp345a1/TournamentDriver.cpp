//
// Created by lia on 03/12/23.
//

#include "TournamentDriver.h"
#include "GameEngine.h"

void testTournament() {

    GameEngine gameEngine;

    LogObserver *logObserver = new LogObserver();

    gameEngine.addObserver(logObserver);
    gameEngine.startupPhase();

}
