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
    
    // As we mention above what does the clicking variable
    // If it is true that means we clicked on an object
    if (clicking)
    {
        // Now we should find that on which object did we clicked 
        // For that we are looping through the objects list

        // First create a temp pointer that points the head of the object list
        Object *ptr = this->objects;

        // IF the objects (head) is NULL that means there is no element in list
        if (this->objects == NULL)
            cout << "There is no object" << endl;


        // Now we are looping through the object list until we found which one is clicked
        while (ptr)
        {
            // In this if check we check every object global bounds does it contains the mouse position when we clicked
            // 
            // If it is true that means we found the object
            // And the ptr pointer points to it
            if (ptr->sprite.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
            {

                cout << "Clicked on obj " << endl;

                // We check is the object locked or not
                // If it is locked that means it is an toolbar object and we should copy it
                if (ptr->locked)
                {
                    // We call the clone function of the object which returns a copy of it
                    // And we store to a Object pointer (new_obj) thus we can add it to our object list
                    Object *new_obj = ptr->Clone();

                    // Add the copied object to the list by calling AddObject function
                    AddObject(new_obj);

                    // By assigning the addres of the new_obj's sprite to the focus pointer
                    // We can update the copied object's location as we move our mouse
                    this->focus = &(new_obj->sprite);
                }
                else
                {
                    // If the object is NOT LOCKED

                    // That means we want to select the object, so we set the object's selected value to true
                    ptr->SetSelected(true);

                    

                    // The pins locations are in the object's sprite bounds
                    // So we need to check did we clicked on a pin
                    
                    // To store the clicked pin we create a clicked_pin pointer and set it initially to nullptr
                    Pin *clicked_pin = nullptr;

                    // In this line we down cast out ptr (Object) pointer to LogicElement
                    // Thus we can reach to the pure virutal function GetClickedPin
                    // The function will compare the mouse's position with all of the pins of the object and returns the clicked pin
                   
                    clicked_pin = dynamic_cast<LogicElement *>(ptr)->GetClickedPin(event.mouseButton.x, event.mouseButton.y);

                    this->focus = &(ptr->sprite);

                    // If the clicked_pin pointer is not NULL that means we clicked on a pin
                    if (clicked_pin)
                    {
                        // Then we do not want the object to be selected
                        ptr->selected = false;

                        // Here we are checking are we drawing a wire or not
                        if (drawing && drawing_wire != nullptr)
                        {
                            // If the clicked pin type is not an OUTPUT pin that means we are finishing our wiring
                            if (clicked_pin->GetType() != Pin::pinType::OUTPUT)
                            {
                                // We call the ConnectPin function  on the temp pointer that points our wire

                                drawing_wire->ConnectPin(clicked_pin);
                                
                                // Setting our clicked pin connected wires
                                clicked_pin->SetWires(clicked_pin->GetNumOfConnections(), this->drawing_wire);
                                // Setting our # of connections of the wire
                                clicked_pin->IncrementNumOfConnections();

                                // Now that we've finished our wiring we can reset our temp drawing_wire pointer & drawing variable
                                this->drawing_wire = nullptr;
                                this->drawing = false;
                            }
                        }
                        else
                        {
                            // If the clicked pin type is a OUTPUT that means we are starting to draw a wire
                            
                            // We create a wire pointer 
                            Wire *wire = new Wire(clicked_pin->GetPosition(), clicked_pin);

                            // Setting our clicked pin connected wires
                            clicked_pin->SetWires(clicked_pin->GetNumOfConnections(), wire);
                            // Setting our # of connections of the wire

                            clicked_pin->IncrementNumOfConnections();

                            // We are adding our new created wire object to the object list
                            this->AddObject(wire);

                            // Now that we've started our wiring we should keep the wire in our temp drawing_wire pointer 
                            // And we set our drawing variable to true

                            this->drawing_wire = wire;
                            this->drawing = true;
                        }
                    }
                }
            }
            // If the clicked Object is not both a LogicElement or a Pin
            // We should check did we clicked on a wire

            // By dynamic casting the ptr to Wire pointer we can know is it a wire or not
            // But we don't know that on which wire did we clicked
            // That's why we should check the mouse position by using the a point's distance to a line formula
            else if (dynamic_cast<Wire *>(ptr) != NULL)
            {
                
                // We down cast our ptr pointer to Wire thus we can use the pDistance function
                Wire *wire = static_cast<Wire *>(ptr);
                // pDistance checks the mouse position's distane to the wire
                // If it is less or equal then 10 than that means we found our clicked wire
                // And we are set our object to selected 
                if (wire->pDistance(event.mouseButton.x, event.mouseButton.y))
                    ptr->SetSelected(true);
                else
                    ptr->SetSelected(false);
            }
            else
            {
                // If we come to the else that means we did not clicked on anything
                // To make sure none of the objects are selected
                // We are setting the object's selected attribue to false
                ptr->SetSelected(false);
                cout << "Not Clicked on obj" << endl;
            }
            // Move to the next object of the list
            ptr = ptr->next;
        }
    }
    // If our event is not clicking 
    // We should check are we trying to finish our wire
    // If the drawing is true & our drawin_wire points to a wire that means are drawing a wire
    else if (drawing && drawing_wire != nullptr)
    {
        // We should update the end point of the wire according to our mouse location
        drawing_wire->UpdateEndPoint(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
    }
    // If it is not clicking or drawing a wire
    // So, we should are we dragging an object
    else if (dragging)
    {
        // With the temp pointer focus which holds the sprite of the object
        // We set the position of the object according to our mouse location
        focus->setPosition(static_cast<float>(mousePos.x) - focus->getGlobalBounds().width / 2, static_cast<float>(mousePos.y) - focus->getGlobalBounds().height / 2);
    }
    // If we are not holding anything that means our temp focus pointer should not point to any object
    else if (!holding)
        focus = nullptr; // There's no sprite being focused
};

// Calls the pollEvents function
void Simulator::update()
{
    this->pollEvents();
};

// Renders our window & draws our elements in the list
// And displays the window
void Simulator::render()
{
    this->window->clear(sf::Color::Black);

    drawElements();

    this->window->display();
};

// Adds the given object to the list
void Simulator::AddObject(Object *obj)
{
    obj->next = objects;
    objects = obj;
};

// Removes the selected object from the list
// For finding the selected object we should loop through the object
// In addition, if the object is connected to any object
// Then we should also delete the connected wires
void Simulator::RemoveObject()
{
    // For removing the object we should keep the previous object
    // Thus we can connect it to next of the deleted object
    Object *ptr = this->objects;
    Object *prev = NULL;

    if (ptr == NULL)
    {
        cout << "There is no object" << endl;
        return;
    }

    // We are getting the # of the wires that are connected
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

            // We are doing a switch case according to object type name
            // thus we can know which LogicElement we are deleting
            switch (ptr->GetTypeName())
            {
            case Object::ObjTypes::O_AND:
                // We are down casting our Object pointer to AndGate thus we can call the getNumberOfWiresConnectedToPins
                getNumOfWires = static_cast<AndGate *>(ptr)->GetNumberOfWiresConnectedToPins();
                // If the # of wires is not 0 that means we should delete them to
                if (getNumOfWires != 0)
                {
                    // We call the RemoveWiresConnectToObj to remove the wires
                    this->RemoveWiresConnectToObj(getNumOfWires);
                }
                // After deleted the wires we can delete the selected object
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

// Deletes the wires that are connected to the selected object
void Simulator::RemoveWiresConnectToObj(int numOfWiresToBeDeleted)
{
    // It has the same functionality as the RemoveObjects function
    // Unlike can maybe delete more than one object
    // So we should loop the list as the number of wires that should be deleted
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

// Loops through our list and draws & simulates our objects
void Simulator::drawElements()
{

    Object *ptr = objects;

    if (objects == NULL)
        cout << "There is no object" << endl;

    while (ptr)
    {
        // If there is any not locked and selected object we should check is it wire or not
        if (ptr->selected && !ptr->locked)
        {
            if (dynamic_cast<Wire *>(ptr))
            {
                // It is wire, so we should change the color to red. Thus, we can denote it is selected
                static_cast<Wire*>(ptr)->SetColor(sf::Color::Red);
            }
            else
            {
                // It is a LogicElement, so we are changing our sprite's color to red
                ptr->sprite.setColor(sf::Color::Red);
            }
        }
        // If it is not selected we are coloring the wires to yellow 
        //                                       the sprites to it's original color                                         
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

        // We do a switch case according to our object type
        switch (ptr->GetTypeName())
        {
        case Object::ObjTypes::O_AND:

            // By using the Simulator Class's window we draw our object's sprite
            this->window->draw(ptr->sprite);

            // We are constantly updating & simulating the objects position
            static_cast<AndGate *>(ptr)->UpdatePosition();
            static_cast<AndGate *>(ptr)->Simulate();

            // In addition, for easy access to the pins we draw them too
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
