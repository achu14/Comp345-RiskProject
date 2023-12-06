#pragma once

#include <iostream>
#include <vector>
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "GameEngine.h"

using namespace std;

class GameEngineLoop;

class PlayerStrategy {
public:
    virtual void issueOrder(Map *map, Player *player, GameEngineLoop *engine) = 0;

    virtual vector<Territory *> toAttack(Player *player, Map *map) = 0;

    virtual vector<Territory *> toDefend(Player *player, Map *map) = 0;

    virtual ~PlayerStrategy() {}

    virtual PlayerStrategy *clone() const = 0;

    string *name = new string("Strategy");
};

/**
 * Subclasses of player strat
 */

class HumanPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Map *map, Player *player, GameEngineLoop *engine) override;

    vector<Territory *> toAttack(Player *player, Map *map) override;

    vector<Territory *> toDefend(Player *player, Map *map) override;

    PlayerStrategy *clone() const override { return new HumanPlayerStrategy(*this); }
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Map *map, Player *player, GameEngineLoop *engine) override;

    vector<Territory *> toAttack(Player *player, Map *map) override;

    vector<Territory *> toDefend(Player *player, Map *map) override;

    PlayerStrategy *clone() const override { return new AggressivePlayerStrategy(*this); }
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Map *map, Player *player, GameEngineLoop *engine) override;

    vector<Territory *> toAttack(Player *player, Map *map) override;

    vector<Territory *> toDefend(Player *player, Map *map) override;

    PlayerStrategy *clone() const override { return new BenevolentPlayerStrategy(*this); }
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Map *map, Player *player, GameEngineLoop *engine) override;

    vector<Territory *> toAttack(Player *player, Map *map) override;

    vector<Territory *> toDefend(Player *player, Map *map) override;

    PlayerStrategy *clone() const override { return new NeutralPlayerStrategy(*this); }
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Map *map, Player *player, GameEngineLoop *engine) override;

    vector<Territory *> toAttack(Player *player, Map *map) override;

    vector<Territory *> toDefend(Player *player, Map *map) override;

    PlayerStrategy *clone() const override { return new CheaterPlayerStrategy(*this); }
};