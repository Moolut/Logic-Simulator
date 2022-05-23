#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

#define MAX_CONNECTION_TO_PIN 32



//-----------------------------------------------Object CLASS-----------------------------------------------

class Object
{
protected:
    Object *next;                   // Pointer to next object in the list
    sf::RenderWindow *window;       // Pointer to SFML render window
    sf::Texture textures[2];        // SFML texture list for the object (if any). Some objects
                                    // such as LED element may have multiple textures(for on
                                    // stateand off state), hence this is a list
    bool state;                     // State of the logic element (may be used to designate
                                    // button state, D - flipflop state or whether LED is on or off)
    bool selected;                  // Whether the object is selected for deletion

    enum ObjTypes                   // Enum for of the object types that we used
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
    ObjTypes objectType;            // Determines the type of the object
    bool locked;                    // Whether the object can move on screen or is fixed
                            
    sf::Sprite sprite;              // SFML sprite for the object (if any)

public:
    friend class Simulator;         // The added this to access the object's attributes in the list
    Object()                        // Constructor -> we are initalizing the pointers to nullptr
    {
        this->next = nullptr;
        this->window = nullptr;
    }
    ~Object();                                  // Deconstructor
    
    Object(const Object &old_obj);              // Copy constructor

             

    // Pure virtual functions
    virtual Object *Clone() const = 0;          //  Clone function 
    virtual ObjTypes GetTypeName() const = 0;   //  GetTypeName function
    virtual void Simulate() = 0;                //  Simulate function
    
    // Getter Setters
    bool IsLocked() { return this->locked; };
    void Lock() { this->locked = true; };
    sf::Sprite GetSprite() { return this->sprite; };
    void SetPosition(sf::Vector2f position) { this->sprite.setPosition(position); }
    void SetSelected(bool isSelected) { this->selected = isSelected; };
};



//-----------------------------------------------Pin CLASS-----------------------------------------------

class Pin
{
public:
    enum pinType                            // enum for pin type (input or output pin)
    {
        INPUT,
        OUTPUT
    };                                     
    enum pinState                           // enum for pin state
    {
        HIGHZ,
        LOW,
        HIGH
    };
protected:                                  
    int index;                               // Index of the pin for an element
                                             // First input pin has index 0, second input
                                             // pin has index 1, output pin has index 2, so
                                             // on�
    
    
    Pin *connectedTo[MAX_CONNECTION_TO_PIN]; // List of other pins this pin is connected to
    
    sf::Vector2f pos;                        // Screen position of the pin
    
    pinState state;                          // Logic state of the signal on this pin
    
    int numConnections;                      // Number of connections on this pin
    pinType type;                            // whether this pin is input or output
    Object* wires[MAX_CONNECTION_TO_PIN];    // Wires connected to this pin
public:
    friend class LogicElement;              // To access the Pin class attributes in LogicElement class
    Pin() {}                                // Default constructor
    ~Pin() {}                               // Deconstructor
    Pin(int index,                          // Constructor
        pinType type,
        sf::Vector2f pos,
        pinState state) : index(index),
                          type(type),
                          pos(pos),
                          state(state),
                          numConnections(0)

    {
        // In the constructor we are setting the wires & connectedTo NULL
        for (int i = 0; i < MAX_CONNECTION_TO_PIN; i++)
        {
            this->connectedTo[i] = nullptr;
            this->wires[i] = nullptr;
        };
    } 
    // Getter Setters
    void SetState(pinState state);
    pinState GetState();
    void SetConnection(Pin* pin) {
        this->connectedTo[0] = pin;
    };
    sf::Vector2f GetPosition() { return this->pos; };
    pinType GetType() { return this->type; };
    int GetNumOfConnections() { return this->numConnections; }
    void SetNumOfConnections(int num) { this->numConnections = num; };
    void DecrementNumOfConnections() { this->numConnections -= 1; };
    void IncrementNumOfConnections() { this->numConnections += 1; };
    Object* GetWires(int i) { return this->wires[i]; };
    void SetWires(int i, Object* wire) { this->wires[i] = wire; };
};



//-----------------------------------------------Wire CLASS-----------------------------------------------

class Wire : public Object
{
protected:
    sf::Vertex line[2];                                 // End point vertices for the wire
    Pin* pins[2];                                       // A list of pins that this wire is connected to
    bool complete = false;                              // Determines whether the wiring is completed or not
public:
    Wire()                                              // Default constructor
    {
        this->objectType = O_WIRE;
        this->next = NULL;
    };
    Wire(sf::Vector2f start_point, Pin *start_pin);     // Constructor with using the start point of wire
    ~Wire();                                            // Deconstructor
    
