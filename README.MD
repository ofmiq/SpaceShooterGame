# SFML Shooter

A basic 2D space shooter game called SpaceGameShooter was created with the help of the Simple and Fast Multimedia Library (SFML). In order to fire down foes and dodge their attacks, the player pilots a spaceship in this simple gameplay mode.

## Requirements

- C++ compiler supporting C++11 or later.
- SFML library (version 2.5 or later) installed on your system.

## How to Run

1. Clone this repository to your local machine:

   ```
   git clone https://github.com/ofmiq/SpaceShooterGame.git
   ```

2. Navigate to the project directory:

   ```
   cd SpaceShooterGame/src
   ```

3. Compile the game using your preferred C++ compiler. For example, using g++:

   ```
   g++ -o SpaceShooter.exe main.cpp Spaceship.cpp -I../include -lsfml-graphics -lsfml-window -lsfml-system
   ```

   Replace `g++` with your compiler command if needed.

4. Run the compiled executable:

   ```
   ./SpaceShooter.exe
   ```

## Controls

- Use the arrow keys to move the player spaceship.
- Press the spacebar to shoot bullets.

## Gameplay

- The player's goal is to survive as long as possible, shooting down enemy spaceships to earn points.
- Each enemy destroyed earns the player 10 points.
- Colliding with an enemy spaceship ends the game.

## License

This project is licensed under the MIT License. See the [LICENSE](https://github.com/ofmiq/SpaceShooterGame/blob/main/LICENCE) file for details.
