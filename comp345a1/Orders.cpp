#include "Orders.h"
#include <iostream>
#include <random>
#include <algorithm>

using std::cout;
using std::endl;
using std::ostream;
using namespace std;

//if a object in included in a vector list
bool isIncluded(const std::string &single, const std::vector<std::string> &list) {
    return std::find(list.begin(), list.end(), single) != list.end();
}


//chances of elimination
/**
 * @param chance from 0~1, determines the chances of performing elimination.
 */
bool elimChance(double chance) {
    static random_device rd;
    static mt19937 gen(rd());
    static std::uniform_real_distribution<> percent(0, 1); // Define the range
    // Generate a random number less than the chance
    return percent(gen) < chance;
}

/**
 * Default Constructor
 */
OrdersList::OrdersList() {
}

/**
 * Copy constructor
 * @param o the copy orderlist
 */
OrdersList::OrdersList(const OrdersList &o) {
    this->ordersList = o.ordersList;
}

/**
 * add order Function
 * @param order the added order
 */
void OrdersList::addOrder(Order *o) {
    ordersList.push_back(o);
}

int OrdersList::getSize() {
    return ordersList.size();
}

Order *OrdersList::getOrder(int index) {
    return ordersList[index - 1];
}

/**
 * Remove an order from list
 * @param index start at 1
 */
void OrdersList::remove(int index) {
    int size = this->getSize();

    if (size == 0) {
        cout << "empty order, unable to remove." << endl;
    } else if (index < 1 || index > size) {
        cout << "invalid index, unable to remove.";
    } else {
        delete ordersList[index - 1];
        ordersList.erase(ordersList.begin() + index - 1);
    }
}

/**
 * Move the order up for to the nearest upper position
 * @param index start at 1
 */
void OrdersList::moveOrderUp(int index) {
    int size = this->getSize();

    if (size <= 1) {
        cout << "invalid list, unable to move." << endl;
    } else if (index == 1) {
        cout << "uppest order, unable to move up." << endl;
    } else if (index < 1 || index > size) {
        cout << "invalid index, unable to move." << endl;
    } else {
        Order *temp = ordersList[index - 1];
        ordersList[index - 1] = ordersList[index - 2];
        ordersList[index - 2] = temp;
    }
}

/**
 * Move the order up for to the nearest lower position
 * @param index start at 1
 */
void OrdersList::moveOrderDown(int index) {
    int size = this->getSize();

    if (size <= 1) {
        cout << "invalid list, unable to move." << endl;
    } else if (index == size) {
        cout << "lowest order, unable to move down." << endl;
    } else if (index < 1 || index > size) {
        cout << "invalid index, unable to move." << endl;
    } else {
        Order *temp = ordersList[index - 1];
        ordersList[index - 1] = ordersList[index];
        ordersList[index] = temp;
    }
}

/**
 * Print the whole order list
 * @param output
 */
ostream &OrdersList::printList(ostream &output) {
    output << "OrderList: " << endl;
    for (Order *o: ordersList) {
        output << " - " << *o << endl;
    }
    return output;
}

/**
 * Stream insertion operator overloaded.
 * @param output
 */
ostream &operator<<(ostream &output, OrdersList &o) {
    return o.printList(output);
}

/**
 * Assignment operator overload if a reference to a list is passed.
 * @param o
 * @return
 */
OrdersList &OrdersList::operator=(const OrdersList &o) {
    if (this != &o) {
        for (Order *order: ordersList) {
            delete order;
        }
        int size = o.ordersList.size();
        this->ordersList = vector<Order *>(size);
        for (int i = 0; i < size; i++) {
            ordersList[i] = o.ordersList[i]->copy();
        }
        return *this;
    }
    return *this;
}

/**
 * Destructor for the OrdersList.
 */
OrdersList::~OrdersList() {
    for (auto order: ordersList) {
        delete order;
    }
    //Swapping content to a non-instantiated vector will deallocate its memory.
    vector<Order *>().swap(this->ordersList);
};

/**
 * Stream insertion operator overloaded
 * @param output
 * @param o
 */
ostream &operator<<(ostream &output, Order &o) {
    return o.printOrder(output);
}

//deploy, advance, bomb, blockade, airlift, and negotiate.

// Deploy order
/**
 * Default Destructor
 */
Deploy::~Deploy() {
}

