#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;




class Object {

    Object* next;               //Pointer to next object in the list

    bool locked;                //Whether the object can move on screen or is fixed
                                //You can use this flag for toolbar items which cannot move

    sf::RenderWindow* window;   //Pointer to SFML render window

    sf::Texture textures[2];    //SFML texture list for the object (if any). Some objects
                                //such as LED element may have multiple textures(for on
                                //stateand off state), hence this is a list

    sf::Sprite sprite;          //SFML sprite for the object (if any)
    bool state;                 //State of the logic element (may be used to designate
                                //button state, D - flipflop state or whether LED is on or off)

    bool selected;              //Whether the object is selected for deletion

protected:
    Object() {
        
        

    }
    void SetEditingMode() {}
    void clickEvent() {}

};

class Wire : Object {

    sf::Vertex line[2];         //End point vertices for the wire

    /*Pin* pins[2];*/               //A list of pins that this wire is connected to
};

class Pin {

    enum pinType { INPUT, OUTPUT };         //enum for pin type (input or output pin)
    
    enum pinState { HIGHZ, LOW, HIGH };      //enum for pin state

    int index;                              //Index of the pin for an element
                                            //First input pin has index 0, second input
                                            //pin has index 1, output pin has index 2, so
                                            //on…

    pinType type;                           //whether this pin is input or output
    
    bool isSrc[3];            //whether this pin is the starting point of
                                            //the wire connected to it or the ending point
                                            //for that wire for every connection it has

    Pin* connectedTo[3];      //List of other pins this pin is connected to

    Object* wires[3];         //Wires connected to this pin

    int numConnections;                     //Number of connections on this pin
    
    sf::Vector2f pos;                       //Screen position of the pin
    
    pinState state;                         //Logic state of the signal on this pin
};


class LogicElement : public Object {
        
    Pin pins[4];                            //List of pins of the logic element

    int numPins;                            //Number of pins of the logic element

protected:
    LogicElement() {}
};


class AndGate : public LogicElement {

private:
    AndGate(){
        
    }

};

class Simulator {

    sf::RenderWindow* window;               //Pointer to SFML render window
    Object* objects;                        //Pointer to a list of objects on screen

};


int main()
{

    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Logic Simulator");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();


        }

        window.clear(sf::Color::Black);

        // draw everything here...

        // end the current frame
        window.display();
    }
    return 0;
};