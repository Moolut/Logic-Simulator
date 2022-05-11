#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

int main()
{
    sf::Texture andGate,led;
    if (!andGate.loadFromFile("../assets/AND.png"))
    {
        cout << "Failed to load texture!" << endl;
        return -1;
    }

    sf::Sprite spriteAND,spriteLED;
    spriteAND.setTexture(andGate);
    spriteAND.setColor(sf::Color(0, 255, 0));
    spriteAND.setRotation(90.f); // absolute angle
    spriteAND.setPosition(sf::Vector2f(400.f, 300.f)); // absolute position
    spriteAND.setScale(sf::Vector2f(4.f, 4.f)); // absolute scale factor

    if (!led.loadFromFile("../assets/LEDON.png"))
    {
        cout << "Failed to load texture!" << endl;
        return -1;
    }

    spriteLED.setTexture(led);
    spriteLED.setScale(sf::Vector2f(2.f, 2.f)); // absolute scale factor


    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Logic Simulator");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    std::cout << "the right button was pressed" << std::endl;
                    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                    std::cout << "mouse y: " << event.mouseButton.y << std::endl;

                    std::cout << "LED GLOBAL BOUNDS: " << spriteLED.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) << std::endl;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    std::cout << "the right button was released" << std::endl;
                    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                    std::cout << "mouse y: " << event.mouseButton.y << std::endl;

                    spriteLED.setPosition(event.mouseButton.x, event.mouseButton.y);

                }
            }
        }

        window.clear(sf::Color::Black);

        // draw everything here...
        window.draw(spriteAND);
        window.draw(spriteLED);

        // end the current frame
        window.display();
    }
    return 0;
}