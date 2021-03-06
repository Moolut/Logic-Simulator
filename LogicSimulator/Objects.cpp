#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.hpp"

#include <chrono> //For clock
#include <thread> //


//-----------------------------------------------OBJECT CLASS-----------------------------------------------
// Object copy constructor
// Copies the values of the object to be copied to the copy object

Object::Object(const Object &old_obj)
{
    this->next = old_obj.next;
    this->locked = false;
    this->window = old_obj.window;
    this->objectType = old_obj.objectType;
    this->textures[0] = old_obj.textures[0];
    this->textures[1] = old_obj.textures[1];
    this->sprite = old_obj.sprite;
    this->state = old_obj.state;
    this->selected = old_obj.selected;
};

Object::~Object()
{

    cout << "Object Deconstructor" << endl;
}




//-----------------------------------------------WIRE CLASS-----------------------------------------------

// Gets the start point and sets it to the vertex line array
// Gets the start pin and sets it to the pin array
Wire::Wire(sf::Vector2f start_point, Pin *start_pin)
{
    this->objectType = O_WIRE;
    this->next = NULL;

    this->line[0].position = start_point;
    this->line[0].color = sf::Color::Yellow;

    this->pins[0] = start_pin;
};

// GETTERS
sf::Vertex* Wire::GetLines()
{
    return this->line;
}

Object::ObjTypes Wire::GetTypeName() const
{
    return this->objectType;
};

// Deconstructor
// Sets the pins' state to HIGHZ
// Decrements the number of connection of the pin
Wire::~Wire()
{

    this->pins[0]->SetState(Pin::pinState::HIGHZ);
    this->pins[0]->DecrementNumOfConnections();

    if (this->pins[1])
    {
        this->pins[1]->SetState(Pin::pinState::HIGHZ);
        this->pins[1]->DecrementNumOfConnections();
    }
}

// Updates the end point of the wire by getting the point location
void Wire::UpdateEndPoint(sf::Vector2f end_point)
{
    this->line[1].position = end_point;
    this->line[1].color = sf::Color::Yellow;
}

// Connects both pin to each other
// Sets the complete to true
void Wire::ConnectPin(Pin *end_pin)
{
    this->line[1].position = end_pin->GetPosition();
    this->line[1].color = sf::Color::Yellow;

    this->pins[1] = end_pin;
    this->complete = true;

    this->pins[1]->SetConnection(this->pins[0]);
    this->pins[0]->SetConnection(this->pins[1]);
};

// Updates the start & end point positions if it is completed
void Wire::UpdatePosition()
{
    if (complete)
    {
        this->line[0].position = this->pins[0]->GetPosition();
        this->line[1].position = this->pins[1]->GetPosition();
        
    }
};

// Takes the start point state and sets it to end point
void Wire::Simulate()
{
    if (complete)
    {
        this->pins[1]->SetState(this->pins[0]->GetState());
    }
};

// Takes x & y position of the mouse
// Checkes whether the mouse position's nearest distance to the wire is less or equal to 10
// And returns boolean value
bool Wire::pDistance(float mouse_x, float mouse_y)
{

    float x1 = this->line[0].position.x;
    float y1 = this->line[0].position.y;
    float x2 = this->line[1].position.x;
    float y2 = this->line[1].position.y;

    float A = mouse_x - x1;
    float B = mouse_y - y1;
    float C = x2 - x1;
    float D = y2 - y1;

    float dot = A * C + B * D;
    float len_sq = C * C + D * D;

    float param = -1;

    if (len_sq != 0)
        param = dot / len_sq;

    float xx, yy;

    if (param < 0)
    {

        xx = x1;
        yy = y1;
    }
    else if (param > 1)
    {
        xx = x2;
        yy = y2;
    }
    else
    {
        xx = x1 + param * C;
        yy = y1 + param * D;
    }

    float dx = mouse_x - xx;
    float dy = mouse_y - yy;

    return sqrt(dx * dx + dy * dy) <= 10;
}


//-----------------------------------------------LogicElement CLASS-----------------------------------------------


// GETTERS
Object::ObjTypes LogicElement::GetTypeName() const
{
    return this->objectType;
};

Pin* LogicElement::GetPins()
{

    return this->pins;
};

int LogicElement::GetNumOfPins()
{
    return this->numPins;
};