Deploy::Deploy() {
    this->player = nullptr;
    this->armyNum = nullptr;
    this->targetTerritory = nullptr;
}

Deploy::Deploy(class Player *player, int *armyNum, Territory *targetTerritory) {
    this->player = player;
    this->armyNum = armyNum;
    this->targetTerritory = targetTerritory;
}


/**
 * Copy constructor
 * @param d
 */
Deploy::Deploy(const Deploy &d) {
    this->player = d.player;
    this->armyNum = d.armyNum;
    this->targetTerritory = d.targetTerritory;
}

ostream &Deploy::printOrder(ostream &output) const {
    output << *(player->getName()) << " deploying " << *(armyNum) << " units to "
           << *(targetTerritory->getId()) << "."
           << " This territory now has " << *(targetTerritory->getArmies()) << " armies." << endl;
    return output;
}

/**
 * Verifies then executes order.
 */
void Deploy::execute() const {
    if (!validate()) {
    } else {
        cout << "---Deploy order executed." << endl;
        int currentArmy = *(targetTerritory->getArmies()) + *(armyNum);
        targetTerritory->setArmies(new int(currentArmy));
        printOrder(std::cout);
    }
}

/**
 * Verifies if given order is valid.
 * @return
 */
bool Deploy::validate() const {
    if (*(targetTerritory->getOwner()) == *(player->getName())) {
        return true;
    } else {
        cout << "---Invalid Deploy Order, unable to execute" << endl;
        cout << *(player->getName()) << " doesn't own " << *(targetTerritory->getId()) << endl;
        return false;
    }
}

Deploy *Deploy::copy() const {
    return new Deploy(*this);
}

Deploy &Deploy::operator=(const Deploy &d) {
    if (this != &d) { //Checking for self assignment
        this->armyNum = d.armyNum;
        this->targetTerritory = d.targetTerritory;
    }
    return *this;
}


//Advance order

/**
 * Default Destructor
 */
Advance::~Advance() {
}

Advance::Advance() {
    this->player = nullptr;
    this->armyNum = 0;
    this->startTerritory = nullptr;
    this->targetTerritory = nullptr;
}

Advance::Advance(class Player *player, int *armyNum, Territory *startTerritory, Territory *targetTerritory) {
    this->player = player;
    this->armyNum = armyNum;
    this->startTerritory = startTerritory;
    this->targetTerritory = targetTerritory;
}


/**
 * Copy constructor
 * @param a
 */
Advance::Advance(const Advance &a) {
    this->player = a.player;
    this->armyNum = a.armyNum;
    this->startTerritory = a.startTerritory;
    this->targetTerritory = a.targetTerritory;
}

ostream &Advance::printOrder(ostream &output) const {
    output << *(player->getName()) << " advancing " << *(armyNum) << " units from "
           << *(startTerritory->getId()) << " to "
           << *(targetTerritory->getId()) << endl;
    return output;
}

/**
 * Verifies if given order is valid.
 * @return
 */
bool Advance::validate() const {
    if (startTerritory == nullptr || targetTerritory == nullptr) {
        cout << "---Invalid Advance Order, unable to execute" << endl;
        cout << "Either start territory or target territory is null" << endl;
        return false;
    }

    // If territories are adjacent, not owned by an ally, and start territory is owned by the player: the order is valid
    bool near = isIncluded(*startTerritory->getId(), *targetTerritory->getAdjacent());
    bool ifAlly = isIncluded(*(targetTerritory->getOwner()), player->allyList);
    if (*(startTerritory->getOwner()) == *(player->getName()) && !ifAlly) {
        return true;
    } else if (*(startTerritory->getOwner()) != *(player->getName())) {
        cout << "---Invalid Advance Order, unable to execute" << endl;
        cout << *(startTerritory->getId()) << " is not owned by " << *(player->getName()) << endl;
        return false;
    } else if (ifAlly) {
        cout << "---Invalid Advance Order, unable to execute" << endl;
        cout << "Cannot attack " << *(targetTerritory->getId()) << " because it's owner "
             << *(targetTerritory->getOwner()) << " is your ally" << endl;
        return false;
    } else if (!near) {
        cout << "---Invalid Advance Order, unable to execute" << endl;
        cout << *(startTerritory->getId()) << " is not adjacent to " << *(targetTerritory->getId()) << endl;
        return false;
    } else {
        return false;
    }
}


/**
 * Verifies then executes order.
 */
