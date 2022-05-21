#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.hpp"

class Simulator
{
private:
    sf::RenderWindow *window;   // Pointer to SFML render window
    sf::VideoMode videoMode;
    Object *objects;            // Pointer to a list of objects on screen
    sf::Event event;
    bool clicked = false;       
    bool holding = false;       // is holding an object
    bool drawing = false;       // is drawing wire
    sf::Sprite *focus;
    Wire *drawing_wire;

    void initVariables();
    void initWindow();
public:
    Simulator();
    ~Simulator();
    const bool isRunning() const { return this->window->isOpen(); }
    void pollEvents();
    void update();
    void render();
    void AddObject(Object *obj);
    void RemoveObject();
    void RemoveWiresConnectToObj(int numOfWiresToBeDeleted);
    void drawElements();
};