// According to our object type we are updating the object's pins relative to it's sprite location
void LogicElement::UpdatePosition()
{
    if (objectType == O_AND || objectType == O_OR || objectType == O_XOR)
    {
        this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 10.f);
        this->pins[1].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 40.f);
        this->pins[2].pos = sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 25.f);
    }
    else if (objectType == O_DFF)
    {
        this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 15.f);
        this->pins[1].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 35.f);
        this->pins[2].pos = sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 15.f);
        this->pins[3].pos = sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 55.f);
    }
    else if (objectType == O_NOT)
    {
        this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 25.f);
        this->pins[1].pos = sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 25.f);
    }
    else if (objectType == O_LED)
    {
        this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 8.f, this->sprite.getPosition().y + 68.f);
        this->pins[1].pos = sf::Vector2f(this->sprite.getPosition().x + 25.f, this->sprite.getPosition().y + 68.f);
    }
    else if (objectType == O_VDD)
    {
        this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 25.f, this->sprite.getPosition().y + 50.f);
    }
    else if (objectType == O_GND)
    {
        this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 20.f, this->sprite.getPosition().y + 1.f);
    }
    else if (objectType == O_CLK)
    {
        this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 70.f, this->sprite.getPosition().y + 25.f);
    }
};


// For better visulization & debugging, we added sprite & texture to pins
// DrawPins draws the pins according to it's state
// If state is  HIGH    -> GREEN
//              LOW     -> RED
//              HIGHZ   -> WhITE
sf::Sprite LogicElement::DrawPins(int i)
{
    sf::Sprite sprite;
    sf::Texture texture;

    if (!texture.create(10, 10))
        exit(0);

    sprite.setTexture(texture);
    sprite.setPosition(pins[i].GetPosition());

    if (pins[i].GetState() == Pin::pinState::HIGH)
    {
        sprite.setColor(sf::Color::Green);
    }
    else if (pins[i].GetState() == Pin::pinState::LOW)
    {
        sprite.setColor(sf::Color::Red);
    }
    else
    {
        sprite.setColor(sf::Color::White);
    }

    return sprite;
};


//-----------------------------------------------Pin CLASS-----------------------------------------------


// SETTERS - GETTERS
void Pin::SetState(pinState state) {
    this->state = state;
};

Pin::pinState Pin::GetState() {
    return this->state;
};




//-----------------------------------------------AndGate CLASS-----------------------------------------------

// !!!!!!!!!!!! (We added comments only to AndGate & Clock objects because the other objects has the same code) !!!!!!!!!!!!

// The constructors on each Logic Element does the same operation
// - Sets the # of pins
// - Sets the object type
// - Sets the pins 
// - Sets the texture & sprite
AndGate::AndGate()
{
    cout << "AND GATE CONSTRUCTOR " << endl;
    this->next = NULL;
    this->numPins = 3;
    this->objectType = O_AND;

    this->pins[0] = *new Pin(
        0,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 10.f),
        Pin::pinState::HIGHZ);
    this->pins[1] = *new Pin(
        1,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 40.f),
        Pin::pinState::HIGHZ);
    ;
    this->pins[2] = *new Pin(
        2,
        Pin::pinType::OUTPUT,
        sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 25.f),
        Pin::pinState::HIGHZ);
    ;

    if (!this->textures[0].loadFromFile("../assets/AND.png"))
        exit(0);
    this->sprite.setTexture(this->textures[0]);
};


// Deconstructor
AndGate::~AndGate()
{

    cout << "AND GATE DECONSTRUCTOR" << endl;
};

// In the clone function we are returning a new instance of the object that is cloned 
Object *AndGate::Clone() const { return new AndGate(*this); };

// Simulate functions checks the INPUT pins of the object and 
// It determines the OUTPUT according to the working principle of each object.

void AndGate::Simulate()
{
    if (pins[0].GetState() == Pin::pinState::HIGH && pins[1].GetState() == Pin::pinState::HIGH)
    {
        pins[2].SetState(Pin::pinState::HIGH);
    }
    else if (pins[0].GetState() == Pin::pinState::LOW || pins[1].GetState() == Pin::pinState::LOW)
    {
        pins[2].SetState(Pin::pinState::LOW);
    }
    else
    {
        pins[2].SetState(Pin::pinState::HIGHZ);
    }
};

