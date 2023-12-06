#include <string>
#include <vector>
#include <iostream>
#include "Cards.h"

using std::cout;
using std::endl;
using std::ostream;
using std::istream;
using namespace std;


/**
 * Default constructor
*/
Card::Card() : cardTypeName("Default") {
}

/**
* Copy constructor
*/
Card::Card(const Card &card) : cardTypeName(card.cardTypeName) {}

Card::Card(cardType type) {
    switch (type) {
        case cardType::Deploy:
            cardTypeName = "Deploy";
            break;
        case cardType::Advance:
            cardTypeName = "Advance";
            break;
        case cardType::Bomb:
            cardTypeName = "Bomb";
            break;
        case cardType::Blockade:
            cardTypeName = "Blockade";
            break;
        case cardType::Airlift:
            cardTypeName = "Airlift";
            break;
        case cardType::Negotiate:
            cardTypeName = "Negotiate";
            break;
    }
}

/**
 * Takes a card from the player's hand,
 * creates an order,
 * puts the card back into the deck
 * @param index number of cards
 * @param hand player's hand
 * @param player current player
 * @param ordersList the orders list
 * @param origin the origin territory
 * @param target the target territory
 */
void
Card::play(int index, Hand *hand, Player *player, OrdersList *ordersList, Territory *origin, Territory *target) {

    //orders for each card

    auto deploy = *new Deploy();
    auto *advance = new Advance();
    auto *bomb = new Bomb();
    auto *blockade = new Blockade();
    auto *airlift = new Airlift();
    auto *negotiate = new Negotiate();

    //temp pointer to the card played
    Card *playedCard = hand->getCardInHand(index);

    string cardTypeName = playedCard->getCardTypeName();

    int card = 0;
    switch (enumToInt(cardTypeName)) {
//        case 0:
//            cout << "Deploy card played" << endl;
//            ordersList->push_back(deploy);
//
//            break;}
//        case 1:
//            cout << "Advance card played" << endl;
//            ordersList->push_back(advance);
//            break;
//        case 2:
//            cout << "Bomb card played" << endl;
//            ordersList.push_back(bomb);
//            break;
//        case 3:
//            cout << "Blockade card played" << endl;
//            ordersList.push_back(blockade);
//            break;
//        case 4:
//            cout << "AirLift card played" << endl;
//            ordersList.push_back(airlift);
//            break;
//        case 5:
//            cout << "Negotiate card played" << endl;
//            ordersList.push_back(negotiate);
//            cout << "test" << endl;
//
//            break;
    }

    //Removes card played from the hand
    hand->removeCard(index);
}

string Card::getCardTypeName() {
    return cardTypeName;
}

int Card::enumToInt(string name) {
    if (name == "Deploy") return 0;
    if (name == "Advance") return 1;
    if (name == "Bomb") return 2;
    if (name == "Blockade") return 3;
    if (name == "Airlift") return 4;
    if (name == "Negotiate") return 5;

    return 5; //error, not the right name entered
}


/**
 * Default constructor
 * Sets the default deck to 5 cards
 * Creates a card of each type
 */
Deck::Deck() : sizeDeck(6) {
    cards.push_back(new Card(Card::cardType::Deploy));
    cards.push_back(new Card(Card::cardType::Advance));
    cards.push_back(new Card(Card::cardType::Bomb));
    cards.push_back(new Card(Card::cardType::Blockade));
    cards.push_back(new Card(Card::cardType::Airlift));
    cards.push_back(new Card(Card::cardType::Negotiate));
}

/**
 * Parameterized constructor
 * Creates at least one of each type of card
 * Randomizes the rest of the cards
 * param number size of the deck
 */
Deck::Deck(int number) : sizeDeck(number) {
    int numberCorrected{number - 6};

    //creates a card of each type
    cards.push_back(new Card(Card::cardType::Deploy));
    cards.push_back(new Card(Card::cardType::Advance));
    cards.push_back(new Card(Card::cardType::Bomb));
    cards.push_back(new Card(Card::cardType::Blockade));
    cards.push_back(new Card(Card::cardType::Airlift));
    cards.push_back(new Card(Card::cardType::Negotiate));

    for (int i = 0; i < numberCorrected; i++) {

        //generates a random number from 0 to 4
        int typeNumber{rand() % 4};

        switch (typeNumber) {
            case 0:
                cards.push_back(new Card(Card::cardType::Deploy));
                break;
            case 1:
                cards.push_back(new Card(Card::cardType::Advance));
                break;
            case 2:
                cards.push_back(new Card(Card::cardType::Bomb));
                break;
            case 3:
                cards.push_back(new Card(Card::cardType::Blockade));
                break;
            case 4:
                cards.push_back(new Card(Card::cardType::Airlift));
                break;
            case 5:
                cards.push_back(new Card(Card::cardType::Negotiate));
                break;
        }
    }

}

