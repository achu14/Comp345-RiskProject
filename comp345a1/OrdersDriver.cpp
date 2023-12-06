#include "OrdersDriver.h"
#include "Orders.h"
#include <iostream>

using std::cout;
using std::endl;

int testOrderExecution() {
    cout << endl;
    cout << endl;
    cout << "==========================================" << endl;
    cout << "=             Testing Orders             =" << endl;
    cout << "==========================================" << endl;
    cout << endl;
    cout << endl;

    int *n1 = new int(1);
    int *n2 = new int(2);
    int *n3 = new int(3);
    int *n4 = new int(4);
    int *n5 = new int(5);
    int *n6 = new int(6);
    int *n7 = new int(7);
    int *n8 = new int(8);
    int *n9 = new int(9);
    int *n10 = new int(10);

    PlayerStrategy *human = new HumanPlayerStrategy;
    std::string *playerName = new std::string("Jake");
    Player *p1 = new Player(playerName);
    playerName = new std::string("Mike");
    Player *p2 = new Player(playerName);

    std::string id1 = "Territory1";
    int x1 = 0;
    int y1 = 0;
    std::string continent1 = "Continent1";
    std::vector<std::string> adjacent1 = {"Territory2"};
    Territory *t1 = new Territory(id1, x1, y1, continent1, adjacent1);

    std::string id2 = "Territory2";
    int x2 = 0;
    int y2 = 0;
    std::vector<std::string> adjacent2 = {"Territory1", "Territory3"};
    Territory *t2 = new Territory(id2, x2, y2, continent1, adjacent2);

    std::string id3 = "Territory3";
    int x3 = 0;
    int y3 = 0;
    std::vector<std::string> adjacent3 = {"Territory2", "Territory4"};
    Territory *t3 = new Territory(id3, x3, y3, continent1, adjacent3);

    std::string id4 = "Territory4";
    int x4 = 0;
    int y4 = 0;
    std::vector<std::string> adjacent4 = {"Territory3"};
    Territory *t4 = new Territory(id4, x4, y4, continent1, adjacent4);

    //initialize player and map data:
    string *pt1 = new string(*p1->getName());
    string *pt2 = new string(*p2->getName());
    t1->setOwner(pt1);
    t1->setArmies(n10);
    t2->setOwner(pt2);
    t2->setArmies(n10);
    t3->setOwner(pt1);
    t3->setArmies(n10);
    t4->setOwner(pt1);
    t4->setArmies(n10);

    //valid samples
    Deploy *dep1 = new Deploy(p1, n9, t1);
    Advance *adv1 = new Advance(p1, n5, t1, t2);
    Bomb *bom1 = new Bomb(p1, t2);
    Blockade *blo1 = new Blockade(p1, t1);
    Airlift *air1 = new Airlift(p1, n2, t3, t4);
    Negotiate *neg1 = new Negotiate(p1, p2);

    //invalid samples
    Deploy *dep2 = new Deploy(p1, n9, t2);
    Advance *adv2 = new Advance(p1, n5, t1, t3);
    Bomb *bom2 = new Bomb(p1, t1);
    Blockade *blo2 = new Blockade(p1, t2);
    Airlift *air2 = new Airlift(p1, n2, t1, t2);
    Negotiate *neg2 = new Negotiate(p1, p1);

    OrdersList *l1 = new OrdersList();
    l1->addOrder(dep1);
    l1->addOrder(adv1);
    l1->addOrder(bom1);
    l1->addOrder(blo1);
    l1->addOrder(air1);
    l1->addOrder(neg1);

    OrdersList *l2 = new OrdersList();
    l2->addOrder(dep2);
    l2->addOrder(adv2);
    l2->addOrder(bom2);
    l2->addOrder(blo2);
    l2->addOrder(air2);
    l2->addOrder(neg2);

    cout << "1. validation before execution test:" << endl;
    cout << "valid examples:" << endl;
    cout << endl;
    for (int i = 1; i <= l1->getSize(); i++) {
        l1->getOrder(i)->execute();
        cout << endl;
    }

    //reinitialize player and map data:
    p1->CardsOwned.clear();
    p2->CardsOwned.clear();
    p1->clearAllyList();
    p2->clearAllyList();
    t1->setOwner(pt1);
    t1->setArmies(n10);
    t2->setOwner(pt2);
    t2->setArmies(n10);
    t3->setOwner(pt1);
    t3->setArmies(n10);
    t4->setOwner(pt1);
    t4->setArmies(n10);

    cout << "invalid examples:" << endl;
    cout << endl;

    for (int i = 1; i <= l2->getSize(); i++) {
        l2->getOrder(i)->execute();
        cout << endl;
    }

    cout << "2. ownership transfer test after successful attack:" << endl;
    cout << endl;
    //reinitialize player and map data:
    p1->CardsOwned.clear();
    p2->CardsOwned.clear();
    p1->clearAllyList();
    p2->clearAllyList();
    t1->setOwner(pt1);
    t1->setArmies(n10);
    t2->setOwner(pt2);
    t2->setArmies(n1);
    t3->setOwner(pt1);
    t3->setArmies(n10);
    t4->setOwner(pt1);
    t4->setArmies(n10);

    Advance *adv3 = new Advance(p1, n8, t1, t2);
    OrdersList *l3 = new OrdersList();
    l3->addOrder(adv3);
    l3->getOrder(1)->execute();
    cout << endl;

    cout << "3. card given to player after conquer test:" << endl;
    cout << endl;
    //reinitialize player and map data:
    p1->CardsOwned.clear();
    p2->CardsOwned.clear();
    p1->clearAllyList();
    p2->clearAllyList();
    t1->setOwner(pt1);
    t1->setArmies(n10);
    t2->setOwner(pt2);
    t2->setArmies(n1);
    t3->setOwner(pt1);
    t3->setArmies(n10);
    t4->setOwner(pt1);
    t4->setArmies(n10);

    cout << "Cards from the player owns before conquer:" << endl;
    for (std::string *Card: p1->CardsOwned) {
        if (Card != nullptr) { // Check for nullptr
            std::cout << *Card << std::endl;
        } else {
            std::cout << "Null" << std::endl;
        }
    }
    cout << endl;
    l3->getOrder(1)->execute();
    cout << endl;
    cout << "Cards from the player owns after conquer:" << endl;
    for (std::string *Card: p1->CardsOwned) {
        if (Card != nullptr) { // Check for nullptr
            std::cout << *Card << std::endl;
        } else {
            std::cout << "Null" << std::endl;
        }
    }

    cout << "4. Negotiation test:" << endl;
    cout << endl;
    //reinitialize player and map data:
    p1->CardsOwned.clear();
    p2->CardsOwned.clear();
    p1->clearAllyList();
    p2->clearAllyList();
    t1->setOwner(pt1);
    t1->setArmies(n10);
    t2->setOwner(pt2);
    t2->setArmies(n10);
    t3->setOwner(pt1);
    t3->setArmies(n10);
    t4->setOwner(pt1);
    t4->setArmies(n10);

    Advance *adv4 = new Advance(p1, n1, t1, t2);
    Negotiate *neg3 = new Negotiate(p1, p2);
    Advance *adv5 = new Advance(p1, n1, t1, t2);
    OrdersList *l4 = new OrdersList();
    l4->addOrder(adv4);
    l4->addOrder(neg3);
    l4->addOrder(adv5);
    for (int i = 1; i <= l4->getSize(); i++) {//run the orderlist
        l4->getOrder(i)->execute();
        cout << endl;
    }
    cout << endl;

    cout << "5. Blockade ownership trasfer test:" << endl;
    cout << endl;
    Blockade *blo3 = new Blockade(p2, t2);
    OrdersList *l5 = new OrdersList();
    l5->addOrder(blo3);
    cout << "Ownership of territory 2 before blockade:" << endl;
    cout << "Territory 2 belongs to " << *(t2->getOwner()) << endl;
    cout << endl;
//    l5->getOrder(1)->execute();
    cout << endl;
    cout << "Ownership of territory 2 after blockade:" << endl;
    cout << "Territory 2 belongs to " << *(t2->getOwner()) << endl;
    cout << endl;

    cout << "6. player run order test:" << endl;
    cout << endl;
    //reinitialize player and map data:
    p1->CardsOwned.clear();
    p2->CardsOwned.clear();
    p1->clearAllyList();
    p2->clearAllyList();
    t1->setOwner(pt1);
    t1->setArmies(n10);
    t2->setOwner(pt2);
    t2->setArmies(n10);
    t3->setOwner(pt1);
    t3->setArmies(n10);
    t4->setOwner(pt1);
    t4->setArmies(n10);
    //"Deploy", "Advance", "Bomb", "Blockade", "Airlift", "Negotiate"
    string *dep = new string("Deploy");
    string *adv = new string("Advance");
    string *bom = new string("Bomb");
    string *blo = new string("Blockade");
    string *air = new string("Airlift");
    string *neg = new string("Negotiate");
    p1->addCard(dep);
    p1->addCard(adv);
    p1->addCard(bom);
    p1->addCard(blo);
    p1->addCard(air);
    p1->addCard(neg);

    Deploy *dep6 = new Deploy(p1, n9, t1);
    Advance *adv6 = new Advance(p1, n5, t1, t2);
    Bomb *bom6 = new Bomb(p1, t2);
    Blockade *blo6 = new Blockade(p1, t1);
    Airlift *air6 = new Airlift(p1, n2, t3, t4);
    Negotiate *neg6 = new Negotiate(p1, p2);

    OrdersList *l6 = new OrdersList();
    l6->addOrder(dep6);
    l6->addOrder(adv6);
    l6->addOrder(bom6);
    l6->addOrder(blo6);
    l6->addOrder(air6);
    l6->addOrder(neg6);

    string pcard;
    std::cout << "Enter the card you want to play: ";
    std::getline(std::cin, pcard);
    if (pcard == "Deploy") {
        if (p1->ownsCard(dep, p1->CardsOwned)) {
            l6->getOrder(1)->execute();
        } else {
            std::cout << "You don't have this card.";
        }
    } else if (pcard == "Advance") {
        if (p1->ownsCard(adv, p1->CardsOwned)) {
            l6->getOrder(2)->execute();
        } else {
            std::cout << "You don't have this card.";
        }
    } else if (pcard == "Bomb") {
        if (p1->ownsCard(bom, p1->CardsOwned)) {
            l6->getOrder(3)->execute();
        } else {
            std::cout << "You don't have this card.";
        }
    } else if (pcard == "Blockade") {
        if (p1->ownsCard(blo, p1->CardsOwned)) {
            l6->getOrder(4)->execute();
        } else {
            std::cout << "You don't have this card.";
        }
    } else if (pcard == "Airlift") {
        if (p1->ownsCard(air, p1->CardsOwned)) {
            l6->getOrder(5)->execute();
        } else {
            std::cout << "You don't have this card.";
        }
    } else if (pcard == "Negotiate") {
        if (p1->ownsCard(neg, p1->CardsOwned)) {
            l6->getOrder(6)->execute();
        } else {
            std::cout << "You don't have this card.";
        }
    } else {
        std::cout << "Invalid card input.";
    }


    return 0;
}