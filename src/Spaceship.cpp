#include "Spaceship.h"

Player::Player(double x, double y) {
  shape.setSize(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT));
  shape.setPosition(x, y);
}

void Player::update() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    shape.move(-PLAYER_SPEED, 0);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    shape.move(PLAYER_SPEED, 0);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    shape.move(0, -PLAYER_SPEED);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    shape.move(0, PLAYER_SPEED);
  }

  if (shape.getPosition().x < 0) {
    shape.setPosition(0, shape.getPosition().y);
  }
  if (shape.getPosition().x > WINDOW_WIDTH - PLAYER_WIDTH) {
    shape.setPosition(WINDOW_WIDTH - PLAYER_WIDTH, shape.getPosition().y);
  }
  if (shape.getPosition().y < 0) {
    shape.setPosition(shape.getPosition().x, 0);
  }
  if (shape.getPosition().y > WINDOW_HEIGHT - PLAYER_HEIGHT) {
    shape.setPosition(shape.getPosition().x, WINDOW_HEIGHT - PLAYER_HEIGHT);
  }
}

void Player::draw(sf::RenderWindow& window) { window.draw(shape); }

Bullet::Bullet(double x, double y) : alive(true) {
  shape.setSize(sf::Vector2f(BULLET_WIDTH, BULLET_HEIGHT));
  shape.setFillColor(sf::Color::Yellow);
  shape.setPosition(x, y);
}

void Bullet::update() {
  shape.move(0, -BULLET_SPEED);
  if (shape.getPosition().y < 0) {
    alive = false;
  }
}

void Bullet::draw(sf::RenderWindow& window) { window.draw(shape); }

Enemy::Enemy(double x, double y)
    : alive(true), timerOffset(static_cast<double>(std::rand() % 60)) {
  shape.setSize(sf::Vector2f(ENEMY_WIDTH, ENEMY_HEIGHT));
  shape.setPosition(x, y);
}

void Enemy::update() {
  shape.move(0, ENEMY_SPEED);

  if (shape.getPosition().y > WINDOW_HEIGHT) {
    alive = false;
  }
}

void Enemy::draw(sf::RenderWindow& window) { window.draw(shape); }

Blast::Blast(double x, double y) : alive(true), radius(1.0) {
  shape.setRadius(radius);
  shape.setFillColor(sf::Color::White);
  shape.setOutlineThickness(1);
  shape.setOutlineColor(sf::Color::Cyan);
  shape.setPosition(x, y);
}

void Blast::update() {
  radius += 1.0;
  if (radius > 8.0) {
    alive = false;
  }
  shape.setRadius(radius);
  shape.setOrigin(radius, radius);
}

void Blast::draw(sf::RenderWindow& window) { window.draw(shape); }

Background::Background() {
  for (int i = 0; i < NUM_STARS; ++i) {
    Star star;
    star.position =
        sf::Vector2f(static_cast<double>(std::rand() % WINDOW_WIDTH),
                     static_cast<double>(std::rand() % WINDOW_HEIGHT));
    star.speed = 1.0 + static_cast<double>(std::rand() % 150) / 100.0;
    stars.push_back(star);
  }
}

void Background::update() {
  for (auto& star : stars) {
    star.position.y += star.speed;
    if (star.position.y >= WINDOW_HEIGHT) star.position.y -= WINDOW_HEIGHT;
  }
}

void Background::draw(sf::RenderWindow& window) {
  for (const auto& star : stars) {
    sf::RectangleShape shape(sf::Vector2f(1, 1));
    shape.setPosition(star.position);
    shape.setFillColor(
        sf::Color(star.speed > 1.8 ? STAR_COLOR_HIGH : STAR_COLOR_LOW,
                  STAR_COLOR_HIGH, STAR_COLOR_LOW));
    window.draw(shape);
  }
}

Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Shooter"),
      scene(SCENE_TITLE),
      background(),
      score(0),
      frameCount(0),
      player(std::make_unique<Player>(WINDOW_WIDTH / 2 - PLAYER_WIDTH / 2,
                                      WINDOW_HEIGHT - 20)) {
  window.setFramerateLimit(60);
  window.setPosition(sf::Vector2i(500, 50));

  if (!font.loadFromFile("../assets/fonts/arial.ttf")) {
    std::cerr << "Failed to load font 'arial.ttf'" << '\n';
    exit(EXIT_FAILURE);
  }

  if (!playerTexture.loadFromFile("../assets/imgs/player.png")) {
    std::cerr << "Failed to load player texture" << '\n';
    exit(EXIT_FAILURE);
  }
  player->shape.setTexture(&playerTexture);

  if (!enemyTexture.loadFromFile("../assets/imgs/enemy.png")) {
    std::cerr << "Failed to load enemy texture" << '\n';
    exit(EXIT_FAILURE);
  }

  text.setFont(font);
  text.setCharacterSize(24);
  text.setFillColor(sf::Color::White);
  text.setStyle(sf::Text::Bold);
}