/**
 * Copy constructor
 */
Deck::Deck(const Deck &deck) : sizeDeck(deck.sizeDeck) {
    for (int i = 0; i < deck.getCardCount(); i++) {
        cards.push_back(deck.cards[i]);
    }
}

/**
 * Sets the size of the deck
 * @param number
 */
void Deck::setSize(int number) {
    sizeDeck = number;
}

/**
 * @return the size of the deck
 */
int Deck::getSize() {
    return sizeDeck;
}

/**
 * Takes a card from the deck and adds it to the player's hand
 * @param return the drawn card
 */
Card *Deck::draw() {
    //generates a random number from 0 to the size of the deck [15]
    int index{rand() % (int) cards.size()};

    //temp pointer to the drawn card
    Card *cardDrawn = cards[index];

    //removes the card from the deck
    cards.erase(cards.begin() + index);

    return cardDrawn;
}

/**
 *
 * @return deck
 */
Deck *Deck::getDeck() {
    Deck *deck;
    return deck;

}

/**
 * Adds a card to the deck
 * @param card
 */
void Deck::addCard(Card *card) {
    cards.push_back(card);
    cout << "\nThe " << card->getCardTypeName() << " card has been added to the deck." << '\n';
}

/**
 * Destructor for the deck object
 */
Deck::~Deck() {
    //prevents the deck from resizing itself
    cards.reserve(cards.size());

    //removes each pointer by pointing them to NULL
    for (int i = 0; i < cards.size(); i++) {
        cards[i] = NULL;
    }

    //clears the vector
    cards.clear();
    delete getDeck();
}

/**
 * Default constructor
 * the default max size of a hand is 3
 */
Hand::Hand() : maxSizeHand(3) {}
//cardsInHand(new std::vector<Card*>()) {}

/**
 * Paramaterized constructor
 * param number takes the size of the hand
 */
Hand::Hand(int number) : maxSizeHand(number) {
    Hand *hand = new Hand();
}

/**
 * Copy constructor
 */
Hand::Hand(const Hand &hand) : maxSizeHand(hand.maxSizeHand) {
    //Copying each element from the original hand to the new one
    for (int i = 0; i < hand.cardsInHand.size(); i++) {
        cardsInHand.push_back(hand.cardsInHand[i]);
    }
}

/**
 * Adds a card to the hand
 */
void Hand::addHand(Card *ptrCard) {
    cardsInHand.push_back(ptrCard);
}

/**
 * Gets the card in the hand at a specific index
 */
Card *Hand::getCardInHand(int index) {
    return cardsInHand.at(index);
}

/**
 * Return all cards in the hand
 */
vector<Card *> Hand::getCardsInHand() {
    return cardsInHand;
}

/**
 * Removes a card at a specified index from the hand
 */
void Hand::removeCard(int index) {
    cardsInHand.erase(cardsInHand.begin() + index);
}

/**
 * Checks if the hand is full or not
 */
bool Hand::handFull() {
    if (cardsInHand.size() == maxSizeHand) {
        cout << "\nYour hand is full." << '\n';
        return true;
    }
    return false;
}

/**
 * Destructor for the hand object
 */
Hand::~Hand() {
    delete getCardInHand(3);
    //prevents the hand from resizing itself
    cardsInHand.reserve(cardsInHand.size());

    //removes each pointer by pointing them to NULL
    for (int i = 0; i < cardsInHand.size(); i++) {
        cardsInHand[i] = NULL;
    }

    //clears the vector
    cardsInHand.clear();
}

/**
 * Overloads the assignment operator
 */
Card &Card::operator=(const Card &card) {
    this->cardTypeName = card.cardTypeName;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Deck &deck) {
    os << "Hand contains: " << deck.getCardCount() << " cards.";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Hand &hand) {
    os << "Hand content here: ";
    //os << "Deck contains: " << hand.getCardCount() << " cards.";
    return os;
}


