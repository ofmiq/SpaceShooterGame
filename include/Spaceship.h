#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 700;

const int PLAYER_WIDTH = 50;
const int PLAYER_HEIGHT = 50;
const double PLAYER_SPEED = 5.0;

const int BULLET_WIDTH = 4;
const int BULLET_HEIGHT = 12;
const double BULLET_SPEED = 4.0;

const int ENEMY_WIDTH = 40;
const int ENEMY_HEIGHT = 40;
const double ENEMY_SPEED = 4.0;

const int NUM_STARS = 100;
const int STAR_COLOR_HIGH = 180;
const int STAR_COLOR_LOW = 100;

enum Scene { SCENE_TITLE, SCENE_PLAY, SCENE_GAME_OVER };

// Interface for all game objects
class GameObject {
 public:
  virtual ~GameObject() = default;
  virtual void update() = 0;
  virtual void draw(sf::RenderWindow& window) = 0;
  virtual bool isAlive() const = 0;
};

class Star {
 public:
  sf::Vector2f position;
  double speed;
};

class Player : public GameObject {
 public:
  sf::RectangleShape shape;
  Player(double x, double y);
  void update() override;
  void draw(sf::RenderWindow& window) override;
  bool isAlive() const override { return true; }
};

class Bullet : public GameObject {
 public:
  sf::RectangleShape shape;
  bool alive;
  Bullet(double x, double y);
  void update() override;
  void draw(sf::RenderWindow& window) override;
  bool isAlive() const override { return alive; }
};

class Enemy : public GameObject {
 public:
  sf::RectangleShape shape;
  bool alive;
  double timerOffset;
  Enemy(double x, double y);
  void update() override;
  void draw(sf::RenderWindow& window) override;
  bool isAlive() const override { return alive; }
};

class Blast : public GameObject {
 public:
  sf::CircleShape shape;
  bool alive;
  double radius;
  Blast(double x, double y);
  void update() override;
  void draw(sf::RenderWindow& window) override;
  bool isAlive() const override { return alive; }
};

class Background {
 public:
  std::vector<Star> stars;
  Background();
  void update();
  void draw(sf::RenderWindow& window);
};

class Game {
 private:
  sf::RenderWindow window;
  sf::Font font;
  sf::Text text;
  sf::Text textScore;
  sf::Clock fireClock;
  Background background;
  std::unique_ptr<Player> player;
  std::vector<std::unique_ptr<GameObject>> gameObjects;
  Scene scene;
  int score;
  double frameCount;
  const double firingRate = 0.5;
  sf::Texture playerTexture;
  sf::Texture enemyTexture;

 public:
  Game();
  void run();

 private:
  void processEvents();
  void update();
  void render();
  void restart();
  void createEnemy();
  void handleCollisions();
};

#endif  // SPACESHIP_H
