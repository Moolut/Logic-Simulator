#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.hpp"
#include "Simulator.hpp"

using namespace std;

int main()
{
    // Initialize Simulator
    Simulator simulator;
    AndGate *_toolbar_and = new AndGate();
    OrGate *_toolbar_or = new OrGate();
    XorGate *_toolbar_xor = new XorGate();

    _toolbar_and->sprite.setPosition(sf::Vector2f(40.f, 40.f));
    _toolbar_and->locked = true;
    _toolbar_or->sprite.setPosition(sf::Vector2f(40.f, 140.f));
    _toolbar_or->locked = true;
    _toolbar_xor->sprite.setPosition(sf::Vector2f(40.f, 240.f));
    _toolbar_xor->locked = true;

    simulator.AddObject(_toolbar_xor);
    simulator.AddObject(_toolbar_or);
    simulator.AddObject(_toolbar_and);

    // run the program as long as the window is open
    while (simulator.isRunning())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        simulator.update();

        simulator.render();
    }
    return 0;
};