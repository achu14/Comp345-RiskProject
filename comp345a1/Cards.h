#ifndef CARDS_H
#define CARDS_H

#pragma once

#include <list>
#include "Player.h"

using namespace std;

class Hand;

class Player;

class Card {
public:
    enum class cardType {
        Deploy, Advance, Bomb, Blockade, Airlift, Negotiate
    };

    Card();

    explicit Card(cardType);

    Card(const Card &);

    Card &operator=(const Card &);

    string getCardTypeName();

    int enumToInt(string);

    void
    play(int index, Hand *hand, Player *player, OrdersList *ordersList, Territory *origin, Territory *target);

private:
    string cardTypeName;

};

class Deck {
    int getCardCount() const { return cards.size(); }

public:
    friend std::ostream &operator<<(std::ostream &os, const Deck &deck);

public:
    Deck();

    Deck(int);

    Deck(const Deck &);

    Deck &operator=(const Deck &);

    void setSize(int sizeDeck);

    int getSize();

    Card *draw();

    void addCard(Card *card);

    ~Deck();

public:

private:
    int sizeDeck{};

    friend std::ostream &operator<<(std::ostream &os, const Deck &deck);


    vector<Card *> cards;

    Deck *getDeck();

public:
    int getCardsCount() const {
        return cards.size();
    }
};

class Hand {
public:
    friend std::ostream &operator<<(std::ostream &os, const Hand &hand);

    Hand();

    Hand(int);

    Hand(const Hand &);

    Hand &operator=(const Hand &);

    void addHand(Card *);

    Card *getCardInHand(int);

    vector<Card *> getCardsInHand();

    void removeCard(int);

    bool handFull();

    ~Hand();

    int maxSizeHand;
    vector<Card *> cardsInHand;
};

#endif // CARDS_H
