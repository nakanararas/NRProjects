#include <iostream>
#include <array>
#include <random>
#include <string>

using namespace std;

class BlackJack {
public:
    array<string, 13> deck;
    int cardOne;
    int cardTwo;
    int playerTotal;
    int compOne;
    int compTwo;
    int compTotal;

    //Random generator
    mt19937 gen;
    uniform_int_distribution<int> dist;

    BlackJack()
        : gen(),
        dist(0, 12)
    {
        std::random_device rd;
        gen.seed(rd());

        reset();
    }

    void reset() {
        static const char* initDeck[13] = {
            "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
        };
        for (int i = 0; i < 13; ++i) {
            deck[i] = initDeck[i];
        }
        cardOne = 0;
        cardTwo = 0;
        playerTotal = 0;
        compOne = 0;
        compTwo = 0;
        compTotal = 0;
    }

    void dealCards() {
        cout << "Dealing Cards..." << endl;

        //Player's Cards
        cardOne = dist(gen);
        cardTwo = dist(gen);
        playerTotal = getValue(cardOne) + getValue(cardTwo);

        //Computer's cards
        compOne = dist(gen);
        compTwo = dist(gen);
        compTotal = getValue(compOne) + getValue(compTwo);
    }

    int getValue(int card) const {
        if (card <= 8) return card + 2;
        if (card <= 11) return 10;
        return 11;
    }

    void printPlayerCards() const {
        cout << "Your Cards: " << deck[cardOne] << " " << deck[cardTwo] << endl;
    }

    //Printing at start of game
    void printInitCompCards() const {
        cout << "Dealer's cards: " << deck[compOne] << " " << "[]" << endl;
    }

    void printCompCards() const {
        cout << "Dealer's cards: " << deck[compOne] << " " << deck[compTwo] << endl;
    }
};

int main() {
    BlackJack game;
    char result = ' ';
    int decision = -1;
    bool playersTurn = true;

    game.reset();
    cout << "Starting new game. RULE: Aces are always 11" << endl;
    game.dealCards();
    game.printInitCompCards();

    //Tie at beginning of game
    if (game.playerTotal == 21 && game.compTotal == 21) {
        game.printCompCards();
        cout << "Push! It's a tie!" << endl;
        return 0;
    }

    //Player wins at beginning of game
    if (game.playerTotal == 21) {
        game.printPlayerCards();
        cout << "You win!" << endl;
        return 0;
    }

    //Dealer wins at beginning of game
    if (game.compTotal == 21) {
        game.printCompCards();
        cout << "Sorry you lost!" << endl;
        return 0;
    }

    game.printPlayerCards();

    while (playersTurn) {
        cout << "Your total: " << game.playerTotal << endl;
        cout << "Do you want to hit (1) or stay (2) ?" << endl;
        cout << "To quit, type 0" << endl;

        //Invalid input
        if (!(cin >> decision)) {
            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "Please enter 1, 2, or 0" << endl;
            continue;
        }

        //Invalid input
        if (decision != 1 && decision != 2 && decision != 0) {
            cout << "Invalid input. Please enter 1, 2, or 0" << endl;
            continue;
        }

        //quit
        if (decision == 0) {
            cout << "Thanks for playing!" << endl;
            return 0;
        }

        //if they hit
        if (decision == 1) {
            int newCard = game.dist(game.gen);

            cout << "This is your new card: " << game.deck[newCard] << endl;
            game.playerTotal += game.getValue(newCard);
            cout << "Your total: " << game.playerTotal << endl;
        }

        //Check if player busts
        if (game.playerTotal > 21) {
            cout << "Sorry you lost!" << endl;
            return 0;
        }

        //if player got 21 already, exit to let computer turn
        if (game.playerTotal == 21) {
            playersTurn = false;
        }

        //if they stay
        if (decision == 2) {
            playersTurn = false;
        }
    }

    cout << "Computer's Turn now!" << endl;
    cout << "Your final total: " << game.playerTotal << endl;
    game.printCompCards();
    cout << "Dealer's total: " << game.compTotal << endl;

    while (game.compTotal <= 16) {
        //computer hits
        int newCard = game.dist(game.gen);
        cout << "This is the dealer's new card: " << game.deck[newCard] << endl;
        game.compTotal += game.getValue(newCard);
        cout << "Dealer's total: " << game.compTotal << endl;

        //if Dealer busts
        if (game.compTotal > 21) {
            cout << "Dealer busts! You win!" << endl;
            return 0;
        }
    }

    //Computer wins
    if (game.compTotal > game.playerTotal) {
        cout << "Sorry you lost!" << endl;
    }
    //Player wins
    else if (game.compTotal < game.playerTotal) {
        cout << "You win!" << endl;
    }
    //Tie
    else if (game.compTotal == game.playerTotal) {
        cout << "Push! It's a tie!" << endl;
    }

    return 0;
}