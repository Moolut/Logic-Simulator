#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.hpp"

Object::Object(const Object &old_obj)
{
    cout << "Copy constructor " << endl;
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

Wire::Wire(sf::Vector2f start_point, Pin* start_pin) {
    this->objectType = O_WIRE;
    this->next = NULL;

    this->line[0].position = start_point;
    this->line[0].color = sf::Color::Red;

    this->pins[0] = start_pin;
};

void Wire::UpdateEndPoint(sf::Vector2f end_point) {
    this->line[1].position = end_point;
    this->line[1].color = sf::Color::Red;
}

void Wire::ConnectPin(Pin* end_pin) {
    this->line[1].position = end_pin->pos;
    this->line[1].color = sf::Color::Red;

    this->pins[1] = end_pin;
    this->complete = true;

    this->pins[1]->connectedTo[0] = this->pins[0];
    this->pins[0]->connectedTo[0] = this->pins[1];
};

void Wire::UpdatePosition() {
    if (complete) {
        this->line[0].position = this->pins[0]->pos;
        this->line[1].position = this->pins[1]->pos;
        cout << this->pins[1] << endl;
    }
};

void Wire::Simulate() {
    if (complete) {
        this->pins[1]->state = this->pins[0]->state;
    }
};



sf::Vertex *Wire::GetLines() {
    return this->line;
}

Object::ObjTypes Wire::GetTypeName() const
{
    return this->objectType;
};

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

AndGate::~AndGate(){};

Object *AndGate::Clone() const { return new AndGate(*this); };

void AndGate::Simulate() {
    if (pins[0].state == Pin::pinState::HIGH && pins[1].state == Pin::pinState::HIGH) {
        pins[2].state = Pin::pinState::HIGH;
    }
    else if (pins[0].state == Pin::pinState::LOW || pins[1].state == Pin::pinState::LOW) {
        pins[2].state = Pin::pinState::LOW;
    }
    else {
        pins[2].state = Pin::pinState::HIGHZ;
    }
};


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

void OrGate::Simulate() {
    if (pins[0].state == Pin::pinState::HIGH || pins[1].state == Pin::pinState::HIGH) {
        pins[2].state = Pin::pinState::HIGH;
    }
    else if (pins[0].state == Pin::pinState::LOW && pins[1].state == Pin::pinState::LOW) {
        pins[2].state = Pin::pinState::LOW;
    }
    else {
        pins[2].state = Pin::pinState::HIGHZ;
    }
};


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

void XorGate::Simulate() {

    if (pins[0].state == Pin::pinState::HIGHZ || pins[1].state == Pin::pinState::HIGHZ) {
        pins[2].state = Pin::pinState::HIGHZ;
    }
    else if (pins[0].state != pins[1].state) {
        pins[2].state = Pin::pinState::HIGH;
    }
    else {
        pins[2].state = Pin::pinState::LOW;
    }
};

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
Led::~Led() {};

Object* Led::Clone() const { return new Led(*this); };

void Led::Simulate() {
    if (pins[0].state > pins[1].state || pins[1].state > pins[0].state) {
        sprite.setTexture(this->textures[1]);
    }
    else {
        sprite.setTexture(this->textures[0]);
    }
}



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

VDD::~VDD() {};

Object* VDD::Clone() const { return new VDD(*this); };

void VDD::Simulate() {
    pins[0].state = Pin::pinState::HIGH;
};

Object::ObjTypes LogicElement::GetTypeName() const
{
    return this->objectType;
};

Pin *LogicElement::GetPins()
{

    return this->pins;
};

int LogicElement::GetNumOfPins()
{
    return this->numPins;
};

void LogicElement::UpdatePosition()
{
    if (numPins == 3) {
        this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 10.f);
        this->pins[1].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 40.f);
        this->pins[2].pos = sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 25.f);
    }
    else if (numPins == 2) {
        this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 8.f, this->sprite.getPosition().y + 68.f);
        this->pins[1].pos = sf::Vector2f(this->sprite.getPosition().x + 25.f, this->sprite.getPosition().y + 68.f);
    }
    else {
        this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 25.f, this->sprite.getPosition().y + 50.f);
    }

};

sf::Sprite LogicElement::DrawPins(int i)
{
        sf::Sprite sprite;
        sf::Texture texture;

        if (!texture.create(10, 10))
            exit(0);


        sprite.setTexture(texture);
        sprite.setPosition(pins[i].pos);

        if (pins[i].state == Pin::pinState::HIGH) {
            sprite.setColor(sf::Color::Green);
        }
        else if (pins[i].state == Pin::pinState::LOW) {
            sprite.setColor(sf::Color::Red);
        }
        else {
            sprite.setColor(sf::Color::White);
        }

        return sprite;
};
