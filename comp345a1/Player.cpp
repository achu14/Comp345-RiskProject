#include <random>
#include "Player.h"
#include "PlayerStrategies.h"
#include "GameEngine.h"

// Player constructor
Player::Player(string *playerName) :
        ownedTerritories(new vector<Territory *>()),
        armies(new int(50)),
        name(playerName),
        ordersList(new OrdersList()),
        strategy(strategy) {}

// Player copy constructor
Player::Player(const Player &other)
        :
        armies(new int(*other.armies)),
        name(new string(*other.name)),
        ordersList(new OrdersList(*other.ordersList)),
        strategy(other.strategy->clone()),
        ownedTerritories(new std::vector<Territory *>(*other.ownedTerritories)) {
    for (Territory *&territory: *ownedTerritories) {
        territory = new Territory(*territory);
    }
    for (int i = 1; i <= ordersList->getSize(); i++) {
        other.ordersList->addOrder(ordersList->getOrder(i)->copy());
    }
}

// Player assignment operator
Player &Player::operator=(const Player &other) {
    if (this != &other) {
        // Deep copy of territories
        for (auto &territory: *ownedTerritories) {
            delete territory; // Clean up existing territory
        }
        ownedTerritories->clear();
        ownedTerritories->reserve(other.ownedTerritories->size());
        for (Territory *territory: *other.ownedTerritories) {
            ownedTerritories->push_back(new Territory(*territory));
        }

        // Copy armies, name, and strategy
        *armies = *other.armies;
        *name = *other.name;
        strategy = other.strategy->clone();

        for (int i = 1; i <= ordersList->getSize(); i++) {
            other.ordersList->addOrder(ordersList->getOrder(i)->copy());
        }
    }
    return *this;
}

// Player destructor
Player::~Player() {
    for (auto &territory: *ownedTerritories) {
        delete territory;
    }
    delete ownedTerritories;
    delete armies;
    delete name;
    for (int i = 1; i <= ordersList->getSize(); i++) {
        delete ordersList->getOrder(i);
    }
    delete ordersList;
    delete strategy;
}

// Player's stream insertion operator
std::ostream &operator<<(std::ostream &out, const Player &player) {
    out << *(player.getName()) << " has " << *(player.getArmies()) << " armies in there reinforcement pool" <<
        " and territories:" << endl;
    for (auto &territory: *(player.getOwnedTerritories())) {
        out << *territory << endl;
    }
    return out;
}

// Neutral Player initializer
PlayerStrategy *neutral = new NeutralPlayerStrategy;
Player Player::Neutral(new string("Neutral Player"));

// Resets player's ally list
void Player::clearAllyList() {
    allyList.clear();
}

///////////////// Getters and Setters ///////////////////////////

string *Player::getName() const {
    return name;
}

int *Player::getArmies() const {
    return armies;
}

string *Player::getStrategyName() const {
    return strategy->name;
}

PlayerStrategy *Player::getStrategy() const {
    return strategy;
}

// Sets the name of the player
void Player::setStrategy(PlayerStrategy *strategy) {
    Player::strategy = strategy;
}

// Sets the name of the player
void Player::setName(string *name) {
    Player::name = new string(*(name) + " [" + *(getStrategyName()) + "]");
}

// Sets the number of armies in player reinforcement pool
void *Player::setArmies(int *armiesNum) {
    armies = armiesNum;
    return nullptr;
}

OrdersList *Player::getOrderList() const {
    return ordersList;
}

vector<Territory *> *Player::getOwnedTerritories() const {
    return ownedTerritories;
}

void Player::setOwnedTerritories(vector<Territory *> *ownedTerritories) {
    Player::ownedTerritories = ownedTerritories;
}

////////////// Player Territory Methods //////////////////////////////

void Player::addTerritory(Territory *const *territory) const {
    ownedTerritories->push_back(*territory);
}