// Gets the mouse's x & y position and loop through it's pins
// Thus, we can check on which pin did we clicked
// Returns the clicked_pin pointer which points the clicked pin
Pin *AndGate::GetClickedPin(float mouse_x_pos, float mouse_y_pos)
{

    Pin *clicked_pin = nullptr;
    for (int i = 0; i < this->numPins; i++)
    {

        float pin_x_max = this->pins[i].GetPosition().x + 10.f;
        float pin_x_min = this->pins[i].GetPosition().x - 10.f;

        float pin_y_max = this->pins[i].GetPosition().y + 10.f;
        float pin_y_min = this->pins[i].GetPosition().y - 10.f;

        if (
            (mouse_x_pos <= pin_x_max && mouse_x_pos >= pin_x_min) &&
            (mouse_y_pos <= pin_y_max && mouse_y_pos >= pin_y_min))
        {
            clicked_pin = &this->pins[i];
        }
    }
    return clicked_pin;
}

// Gets the number of wires that is connect to the object
// We are looping throug all of the pins & pins' wires
// We call this function when the object will be deleted
// In this function we also set the wires to selected
// Thus in RemoveObject function which is in Simulator Class can find the wires by looking their selected attribute
// And it will delete the wire of the object
int AndGate::GetNumberOfWiresConnectedToPins()
{

    int count = 0;

    for (int i = 0; i < this->numPins; i++)
    {

        for (int j = 0; j < this->pins[i].GetNumOfConnections(); j++)
        {

            if (this->pins[i].GetWires(j))
            {

                this->pins[i].GetWires(j)->SetSelected(true);

                count++;
            }
        }
    }

    return count;
}



//-----------------------------------------------OrGate CLASS-----------------------------------------------

OrGate::OrGate()
{

    this->next = NULL;
    this->numPins = 3;
    this->objectType = O_OR;

    this->pins[0] = *new Pin(
        0,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 10.f),
        Pin::pinState::HIGHZ);
    this->pins[1] = *new Pin(
        1,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 40.f),
        Pin::pinState::HIGHZ);
    ;
    this->pins[2] = *new Pin(
        2,
        Pin::pinType::OUTPUT,
        sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 25.f),
        Pin::pinState::HIGHZ);
    ;

    if (!this->textures[0].loadFromFile("../assets/OR.png"))
        exit(0);
    this->sprite.setTexture(this->textures[0]);
};

OrGate::~OrGate(){};

Object *OrGate::Clone() const { return new OrGate(*this); };

void OrGate::Simulate()
{
    if (pins[0].GetState() == Pin::pinState::HIGH || pins[1].GetState() == Pin::pinState::HIGH)
    {
        pins[2].SetState(Pin::pinState::HIGH);
    }
    else if (pins[0].GetState() == Pin::pinState::LOW && pins[1].GetState() == Pin::pinState::LOW)
    {
        pins[2].SetState(Pin::pinState::LOW);
    }
    else
    {
        pins[2].SetState(Pin::pinState::HIGHZ);
    }
};

Pin *OrGate::GetClickedPin(float mouse_x_pos, float mouse_y_pos)
{

    Pin *clicked_pin = nullptr;
    for (int i = 0; i < this->numPins; i++)
    {

        float pin_x_max = this->pins[i].GetPosition().x + 10.f;
        float pin_x_min = this->pins[i].GetPosition().x - 10.f;

        float pin_y_max = this->pins[i].GetPosition().y + 10.f;
        float pin_y_min = this->pins[i].GetPosition().y - 10.f;

        if (
            (mouse_x_pos <= pin_x_max && mouse_x_pos >= pin_x_min) &&
            (mouse_y_pos <= pin_y_max && mouse_y_pos >= pin_y_min))
        {
            clicked_pin = &this->pins[i];
        }
    }
    return clicked_pin;
}

int OrGate::GetNumberOfWiresConnectedToPins()
{

    int count = 0;

    for (int i = 0; i < this->numPins; i++)
    {

        for (int j = 0; j < this->pins[i].GetNumOfConnections(); j++)
        {

            if (this->pins[i].GetWires(j))
            {
                this->pins[i].GetWires(j)->SetSelected(true);
                count++;
            }
        }
    }

    return count;
}


