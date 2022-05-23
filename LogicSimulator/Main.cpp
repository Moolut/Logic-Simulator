#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.hpp"
#include "Simulator.hpp"

using namespace std;

void InitToolbar(Simulator *sim);

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

void InitToolbar(Simulator *sim)
{
    AndGate *_toolbar_and = new AndGate();
    OrGate *_toolbar_or = new OrGate();
    XorGate *_toolbar_xor = new XorGate();
    NotGate *_toolbar_not = new NotGate();
    DFFGate *_toolbar_dff = new DFFGate();

    Led *_toolbar_led = new Led();
    VDD *_toolbar_vdd = new VDD();
    GND *_toolbar_gnd = new GND();
    CLK *_toolbar_clk = new CLK();

    _toolbar_and->SetPosition(sf::Vector2f(20.f, 20.f));
    _toolbar_and->Lock();
    _toolbar_or->SetPosition(sf::Vector2f(20.f, 120.f));
    _toolbar_or->Lock();
    _toolbar_xor->SetPosition(sf::Vector2f(20.f, 220.f));
    _toolbar_xor->Lock();
    _toolbar_led->SetPosition(sf::Vector2f(20.f, 320.f));
    _toolbar_led->Lock();
    _toolbar_vdd->SetPosition(sf::Vector2f(20.f, 420.f));
    _toolbar_vdd->Lock();
    _toolbar_not->SetPosition(sf::Vector2f(20.f, 520.f));
    _toolbar_not->Lock();
    _toolbar_gnd->SetPosition(sf::Vector2f(20.f, 620.f));
    _toolbar_gnd->Lock();
    _toolbar_dff->SetPosition(sf::Vector2f(20.f, 720.f));
    _toolbar_dff->Lock();
    _toolbar_clk->SetPosition(sf::Vector2f(20.f, 820.f));
    _toolbar_clk->Lock();

    sim->AddObject(_toolbar_clk);
    sim->AddObject(_toolbar_dff);
    sim->AddObject(_toolbar_gnd);
    sim->AddObject(_toolbar_not);
    sim->AddObject(_toolbar_vdd);
    sim->AddObject(_toolbar_led);
    sim->AddObject(_toolbar_xor);
    sim->AddObject(_toolbar_or);
    sim->AddObject(_toolbar_and);
};