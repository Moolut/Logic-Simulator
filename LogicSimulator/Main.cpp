#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;




class Object {

protected:
    Object* next;               //Pointer to next object in the list

                    //Whether the object can move on screen or is fixed
                                //You can use this flag for toolbar items which cannot move

    sf::RenderWindow* window;   //Pointer to SFML render window

    sf::Texture textures[2];    //SFML texture list for the object (if any). Some objects
                                //such as LED element may have multiple textures(for on
                                //stateand off state), hence this is a list

              //SFML sprite for the object (if any)
    bool state;                 //State of the logic element (may be used to designate
                                //button state, D - flipflop state or whether LED is on or off)

    bool selected;              //Whether the object is selected for deletion

public:
    bool locked;
    sf::Sprite sprite;
    friend class Simulator;
    Object() {
       
    }
    ~Object() {
        
        delete this->next;
        delete this->window;

    }
    Object(const Object& old_obj) {

        cout << "Copy constructor " << endl;
        this->next = old_obj.next;
        this->locked = false;
        this->window = old_obj.window;
        this->textures[0] = old_obj.textures[0];
        this->textures[1] = old_obj.textures[1];
        this->sprite = old_obj.sprite;
        this->sprite.setPosition(sf::Vector2f(100.f, 200.f));
        this->state = old_obj.state;
        this->selected = old_obj.selected;
    }
    virtual Object* Clone() const = 0;


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

    Object* Clone() const { return new AndGate(*this); }

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
    Object* Clone() const { return new OrGate(*this); }


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
    Object* Clone() const { return new XorGate(*this); }


};



class Simulator {

    sf::RenderWindow* window;               //Pointer to SFML render window
    sf::VideoMode videoMode;
    Object* objects;                        //Pointer to a list of objects on screen
    sf::Event event;
    bool clicked = false;
    bool holding = false;
    sf::Sprite* focus;

private:
    void initVariables() {

        this->window = nullptr;
        this->objects = nullptr;
        this->focus = nullptr;

    }
    void initWindow() {
        
        this->videoMode.height = 600;
        this->videoMode.width = 800;

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
    
        bool creating = false;
        bool moving = false;
        bool clicking = false;
        bool dragging = false;

        sf::Vector2i mousePos;


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
                case sf::Event::MouseMoved:
                    mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
                    moving = true;
                    break;
                case sf::Event::MouseButtonPressed:

                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        this->clicked = clicking = true;
                        mousePos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);

                       /* std::cout << "the left button was pressed" << std::endl;
                        std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                        std::cout << "mouse y: " << event.mouseButton.y << std::endl;*/
                    }
                    

                    break;
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        this->clicked = clicking = false;
                        /*std::cout << "the left button was pressed" << std::endl;
                        std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                        std::cout << "mouse y: " << event.mouseButton.y << std::endl;*/
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

        if (clicking)
        {
            Object* ptr = this->objects;

            if (this->objects == NULL)
                cout << "There is no object" << endl;

            while (ptr) {

                if (ptr->sprite.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    cout << "Clicked on obj" << endl;
                    if (ptr->locked) {
                        Object* new_obj = ptr->Clone();

                        AddObject(new_obj);
                    }
                    else {
                        this->focus = &(ptr->sprite);
                        std::clog << this->focus << std::endl;
                    }

                    break;
                }
                else {
                    cout << "Not Clicked on obj" << endl;

                }
                ptr = ptr->next;

            }
        }
        else
            if (dragging)
            {
                focus->setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            }
            else if (!holding)
                focus = nullptr; //I'm not doing anything so I can assume there's no sprite being focused


    }

    void update() {
        
        this->pollEvents();

        //cout << "Mouse position-> x: " << sf::Mouse::getPosition(*this->window).x << " y: " << sf::Mouse::getPosition(*this->window).y << endl;
    }

    void render() {
        
        this->window->clear(sf::Color::Black);


        drawElements();

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
    Object* _toolbar_and = new AndGate();
    Object* _toolbar_or = new OrGate();
    Object* _toolbar_xor = new XorGate();


    _toolbar_and->sprite.setPosition(sf::Vector2f(40.f, 40.f));
    _toolbar_and->locked = true;
    _toolbar_or->sprite.setPosition(sf::Vector2f(40.f, 140.f));
    _toolbar_or->locked = true;
    _toolbar_xor->sprite.setPosition(sf::Vector2f(40.f, 240.f));
    _toolbar_xor->locked = true;

    /*this->window->draw(_toolbar_and->sprite);
    this->window->draw(_toolbar_or->sprite);
    this->window->draw(_toolbar_xor->sprite);*/

    simulator.AddObject(_toolbar_and);
    simulator.AddObject(_toolbar_or);
    simulator.AddObject(_toolbar_xor);

    // run the program as long as the window is open
    while (simulator.isRunning())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        simulator.update();

        simulator.render();

    }
    return 0;
};