//-----------------------------------------------XorGate CLASS-----------------------------------------------

XorGate::XorGate()
{

    this->next = NULL;
    this->objectType = O_XOR;

    if (!this->textures[0].loadFromFile("../assets/XOR.png"))
        exit(0);
    this->sprite.setTexture(this->textures[0]);

    this->numPins = 3;

    this->pins[0] = *new Pin(
        0,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 10.f),
        Pin::pinState::HIGHZ);
    this->pins[1] = *new Pin(
        1,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 40.f),
        Pin::pinState::HIGHZ);
    ;
    this->pins[2] = *new Pin(
        2,
        Pin::pinType::OUTPUT,
        sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 25.f),
        Pin::pinState::HIGHZ);
    ;
};

XorGate::~XorGate(){};

Object *XorGate::Clone() const { return new XorGate(*this); };

void XorGate::Simulate()
{

    if (pins[0].GetState() == Pin::pinState::HIGHZ || pins[1].GetState() == Pin::pinState::HIGHZ)
    {
        pins[2].SetState(Pin::pinState::HIGHZ);
    }
    else if (pins[0].GetState() != pins[1].GetState())
    {
        pins[2].SetState(Pin::pinState::HIGH);
    }
    else
    {
        pins[2].SetState(Pin::pinState::LOW);
    }
};

Pin *XorGate::GetClickedPin(float mouse_x_pos, float mouse_y_pos)
{

    Pin *clicked_pin = nullptr;
    for (int i = 0; i < this->numPins; i++)
    {

        float pin_x_max = this->pins[i].GetPosition().x + 10.f;
        float pin_x_min = this->pins[i].GetPosition().x - 10.f;

        float pin_y_max = this->pins[i].GetPosition().y + 10.f;
        float pin_y_min = this->pins[i].GetPosition().y - 10.f;

        if (
            (mouse_x_pos <= pin_x_max && mouse_x_pos >= pin_x_min) &&
            (mouse_y_pos <= pin_y_max && mouse_y_pos >= pin_y_min))
        {
            clicked_pin = &this->pins[i];
        }
    }
    return clicked_pin;
}

int XorGate::GetNumberOfWiresConnectedToPins()
{

    int count = 0;

    for (int i = 0; i < this->numPins; i++)
    {

        for (int j = 0; j < this->pins[i].GetNumOfConnections(); j++)
        {

            if (this->pins[i].GetWires(j))
            {

                this->pins[i].GetWires(j)->SetSelected(true);

                count++;
            }
        }
    }

    return count;
}


//-----------------------------------------------NotGate CLASS-----------------------------------------------

NotGate::NotGate()
{

    this->next = NULL;
    this->objectType = O_NOT;

    if (!this->textures[0].loadFromFile("../assets/NOT.png"))
        exit(0);
    this->sprite.setTexture(this->textures[0]);

    this->numPins = 2;

    this->pins[0] = *new Pin(
        0,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 25.f),
        Pin::pinState::HIGHZ);
    this->pins[1] = *new Pin(
        1,
        Pin::pinType::OUTPUT,
        sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 25.f),
        Pin::pinState::HIGHZ);
    ;
};

NotGate::~NotGate(){};

Object *NotGate::Clone() const { return new NotGate(*this); };

void NotGate::Simulate()
{

    if (pins[0].GetState() == Pin::pinState::HIGH)
    {
        pins[1].SetState(Pin::pinState::LOW);
    }
    else if (pins[0].GetState() == Pin::pinState::LOW)
    {
        pins[1].SetState(Pin::pinState::HIGH);
    }
};

Pin *NotGate::GetClickedPin(float mouse_x_pos, float mouse_y_pos)
{

    Pin *clicked_pin = nullptr;
    for (int i = 0; i < this->numPins; i++)
    {

        float pin_x_max = this->pins[i].GetPosition().x + 10.f;
        float pin_x_min = this->pins[i].GetPosition().x - 10.f;

        float pin_y_max = this->pins[i].GetPosition().y + 10.f;
        float pin_y_min = this->pins[i].GetPosition().y - 10.f;

        if (
            (mouse_x_pos <= pin_x_max && mouse_x_pos >= pin_x_min) &&
            (mouse_y_pos <= pin_y_max && mouse_y_pos >= pin_y_min))
        {
            clicked_pin = &this->pins[i];
        }
    }
    return clicked_pin;
}

