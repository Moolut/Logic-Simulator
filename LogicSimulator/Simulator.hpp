#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.hpp"

class Simulator
{
private:
    sf::RenderWindow *window;   // Pointer to SFML render window
    sf::VideoMode videoMode;    // Sets the window properties
    Object *objects;            // Pointer to a list of objects on screen
    sf::Event event;            // 
    bool clicked = false;       // A boolean that checks the click event type
    bool holding = false;       // A boolean that checks the hold event type which occurs is in drag & drop
    bool drawing = false;       // A boolean that checks are we drawing a wire or not
    sf::Sprite *focus;          // Pointer to sprite
                                // We use it to determine are we holding the object or not
                                // Also helps us to changing the dragged object's position
                                
    Wire *drawing_wire;         // Pointer to wire 
                                // When we start to draw a wire, we are assigning our wire to this pointer
                                // Thus we can hold datas of the new created wire
                                // When we finish our wire drawing, with that pointer we can get the 
                                // end point of the wire

    void initVariables();       // Initialize our variable of the class object (Called in constructor)
    void initWindow();          // Initiazlie our window of the class object (Called in constructor)
public:
    Simulator();                // Constructor
    ~Simulator();               // Deconstructor
    const bool isRunning() const { return this->window->isOpen(); } // Returns the SFML window isOpen value
    void pollEvents();          // In this function we handle the events
    void update();              // Simulator window update function -> It calls only the pollEvents 
    void render();              // Renders our window
    void AddObject(Object *obj); // Add new object to the objects list
    void RemoveObject();         // Remove object from the objects list
    void RemoveWiresConnectToObj(int numOfWiresToBeDeleted);    // Removes the wire of the object which we deleted
                                                                // It takes the # of wires that is connected to object
    void drawElements();        // Draws our objects list elements
};
