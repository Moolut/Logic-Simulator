#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.hpp"
#include "Simulator.hpp"

#include <windows.h>;
#include <stdio.h>;
#pragma comment(lib, "user32.lib")

using namespace std;

// According to our pc's windows size define the width & height of the app windor
#define WIN_H GetSystemMetrics(SM_CYSCREEN) * 0.9;
#define WIN_W GetSystemMetrics(SM_CXSCREEN) * 0.75;


// Simulator constructor
// We seperated the constructor into 2 function 
//                                   -> initVariables
//                                   -> initWindow
Simulator::Simulator()
{
    this->initVariables();
    this->initWindow();
};

// Simulator destructor
// Deletes the pointers of the simulator obj
Simulator::~Simulator()
{
    delete this->window;
    delete this->objects;
};

// Initialize the attributes of the simulator obj
// The pointers are set to "nullptr" at initial
void Simulator::initVariables()
{
    this->window = nullptr;
    this->objects = nullptr;
    this->focus = nullptr;
    this->drawing_wire = nullptr;
};

// Initialize our app window
// It determines the width, height, the title of the app
void Simulator::initWindow()
{

    this->videoMode.height = WIN_H;
    this->videoMode.width = WIN_W;

    this->window = new sf::RenderWindow(this->videoMode, "Logic Simulator", sf::Style::Titlebar | sf::Style::Close);
};

// In this function we handle our mouse & keyboard click events

// MOUSE LEFT CLICK     -> clicking = true | To detect click on obj, pin, wire 
//                      -> clicked  = true | To detect are we holding & dragging obj
//                                       -> If we only click       : Sets the obj to selected
//                                       -> If we are holding it   : Drags the clicked object
//                                                                              
// MOUSE RIGHT CLICK    -> cancel   = true  
//                                       -> When clicked cancels the wire drawing process
// DELETE BUTTON        -> deleteObj = true 
//                                       -> If pressed deletes the selected obj

// ESCAPE BUTTON        -> Closes the window

// MOUSE MOVSE          -> moving = true

