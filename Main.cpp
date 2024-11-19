#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

const int WITDH = 500, HIEGHT = 900;

static class Input {
public:
    static int Axis(std::string _axe) {
        return (_axe == "Horizontal") ?
            (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ? -1 :
            (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ? -1 :
            (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ? 1 :
            (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ? 1 : 0 :
            (_axe == "Vertical") ?
            (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ? -1 :
            (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ? -1 :
            (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ? 1 :
            (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ? 1 : 0 : 0;
    }
};

struct Bullet {
private:
    float speed = 5;
    sf::Vector2f size, pos;
    sf::RectangleShape body;

public:
    Bullet() {}

    Bullet(sf::Vector2f _pos, sf::Vector2f _size, float _speed) {
        body.setFillColor(sf::Color::Red);
        pos = sf::Vector2f(_pos.x -= (_size.x / 2), _pos.y -= (_size.y / 2));
        body.setPosition(sf::Vector2f(pos));
        body.setSize(_size);
        size = _size;
        speed = _speed;
    }

    void Update() {
        pos.y -= speed;
        body.setPosition(pos);
    }

    sf::RectangleShape Draw() {
        return body;
    }

};

struct Player {
private:
    float speed = 5;

    float currentTime, timeMax = 1, decTime = 0.1;

    sf::Vector2f size, pos;
    sf::RectangleShape body;

    std::vector<Bullet> bullets;

    void Move() {
        float x = Input::Axis("Horizontal");

        pos.x += speed * x;
    }

    void Bounds() {
        pos.x = (pos.x + (size.x / 2) > WITDH) ? 0 - (size.x / 2) : (pos.x + (size.x / 2) < 0) ? WITDH - (size.x / 2) : pos.x;
    }

    void Shoot() {
        currentTime -= decTime;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && currentTime < 0) {
            Bullet b = Bullet(sf::Vector2f(pos.x + size.x / 2, pos.y + size.y / 2), sf::Vector2f(10, 10), 15);
            bullets.push_back(b);
            currentTime = timeMax;
        }
    }

public:
    Player() {}

    Player(sf::Vector2f _pos, sf::Vector2f _size, float _speed) {
        currentTime = timeMax;
        pos = sf::Vector2f(_pos.x -= (_size.x / 2), _pos.y -= (_size.y / 2));
        body.setPosition(sf::Vector2f(pos));
        body.setSize(_size);
        body.setFillColor(sf::Color::Blue);
        size = _size;
        speed = _speed;
    }

    void Update() {
        Shoot();
        for (int i = 0; i < GetBullets(); i++) {
            bullets[i].Update();
        }
        Move();
        Bounds();
        body.setPosition(pos);

    }

    sf::RectangleShape Draw() {
        return body;
    }

    Bullet GetBullet(int i) {
        return bullets[i];
    }

    int GetBullets() {
        return bullets.size();
    }

};

struct Enemy
{
private:
    float speed = 5;
    sf::Vector2f size, pos;
    sf::RectangleShape body;

    void Bounds() {
        pos.x = (pos.x + (size.x / 2) > WITDH) ? 0 - (size.x / 2) : (pos.x + (size.x / 2) < 0) ? WITDH - (size.x / 2) : pos.x;
    }

public:
    Enemy() {}

    Enemy(sf::Vector2f _pos, sf::Vector2f _size, float _speed) {
        body.setFillColor(sf::Color::Red);
        pos = sf::Vector2f(_pos.x -= (_size.x / 2), _pos.y -= (_size.y / 2));
        body.setPosition(sf::Vector2f(pos));
        body.setSize(_size);
        size = _size;
        speed = _speed;
    }

    void Update() {

        Bounds();
        body.setPosition(pos);
    }

    sf::RectangleShape Draw() {
        return body;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WITDH, HIEGHT), "gal");

    Player player = Player(sf::Vector2f(window.getSize().x / 2, window.getSize().y - window.getSize().y / 8), sf::Vector2f(30, 30), 5);
    Enemy enemy = Enemy(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 8), sf::Vector2f(30, 30), 5);

    while (window.isOpen()) {

        window.setFramerateLimit(30);
        window.setSize(sf::Vector2u(WITDH, HIEGHT));

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        player.Update();

        window.clear();
        window.draw(player.Draw());
        window.draw(enemy.Draw());
        for (int i = 0; i < player.GetBullets(); i++) {
            window.draw(player.GetBullet(i).Draw());
        }
        window.display();
    }


}

