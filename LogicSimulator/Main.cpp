#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.hpp"
#include "Simulator.hpp"

using namespace std;

void InitToolbar(Simulator* sim);

int main()
{
    // Initialize Simulator
    Simulator simulator;
    InitToolbar(&simulator);

    // run the program as long as the window is open
    while (simulator.isRunning())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        simulator.update();

        simulator.render();
    }
    return 0;
};

void InitToolbar(Simulator* sim) {
    AndGate* _toolbar_and = new AndGate();
    OrGate* _toolbar_or = new OrGate();
    XorGate* _toolbar_xor = new XorGate();
    Led* _toolbar_led = new Led();
    VDD* _toolbar_vdd = new VDD();

    _toolbar_and->sprite.setPosition(sf::Vector2f(40.f, 40.f));
    _toolbar_and->locked = true;
    _toolbar_or->sprite.setPosition(sf::Vector2f(40.f, 140.f));
    _toolbar_or->locked = true;
    _toolbar_xor->sprite.setPosition(sf::Vector2f(40.f, 240.f));
    _toolbar_xor->locked = true;
    _toolbar_led->sprite.setPosition(sf::Vector2f(40.f, 340.f));
    _toolbar_led->locked = true;
    _toolbar_vdd->sprite.setPosition(sf::Vector2f(40.f, 440.f));
    _toolbar_vdd->locked = true;

    sim->AddObject(_toolbar_vdd);
    sim->AddObject(_toolbar_led);
    sim->AddObject(_toolbar_xor);
    sim->AddObject(_toolbar_or);
    sim->AddObject(_toolbar_and);
};