#include <iostream>
#include <istream>

#include "CardsDriver.h"

using namespace std;

void testCards() {

    cout << "==========================================" << endl;
    cout << "=             Testing Cards             =" << endl;
    cout << "==========================================" << endl;

    //generates the seed for the random function
    srand((int) time(NULL));

    //creates a deck object
    //sets 6 cards in deck
    cout << "\nInitializing hand" << endl;
    Deck *deck = new Deck(4);
    cout << *deck;

    //sets the number of the cards in a hand to 3
    int cardsHand{};
    cardsHand = 5;
    cout << endl << "You can have a hand of at most 6 cards" << endl;

    /**
    * creates an hand and player object
    * creates a vector of pointers to orders
    * creates an object list of orders
    */
    Hand *hand = new Hand();

    cout << "Creating player object" << endl;
    PlayerStrategy *human = new HumanPlayerStrategy;
    Player *player1 = new Player(new string("Player1"));
    //OrdersList_A2 orders{};

    cout << "Creating orders list" << endl;
    OrdersList ordersList;

    //creates card object
    cout << "Creating card object" << endl;
    Card *card = new Card();

    cout << "Creating origin and target territories" << endl;
    //creates the origin and target territory objects
    Territory *origin = new Territory();
    Territory *target = new Territory();

    cout << endl << "Drawing and adding cards to hand" << endl;
    player1->dealHand();
    cout << *deck << endl;


    int index{};
    float indexf{};
    cout << endl << "What card do you want to play? Enter the index [between 0-2]: ";
    cin >> indexf;

    if (indexf > 3) {
        cout << endl << "Try again";
    } else {
        index = int(indexf);
//        card->play(index, hand, player1, ordersList, origin, target);
//        cout << *deck << *ordersList;
    }
}