bool Player::playerTerritoryCheck(Map *map, const string &territoryName) const {
    if (map == nullptr) {
        return false;
    }

    for (auto continent: *map->getContinents()) {
        for (auto territory: *continent->getTerritories()) {
            if (*(territory->getId()) == territoryName) {
                if (*(territory->getOwner()) == *(getName())) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Player::printPlayerTerritoryList() {
    for (auto territory: *getOwnedTerritories()) {
        cout << *(territory->getId()) << ", ";
    }
    cout << endl << endl;
}


// Displays and returns list of territories player can defend and the territories adjacent to them
vector<Territory *> Player::toDefend(Map *map) const {
    std::vector<Territory *> territoriesToDefend;
    string choice;
    cout << "Do you want to see your To-Defend list? [yes or no]" << endl;
    cin >> choice;
    if (choice == "yes") {
        cout << endl << "The To-Defend list: " << endl;

        //Iterate through each of the players territories
        for (Territory *ownedTerritory: *getOwnedTerritories()) {
            std::vector<std::string> *adjacentIds = ownedTerritory->getAdjacent();

            // Print the territory and its adjacent territories
            cout << "==========================================" << endl;
            cout << "----For territory " << *(ownedTerritory->getId()) << "----" << endl;
            cout << "Adjacent territories to defend are: " << endl;
            for (const std::string &adjacentId: *adjacentIds) {

                // Retrieve the adjacent territory using the map
                Territory *adjacentTerritory = map->getTerritoryById(adjacentId);

                // Check if the adjacent territory is owned by a different player
                if (adjacentTerritory && *(adjacentTerritory->getOwner()) == *(getName())) {
                    cout << *(adjacentTerritory->getId()) << endl;
                    territoriesToDefend.push_back(ownedTerritory);
                    break; // Break to avoid adding the same territory multiple times
                }
            }
        }
    }
    return territoriesToDefend;
}

// Displays list of territories player can attack and the territories adjacent to them
vector<Territory *> Player::toAttack(Map *map) {
    vector<Territory *> territoriesToAttack;
    string choice;
    cout << "Do you want to see your To-Attack list? [yes or no]" << endl;
    cin >> choice;
    if (choice == "yes") {
        cout << endl << "The To-Attack list: " << endl;

        // Go through each territory on the map and get their adjacent territories list
        for (auto continent: *map->getContinents()) {
            for (auto targetTerritory: *continent->getTerritories()) {
                std::vector<std::string> *adjacentIds = targetTerritory->getAdjacent();

                // If Player isn't the owner, print the territory and its adjacent territories
                if (playerTerritoryCheck(map, *(targetTerritory->getId()))) {
                    cout << "==========================================" << endl;
                    cout << "----For territory " << *(targetTerritory->getId()) << "----" << endl;
                    cout << "Adjacent territories to attack are: " << endl;
                    for (const std::string &adjacentId: *adjacentIds) {
                        Territory *adjacentTerritory = map->getTerritoryById(adjacentId);
                        if (adjacentTerritory && *(adjacentTerritory->getOwner()) != *(getName())) {
                            cout << *(adjacentTerritory->getId()) << endl;
                            territoriesToAttack.push_back(adjacentTerritory);
                        }
                    }
                }
            }
        }
    }
    return territoriesToAttack;
}

////////////////////////// Issue Order Methods ////////////////////////////////////

// Method to issue whichever order decided by player
void Player::issueOrder(const std::string &orderType, Map *map, Player *player, GameEngineLoop *engine) {

    // Issue the order matching the played card order type
    if (orderType == "bomb") {
        issueBombOrder(map, player, engine);
    } else if (orderType == "blockade") {
        issueBlockadeOrder(map, player);
    } else if (orderType == "airlift") {
        issueAirliftOrder(map, player);
    } else if (orderType == "negotiate") {
        issueNegotiateOrder(map, player, engine);
    }
}

// Create territory strings for issue order methods below
string targetTerritory;
string originTerritory;

void Player::issueDeployOrder(Map *map, Player *player) {
    cout << "-----------------------" << endl;
    cout << endl << "Hello " << *(getName());
    cout << endl << "Let's start by deploying and advancing your armies" << endl;
    cout << "The territories you own are:" << endl;
    printPlayerTerritoryList();

    cout << "You currently have " << *(getArmies()) << " armies in your reinforcement pool" << endl;

    int *deployedArmiesNum = new int(0);

    // Issue deploy orders first
    while (*(getArmies()) > 0) {
        cout << endl << "You still have armies in your reinforcement pool.";
        cout << endl << "How many armies do you want to deploy?: " << endl;
        while (!(std::cin >> *deployedArmiesNum)) { // Check if input operation failed
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
            cout << "Invalid input. Please enter a number: " << endl;
        }

        // Make sure player doesn't try to deploy more armies than they have
        if (*(deployedArmiesNum) > *(getArmies())) {
            while (*(deployedArmiesNum) > *(getArmies())) {
                cout << *(getName()) << ", you don't have that many armies" << endl;
                cout << "Let's try this again" << endl;
                cout << "How many armies do you want to deploy?: " << endl;
                while (!(std::cin >> *deployedArmiesNum)) { // Check if input operation failed
                    std::cin.clear(); // Clear error flags
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
                    cout << "Invalid input. Please enter a number: " << endl;
                }
            }
            cout << "Ok but just to double check";
        } else {
            cout << "Where do you want to deploy them?: " << endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
            getline(cin, targetTerritory);

            // Check if territory exists on the map
            bool territoryExist = map->territoryExist(targetTerritory);
            if (!territoryExist) {
                while (!territoryExist) {
                    cout << targetTerritory + ", doesn't exist on the map. Let's try this again" << endl;
                    cout << "Where do you want to deploy them?: " << endl;
                    getline(cin, targetTerritory);
                    territoryExist = map->territoryExist(targetTerritory);
                }
            }

            // Issues deploy order
            cout << "Deploy order added to " << *(getName()) << "'s order list" << endl;
            cout << *(deployedArmiesNum) << " armies will be deployed to " << targetTerritory << endl;
            auto deployOrder = new Deploy(player, deployedArmiesNum, map->getTerritoryById(targetTerritory));
            ordersList->addOrder(deployOrder);

            // Calculates the remaining armies in player reinforcement pool
            int *newPlayerArmiesNum = new int(*(getArmies()) - *(deployedArmiesNum));

            // Sets the new number of armies left in player's reinforcement pool
            setArmies(newPlayerArmiesNum);
            cout << *(getName()) << ", you now have " << *(newPlayerArmiesNum)
                 << " armies in your reinforcement pool" << endl << endl;
        }
    }
}

void Player::issueAdvanceOrder(Map *map, Player *player, GameEngineLoop *engine) {
    string useCard;
    cout << "Do you want to advance any of your armies this turn? [yes or no]" << endl;
    cin >> useCard;

    if (useCard == "no") {
        cout << "Ok " << *(getName()) << ", none of your armies will be advanced this turn" << endl;
    } else {

        // Prompt player to display To-Attack & To-Defend list
        toAttack(map);
        toDefend(map);

        int *advancedArmiesNum = new int(0);
        cout << endl << "How many armies do you want to advance?: " << endl;
        while (!(cin >> *advancedArmiesNum)) { // Check if input operation failed
            cin.clear(); // Clear error flags
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
            cout << "Invalid input. Please enter a number: " << endl;
        }

        cout << "Which territory are they coming from?: " << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
        getline(cin, originTerritory);

        // Check if territory exists on the map
        bool territoryExist = map->territoryExist(originTerritory);
        if (!territoryExist) {
            while (!territoryExist) {
                cout << originTerritory + ", doesn't exist on the map. Let's try this again" << endl;
                cout << "Which territory are they coming from?: " << endl;
                std::getline(std::cin, originTerritory);
                territoryExist = map->territoryExist(originTerritory);
            }
        }

        cout << "Where do you want to advance them?: " << endl;
        std::getline(std::cin, targetTerritory);

        // Check if territory exists on the map
        territoryExist = map->territoryExist(targetTerritory);
        if (!territoryExist) {
            while (!territoryExist) {
                cout << targetTerritory + ", doesn't exist on the map. Let's try this again" << endl;
                cout << "Where do you want to advance them?: " << endl;
                std::getline(std::cin, targetTerritory);
                territoryExist = map->territoryExist(targetTerritory);
            }
        }

        //Issue advance order
        auto advanceOrder = new Advance(player, advancedArmiesNum, (map->getTerritoryById(originTerritory)),
                                        (map->getTerritoryById(targetTerritory)));
        ordersList->addOrder(advanceOrder);

        // If player attacked a neutral player, change neutral player to aggressive player
        if (advanceOrder->validate()) {
            engine->neutralPlayerCheck(map->getTerritoryById(targetTerritory));
        }
        cout << "Advance order added to " << *(getName()) << "'s order list" << endl;
        cout << *(getName()) << " will advance " << *(advancedArmiesNum) << " armies from " << originTerritory
             << " to " << targetTerritory << endl;

        // Continue advancing armies if player wants to
        string advanceMore;
        cout << endl << "Do you want to advance anymore armies? [yes or no]" << endl;
        cin >> advanceMore;
        if (advanceMore == "yes") {
            issueAdvanceOrder(map, player, engine);
        }
    }
}

void Player::issueBombOrder(Map *map, Player *player, GameEngineLoop *gameEngine) {
    cout << endl << "Bomb card from earlier is now available to be played" << endl;
    cout << "Do you still want to use it? [yes or no]" << endl;
    string useCard;
    cin >> useCard;

    if (useCard == "no") {
        cout << "Ok " << *(getName()) << ", Bomb card will be ignored." << endl;
    } else {
        // Prompt player to display To-Attack list
        toAttack(map);

        cout << "Where do you want to bomb?: " << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
        std::getline(std::cin, targetTerritory);

        bool territoryExist = map->territoryExist(targetTerritory);
        if (!territoryExist) {
            while (!territoryExist) {
                cout << "Territory, doesn't exist on the map. Let's try this again" << endl;
                cout << "Where do you want to bomb?: " << endl;
                std::getline(std::cin, targetTerritory);
                territoryExist = map->territoryExist(targetTerritory);
            }
        }

        //Calculates number of armies in target territory after bombed
        int *bombCount = new int(*(map->getTerritoryById(targetTerritory)->getArmies()) / 2);

        //Adds bomb order to player order list
        auto bombOrder = new Bomb(player, (map->getTerritoryById(targetTerritory)));
        ordersList->addOrder(bombOrder);
        cout << "Bomb order added to " << *(getName()) << "'s order list" << endl;
        cout << targetTerritory << "'s armies are set to be reduced in half to " << *(bombCount) << endl;

        // If player attacked a neutral player, change neutral player to aggressive
        if (bombOrder->validate()) {
            gameEngine->neutralPlayerCheck(map->getTerritoryById(targetTerritory));
        }
    }
}

void Player::issueBlockadeOrder(Map *map, Player *player) {
    cout << endl << "Blockade card from earlier is now available to be played" << endl;
    cout << "Do you still want to use it? [yes or no]" << endl;
    string useCard;
    cin >> useCard;

    if (useCard == "no") {
        cout << "Ok " << *(getName()) << ", Blockade card will be ignored." << endl;
    } else {
        // Prompt player to display To-Defend list
        toDefend(map);

        cout << "Where do you want to set up a blockade?: " << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
        std::getline(std::cin, targetTerritory);

        bool territoryExist = map->territoryExist(targetTerritory);
        if (!territoryExist) {
            while (!territoryExist) {
                cout << "Territory, doesn't exist on the map. Let's try this again" << endl;
                cout << "Where do you want to set up a blockade?: " << endl;
                std::getline(std::cin, targetTerritory);
                territoryExist = map->territoryExist(targetTerritory);
            }
        }

        // Calculates target territories new armies number after blockade is executed
        int *blockadeCount = new int(*(map->getTerritoryById(targetTerritory)->getArmies()) * 2);
        cout << targetTerritory << " has " << *(map->getTerritoryById(targetTerritory)->getArmies()) << " armies"
             << endl;

        //Adds blockade order to player order list
        auto blockadeOrder = new Blockade(player, map->getTerritoryById(targetTerritory));
        ordersList->addOrder(blockadeOrder);
        cout << "Blockade order added to " << *(getName()) << "'s order list" << endl;
        cout << targetTerritory << " is now set to be have its armies doubled to " << *(blockadeCount) << " armies"
             << endl;
    }
}

void Player::issueAirliftOrder(Map *map, Player *player) {
    int *airliftArmiesNum = new int(0);
    cout << endl << "Airlift card from earlier is now available to be played" << endl;
    cout << "Do you still want to use it? [yes or no]" << endl;
    string useCard;
    cin >> useCard;

    if (useCard == "no") {
        cout << "Ok " << *(getName()) << ", Airlift card will be ignored." << endl;
    } else {
        // Prompt player to display To-Defend list
        toDefend(map);

        cout << endl << "How many armies do you want to airlift?: " << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
        while (!(std::cin >> *airliftArmiesNum)) { // Check if input operation failed
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
            cout << "Invalid input. Please enter a number: " << endl;
        }
        cout << "Where do you want to airlift them?: " << endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
        std::getline(std::cin, targetTerritory);

        bool territoryExist = map->territoryExist(targetTerritory);
        if (!territoryExist) {
            while (!territoryExist) {
                cout << "Territory, doesn't exist on the map. Let's try this again" << endl;
                cout << "Where do you want to airlift them?: " << endl;
                std::getline(std::cin, targetTerritory);
                territoryExist = map->territoryExist(targetTerritory);
            }
        }

        cout << "Which territory are they coming from?: " << endl;
        std::getline(std::cin, originTerritory);

        territoryExist = map->territoryExist(originTerritory);
        if (!territoryExist) {
            while (!territoryExist) {
                cout << "Territory, doesn't exist on the map. Let's try this again" << endl;
                cout << "Which territory are they coming from: " << endl;
                std::getline(std::cin, originTerritory);
                territoryExist = map->territoryExist(originTerritory);
            }
        }

        //Adds order to player order list
        auto airliftOrder = new Airlift(player, airliftArmiesNum, map->getTerritoryById(originTerritory),
                                        map->getTerritoryById(targetTerritory));
        ordersList->addOrder(airliftOrder);
        cout << "Airlift order added to " << *(getName()) << "'s order list" << endl;
        cout << *(getName()) << " will airlift " << *(airliftArmiesNum) << " armies from " << originTerritory
             << " to " << targetTerritory << endl;
    }
}

void Player::issueNegotiateOrder(Map *map, Player *player, GameEngineLoop *gameEngine) {
    cout << endl << "Negotiate card from earlier is now available to be played" << endl;
    cout << "Do you still want to use it? [yes or no]" << endl;
    string useCard;
    cin >> useCard;

    if (useCard == "no") {
        cout << "Ok " << *(getName()) << ", Negotiate card will be ignored." << endl;
    } else {
        cout << "Who do you want to negotiate with?" << endl << "[";

        // Display players to negotiate with
        for (auto *otherPlayer: *(gameEngine->getPlayers())) {
            if (otherPlayer->getName() != player->getName()) {
                cout << *(otherPlayer->getName()) << "| ";
            }
        }
        cout << "]" << endl;
        string allyPlayerName;
        cin >> allyPlayerName;

        bool playerInGame = false;
        auto *allyPlayer = new Player(new string(allyPlayerName));

        // Check if chosen ally is in the game
        while (!playerInGame) {
            for (auto &currentPlayer: *(gameEngine->getPlayers())) {
                if (*(currentPlayer->getName()) == allyPlayerName) {
                    allyPlayer = currentPlayer;
                    playerInGame = true;
                    break;
                }
            }
            if (!playerInGame) {
                cout << "There's no player in the game by that name." << endl;
                cout << "Re-enter who you want to negotiate with: " << endl;
                cin >> allyPlayerName;
            }
        }

        //Adds order to player order list
        auto negotiateOrder = new Negotiate(player, allyPlayer);
        ordersList->addOrder(negotiateOrder);
        cout << "Negotiate order added to " << *(getName()) << "'s order list" << endl;
    }
}

////////////////////////// Card Methods ////////////////////////////////////

// Give a random card to player
string *Player::giveCard() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, CardTypes.size() - 1);
    // Select a random card type
    int index = dis(gen);
    std::string *newCard = new std::string(CardTypes[index]);
    CardsOwned.push_back(newCard);
    return newCard;
}

// Add card to player's hand
void Player::addCard(string *card) {
    bool cardIsValid = false;
    for (auto cardType: CardTypes) {
        if (*(card) == cardType) {
            CardsOwned.push_back(card);
            cardIsValid = true;
        }
    }
    if (!cardIsValid) {
        cout << "*Invalid Card Type*" << endl;
    }
}

// Removes a card from player's hand
void Player::removeCard(std::string *card) {
    std::vector<std::string *>::iterator it = CardsOwned.begin();
    while (it != CardsOwned.end()) {
        if (**it == *card) { // Compare the strings, not the pointers
            it = CardsOwned.erase(it); // Erase and update the iterator
            break; // If you only want to remove the first occurrence
        } else {
            ++it; // Increment the iterator if no removal
        }
    }
}

// Check if player owns a card
bool Player::ownsCard(string *card, vector<std::string *> hand) {
    for (const auto &cardInHand: hand) {
        if (*(cardInHand) == *(card)) {
            return true;
        }
    }
    return false;
}

// Prints the players hand of cards
void Player::printHand() {
    for (auto card: CardsOwned) {
        cout << *(card) << "| ";
    }
    cout << "]" << endl;
}

// Deals out 5 cards to the player
void Player::dealHand() {
    cout << *(getName()) << "'s hand is:" << endl << "[";
    for (int i = 0; i < 5; i++) {
        cout << *(giveCard()) << "| ";
    }
    cout << "]" << endl << endl;
}

// Ensures that player selects a valid card (represented by int [1-4])
int Player::validateCard(int cardSelected) {
    bool invalidCard = true;
    while (cardSelected < 1 || cardSelected > 4) {
        while (invalidCard) {
            cout << "Invalid Card: Choose an int between 0-4 that matches a card in your hand" << endl;
            std::cin >> cardSelected;
            if (cardSelected >= 1 && cardSelected <= 4) {
                return cardSelected;
            }
        }
    }
    return cardSelected;
}

// Prompts player to pick a card from their hand
int Player::chooseCard() {
    int cardSelected;
    cout << endl << "Which card from your hand do you want to play?" << endl << " Hand:[";
    printHand();
    cout << "Enter the number of the corresponding card: Bomb[1], Blockade[2], Airlift[3], Negotiate[4]" << endl;
    cout << "*Note that after your selection, you'll enter the deploying and advancing"
            " armies phase of the game*" << endl;
    std::cin >> cardSelected;

    // Checks to make sure the card is in the players hand
    cardSelected = validateCard(cardSelected);
    return cardSelected;
}

void Player::changePlayerStrategy(PlayerStrategy *newStrategy) {
    if (this == nullptr || newStrategy == nullptr) {
        throw std::invalid_argument("Player or strategy cannot be null");
    }

    // Delete the old strategy to prevent memory leaks
    delete getStrategy();

    // Set the new strategy
    setStrategy(newStrategy);
}