void Simulator::pollEvents()
{   

    bool moving = false, clicking = false, dragging = false, cancel = false, deleteObj = false;


    // To save the mouse position at a certain event
    sf::Vector2i mousePos;

    // Loop through the poll events of SFML window
    while (this->window->pollEvent(this->event))
    {
        deleteObj = false;

        // Switch through the event type that occurs
        switch (this->event.type)
        {
        case sf::Event::Closed:
            // Close the window
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->event.key.code == sf::Keyboard::Escape)
                // Close the window
                this->window->close();
            else if (this->event.key.code == sf::Keyboard::Delete)
                // Set the deleteObj to true thus we can start the remove object process
                deleteObj = true;
            break;
        case sf::Event::MouseMoved:
            // Get the mouse position into the mousePos variable
            mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
            // Set the moving variable to true
            moving = true;
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // Set the clicked (Simulator's attribute) & clicking (poll event variable) to true

                this->clicked = clicking = true;
                // Get the mouse position
                mousePos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
            }
            else if (event.mouseButton.button == sf::Mouse::Right)
            {
                // Set the cancel variable to true. By doing that we cancel the drawing wire process
                cancel = true;
                // Get the mouse position
                mousePos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                // Set the clicked (Simulator's attribute) & clicking (poll event variable) to false
                this->clicked = clicking = false;
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                // Set the cancel to false
                cancel = false;
            }
            // Get the mouse position
            mousePos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
            break;
        default:
            break;
        }

    }

    //  --------Determine the dragging & holding logic--------

    // Clicked -> determines did we clicked on to the left mouse button
    // focus   -> determines did we clickec on to any object

    // If both are true that means we are holding an object

    // And if we are holding object while moving our mouse that means we are dragging it

    this->holding = clicked && (this->focus != nullptr);
    dragging = (this->holding && moving);


    // In this if check we are control the wire drawing process
    // If cancel is true that means we want to abort the process
    if (cancel)
    {
        // Drawing -> determines are we drawing a wire or not
        // drawing_wire -> if it is NOT NULL that means we started to draw a wire and we a pointer that 
        //                 points to it

        // If the drawing_wire is not null we call the isComplete function of the wire
        // Which gives us information about if we finished drawing the wire or not

        if (drawing && drawing_wire != nullptr && !drawing_wire->isComplete())
        {
            // We set the wire to be selected and we call the removeObject function
            // In the removeObject function we look for the selected obj and delete it

            this->drawing_wire->selected = true;
            this->RemoveObject();

            // After deleted the wire we set out drawing_wire pointer to null & drawing to false
            this->drawing_wire = nullptr;
            this->drawing = false;
        }
    }

    // We check that did we clicked on DELETE button or not 
    // If yes that means we want to delete an object
    // So we are calling the RemoveObject function
    if (deleteObj)
    {
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

                    Pin *clicked_pin = nullptr;

                    clicked_pin = dynamic_cast<LogicElement *>(ptr)->GetClickedPin(event.mouseButton.x, event.mouseButton.y);

                    this->focus = &(ptr->sprite);

                    // IF CLICKED ON ANY PIN OF THE CLICKED OBJ
                    if (clicked_pin)
                    {
                        ptr->selected = false;

                        if (drawing && drawing_wire != nullptr)
                        {
                            // FINISH TO DRAW WIRE
                            if (clicked_pin->GetType() != Pin::pinType::OUTPUT)
                            {
                                drawing_wire->ConnectPin(clicked_pin);
                                clicked_pin->SetWires(clicked_pin->GetNumOfConnections(), this->drawing_wire);
                                clicked_pin->IncrementNumOfConnections();
                                this->drawing_wire = nullptr;
                                this->drawing = false;
                            }
                        }
                        else
                        {
                            // START TO DRAW WIRE
                            Wire *wire = new Wire(clicked_pin->GetPosition(), clicked_pin);
                            clicked_pin->SetWires(clicked_pin->GetNumOfConnections(), wire);
                            clicked_pin->IncrementNumOfConnections();
                            this->AddObject(wire);
                            this->drawing_wire = wire;
                            this->drawing = true;
                        }
                    }
                }
            }
            else if (dynamic_cast<Wire *>(ptr) != NULL)
            {
                Wire *wire = static_cast<Wire *>(ptr);
                if (wire->pDistance(event.mouseButton.x, event.mouseButton.y))
                    ptr->selected = true;
                else
                    ptr->selected = false;
            }
            else
            {
                ptr->selected = false;
                cout << "Not Clicked on obj" << endl;
            }
            ptr = ptr->next;
        }
    }
    else if (drawing && drawing_wire != nullptr)
    {
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
};

void Simulator::render()
{
    this->window->clear(sf::Color::Black);

    drawElements();

    this->window->display();
};

void Simulator::AddObject(Object *obj)
{
    obj->next = objects;
    objects = obj;
};

