#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.hpp"
#include "Simulator.hpp"
using namespace std;

#define WIN_H 600
#define WIN_W 800

Simulator::Simulator()
{
    this->initVariables();
    this->initWindow();
};

Simulator::~Simulator()
{

    delete this->window;
    delete this->objects;
};

void Simulator::initVariables()
{

    this->window = nullptr;
    this->objects = nullptr;
    this->focus = nullptr;
    this->drawing_wire = nullptr;
};

void Simulator::initWindow()
{

    this->videoMode.height = WIN_H;
    this->videoMode.width = WIN_W;

    this->window = new sf::RenderWindow(this->videoMode, "Logic Simulator", sf::Style::Titlebar | sf::Style::Close);
};

void Simulator::pollEvents()
{

    bool creating = false;
    bool moving = false;
    bool clicking = false;
    bool dragging = false;
    bool cancel = false;
    bool deleteObj = false;

    sf::Vector2f start_pos;


    sf::Vector2i mousePos;

    while (this->window->pollEvent(this->event))
    {
        deleteObj = false;
        switch (this->event.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->event.key.code == sf::Keyboard::Escape)
                this->window->close();
            else if (this->event.key.code == sf::Keyboard::Delete)
                deleteObj = true;
            break;
        case sf::Event::MouseMoved:
            mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
            moving = true;
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                this->clicked = clicking = true;
                mousePos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
            }
            else if (event.mouseButton.button == sf::Mouse::Right)
            {
                cancel = true;
                mousePos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                this->clicked = clicking = false;
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                cancel = false;
            }
            mousePos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
            break;
        default:
            break;
        }
        // "close requested" event: we close the window
        if (this->event.type == sf::Event::Closed)
            this->window->close();
    }
    this->holding = clicked && (this->focus != nullptr);
    dragging = (this->holding && moving);

    if (cancel) {
        cout << "dropped" << endl;
        if (drawing && drawing_wire != nullptr && drawing_wire->complete == false) {
            
            this->drawing_wire->selected = true;
            this->RemoveObject();

            this->drawing_wire = nullptr;
            this->drawing = false;
        }
    }

    if (deleteObj) {
        
        this->RemoveObject();
    }

    if (clicking)
    {
        Object *ptr = this->objects;

        if (this->objects == NULL)
            cout << "There is no object" << endl;

        while (ptr)
        {
            // SPRITE DETECTION
            if (ptr->sprite.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
            {
                cout << "Clicked on obj " << endl;
                if (ptr->locked)
                {
                    Object *new_obj = ptr->Clone();

                    AddObject(new_obj);
                    this->focus = &(new_obj->sprite);
                }
                else
                {
                    ptr->selected = true;

                    Pin *pins = new Pin[3];
                    int numOfPins;
                    switch (ptr->GetTypeName())
                    {
                    case Object::ObjTypes::O_AND:
                        pins = static_cast<AndGate *>(ptr)->GetPins();
                        numOfPins = static_cast<AndGate *>(ptr)->GetNumOfPins();
                        break;
                    case Object::ObjTypes::O_OR:
                        pins = static_cast<OrGate *>(ptr)->GetPins();
                        numOfPins = static_cast<OrGate*>(ptr)->GetNumOfPins();

                        break;
                    case Object::ObjTypes::O_XOR:
                        pins = static_cast<XorGate *>(ptr)->GetPins();
                        numOfPins = static_cast<XorGate*>(ptr)->GetNumOfPins();

                        break;
                    case Object::ObjTypes::O_LED:
                        pins = static_cast<Led*>(ptr)->GetPins();
                        numOfPins = static_cast<Led*>(ptr)->GetNumOfPins();

                        break;
                    case Object::ObjTypes::O_VDD:
                        pins = static_cast<VDD*>(ptr)->GetPins();
                        numOfPins = static_cast<VDD*>(ptr)->GetNumOfPins();
                        break;
                    default:
                        break;
                    }

                    this->focus = &(ptr->sprite);
                    std::clog << this->focus << std::endl;

                    //cout << "MOUSE POSITION x: " << event.mouseButton.x << " y: " << event.mouseButton.y << endl;

                    for (int i = 0; i < numOfPins; i++)
                    {
                        //cout << "PIN " << pins[i].index << " LOCATION x: " << pins[i].pos.x << " y: " << pins[i].pos.y << endl;

                        float pin_x_max = pins[i].pos.x + 10.f;
                        float pin_x_min = pins[i].pos.x - 10.f;

                        float pin_y_max = pins[i].pos.y + 10.f;
                        float pin_y_min = pins[i].pos.y - 10.f;

                        if (
                            (event.mouseButton.x <= pin_x_max && event.mouseButton.x >= pin_x_min) &&
                            (event.mouseButton.y <= pin_y_max && event.mouseButton.y >= pin_y_min))
                        {
                            //cout << "CLICKED ON PIN " << pins[i].index << endl;
                            ptr->selected = false;

                            if (drawing && drawing_wire != nullptr) {
                                // FINISH TO DRAW WIRE
                                if (pins[i].type != Pin::pinType::OUTPUT) {
                                    drawing_wire->ConnectPin(&pins[i]);
                                    pins[i].wires[0] = this->drawing_wire;
                                    this->drawing_wire = nullptr;
                                    this->drawing = false;
                                }
                            }
                            else {
                                // START TO DRAW WIRE
                                Wire* wire = new Wire(pins[i].pos, &pins[i]);
                                pins[i].wires[0] = wire;
                                this->AddObject(wire);
                                this->drawing_wire = wire;
                                this->drawing = true;
                            } 
                        }                       
                        else {
                            cout << "NOT CLICKED ON PIN " << endl;
                        }
                    }

                }
            }
            else if (dynamic_cast<Wire*>(ptr) != NULL) {
                cout << "THERE IS ANY WIRE " << endl;

                // LOOP THROUGH THE WIRES AND FIND THE SELECTED WIRE
            }
            else
            {
                ptr->selected = false;
                cout << "Not Clicked on obj" << endl;
            }

            ptr = ptr->next;
        }
    }
    else if (drawing && drawing_wire != nullptr) {
       drawing_wire->UpdateEndPoint(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
    }
    else if (dragging)
    {
        focus->setPosition(static_cast<float>(mousePos.x) - focus->getGlobalBounds().width / 2, static_cast<float>(mousePos.y) - focus->getGlobalBounds().height / 2);
    }
    else if (!holding)
        focus = nullptr; // I'm not doing anything so I can assume there's no sprite being focused
    
};

void Simulator::update()
{

    this->pollEvents();

    // cout << "Mouse position-> x: " << sf::Mouse::getPosition(*this->window).x << " y: " << sf::Mouse::getPosition(*this->window).y << endl;
};

void Simulator::render()
{

    this->window->clear(sf::Color::Black);

    drawElements();

    this->window->display();
};

void Simulator::AddObject(Object *obj)
{
    cout << "Adding new object" << endl;

    obj->next = objects;
    objects = obj;
};

void Simulator::RemoveObject()
{
    Object* ptr = this->objects;
    Object* prev = NULL;

    if (ptr == NULL)
        cout << "There is no object" << endl;


    while (ptr)
    {
        if (ptr->selected) {

            if (prev) {

                prev->next = ptr->next;
            }
            else {
                this->objects = ptr->next;
            }

            switch (ptr->GetTypeName())
            {
            case Object::ObjTypes::O_AND:
                delete static_cast<AndGate*>(ptr);
                break;
            case Object::ObjTypes::O_OR:
                delete static_cast<OrGate*>(ptr);
                break;
            case Object::ObjTypes::O_XOR:
                delete static_cast<XorGate*>(ptr);
                break;
            case Object::ObjTypes::O_LED:
                delete static_cast<Led*>(ptr);
                break;
            case Object::ObjTypes::O_VDD:
                delete static_cast<VDD*>(ptr);
                break;
            case Object::ObjTypes::O_WIRE:
                delete static_cast<Wire*>(ptr);
                break;
            default:
                delete ptr;
                break;
            }

            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }
};

void Simulator::drawElements()
{

    Object *ptr = objects;

    if (objects == NULL)
        cout << "There is no object" << endl;

    while (ptr)
    {
        if (ptr->selected && !ptr->locked) {
            
            ptr->sprite.setColor(sf::Color::Red);
            
        }
        else {
            ptr->sprite.setColor(sf::Color(255, 255, 255));
        }

        switch (ptr->GetTypeName())
        {
        case Object::ObjTypes::O_AND:
            this->window->draw(ptr->sprite);

            static_cast<AndGate *>(ptr)->UpdatePosition();
            static_cast<AndGate*>(ptr)->Simulate();


            for (int i = 0; i < static_cast<AndGate*>(ptr)->GetNumOfPins(); i++) {
                this->window->draw(static_cast<AndGate*>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_OR:
            this->window->draw(ptr->sprite);

            static_cast<OrGate *>(ptr)->UpdatePosition();
            static_cast<OrGate*>(ptr)->Simulate();


            for (int i = 0; i < static_cast<OrGate*>(ptr)->GetNumOfPins(); i++) {
                this->window->draw(static_cast<OrGate*>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_XOR:
            this->window->draw(ptr->sprite);

            static_cast<XorGate *>(ptr)->UpdatePosition();
            static_cast<XorGate*>(ptr)->Simulate();

            
            for (int i = 0; i < static_cast<XorGate*>(ptr)->GetNumOfPins(); i++) {
                this->window->draw(static_cast<XorGate*>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_LED:
            this->window->draw(ptr->sprite);

            static_cast<Led*>(ptr)->UpdatePosition();
            static_cast<Led*>(ptr)->Simulate();

            for (int i = 0; i < static_cast<Led*>(ptr)->GetNumOfPins(); i++) {
                this->window->draw(static_cast<Led*>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_VDD:
            this->window->draw(ptr->sprite);

            static_cast<VDD*>(ptr)->UpdatePosition();
            static_cast<VDD*>(ptr)->Simulate();


            for (int i = 0; i < static_cast<VDD*>(ptr)->GetNumOfPins(); i++) {
                this->window->draw(static_cast<VDD*>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_WIRE:
            static_cast<Wire*>(ptr)->UpdatePosition();
            static_cast<Wire*>(ptr)->Simulate();

            this->window->draw(static_cast<Wire*>(ptr)->GetLines(), 2, sf::Lines);
            break;
        default:
            break;
        }
        ptr = ptr->next;
    }
};
