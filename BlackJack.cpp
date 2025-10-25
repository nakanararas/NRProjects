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
    double balance;
    bool playerWins;
    int round;
    int playersHands[2];
    bool multipleHands;

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
        playerWins = false;
        round = 1;
        multipleHands = false;

    }

    void dealCards() {
        cout << "Dealing Cards..." << endl;

        //Player's Cards
        cardOne = dist(gen);
        cardTwo = dist(gen);
        if (deck[cardOne] == "A" && deck[cardTwo] == "A")
            playerTotal = 2;
        else
            playerTotal = getValue(cardOne) + getValue(cardTwo);

        //Computer's cards
        compOne = dist(gen);
        compTwo = dist(gen);

        if (deck[compOne] == "A" && deck[compTwo] == "A")
            compTotal = 2;
        else
            compTotal = getValue(compOne) + getValue(compTwo);
    }

    void placeBet(double bet) {
        if (playerWins) {
            balance += bet;
        }
        else {
            balance -= bet;
        }
    }

    int getValue(int card) const {
        if (card <= 8) return card + 2;
        if (card <= 11) return 10;
        return 11;
    }

    void playerHits() {
        int newCard = dist(gen);
        cout << "This is your new card: " << deck[newCard] << endl;
        playerTotal += getValue(newCard);
        cout << "Your total is: " << playerTotal << endl;
    }

    int playerSplits(int card) {
        cout << "Hitting for this card: " << deck[card] << endl;
        int hand = getValue(card);
        bool keepHitting = true;
        int answer = 0;
        while (keepHitting) {
            int newCard = dist(gen);
            cout << "This is your new card: " << deck[newCard] << endl;
            hand += getValue(newCard);
            cout << "Your total for this hand is: " << hand << endl;
            if(hand > 21) {
                cout << "Sorry you bust!" << endl;
                return -1;
            }
            cout << "Hit (1) or Stay (2)" << endl;
            cin >> answer;
            //Add if the input is not 1 or 2
            // if (answer != 1 || answer != 2) {
            //     cout << "Please select 1 or 2" << endl;
            // }
            if (answer == 2) {
                keepHitting = false;
                cout << "Your total for this hand is " << hand << endl;
            }
        }
        return hand;
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

    bool askToPlayAgain() {
        char answer = ' ';
        while (true) {
            cout << "Would you like to play again? (y) for yes. (n) or no" << endl;
            cin >> answer;

            if (answer == 'y')
                return true;
            else if (answer == 'n')
                return false;
            else {
                cout << "Please select y or n" << endl;
                continue;
            }
        }
    }
};

