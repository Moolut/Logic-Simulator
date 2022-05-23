#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.hpp"
#include "Simulator.hpp"

using namespace std;


// InitToolbar function initialize the toolbar gates 
// It sets their position to the left side of the screen
// And locks them
void InitToolbar(Simulator *sim);

int main()
{
    // Create an instance of simulator class
    Simulator simulator;

    // Call the InitToolbar function by giving the address of simulator object
    InitToolbar(&simulator);

    // Call the simulator's isRunning method which indicates is the window open or not
    
    while (simulator.isRunning())
    {
        // Call the update function of the simulator object in loop
        simulator.update();

        // Call the render function of the simulator object
        simulator.render();
    }
    return 0;
};

void InitToolbar(Simulator *sim)
{

    // Create pointer instances of the toolbar gates
    AndGate *_toolbar_and = new AndGate();
    OrGate *_toolbar_or = new OrGate();
    XorGate *_toolbar_xor = new XorGate();
    NotGate *_toolbar_not = new NotGate();
    DFFGate *_toolbar_dff = new DFFGate();

    Led *_toolbar_led = new Led();
    VDD *_toolbar_vdd = new VDD();
    GND *_toolbar_gnd = new GND();
    CLK *_toolbar_clk = new CLK();


    // Set the toolbar objects positions & and lock them
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


    // Add the toolbar objects to the Simulator class objects list
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