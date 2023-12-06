#include "PlayerStrategiesDriver.h"

void testPlayerStrategies() {

    cout << endl << "==========================================";
    cout << endl << "=        Initialization Game Phase       =";
    cout << endl << "==========================================" << endl << endl;

    cout << "Initialize game engine" << endl;
    auto gameEngine = new GameEngineLoop();

    cout << "Creating two players and setting Player1 to [Human] and Player2 to [Benevolent]" << endl;
    Player *player1 = new Player(new string("Player1"));
    player1->setStrategy(new HumanPlayerStrategy());
    Player *player2 = new Player(new string("Player2"));
    player2->setStrategy(new BenevolentPlayerStrategy());

    // To show the capability of each strategy
    cout << "Adding each specialty card to Player1's hand" << endl;
    player1->addCard(new string("Bomb"));
    player1->addCard(new string("Airlift"));
    player1->addCard(new string("Blockade"));
    player1->addCard(new string("Negotiate"));

    cout << "Adding players to the game engine" << endl;
    gameEngine->addPlayer(player1);
    gameEngine->addPlayer(player2);

    cout << "Assigning each player a default of 50 armies" << endl;
    gameEngine->assign_armies(*gameEngine->getPlayers());

    cout << "Dealing out a hand of cards to each player" << endl;
    gameEngine->draw_initial_hand(*gameEngine->getPlayers());

    cout << "Load the Europe map into game engine" << endl;
    gameEngine->setMapSelected("Europe");
    Map *map = gameEngine->getSelectedMap();
    gameEngine->addMapToGameEngine(map);
    map->printMap();

    cout << "Distributing territories to each player" << endl;
    gameEngine->distributeTerritories(map);

    // Initialize the armies of the first territory each player owns
    (*player1->getOwnedTerritories())[2]->setArmies(new int(60));
    (*player2->getOwnedTerritories())[3]->setArmies(new int(30));

    // Proceed to issue orders for each player using each strategy
    cout << endl << "==========================================";
    cout << endl << "=        Issue Orders Game Phase       =";
    cout << endl << "==========================================" << endl;
    gameEngine->keyToContinue();

    cout << "Now issuing an order for Player1 using human strategy [interactive]" << endl;
    player1->getStrategy()->issueOrder(map, player1, gameEngine);
    gameEngine->keyToContinue();

    cout << endl << "*Changing Player1's strategy to aggressive*" << endl;
    player1->changePlayerStrategy(new AggressivePlayerStrategy());
    player1->setArmies(new int(50)); // reset reinforcement pool
    player1->getStrategy()->issueOrder(map, player1, gameEngine);
    gameEngine->keyToContinue();

    cout << endl << "*Changing Player1's strategy to benevolent*" << endl;
    player1->changePlayerStrategy(new BenevolentPlayerStrategy());
    player1->setArmies(new int(50)); // reset reinforcement pool
    player1->getStrategy()->issueOrder(map, player1, gameEngine);
    gameEngine->keyToContinue();

    cout << endl << "*Changing Player1's strategy to cheater*" << endl;
    player1->changePlayerStrategy(new CheaterPlayerStrategy());
    player1->setArmies(new int(50)); // reset reinforcement pool
    player1->getStrategy()->issueOrder(map, player1, gameEngine);
    gameEngine->keyToContinue();

    cout << endl << "*Changing Player1's strategy to neutral*" << endl;
    player1->changePlayerStrategy(new NeutralPlayerStrategy());
    player1->setArmies(new int(50)); // reset reinforcement pool
    player1->getStrategy()->issueOrder(map, player1, gameEngine);
    gameEngine->keyToContinue();

    cout << endl << "*Issue order for Player2 [Benevolent]*" << endl;
    player1->setArmies(new int(50)); // reset reinforcement pool
    player2->getStrategy()->issueOrder(map, player2, gameEngine);

    // Execute each player's orders
    cout << endl << "==========================================";
    cout << endl << "=        Executing Orders Game Phase       =";
    cout << endl << "==========================================" << endl;
    gameEngine->keyToContinue();
    gameEngine->updateOwnedTerritories();
    gameEngine->executeOrdersPhase();

    cout << endl << "**End of Player Strategy Test**" << endl;
    gameEngine->keyToContinue();
}