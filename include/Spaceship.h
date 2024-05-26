#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 700;

const int PLAYER_WIDTH = 10;
const int PLAYER_HEIGHT = 10;
const double PLAYER_SPEED = 2.0;

const int BULLET_WIDTH = 2;
const int BULLET_HEIGHT = 8;
const double BULLET_SPEED = 4.0;

const int ENEMY_WIDTH = 10;
const int ENEMY_HEIGHT = 10;
const double ENEMY_SPEED = 1.5;

const int NUM_STARS = 100;
const int STAR_COLOR_HIGH = 180;
const int STAR_COLOR_LOW = 100;

enum Scene { SCENE_TITLE, SCENE_PLAY, SCENE_GAME_OVER };

class Star {
 public:
  sf::Vector2f position;
  double speed;
};

class Player {
 public:
  sf::RectangleShape shape;
  bool isAlive;
  Player(double x, double y);
  void update();
};

class Bullet {
 public:
  sf::RectangleShape shape;
  bool isAlive;
  Bullet(double x, double y);
  void update();
};

class Enemy {
 public:
  sf::RectangleShape shape;
  bool isAlive;
  double timerOffset;
  Enemy(double x, double y);
  void update(double frameCount);
};

class Blast {
 public:
  sf::CircleShape shape;
  bool isAlive;
  double radius;
  Blast(double x, double y);
  void update();
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
  sf::Clock fireClock;
  Background background;
  Player player;
  std::vector<Bullet> bullets;
  std::vector<Enemy> enemies;
  std::vector<Blast> blasts;
  Scene scene;
  int score;
  double frameCount;
  const double firingRate = 0.5;
  void processEvents();
  void update();
  void render();
  void restart();
 public:
  Game();
  void run();
};

#endif // SPACESHIP