void Advance::execute() const {
    if (!validate()) {
        return;
    }

    cout << "---Advance order executed." << endl;
    printOrder(cout);

    if (!startTerritory || !targetTerritory || !armyNum) {
        cout << "Invalid territories or army number in Advance order." << endl;
        return;
    }

    int atkkill = 0; // Number of army units killed by the attack side
    int defkill = 0; // Number of army units killed by the defense side

    // Simulate battle if attacking an enemy territory
    if (*(targetTerritory->getOwner()) != *(player->getName())) {
        for (int i = 0; i < *(targetTerritory->getArmies()); i++) {
            if (elimChance(0.7)) {
                defkill++;
            }
        }

        for (int i = 0; i < *(armyNum); i++) {
            if (elimChance(0.6)) {
                atkkill++;
            }
        }

        int remainingDefArmy = std::max(*(targetTerritory->getArmies()) - atkkill, 0);
        int remainingAtkArmy = std::max(*(armyNum) - defkill, 0);

        // Set armies for target territory
        delete targetTerritory->getArmies(); // Free previously allocated memory
        targetTerritory->setArmies(new int(remainingDefArmy)); // Allocate new memory

        // Set armies for start territory
        delete startTerritory->getArmies(); // Free previously allocated memory
        startTerritory->setArmies(new int(remainingAtkArmy)); // Allocate new memory

        cout << *(player->getName()) << " attacked " << *(targetTerritory->getOwner()) << "'s territory. " << endl;

        bool conquered = false;
        // Check if the territory has been conquered
        if (remainingDefArmy <= 0) {
            targetTerritory->setOwner(new string(*(player->getName())));
            targetTerritory->setArmies(new int(remainingAtkArmy));
            conquered = true;
            player->giveCard();
        }
        if (conquered) {
            cout << *(player->getName()) << " now owns " << *(targetTerritory->getId()) << endl;
        }

        cout << "Remaining armies at " << *(startTerritory->getId()) << ": " << *(startTerritory->getArmies())
             << ", at " << *(targetTerritory->getId()) << ": " << *(targetTerritory->getArmies()) << endl;
    } else {
        // Moving armies within player's own territories
        int newStartArmy = *(startTerritory->getArmies()) - *armyNum;
        int newTargetArmy = *(targetTerritory->getArmies()) + *armyNum;

        if (newStartArmy < 0) {
            newStartArmy = 0;
        }

        // Set armies for start territory
        delete startTerritory->getArmies(); // Free previously allocated memory
        startTerritory->setArmies(new int(newStartArmy)); // Allocate new memory

        // Set armies for target territory
        delete targetTerritory->getArmies(); // Free previously allocated memory
        targetTerritory->setArmies(new int(newTargetArmy)); // Allocate new memory

        cout << "Moving armies to player's own territory. Player now has "
             << *(startTerritory->getArmies()) << " armies at " << *(startTerritory->getId())
             << " and " << *(targetTerritory->getArmies()) << " armies at " << *(targetTerritory->getId()) << "."
             << endl;
    }
}

Advance *Advance::copy() const {
    return new Advance(*this);
}

/**
 * Assignment operator overloaded
 * @param a
 * @return
 */
Advance &Advance::operator=(const Advance &a) {
    if (this != &a) {
        this->player = a.player;
        this->armyNum = a.armyNum;
        this->startTerritory = a.startTerritory;
        this->targetTerritory = a.targetTerritory;
    }
    return *this;
}

//Bomb Order

/**
 * Default Destructor
 */
Bomb::~Bomb() {
}

Bomb::Bomb() {
    this->player = nullptr;
    this->targetTerritory = nullptr;
}

Bomb::Bomb(class Player *player, Territory *targetTerritory) {
    this->player = player;
    this->targetTerritory = targetTerritory;
}

/**
 * Copy constructor
 * @param b
 */
Bomb::Bomb(const Bomb &b) {
    this->player = b.player;
    this->targetTerritory = b.targetTerritory;
}

ostream &Bomb::printOrder(ostream &output) const {
    output << *(player->getName()) << " bombed " << *(targetTerritory->getId()) << ".";
    output << endl;
    return output;
}

/**
 * Verifies then executes order.
 */