int NotGate::GetNumberOfWiresConnectedToPins()
{

    int count = 0;

    for (int i = 0; i < this->numPins; i++)
    {

        for (int j = 0; j < this->pins[i].GetNumOfConnections(); j++)
        {

            if (this->pins[i].GetWires(j))
            {

                this->pins[i].GetWires(j)->SetSelected(true);

                count++;
            }
        }
    }

    return count;
}


//-----------------------------------------------DFF CLASS-----------------------------------------------

DFFGate::DFFGate()
{

    this->next = NULL;
    this->objectType = O_DFF;

    if (!this->textures[0].loadFromFile("../assets/DFF.png"))
        exit(0);
    this->sprite.setTexture(this->textures[0]);

    this->numPins = 4;

    this->pins[0] = *new Pin(
        0,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 15.f),
        Pin::pinState::HIGHZ);
    this->pins[1] = *new Pin( // CLOCK
        1,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 35.f),
        Pin::pinState::HIGHZ);
    ;
    this->pins[2] = *new Pin(
        2,
        Pin::pinType::OUTPUT,
        sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 15.f),
        Pin::pinState::HIGHZ);
    ;
    this->pins[3] = *new Pin(
        3,
        Pin::pinType::OUTPUT,
        sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 55.f),
        Pin::pinState::HIGHZ);
    ;
};

DFFGate::~DFFGate(){};

Object *DFFGate::Clone() const { return new DFFGate(*this); };

void DFFGate::Simulate()
{

    if (pins[0].GetState() == Pin::pinState::LOW)
    {
        pins[2].SetState(Pin::pinState::LOW);
        pins[3].SetState(Pin::pinState::HIGH);
    }
    else if (pins[0].GetState() == Pin::pinState::HIGH && pins[1].GetState() == Pin::pinState::LOW)
    {
        pins[2].SetState(Pin::pinState::LOW);
        pins[3].SetState(Pin::pinState::HIGH);
    }
    else if (pins[0].GetState() == Pin::pinState::HIGH && pins[1].GetState() == Pin::pinState::HIGH)
    {
        if (pins[2].GetState() == Pin::pinState::LOW)
        {
            pins[2].SetState(Pin::pinState::HIGH);
            pins[3].SetState(Pin::pinState::LOW);
        }
        else
        {
            pins[2].SetState(pins[2].GetState());
            pins[3].SetState(pins[3].GetState());
        }
    }
};

Pin *DFFGate::GetClickedPin(float mouse_x_pos, float mouse_y_pos)
{

    Pin *clicked_pin = nullptr;
    for (int i = 0; i < this->numPins; i++)
    {

        float pin_x_max = this->pins[i].GetPosition().x + 10.f;
        float pin_x_min = this->pins[i].GetPosition().x - 10.f;

        float pin_y_max = this->pins[i].GetPosition().y + 10.f;
        float pin_y_min = this->pins[i].GetPosition().y - 10.f;

        if (
            (mouse_x_pos <= pin_x_max && mouse_x_pos >= pin_x_min) &&
            (mouse_y_pos <= pin_y_max && mouse_y_pos >= pin_y_min))
        {
            clicked_pin = &this->pins[i];
        }
    }
    return clicked_pin;
}

int DFFGate::GetNumberOfWiresConnectedToPins()
{

    int count = 0;

    for (int i = 0; i < this->numPins; i++)
    {

        for (int j = 0; j < this->pins[i].GetNumOfConnections(); j++)
        {

            if (this->pins[i].GetWires(j))
            {

                this->pins[i].GetWires(j)->SetSelected(true);

                count++;
            }
        }
    }

    return count;
}


//-----------------------------------------------Led CLASS-----------------------------------------------

