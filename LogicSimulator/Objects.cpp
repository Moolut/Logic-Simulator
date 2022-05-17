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
};

void Wire::UpdatePosition() {
    if (complete) {
        this->line[0].position = this->pins[0]->pos;
        this->line[1].position = this->pins[1]->pos;
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

Object::ObjTypes AndGate::GetTypeName() const
{
    return this->objectType;
};

Pin *AndGate::GetPins()
{

    return this->pins;
};

int AndGate::GetNumOfPins()
{
    return this->numPins;
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

Object::ObjTypes OrGate::GetTypeName() const
{
    return this->objectType;
};

Pin *OrGate::GetPins()
{

    return this->pins;
};

int OrGate::GetNumOfPins()
{
    return this->numPins;
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

Object::ObjTypes XorGate::GetTypeName() const
{

    return this->objectType;
};

Pin *XorGate::GetPins()
{

    return this->pins;
};

int XorGate::GetNumOfPins()
{
    return this->numPins;
};

void AndGate::UpdatePosition()
{
    this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 10.f);
    this->pins[1].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 40.f);
    this->pins[2].pos = sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 25.f);
};

void OrGate::UpdatePosition()
{
    this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 10.f);
    this->pins[1].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 40.f);
    this->pins[2].pos = sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 25.f);
};

void XorGate::UpdatePosition()
{
    this->pins[0].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 10.f);
    this->pins[1].pos = sf::Vector2f(this->sprite.getPosition().x + 1.f, this->sprite.getPosition().y + 40.f);
    this->pins[2].pos = sf::Vector2f(this->sprite.getPosition().x + 90.f, this->sprite.getPosition().y + 25.f);
};


sf::Sprite AndGate::DrawPins(int i)
{

        sf::Sprite sprite;
        sf::Texture texture;

        if (!texture.create(10, 10))
            exit(0);

        sprite.setTexture(texture);
        sprite.setPosition(pins[i].pos);
        return sprite;

};
sf::Sprite OrGate::DrawPins(int i)
{

        sf::Sprite sprite;
        sf::Texture texture;

        if (!texture.create(10, 10))
            exit(0);

        sprite.setTexture(texture);
        sprite.setPosition(pins[i].pos);
        return sprite;
    
};
sf::Sprite XorGate::DrawPins(int i)
{

        sf::Sprite sprite;
        sf::Texture texture;

        if (!texture.create(10, 10))
            exit(0);

        sprite.setTexture(texture);
        sprite.setPosition(pins[i].pos);
        return sprite;
    
};