    // Methods
    void UpdateEndPoint(sf::Vector2f end_point);        
    void ConnectPin(Pin *end_pin);
    void UpdatePosition();
    bool pDistance(float mouse_x, float mouse_y);

    // Getter-Setter
    bool isComplete() { return this->complete; };
    void SetColor(sf::Color color) {
        line[0].color = color;
        line[1].color = color;
    }
    sf::Vertex *GetLines();

    // The functions that overrides
    Object *Clone() const { return 0; };
    ObjTypes GetTypeName() const;
   
    void Simulate();
};


//-----------------------------------------------LogicElement CLASS-----------------------------------------------

class LogicElement : public Object
{

protected:
    Pin pins[4];                        // List of pins of the logic element

    int numPins;                        // Number of pins of the logic element

public:
    LogicElement() {}                  // Default constructor
    ~LogicElement(){};                 // Deconstructor

    // Methods
    sf::Sprite DrawPins(int i);        
    void UpdatePosition();
    
    // Getters-Setters
    Pin *GetPins();
    int GetNumOfPins();

    // Overrides
    ObjTypes GetTypeName() const;

    // Pure virtual functions
    virtual Pin *GetClickedPin(float mouse_x_pos, float mouse_y_pos) = 0;
    virtual int GetNumberOfWiresConnectedToPins() = 0;
};


//-----------------------------------------------AndGate CLASS-----------------------------------------------


class AndGate : public LogicElement
{
public:
    AndGate();                  // Constructor
    ~AndGate();                 // Deconstructor


    // Overrides
    Object *Clone() const;
    void Simulate();
    Pin *GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();
};


//-----------------------------------------------OrGate CLASS-----------------------------------------------

class OrGate : public LogicElement
{
public:
    OrGate();                   // Constructor
    ~OrGate();                  // Deconstructor

    // Overrides
    Object *Clone() const;
    void Simulate();
    Pin *GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();
};


//-----------------------------------------------XorGate CLASS-----------------------------------------------

class XorGate : public LogicElement
{
public:
    XorGate();                  // Constructor
    ~XorGate();                 // Deconstructor

    // Overrides
    Object *Clone() const;
    void Simulate();
    Pin *GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();
};


//-----------------------------------------------NotGate CLASS-----------------------------------------------

class NotGate : public LogicElement
{
public:
    NotGate();                  // Constructor
    ~NotGate();                 // Deconstructor

    // Overrides
    Object *Clone() const;
    void Simulate();
    Pin *GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();
};


//-----------------------------------------------DFFGate CLASS-----------------------------------------------

class DFFGate : public LogicElement
{
public:
    DFFGate();                  // Constructor
    ~DFFGate();                 // Deconstructor

    // Overrides
    Object *Clone() const;
    void Simulate();
    Pin *GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();
};


//-----------------------------------------------Led CLASS-----------------------------------------------

class Led : public LogicElement
{
public:
    Led();                      // Constructor
    ~Led();                     // Deconstructor

    // Overrides
    Object *Clone() const;
    void Simulate();
    Pin *GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();
};


//-----------------------------------------------VDD CLASS-----------------------------------------------

class VDD : public LogicElement
{
public:
    VDD();                      // Constructor
    ~VDD();                     // Deconstructor

    // Overrides
    Object *Clone() const;
    void Simulate();
    Pin *GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();
};


//-----------------------------------------------GND CLASS-----------------------------------------------

class GND : public LogicElement
{
public:
    GND();                      // Constructor
    ~GND();                     // Deconstructor

    // Overrides
    Object *Clone() const;
    void Simulate();
    Pin *GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();
};


//-----------------------------------------------CLK CLASS-----------------------------------------------

class CLK : public LogicElement
{
protected:

    // Unlike the other logic elements the CLOCK has extra attributes

    int count = 0;                  // With count we check whether the pin should LOW or HIGH
    const float m_fDelay = 1.f;     // Determines the delay of the clock
    sf::Clock m_Clock;
public:
    CLK();                          // Constructor
    ~CLK();                         // Deconstructor

    // Overrides
    Object *Clone() const;
    void Simulate();
    Pin *GetClickedPin(float mouse_x_pos, float mouse_y_pos);
    int GetNumberOfWiresConnectedToPins();
};