void Bomb::execute() const {
    if (!validate()) {
    } else {
        cout << "---Bomb order executed." << endl;
        printOrder(cout);
        int *halfArmies = new int(*(targetTerritory->getArmies()) / 2);
        targetTerritory->setArmies(halfArmies);
        cout << *(targetTerritory->getId()) << " now has " << *(targetTerritory->getArmies()) << " armies left.";
        cout << endl;
    }
}

/**
 * Verifies if given order is valid.
 * @return
 */
bool Bomb::validate() const {
    if (*(targetTerritory->getOwner()) != *(player->getName())) {
        return true;
    } else {
        cout << "---Invalid Bomb Order, unable to execute" << endl;
        cout << *(player->getName()) << " owns " << *(targetTerritory->getId()) << endl;
        return false;
    }
}

Bomb *Bomb::copy() const {
    return new Bomb(*this);
}

/**
 * Assignment operator overload for Bomb class.
 * @param b
 * @return
 */
Bomb &Bomb::operator=(const Bomb &b) {
    if (this != &b) {
        this->player = b.player;
        this->targetTerritory = b.targetTerritory;
    }
    return *this;
}

//Blockade order

/**
 * Default Destructor
 */
Blockade::~Blockade() {
}

Blockade::Blockade() {
    this->player = nullptr;
    this->targetTerritory = nullptr;
}

Blockade::Blockade(class Player *player, Territory *targetTerritory) {
    this->player = player;
    this->targetTerritory = targetTerritory;
}


/**
 * Copy constructor
 * @param b
 */
Blockade::Blockade(const Blockade &b) {
    this->player = b.player;
    this->targetTerritory = b.targetTerritory;
}

ostream &Blockade::printOrder(ostream &output) const {
    int originArmy = *(targetTerritory->getArmies()) / 2;
    output << *(player->getName()) << " blockade on " << *(targetTerritory->getId()) << " territory.";
    output << " This territory used to have " << originArmy << " armies, now it's neutral with ";
    output << *(targetTerritory->getArmies()) << " armies" << endl;
    return output;
}

/**
 * Verifies then executes order.
 */
void Blockade::execute() const {
    if (!validate()) {
    } else {
        cout << "---Blockade order executed." << endl;
        int *currentArmy = new int(*(targetTerritory->getArmies()) * 2);
        cout << "Current number of armies in " << *(targetTerritory->getId()) << ": " << *(currentArmy) << endl;
        targetTerritory->setArmies(currentArmy);
        cout << "Neutral Player now owns this territory" << endl;
        targetTerritory->setOwner(Player::Neutral.getName());
        printOrder(cout);
    }
}

/**
 * Verifies if given order is valid.
 * @return
 */
bool Blockade::validate() const {
    if (*(targetTerritory->getOwner()) == *(player->getName())) {
        return true;
    } else {
        cout << "---Invalid Blockade Order, unable to execute" << endl;
        cout << *(player->getName()) << " doesn't own " << *(targetTerritory->getId()) << endl;
        return false;
    }
}

/**
 * Used to make deep copies of Order objects.
 * @return Reference to a new cloned Order object.
 */
Blockade *Blockade::copy() const {
    return new Blockade(*this);
}

/**
 * Assignment operator overload for Blockade class.
 * @param b
 * @return
 */
Blockade &Blockade::operator=(const Blockade &b) {
    if (this != &b) { //Checking for self assignment
        this->player = b.player;
        this->targetTerritory = b.targetTerritory;
    }
    return *this;
}

//Airlift order

/**
 * Default Destructor
 */
Airlift::~Airlift() {
}

Airlift::Airlift() {
    this->player = nullptr;
    this->armyNum = 0;
    this->startTerritory = nullptr;
    this->targetTerritory = nullptr;
}

Airlift::Airlift(class Player *player, int *armyNum, Territory *startTerritory, Territory *targetTerritory) {
    this->player = player;
    this->armyNum = armyNum;
    this->startTerritory = startTerritory;
    this->targetTerritory = targetTerritory;
}


/**
 * Copy constructor
 * @param a
 */
Airlift::Airlift(const Airlift &a) {
    this->player = a.player;
    this->armyNum = a.armyNum;
    this->startTerritory = a.startTerritory;
    this->targetTerritory = a.targetTerritory;
}

ostream &Airlift::printOrder(ostream &output) const {
    output << *(player->getName()) << " airlifted " << *(armyNum) << " armies from ";
    output << *(startTerritory->getId()) << " territory to ";
    output << *(targetTerritory->getId()) << " territory." << endl;
    output << *(player->getName()) << " now has " << *(startTerritory->getArmies());
    output << " armies at " << *(startTerritory->getId()) << " and ";
    output << *(targetTerritory->getArmies()) << " armies at " << *(targetTerritory->getId()) << "." << endl;
    return output;
}

