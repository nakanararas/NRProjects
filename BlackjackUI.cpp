#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <random>
#include <string>
#include <sstream>

using namespace std;


class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;

    Button(const sf::Font& font, const std::string& label, sf::Vector2f size, sf::Vector2f pos)
        : text(font, label, 24)
    {
        shape.setSize(size);
        shape.setPosition(pos);
        shape.setFillColor(sf::Color(50, 50, 50));
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);

        // Center text inside the rectangle
        auto bounds = text.getLocalBounds();
        text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        text.setPosition({pos.x + size.x / 2.f, pos.y + size.y / 2.f - 5.f});
    }

    bool isMouseOver(sf::RenderWindow& window) {
        auto pixelPos = sf::Mouse::getPosition(window);
        auto worldPos = window.mapPixelToCoords(pixelPos);
        return shape.getGlobalBounds().contains(worldPos);
    }

    bool isClicked(sf::RenderWindow& window, sf::Event& event) {
        return isMouseOver(window) && event.is<sf::Event::MouseButtonPressed>();
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
};

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

    void printPlayerWins() const {
        cout << "You Win!" << endl;
    }

    void printPlayerLoses() const {
        cout << "Sorry you lost :(" << endl;
    }

    void printPlayerTies() const {
        cout << "Push! It's a tie!" << endl;
    }

    void printBalance() const {
        cout << "Balance: " << balance << endl;
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

    bool betting = true;
    bool playing = false;
    bool results = false;

    sf::VideoMode mode({800, 600});
    sf::RenderWindow window(mode, "Blackjack", sf::State::Windowed);

    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Font failed to load!\n";
        return 1;
    }

    Button betFiveBtn(font, "+ $5", {80, 50}, {250, 200});
    Button betTenBtn(font, "+ $10", {80, 50}, {350, 200});
    Button betTwnBtn(font, "+ $20", {80, 50}, {450, 200});
    Button minFiveBtn(font, "- $5", {80, 50}, {250, 300});
    Button minTenBtn(font, "- $10", {80, 50}, {350, 300});
    Button minTwnBtn(font, "- $20", {80, 50}, {450, 300});

    Button playBtn(font, "Play", {80, 50}, {350, 400});

    Button hitBtn(font, "Hit", {120, 50}, {200, 500});
    Button stayBtn(font, "Stay", {120, 50}, {480, 500});

    Button playAgainBtn(font, "Play again?", {150, 50}, {325, 300});

    sf::Text balanceText(font);
    balanceText.setCharacterSize(24);
    balanceText.setFillColor(sf::Color::White);
    balanceText.setPosition({20.f,40.f});

    sf::Text betText(font);
    betText.setCharacterSize(24);
    betText.setFillColor(sf::Color::White);
    betText.setPosition({20.f, 70.f});

    sf::Text playerCard1(font);
    playerCard1.setString(game.deck[game.cardOne]);
    playerCard1.setPosition(sf::Vector2f(340.f,400.f));

    sf::Text playerCard2(font);
    playerCard2.setString(game.deck[game.cardTwo]);
    playerCard2.setPosition(sf::Vector2f(440.f,400.f));

    sf::Text comp1(font);
    comp1.setString(game.deck[game.compOne]);
    comp1.setPosition(sf::Vector2f(340.f,50.f));

    sf::Text comp2(font);
    comp2.setString(game.deck[game.compTwo]);
    comp2.setPosition(sf::Vector2f(440.f,50.f));


    while (window.isOpen()) {
        while (auto eventOpt = window.pollEvent()) {
            sf::Event event = *eventOpt;
            if (event.is<sf::Event::Closed>())
                window.close();

        if(betting) {
            if (betFiveBtn.isClicked(window, event))
                bet += 5.0;
            if (betTenBtn.isClicked(window, event))
                bet += 10.0;
            if (betTwnBtn.isClicked(window, event))
                bet += 20.0;
            if(bet >= game.balance) {
                betting = false;
                playing = true;
                game.reset();
                game.dealCards();
            }
            if (minFiveBtn.isClicked(window, event))
                bet -= 5.0;
            if (minTenBtn.isClicked(window, event))
                bet -= 10.0;
            if (minTwnBtn.isClicked(window, event))
                bet -= 20.0;
            if(bet <= 0) {
                bet = 0.0;
            }
            if (playBtn.isClicked(window, event)) {
                betting = false;
                playing = true;
                game.reset();
                game.dealCards();
            }
        }
        else if (playing) {
            playerCard1.setString(game.deck[game.cardOne]);
            playerCard2.setString(game.deck[game.cardTwo]);

            comp1.setString(game.deck[game.compOne]);
            comp2.setString(game.deck[game.compTwo]);

            if (hitBtn.isClicked(window, event)) {
                game.playerHits();
                if (game.playerTotal > 21) {
                    playing = false;
                    results = true;
                }
            }
            if (stayBtn.isClicked(window, event)) {
                playing = false;
                results = true;
            }
        }

        else if (results) {
            if (playAgainBtn.isClicked(window, event)) {
                betting = true;
                results = false;
                bet = 0.0;
            }
        }

        betFiveBtn.shape.setFillColor(betFiveBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(50, 100, 50));
        betTenBtn.shape.setFillColor(betTenBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(50, 100, 50));
        betTwnBtn.shape.setFillColor(betTwnBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(50, 100, 50));
        minFiveBtn.shape.setFillColor(minFiveBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(100, 50, 50));
        minTenBtn.shape.setFillColor(minTenBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(100, 50, 50));
        minTwnBtn.shape.setFillColor(minTwnBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(100, 50, 50));
        playBtn.shape.setFillColor(playBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50));

        hitBtn.shape.setFillColor(hitBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50));
        stayBtn.shape.setFillColor(stayBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50));

        playAgainBtn.shape.setFillColor(playAgainBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50));


        window.clear(sf::Color(0, 100, 0));

        ostringstream ss;

        if (betting) {
            ss << fixed << setprecision(2);
            ss << game.balance;
            balanceText.setString("Balance: $" + ss.str());
            window.draw(balanceText);

            ss.str("");
            ss << fixed << setprecision(2) << bet;  
            betText.setString("Bet: $" + ss.str());
            window.draw(betText);

            betFiveBtn.draw(window);
            betTenBtn.draw(window);
            betTwnBtn.draw(window);
            minFiveBtn.draw(window);
            minTenBtn.draw(window);
            minTwnBtn.draw(window);
            playBtn.draw(window);
        }
        else if (playing) {
            window.draw(betText);
            
            window.draw(playerCard1);
            window.draw(playerCard2);
            window.draw(comp1);
            window.draw(comp2);

            hitBtn.draw(window);
            stayBtn.draw(window);
        }
        else if (results) {
            window.clear(sf::Color(0, 100, 0));
            playAgainBtn.draw(window);
        }

        
        window.display();




        }
    }

    return 0;
}
