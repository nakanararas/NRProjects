#include <SFML/Graphics.hpp>
#include <iostream>

class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;

    Button(const sf::Font& font, const std::string& label, sf::Vector2f size, sf::Vector2f pos)
        : text(font, label, 24)  // ✅ must initialize sf::Text in the initializer list
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
        auto mouse = sf::Mouse::getPosition(window);
        sf::Vector2f point(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
        return shape.getGlobalBounds().contains(point); // ✅ contains() now takes a Vector2f
    }

    bool isClicked(sf::RenderWindow& window, sf::Event& event) {
        return isMouseOver(window) && event.is<sf::Event::MouseButtonPressed>();
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
};

int main() {
    sf::VideoMode mode({800, 600});
    sf::RenderWindow window(mode, "Blackjack", sf::State::Windowed);

    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Font failed to load!\n";
        return 1;
    }

    Button hitBtn(font, "Hit", {120, 50}, {200, 500});
    Button standBtn(font, "Stay", {120, 50}, {480, 500});

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (hitBtn.isClicked(window, *event))
                std::cout << "You clicked HIT!\n";
            if (standBtn.isClicked(window, *event))
                std::cout << "You clicked STAY!\n";
        }

        hitBtn.shape.setFillColor(hitBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50));
        standBtn.shape.setFillColor(standBtn.isMouseOver(window) ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50));

        window.clear(sf::Color(0, 100, 0));
        hitBtn.draw(window);
        standBtn.draw(window);
        window.display();
    }

    return 0;
}