/**
 * Verifies then executes order.
 */
void Airlift::execute() const {
    if (!validate()) {
    } else {
        cout << "---Airlift order executed." << endl;
        int *currentArmy = new int(*(startTerritory->getArmies()) - *(armyNum));
        cout << "Current number of armies in start territory " << *(startTerritory->getId()) << ": "
             << *(currentArmy) << endl;
        startTerritory->setArmies(currentArmy);
        currentArmy = new int(*(targetTerritory->getArmies()) + *(armyNum));
        cout << "Current number of armies in target territory " << *(targetTerritory->getId()) << ": "
             << *(currentArmy) << endl;
        targetTerritory->setArmies(currentArmy);
        printOrder(cout);
    }
}

/**
 * Verifies if given order is valid.
 * @return
 */
bool Airlift::validate() const {
    if (*(startTerritory->getOwner()) == *(player->getName()) &&
        *(targetTerritory->getOwner()) == *(player->getName())) {
        return true;
    } else if (*(startTerritory->getOwner()) != *(player->getName())) {
        cout << "---Invalid Airlift Order, unable to execute" << endl;
        cout << *(player->getName()) << " doesn't own " << *(startTerritory->getId()) << endl;
        return false;
    } else if (*(targetTerritory->getOwner()) != *(player->getName())) {
        cout << "---Invalid Airlift Order, unable to execute" << endl;
        cout << *(player->getName()) << " doesn't own " << *(targetTerritory->getId()) << endl;
        return false;
    } else {
        cout << "---Invalid Airlift Order, unable to execute" << endl;
        return false;
    }

}

/**
 * Used to make deep copies of Order objects.
 * @return Reference to a new cloned Order object.
 */
Airlift *Airlift::copy() const {
    return new Airlift(*this);
}

/**
 * Assignment operator overload for Airlift class.
 * @param a
 * @return
 */
Airlift &Airlift::operator=(const Airlift &a) {
    if (this != &a) { //Checking for self assignment
        this->player = a.player;
        this->armyNum = a.armyNum;
        this->startTerritory = a.startTerritory;
        this->targetTerritory = a.targetTerritory;
    }
    return *this;
}

//Negotiate order

/**
 * Default Destructor
 */
Negotiate::~Negotiate() {
}

Negotiate::Negotiate() {
    this->player = nullptr;
    this->targetPlayer = nullptr;
}

Negotiate::Negotiate(class Player *player, class Player *targetPlayer) {
    this->player = player;
    this->targetPlayer = targetPlayer;
}


/**
 * Copy constructor
 * @param n
 */
Negotiate::Negotiate(const Negotiate &n) {
    this->player = player;
    this->targetPlayer = n.targetPlayer;
}

ostream &Negotiate::printOrder(ostream &output) const {
    output << *(targetPlayer->getName()) << " and " << *(player->getName()) << " will not be able to target one another"
                                                                               " for the rest of the turn" << endl;
    return output;
}

/**
 * Verifies then executes order.
 */
void Negotiate::execute() const {
    if (!validate()) {
    } else {
        cout << "---Negotiate order executed." << endl;
        player->allyList.push_back(*(targetPlayer->getName()));
        targetPlayer->allyList.push_back(*(player->getName()));
        printOrder(cout);
    }
}

/**
 * Verifies if given order is valid.
 * @return
 */
bool Negotiate::validate() const {
    if (*(targetPlayer->getName()) != *(player->getName())) {
        return true;
    } else {
        cout << "---Invalid Negotiate Order, unable to execute" << endl;
        cout << *(player->getName()) << ", you can't negotiate with yourself" << endl;
        return false;
    }
}

/**
 * Used to make deep copies of Order objects.
 * @return Reference to a new cloned Order object.
 */
Negotiate *Negotiate::copy() const {
    return new Negotiate(*this);
}

/**
 * Assignment operator overload for Negotiate class.
 * @param n
 * @return
 */
Negotiate &Negotiate::operator=(const Negotiate &n) {
    if (this != &n) {  //Checking for self assignment
        this->player = n.player;
        this->targetPlayer = n.targetPlayer;
    }

    return *this;
}