// Unlike the other logic elements it has 2 types of texture
//                                              - one for led is on
//                                              - one for led is off
Led::Led()
{

    this->next = NULL;
    this->objectType = O_LED;

    if (!this->textures[0].loadFromFile("../assets/LEDOFF.png"))
        exit(0);
    if (!this->textures[1].loadFromFile("../assets/LEDON.png"))
        exit(0);

    this->sprite.setTexture(this->textures[0]);

    this->numPins = 2;

    this->pins[0] = *new Pin(
        0,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 8.f, this->sprite.getPosition().y + 68.f),
        Pin::pinState::HIGHZ);
    this->pins[1] = *new Pin(
        1,
        Pin::pinType::INPUT,
        sf::Vector2f(this->sprite.getPosition().x + 25.f, this->sprite.getPosition().y + 68.f),
        Pin::pinState::HIGHZ);
    ;
};
Led::~Led(){};

Object *Led::Clone() const { return new Led(*this); };

void Led::Simulate()
{
    if (pins[0].GetState() == Pin::pinState::HIGH && (pins[1].GetState() == Pin::pinState::LOW || pins[1].GetState() == Pin::pinState::HIGHZ))
    {
        sprite.setTexture(this->textures[1]);
    }
    else
    {
        sprite.setTexture(this->textures[0]);
    }
};

Pin *Led::GetClickedPin(float mouse_x_pos, float mouse_y_pos)
{

    Pin *clicked_pin = nullptr;
    for (int i = 0; i < this->numPins; i++)
    {

        float pin_x_max = this->pins[i].GetPosition().x + 10.f;
        float pin_x_min = this->pins[i].GetPosition().x - 10.f;

        float pin_y_max = this->pins[i].GetPosition().y + 10.f;
        float pin_y_min = this->pins[i].GetPosition().y - 10.f;

        if (
            (mouse_x_pos <= pin_x_max && mouse_x_pos >= pin_x_min) &&
            (mouse_y_pos <= pin_y_max && mouse_y_pos >= pin_y_min))
        {
            clicked_pin = &this->pins[i];
        }
    }
    return clicked_pin;
}

int Led::GetNumberOfWiresConnectedToPins()
{

    int count = 0;

    for (int i = 0; i < this->numPins; i++)
    {

        for (int j = 0; j < this->pins[i].GetNumOfConnections(); j++)
        {

            if (this->pins[i].GetWires(j))
            {

                this->pins[i].GetWires(j)->SetSelected(true);

                count++;
            }
        }
    }

    return count;
}


//-----------------------------------------------VDD CLASS-----------------------------------------------

VDD::VDD()
{

    this->next = NULL;
    this->objectType = O_VDD;

    if (!this->textures[0].loadFromFile("../assets/VDD.png"))
        exit(0);

    this->sprite.setTexture(this->textures[0]);

    this->numPins = 1;

    this->pins[0] = *new Pin(
        0,
        Pin::pinType::OUTPUT,
        sf::Vector2f(this->sprite.getPosition().x + 25.f, this->sprite.getPosition().y + 50.f),
        Pin::pinState::HIGH);
};

VDD::~VDD(){};

Object *VDD::Clone() const { return new VDD(*this); };

void VDD::Simulate()
{
    pins[0].SetState(Pin::pinState::HIGH);
};

Pin *VDD::GetClickedPin(float mouse_x_pos, float mouse_y_pos)
{

    Pin *clicked_pin = nullptr;
    for (int i = 0; i < this->numPins; i++)
    {

        float pin_x_max = this->pins[i].GetPosition().x + 10.f;
        float pin_x_min = this->pins[i].GetPosition().x - 10.f;

        float pin_y_max = this->pins[i].GetPosition().y + 10.f;
        float pin_y_min = this->pins[i].GetPosition().y - 10.f;

        if (
            (mouse_x_pos <= pin_x_max && mouse_x_pos >= pin_x_min) &&
            (mouse_y_pos <= pin_y_max && mouse_y_pos >= pin_y_min))
        {
            clicked_pin = &this->pins[i];
        }
    }
    return clicked_pin;
}

int VDD::GetNumberOfWiresConnectedToPins()
{

    int count = 0;

    for (int i = 0; i < this->numPins; i++)
    {

        for (int j = 0; j < this->pins[i].GetNumOfConnections(); j++)
        {

            if (this->pins[i].GetWires(j))
            {

                this->pins[i].GetWires(j)->SetSelected(true);

                count++;
            }
        }
    }

    return count;
}


//-----------------------------------------------GND CLASS-----------------------------------------------

