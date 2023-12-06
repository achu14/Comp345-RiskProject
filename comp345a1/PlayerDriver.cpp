#include "Player.h"

void printTerritory(Territory *territory) {
    cout << "\t{ Territory : ";
    cout << "{ id : " << *territory->getId() << "}," << "{ continent : " << *territory->getContinent() << "}";
    cout << "},";
}

void printTerritories(const vector<Territory *> &terList) {
    for (Territory *t: terList) {
        printTerritory(t);
    }
}

void printPlayer(Player player) {

    // Print territories
    cout << "Player instantiated representation : \n{ Player :\n";
    cout << "\t {Territories :";

    player.printPlayerTerritoryList();
    cout << "\n";

    // Print orders
    OrdersList &orderList = *player.getOrderList();
    cout << "\t {Orders :";
    int n1 = 5;
    std::string id1 = "Territory1";
    int x1 = 0;
    int y1 = 0;
    std::string continent1 = "Continent1";
    std::vector<std::string> adjacent1 = {"Territory2"};
    Territory *t1 = new Territory(id1, x1, y1, continent1, adjacent1);
    PlayerStrategy *human = new HumanPlayerStrategy;
    Player *p1 = new Player((string *) "Jake");
    auto *dep1 = new Deploy(p1, &n1, t1);
    orderList.addOrder(dep1);
    orderList.printList(cout);
    cout << "},";
    cout << "\n// listsize:" << orderList.getSize();

    // Give hand
    cout << "\n" << "\t {CardHand :";
    for (int i = 0; i < 5; i++) {
        cout << " {card: " << player.giveCard() << "}";
    }
    cout << "\n}";

    // Print hand
    player.printHand();

}

void testPlayers() {

    cout << "~~~~~~~~~~~~~~~~SETUP~~~~~~~~~~~~~~~~\n";

    // Load the Europe map
    cout << "\nLoading Europe map." << endl;
    auto *map = MapLoader::loadMap("maps/Europe.map");
    if (!map->validate()) {
        std::cerr << "\nMap is not valid." << endl;
    }
    cout << "Europe map successfully loaded and validated" << endl << endl;

    Territory testTerritory1 = Territory("1", 2, 5, "Europe", static_cast<vector<string>>(0));
    Territory testTerritory2 = Territory("2", 3, 5, "Asia", static_cast<vector<string>>(0));

    list<Territory> territoryList1{testTerritory1, testTerritory2};
    list<Territory> territoryList2{testTerritory2};

    cout << "Initializing territory with the following id :" << *testTerritory1.getId();
    cout << "\nInitializing territory with the following id :" << *testTerritory2.getId();
    cout << "\nInitializing territory list with the following id's : ";
    for (auto it = territoryList1.begin(); it != territoryList1.end(); ++it) {
        cout << "{" << *it->getId() << "}";
    }

    cout << "\nInitializing order list with 1 item : \n";
    cout << "Initializing hand of cards: ";

    cout << "\n~~~~~~~~~~~~~~~~PLAYER CREATION TEST~~~~~~~~~~~~~~~~\n";
    PlayerStrategy *human = new HumanPlayerStrategy;
    Player testPlayer(new string("TestPlayer"));
    testPlayer.setStrategy(human);
    printPlayer(testPlayer);


    cout << "\n~~~~~~~~~~~~~~~~ISSUE ORDER TEST~~~~~~~~~~~~~~~~\n";
    cout << "~Appending a new order to the previously displayed player~\n";
    printPlayer(testPlayer);

    cout << "\n~~~~~~~~~~~~~~~~ATTACK TEST~~~~~~~~~~~~~~~~\n";
    cout << "Arbitrary method expected to return last territory from the player's list\n";
    printTerritories(testPlayer.toAttack(map));

    cout << "\n~~~~~~~~~~~~~~~~DEFEND TEST~~~~~~~~~~~~~~~~\n";
    cout << "Arbitrary method expected to return first territory from the player's list\n";
    printTerritories(testPlayer.toDefend(map));
}