void Simulator::RemoveObject()
{
    Object *ptr = this->objects;
    Object *prev = NULL;

    if (ptr == NULL)
    {
        cout << "There is no object" << endl;
        return;
    }

    int getNumOfWires = 0;
    while (ptr)
    {
        if (ptr->selected)
        {

            if (prev)
            {
                prev->next = ptr->next;
            }
            else
            {
                this->objects = ptr->next;
            }

            switch (ptr->GetTypeName())
            {
            case Object::ObjTypes::O_AND:
                getNumOfWires = static_cast<AndGate *>(ptr)->GetNumberOfWiresConnectedToPins();
                if (getNumOfWires != 0)
                {
                    this->RemoveWiresConnectToObj(getNumOfWires);
                }
                delete static_cast<AndGate *>(ptr);
                break;
            case Object::ObjTypes::O_OR:
                getNumOfWires = static_cast<OrGate *>(ptr)->GetNumberOfWiresConnectedToPins();
                if (getNumOfWires != 0)
                {
                    this->RemoveWiresConnectToObj(getNumOfWires);
                }
                delete static_cast<OrGate *>(ptr);
                break;
            case Object::ObjTypes::O_XOR:
                getNumOfWires = static_cast<XorGate *>(ptr)->GetNumberOfWiresConnectedToPins();
                if (getNumOfWires != 0)
                {
                    this->RemoveWiresConnectToObj(getNumOfWires);
                }
                delete static_cast<XorGate *>(ptr);
                break;
            case Object::ObjTypes::O_NOT:
                getNumOfWires = static_cast<NotGate *>(ptr)->GetNumberOfWiresConnectedToPins();
                if (getNumOfWires != 0)
                {
                    this->RemoveWiresConnectToObj(getNumOfWires);
                }
                delete static_cast<NotGate *>(ptr);
                break;
            case Object::ObjTypes::O_DFF:
                getNumOfWires = static_cast<DFFGate *>(ptr)->GetNumberOfWiresConnectedToPins();
                if (getNumOfWires != 0)
                {
                    this->RemoveWiresConnectToObj(getNumOfWires);
                }
                delete static_cast<DFFGate *>(ptr);
                break;
            case Object::ObjTypes::O_LED:
                getNumOfWires = static_cast<Led *>(ptr)->GetNumberOfWiresConnectedToPins();
                if (getNumOfWires != 0)
                {
                    this->RemoveWiresConnectToObj(getNumOfWires);
                }
                delete static_cast<Led *>(ptr);
                break;
            case Object::ObjTypes::O_VDD:
                getNumOfWires = static_cast<VDD *>(ptr)->GetNumberOfWiresConnectedToPins();
                if (getNumOfWires != 0)
                {
                    this->RemoveWiresConnectToObj(getNumOfWires);
                }
                delete static_cast<VDD *>(ptr);
                break;
            case Object::ObjTypes::O_GND:
                getNumOfWires = static_cast<GND *>(ptr)->GetNumberOfWiresConnectedToPins();
                if (getNumOfWires != 0)
                {
                    this->RemoveWiresConnectToObj(getNumOfWires);
                }
                delete static_cast<GND *>(ptr);
                break;
            case Object::ObjTypes::O_CLK:
                getNumOfWires = static_cast<CLK *>(ptr)->GetNumberOfWiresConnectedToPins();
                if (getNumOfWires != 0)
                {
                    this->RemoveWiresConnectToObj(getNumOfWires);
                }
                delete static_cast<CLK *>(ptr);
                break;
            case Object::ObjTypes::O_WIRE:
                delete static_cast<Wire *>(ptr);
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

    return;
};

void Simulator::RemoveWiresConnectToObj(int numOfWiresToBeDeleted)
{

    Object *ptr = this->objects;
    Object *prev = NULL;

    if (ptr == NULL)
    {
        cout << "There is no object" << endl;
        return;
    }

    for (int i = 0; i < numOfWiresToBeDeleted; i++)
    {
        while (ptr)
        {
            if (ptr->selected)
            {
                if (prev)
                {
                    prev->next = ptr->next;
                }
                else
                {
                    this->objects = ptr->next;
                }

                switch (ptr->GetTypeName())
                {
                case Object::ObjTypes::O_WIRE:
                    delete static_cast<Wire *>(ptr);
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

        ptr = this->objects;
        prev = NULL;
    }
    return;
}

void Simulator::drawElements()
{

    Object *ptr = objects;

    if (objects == NULL)
        cout << "There is no object" << endl;

    while (ptr)
    {
        if (ptr->selected && !ptr->locked)
        {
            if (dynamic_cast<Wire *>(ptr))
            {
                // It is wire
                static_cast<Wire*>(ptr)->SetColor(sf::Color::Red);
            }
            else
            {
                ptr->sprite.setColor(sf::Color::Red);
            }
        }
        else
        {
            if (dynamic_cast<Wire *>(ptr))
            {
                // It is wire
                static_cast<Wire *>(ptr)->SetColor(sf::Color::Yellow);
            }
            else
            {
                ptr->sprite.setColor(sf::Color(255, 255, 255));
            }
        }

        switch (ptr->GetTypeName())
        {
        case Object::ObjTypes::O_AND:
            this->window->draw(ptr->sprite);

            static_cast<AndGate *>(ptr)->UpdatePosition();
            static_cast<AndGate *>(ptr)->Simulate();

            for (int i = 0; i < static_cast<AndGate *>(ptr)->GetNumOfPins(); i++)
            {
                this->window->draw(static_cast<AndGate *>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_OR:
            this->window->draw(ptr->sprite);

            static_cast<OrGate *>(ptr)->UpdatePosition();
            static_cast<OrGate *>(ptr)->Simulate();

            for (int i = 0; i < static_cast<OrGate *>(ptr)->GetNumOfPins(); i++)
            {
                this->window->draw(static_cast<OrGate *>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_XOR:
            this->window->draw(ptr->sprite);

            static_cast<XorGate *>(ptr)->UpdatePosition();
            static_cast<XorGate *>(ptr)->Simulate();

            for (int i = 0; i < static_cast<XorGate *>(ptr)->GetNumOfPins(); i++)
            {
                this->window->draw(static_cast<XorGate *>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_NOT:
            this->window->draw(ptr->sprite);

            static_cast<NotGate *>(ptr)->UpdatePosition();
            static_cast<NotGate *>(ptr)->Simulate();

            for (int i = 0; i < static_cast<NotGate *>(ptr)->GetNumOfPins(); i++)
            {
                this->window->draw(static_cast<NotGate *>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_DFF:
            this->window->draw(ptr->sprite);

            static_cast<DFFGate *>(ptr)->UpdatePosition();
            static_cast<DFFGate *>(ptr)->Simulate();

            for (int i = 0; i < static_cast<DFFGate *>(ptr)->GetNumOfPins(); i++)
            {
                this->window->draw(static_cast<DFFGate *>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_LED:
            this->window->draw(ptr->sprite);

            static_cast<Led *>(ptr)->UpdatePosition();
            static_cast<Led *>(ptr)->Simulate();

            for (int i = 0; i < static_cast<Led *>(ptr)->GetNumOfPins(); i++)
            {
                this->window->draw(static_cast<Led *>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_VDD:
            this->window->draw(ptr->sprite);

            static_cast<VDD *>(ptr)->UpdatePosition();
            static_cast<VDD *>(ptr)->Simulate();

            for (int i = 0; i < static_cast<VDD *>(ptr)->GetNumOfPins(); i++)
            {
                this->window->draw(static_cast<VDD *>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_GND:
            this->window->draw(ptr->sprite);

            static_cast<GND *>(ptr)->UpdatePosition();
            static_cast<GND *>(ptr)->Simulate();

            for (int i = 0; i < static_cast<GND *>(ptr)->GetNumOfPins(); i++)
            {
                this->window->draw(static_cast<GND *>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_CLK:
            this->window->draw(ptr->sprite);

            static_cast<CLK *>(ptr)->UpdatePosition();
            static_cast<CLK *>(ptr)->Simulate();

            for (int i = 0; i < static_cast<CLK *>(ptr)->GetNumOfPins(); i++)
            {
                this->window->draw(static_cast<CLK *>(ptr)->DrawPins(i));
            }

            break;
        case Object::ObjTypes::O_WIRE:
            static_cast<Wire *>(ptr)->UpdatePosition();
            static_cast<Wire *>(ptr)->Simulate();

            this->window->draw(static_cast<Wire *>(ptr)->GetLines(), 2, sf::Lines);
            break;
        default:
            break;
        }
        ptr = ptr->next;
    }
};
