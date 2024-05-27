#include "Spaceship.h"

int main() {
  std::srand(static_cast<unsigned>(std::time(nullptr)));
  Game game;
  game.run();
}