int main() {
    BlackJack game;
    char result = ' ';
    bool keepPlaying = true;
    double bet = 0.0;
    game.balance = 20.0;

    while (keepPlaying) {
        game.reset();
        int decision = -1;
        bool playersTurn = true;

        cout << "------------------------------------------------" << endl;
        cout << "Starting new game. RULE: Aces are always 11" << endl;
        cout << "Your balance is $" << game.balance;
        cout << ". How much would you like to bet?" << endl;
        cin >> bet;

        game.dealCards();
        game.printInitCompCards();

        //Tie at beginning of game
        if (game.playerTotal == 21 && game.compTotal == 21) {
            game.printCompCards();
            cout << "Push! It's a tie!" << endl;
            keepPlaying = game.askToPlayAgain();
            continue;
        }

        //Player wins at beginning of game
        if (game.playerTotal == 21) {
            game.printPlayerCards();
            cout << "You win!" << endl;
            game.playerWins = true;
            game.placeBet(bet * 1.5);
            cout << "New balance: $" << game.balance << endl;
            keepPlaying = game.askToPlayAgain();
            continue;
        }

        //Dealer wins at beginning of game
        if (game.compTotal == 21) {
            game.printCompCards();
            cout << "Sorry you lost!" << endl;
            game.playerWins = false;
            game.placeBet(bet);
            cout << "New balance: $" << game.balance << endl;
            if (game.balance <= 0.0) {
                cout << "Sorry you can't play anymore you broke!!" << endl;
                return 0;
            }
            keepPlaying = game.askToPlayAgain();
            continue;
        }

        game.printPlayerCards();

        while (playersTurn) {
            cout << "Your total: " << game.playerTotal << endl;
            if (game.balance < (bet * 2.0)) {
                cout << "Do you want to hit (1) or stay (2)?" << endl;
            }
            else {
                if((game.round == 1) && (game.getValue(game.cardOne) == game.getValue(game.cardTwo))) {
                    cout << "Do you want to hit (1), stay (2), double down (3), or split (4)?" << endl;
                }
                else if((game.round == 1) && (game.getValue(game.cardOne) != game.getValue(game.cardTwo))) {
                    cout << "Do you want to hit (1), stay (2), or double down (3)?" << endl;
                }
                else {
                    cout << "Do you want to hit (1) or stay (2)?" << endl;
                }
            }
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
            if (decision < 0 && decision > 3) {
                cout << "Invalid input. Please enter 1, 2, 3, or 0" << endl;
                continue;
            }

            //quit
            if (decision == 0) {
                cout << "Thanks for playing!" << endl;
                return 0;
            }

            //if they hit
            if (decision == 1) {
                game.playerHits();
            }
            
            //double down (only one card)
            if (decision == 3) {
                game.playerHits();
                bet *= 2.0;
                playersTurn = false;
            }

            //split
            if(decision == 4) {
                int handOne = game.playerSplits(game.cardOne);
                int handTwo = game.playerSplits(game.cardTwo);
                game.playersHands[0] = handOne;
                game.playersHands[1] = handTwo;
                for (int i = 0; i < 2; ++i) {
                    if (handOne == -1 && handTwo != -1) {
                        game.playerTotal = handTwo;
                    }
                    else if (handOne != -1 && handTwo == -1) {
                        game.playerTotal = handOne;
                    }
                    else if (handOne == -1 && handTwo == -1) {
                        keepPlaying = game.askToPlayAgain();
                        goto end_of_round;
                    }
                    else {
                        game.multipleHands = true;
                    }
                }
                playersTurn = false;
            }

            //Check if player busts
            if (game.playerTotal > 21) {
                cout << "Sorry you lost!" << endl;
                game.playerWins = false;
                game.placeBet(bet);
                cout << "New balance: $" << game.balance << endl;
                if (game.balance <= 0.0) {
                    cout << "Sorry you can't play anymore you broke!!" << endl;
                    return 0;
                }
                keepPlaying = game.askToPlayAgain();
                goto end_of_round;
            }

            //if they stay
            if (decision == 2) {
                playersTurn = false;
            }

            game.round++;
        }

        cout << "------------------------------------------------" << endl;
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
                game.playerWins = true;
                if (game.multipleHands) {
                    game.placeBet(bet * 2);
                }
                else {
                    game.placeBet(bet);
                }
                cout << "New balance: $" << game.balance << endl;
                keepPlaying = game.askToPlayAgain();
                goto end_of_round;
            }
        }

        if(game.multipleHands) {
            int wonHands = 0;
            int tiedHands = 0;
            bool tied = false;
            for (int i = 0; i < 2; ++i) {
                cout << "Comparing hand of " << game.playersHands[i];
                if (game.playersHands[i] < game.compTotal) {
                    cout << " ...... Sorry you lost this hand" << endl;
                }
                else if (game.playersHands[i] > game.compTotal) {
                    cout << " ...... You won this hand!" << endl;
                    wonHands++;
                }
                else {
                    cout << " ...... You pushed on this hand" << endl;
                    tiedHands++;
                }
            }
            if (wonHands == 2) {
                game.playerWins = true;
                game.placeBet(bet * 2.0);
            }
            else if (wonHands == 1 && tiedHands == 1) {
                game.playerWins = true;
                game.placeBet(bet);
            }
            else if (wonHands == 1 || tiedHands == 2) {
                //nothing happens
            }
            else if (tiedHands == 1) {
                game.playerWins = false;
                game.placeBet(bet);
            }
            else if (wonHands == 0) {
                game.playerWins = false;
                game.placeBet(bet * 2);
            }
            cout << "Balance: $" << game.balance << endl;
            keepPlaying = game.askToPlayAgain();
            goto end_of_round;
        }

        //Computer wins
        if (game.compTotal > game.playerTotal) {
            cout << "Sorry you lost!" << endl;
            game.playerWins = false;
            game.placeBet(bet);
            cout << "New balance: $" << game.balance << endl;
        }
        //Player wins
        else if (game.compTotal < game.playerTotal) {
            cout << "You win!" << endl;
            game.playerWins = true;
            game.placeBet(bet);
            cout << "New balance: $" << game.balance << endl;
        }
        //Tie
        else if (game.compTotal == game.playerTotal) {
            cout << "Push! It's a tie!" << endl;
        }

        if (game.balance <= 0.0) {
            cout << "Sorry you can't play anymore you broke!!" << endl;
            return 0;
        }
        keepPlaying = game.askToPlayAgain();

    end_of_round:
        ;
    }
    cout << "Thanks for playing!" << endl;

    return 0;
}