void Game::run() {
  while (window.isOpen()) {
    processEvents();
    update();
    render();
  }
}

void Game::processEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed ||
        event.key.code == sf::Keyboard::Escape) {
      window.close();
    }
    if (scene == SCENE_TITLE && (event.type == sf::Event::KeyPressed &&
                                 event.key.code == sf::Keyboard::Return)) {
      scene = SCENE_PLAY;
    } else if (scene == SCENE_GAME_OVER &&
               (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Return)) {
      restart();
    }
  }
}

void Game::update() {
  background.update();

  if (scene == SCENE_PLAY) {
    frameCount += 1.0;
    player->update();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      if (fireClock.getElapsedTime().asSeconds() >= firingRate) {
        gameObjects.push_back(std::make_unique<Bullet>(
            player->shape.getPosition().x + PLAYER_WIDTH / 2 - BULLET_WIDTH / 2,
            player->shape.getPosition().y - BULLET_HEIGHT));
        fireClock.restart();
      }
    }

    if (static_cast<int>(frameCount) % 60 == 0) {
      createEnemy();
    }

    for (auto& gameObject : gameObjects) {
      gameObject->update();
    }

    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
                       [](const std::unique_ptr<GameObject>& obj) {
                         return !obj->isAlive();
                       }),
        gameObjects.end());

    handleCollisions();
  }
}

void Game::render() {
  window.clear();
  background.draw(window);
  switch (scene) {
    case SCENE_TITLE:
      text.setString("  SFML Shooter\n- PRESS ENTER -");
      text.setPosition(WINDOW_WIDTH / 2 - 100, 70);
      window.draw(text);
      break;
    case SCENE_PLAY:
      player->draw(window);
      for (const auto& gameObject : gameObjects) {
        gameObject->draw(window);
      }
      text.setString("SCORE: " + std::to_string(score));
      text.setPosition(10, 10);
      window.draw(text);
      break;
    case SCENE_GAME_OVER:
      text.setString("   GAME OVER\n- PRESS ENTER -\n\n\n\n\n  Your score: " +
                     std::to_string(score));
      text.setPosition(WINDOW_WIDTH / 2 - 100, 70);
      window.draw(text);
      break;
  }
  window.display();
}

void Game::restart() {
  scene = SCENE_PLAY;
  player->shape.setPosition(WINDOW_WIDTH / 2 - PLAYER_WIDTH / 2,
                            WINDOW_HEIGHT - 20);
  score = 0;
  gameObjects.clear();
  frameCount = 0;
}

void Game::createEnemy() {
  double x = std::rand() % (WINDOW_WIDTH - ENEMY_WIDTH);
  auto enemy = std::make_unique<Enemy>(x, 0);
  enemy->shape.setTexture(&enemyTexture);
  gameObjects.push_back(std::move(enemy));
}

void Game::handleCollisions() {
  std::vector<std::unique_ptr<GameObject>> newObjects;

  for (auto& gameObject : gameObjects) {
    if (Enemy* enemy = dynamic_cast<Enemy*>(gameObject.get())) {
      for (auto& bulletObj : gameObjects) {
        if (Bullet* bullet = dynamic_cast<Bullet*>(bulletObj.get())) {
          if (enemy->shape.getGlobalBounds().intersects(
                  bullet->shape.getGlobalBounds())) {
            enemy->alive = false;
            bullet->alive = false;
            newObjects.push_back(std::make_unique<Blast>(
                enemy->shape.getPosition().x + ENEMY_WIDTH / 2,
                enemy->shape.getPosition().y + ENEMY_HEIGHT / 2));
            score += 10;
          }
        }
      }

      if (enemy->shape.getGlobalBounds().intersects(
              player->shape.getGlobalBounds())) {
        enemy->alive = false;
        newObjects.push_back(std::make_unique<Blast>(
            player->shape.getPosition().x + PLAYER_WIDTH / 2,
            player->shape.getPosition().y + PLAYER_HEIGHT / 2));
        scene = SCENE_GAME_OVER;
      }
    }
  }

  for (auto& newObj : newObjects) {
    gameObjects.push_back(std::move(newObj));
  }
}