GND::GND()
{

    this->next = NULL;
    this->objectType = O_GND;

    if (!this->textures[0].loadFromFile("../assets/GND.png"))
        exit(0);

    this->sprite.setTexture(this->textures[0]);

    this->numPins = 1;

    this->pins[0] = *new Pin(
        0,
        Pin::pinType::OUTPUT,
        sf::Vector2f(this->sprite.getPosition().x + 20.f, this->sprite.getPosition().y + 1.f),
        Pin::pinState::LOW);
};

GND::~GND(){};

Object *GND::Clone() const { return new GND(*this); };

void GND::Simulate()
{
    pins[0].SetState(Pin::pinState::LOW);
};

Pin *GND::GetClickedPin(float mouse_x_pos, float mouse_y_pos)
{

    Pin *clicked_pin = nullptr;
    for (int i = 0; i < this->numPins; i++)
    {

        float pin_x_max = this->pins[i].GetPosition().x + 10.f;
        float pin_x_min = this->pins[i].GetPosition().x - 10.f;

        float pin_y_max = this->pins[i].GetPosition().y + 10.f;
        float pin_y_min = this->pins[i].GetPosition().y - 10.f;

        if (
            (mouse_x_pos <= pin_x_max && mouse_x_pos >= pin_x_min) &&
            (mouse_y_pos <= pin_y_max && mouse_y_pos >= pin_y_min))
        {
            clicked_pin = &this->pins[i];
        }
    }
    return clicked_pin;
}

int GND::GetNumberOfWiresConnectedToPins()
{

    int count = 0;

    for (int i = 0; i < this->numPins; i++)
    {

        for (int j = 0; j < this->pins[i].GetNumOfConnections(); j++)
        {

            if (this->pins[i].GetWires(j))
            {

                this->pins[i].GetWires(j)->SetSelected(true);

                count++;
            }
        }
    }

    return count;
}


//-----------------------------------------------CLK CLASS-----------------------------------------------


CLK::CLK()
{

    this->next = NULL;
    this->objectType = O_CLK;

    if (!this->textures[0].loadFromFile("../assets/CLOCK.png"))
        exit(0);

    this->sprite.setTexture(this->textures[0]);

    this->numPins = 1;

    this->pins[0] = *new Pin(
        0,
        Pin::pinType::OUTPUT,
        sf::Vector2f(this->sprite.getPosition().x + 70.f, this->sprite.getPosition().y + 25.f),
        Pin::pinState::LOW);
};

CLK::~CLK(){};

Object *CLK::Clone() const { return new CLK(*this); };



// Unlike the other logic elements the CLK does work differently

// From the SFML library we compare the m_Clock's time with our delay
// If the clock reaches to delay we increase counter value by 1
// And we are resetting the clock
// When the counter is even we set the Clock's pin to LOW
//                     odd we set the Clock's pin to HIGH
void CLK::Simulate()
{

    if (m_Clock.getElapsedTime().asSeconds() < m_fDelay)
        return;

    m_Clock.restart();
    this->count += 1;
    pins[0].SetState(this->count % 2 == 0 ? Pin::pinState::LOW : Pin::pinState::HIGH);
};

Pin *CLK::GetClickedPin(float mouse_x_pos, float mouse_y_pos)
{

    Pin *clicked_pin = nullptr;
    for (int i = 0; i < this->numPins; i++)
    {

        float pin_x_max = this->pins[i].GetPosition().x + 10.f;
        float pin_x_min = this->pins[i].GetPosition().x - 10.f;

        float pin_y_max = this->pins[i].GetPosition().y + 10.f;
        float pin_y_min = this->pins[i].GetPosition().y - 10.f;

        if (
            (mouse_x_pos <= pin_x_max && mouse_x_pos >= pin_x_min) &&
            (mouse_y_pos <= pin_y_max && mouse_y_pos >= pin_y_min))
        {
            clicked_pin = &this->pins[i];
        }
    }
    return clicked_pin;
}

int CLK::GetNumberOfWiresConnectedToPins()
{

    int count = 0;

    for (int i = 0; i < this->numPins; i++)
    {

        for (int j = 0; j < this->pins[i].GetNumOfConnections(); j++)
        {

            if (this->pins[i].GetWires(j))
            {

                this->pins[i].GetWires(j)->SetSelected(true);

                count++;
            }
        }
    }

    return count;
}

