#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "OrdersDriver.h"
#include "GameEngineDriver.h"
#include "MapDriver.h"
#include "CommandProcessingDriver.h"
#include "GameLoopDriver.h"
#include "PlayerStrategiesDriver.h"
#include "TournamentDriver.h"

int main() {



    //testCommandProcessor("-console"); // For console command processing
    //testCommandProcessor("-file", "command_files/commands_1.txt"); // For file command processing
    //testStartupPhase();
    //testMainGameLoop();
    //testOrderExecution();//part 4 order execution test.

    testPlayerStrategies();
    //testTournament();

    return 0;
}