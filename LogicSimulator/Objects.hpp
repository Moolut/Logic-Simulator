#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

#define MAX_CONNECTION_TO_PIN 3

class Object
{
protected:
    Object *next;             // Pointer to next object in the list
    sf::RenderWindow *window; // Pointer to SFML render window
    sf::Texture textures[2];  // SFML texture list for the object (if any). Some objects
                              // such as LED element may have multiple textures(for on
                              // stateand off state), hence this is a list
    bool state;               // State of the logic element (may be used to designate
                              // button state, D - flipflop state or whether LED is on or off)
    bool selected;            // Whether the object is selected for deletion

    enum ObjTypes
    {
        O_AND,
        O_OR,
        O_XOR,
        O_NOT,
        O_DFF,
        O_WIRE,
        O_LED,
        O_VDD,
        O_GND,
        O_CLK
    };
    ObjTypes objectType;

public:
    bool locked;       // Whether the object can move on screen or is fixed
                       // You can use this flag for toolbar items which cannot move
    sf::Sprite sprite; // SFML sprite for the object (if any)
    friend class Simulator;
    Object() {
        this->next = nullptr;
        this->window = nullptr;
    }
    ~Object();
       // TO DO MEMORY DE ALLOCATION SHOULD ADDED
    Object(const Object &old_obj);

    void SetSelected(bool isSelected);

    virtual Object *Clone() const = 0;
    virtual ObjTypes GetTypeName() const = 0;
    virtual void Simulate() = 0;

};

class Pin
{
public:
    enum pinType {INPUT,OUTPUT};      // enum for pin type (input or output pin)
    enum pinState {HIGHZ,LOW,HIGH};    // enum for pin state
    int index;           // Index of the pin for an element
                         // First input pin has index 0, second input
                         // pin has index 1, output pin has index 2, so
                         // on�
    pinType type;        // whether this pin is input or output
    bool isSrc[MAX_CONNECTION_TO_PIN];       // whether this pin is the starting point of
                         // the wire connected to it or the ending point
                         // for that wire for every connection it has
    Pin *connectedTo[MAX_CONNECTION_TO_PIN]; // List of other pins this pin is connected to
    Object *wires[MAX_CONNECTION_TO_PIN];    // Wires connected to this pin
    int numConnections;  // Number of connections on this pin
    sf::Vector2f pos;    // Screen position of the pin
    pinState state;      // Logic state of the signal on this pin
public:
    friend class LogicElement;
    Pin() {}
    ~Pin() {}
    Pin(int index,
        pinType type,
        sf::Vector2f pos,
        pinState state) : index(index),
        type(type),
        pos(pos),
        state(state),
        numConnections(0)

    {
       for (int i = 0; i < MAX_CONNECTION_TO_PIN; i++) {

           this->connectedTo[i] = nullptr;
           this->wires[i] = nullptr;
       };
    }
};


class Wire : public Object
{
public:

    sf::Vertex line[2]; // End point vertices for the wire
    Pin* pins[2];       // A list of pins that this wire is connected to
    bool complete = false;

    Wire() {
        this->objectType = O_WIRE;
        this->next = NULL;
    };
    Wire(sf::Vector2f start_point, Pin* start_pin);
    ~Wire();

    void UpdateEndPoint(sf::Vector2f end_point);
    void ConnectPin(Pin* end_pin);
    void UpdatePosition();
    sf::Vertex* GetLines();

    Object* Clone() const { return 0; };

    ObjTypes GetTypeName() const;

    bool pDistance(float mouse_x, float mouse_y);

    void Simulate();
};


class LogicElement : public Object
{

protected:
    Pin pins[4]; // List of pins of the logic element

    int numPins; // Number of pins of the logic element

public:
    LogicElement() {}
    ~LogicElement() {};
    sf::Sprite DrawPins(int i);
    void UpdatePosition();
    Pin* GetPins();
    int GetNumOfPins();
    ObjTypes GetTypeName() const;
    virtual Pin* GetClickedPin(float mouse_x_pos, float mouse_y_pos)  = 0;
    virtual int GetNumberOfWiresConnectedToPins() = 0;

};

class AndGate : public LogicElement
{

public:
    AndGate();

    ~AndGate();

    Object *Clone() const;

    void Simulate();

    Pin* GetClickedPin(float mouse_x_pos, float mouse_y_pos);

    int GetNumberOfWiresConnectedToPins();
};

class OrGate : public LogicElement
{

public:
    OrGate();

    ~OrGate();
    Object *Clone() const;
    void Simulate();

    Pin* GetClickedPin(float mouse_x_pos, float mouse_y_pos);

    int GetNumberOfWiresConnectedToPins();

};

class XorGate : public LogicElement
{

public:
    XorGate();

    ~XorGate();
    Object *Clone() const;
    void Simulate();

    Pin* GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();

};

class NotGate : public LogicElement
{
public:
    NotGate();
    ~NotGate();
    Object* Clone() const;
    void Simulate();
    Pin* GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();

};

class DFFGate : public LogicElement
{
public:
    DFFGate();
    ~DFFGate();
    Object* Clone() const;
    void Simulate();
    Pin* GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();

};

class Led : public LogicElement {
public:
    Led();
    ~Led();
    Object* Clone() const;
    void Simulate();
    Pin* GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();

};

class VDD : public LogicElement {
public:
    VDD();
    ~VDD();
    Object* Clone() const;
    void Simulate();
    Pin* GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();

};

class GND : public LogicElement {
public:
    GND();
    ~GND();
    Object* Clone() const;
    void Simulate();
    Pin* GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();

};

class CLK : public LogicElement {
public:
    int count = 0;
    const float m_fDelay = 1.f;
    sf::Clock m_Clock;
    CLK();
    ~CLK();
    Object* Clone() const;
    void Simulate();
    Pin* GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();

};