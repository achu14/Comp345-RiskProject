#pragma once

#include "Player.h"
#include "Map.h"
#include <iostream>
#include <vector>

using namespace std;

class OrdersList;

class Order {
public:
    class Player *player;
    int *armyNum;
    Territory *startTerritory;
    Territory *targetTerritory;
    class Player *targetPlayer;
    static bool neutralPlayerAttacked;

    virtual ostream &printOrder(ostream &output) const = 0;

    virtual bool validate() const = 0;

    virtual void execute() const = 0;

    virtual Order *copy() const = 0;

    virtual ~Order() = default;

private:
    friend ostream &operator<<(ostream &output, Order &o);
};

class OrdersList {
public:
    OrdersList();

    OrdersList(const OrdersList &o);

    void addOrder(Order *order);

    void remove(int index);

    void moveOrderUp(int index);

    void moveOrderDown(int index);

    int getSize();

    ostream &printList(ostream &output);

    Order *getOrder(int index);

    OrdersList &operator=(const OrdersList &o);

    ~OrdersList();

private:
    vector<Order *> ordersList;

    friend ostream &operator<<(ostream &output, OrdersList &o);
};

//order types
class Deploy : public Order {
public:
    Deploy();

    Deploy(const Deploy &d);

    Deploy(class Player *player, int *armyNum, Territory *targetTerritory);

    ostream &printOrder(ostream &output) const override;

    void execute() const override;

    bool validate() const override;

    Deploy *copy() const override;

    Deploy &operator=(const Deploy &d);

    ~Deploy() override;

};

class Advance : public Order {
public:
    Advance();

    Advance(const Advance &a);

    Advance(class Player *player, int *armyNum, Territory *startTerritory, Territory *targetTerritory);

    ostream &printOrder(ostream &output) const override;

    void execute() const override;

    bool validate() const override;

    Advance *copy() const override;

    Advance &operator=(const Advance &a);

    ~Advance() override;

};

class Bomb : public Order {
public:
    Bomb();

    Bomb(const Bomb &b);

    explicit Bomb(class Player *player, Territory *targetTerritory);

    ostream &printOrder(ostream &output) const override;

    void execute() const override;

    bool validate() const override;

    Bomb *copy() const override;

    Bomb &operator=(const Bomb &b);

    ~Bomb() override;


};

class Blockade : public Order {
public:
    Blockade();

    Blockade(const Blockade &b);

    explicit Blockade(class Player *player, Territory *targetTerritory);

    ostream &printOrder(ostream &output) const override;

    void execute() const override;

    bool validate() const override;

    Blockade *copy() const override;

    Blockade &operator=(const Blockade &b);

    ~Blockade() override;

};

class Airlift : public Order {
public:
    Airlift();

    Airlift(const Airlift &b);

    Airlift(class Player *player, int *armyNum, Territory *sourceTerritory, Territory *targetTerritory);

    ostream &printOrder(ostream &output) const override;

    void execute() const override;

    bool validate() const override;

    Airlift *copy() const override;

    Airlift &operator=(const Airlift &a);

    ~Airlift() override;

};

class Negotiate : public Order {
public:
    Negotiate();

    Negotiate(const Negotiate &b);

    explicit Negotiate(class Player *player, class Player *targetPlayer);

    ostream &printOrder(ostream &output) const override;

    void execute() const override;

    bool validate() const override;

    Negotiate *copy() const override;

    Negotiate &operator=(const Negotiate &n);

    ~Negotiate() override;

};