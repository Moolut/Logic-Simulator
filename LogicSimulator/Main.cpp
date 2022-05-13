#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;




class Object {

protected:
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

public:
    friend class Simulator;
    Object() {
       
    }
    ~Object() {
        
        delete this->next;
        delete this->window;

    }


    void onClick() {
      

    }



};

class Wire : Object {

    sf::Vertex line[2];         //End point vertices for the wire

    /*Pin* pins[2]; */              //A list of pins that this wire is connected to
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


public:
    LogicElement() {}
};


class AndGate : public LogicElement {
    
    
public:
    AndGate() {
            
        this->next = NULL;
        
        if (!this->textures[0].loadFromFile("../assets/AND.png"))
            exit(0);
        this->sprite.setTexture(this->textures[0]);


    }

    ~AndGate() {
    }

};


class OrGate : public LogicElement {


public:
    OrGate() {

        this->next = NULL;

        if (!this->textures[0].loadFromFile("../assets/OR.png"))
            exit(0);
        this->sprite.setTexture(this->textures[0]);


    }

    ~OrGate() {
    }

};


class XorGate : public LogicElement {


public:
    XorGate() {

        this->next = NULL;

        if (!this->textures[0].loadFromFile("../assets/XOR.png"))
            exit(0);
        this->sprite.setTexture(this->textures[0]);


    }

    ~XorGate() {
    }

};



class Simulator {

    sf::RenderWindow* window;               //Pointer to SFML render window
    sf::VideoMode videoMode;
    Object* objects;                        //Pointer to a list of objects on screen
    sf::Event event;
private:
    void initVariables() {

        this->window = nullptr;
        this->objects = nullptr;

    }
    void initWindow() {
        
        this->videoMode.height = 1800;
        this->videoMode.width = 2400;

        this->window = new sf::RenderWindow(this->videoMode, "Logic Simulator", sf::Style::Titlebar | sf::Style::Close);
    }
    

public:

    Simulator() {
        this->initVariables();
        this->initWindow();
        
    }

    ~Simulator() {
        
        delete this->window;
        delete this->objects;
    }

    const bool isRunning() const {
        
        return this->window->isOpen();
    }

    void pollEvents() {
    
        while (this->window->pollEvent(this->event))
        {

            switch (this->event.type)
            {
                case sf::Event::Closed:
                    this->window->close();
                    break;
                case sf::Event::KeyPressed:
                    if (this->event.key.code == sf::Keyboard::Escape)
                        this->window->close();
                    break;
                default:
                    break;
            }
            // "close requested" event: we close the window
            if (this->event.type == sf::Event::Closed)
                this->window->close();


        }
    }

    void update() {
        
        this->pollEvents();

        cout << "Mouse position-> x: " << sf::Mouse::getPosition(*this->window).x << " y: " << sf::Mouse::getPosition(*this->window).y << endl;
    }

    void render() {
        
        this->window->clear(sf::Color::Black);

        Object* _toolbar_and = new AndGate();
        Object* _toolbar_or = new OrGate();
        Object* _toolbar_xor = new XorGate();


        _toolbar_and->sprite.setPosition(sf::Vector2f(40.f, 40.f));
        _toolbar_or->sprite.setPosition(sf::Vector2f(40.f, 140.f));
        _toolbar_xor->sprite.setPosition(sf::Vector2f(40.f, 240.f));
        
        this->window->draw(_toolbar_and->sprite);
        this->window->draw(_toolbar_or->sprite);
        this->window->draw(_toolbar_xor->sprite);

        /*AddObject(_toolbar_and);
        AddObject(_toolbar_or);
        drawElements();*/

        this->window->display();
    }

    void AddObject(Object *obj) {
            
        obj->next = objects;
        objects = obj;
    }


    void drawElements() {
        
        Object* ptr = objects;

        if (objects == NULL)
            cout << "There is no object" << endl;

        while (ptr) {
            
            this->window->draw(ptr->sprite);
            ptr = ptr->next;
        }
    }

};


int main()
{


    // Initialize Simulator

    Simulator simulator;

    // run the program as long as the window is open
    while (simulator.isRunning())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        simulator.update();

        simulator.render();

    }
    return 0;
};