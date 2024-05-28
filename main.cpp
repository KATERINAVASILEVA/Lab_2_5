#include <SFML/Graphics.hpp>
#include "Circle.hpp"
#include "Game.hpp"

int main()
{
    mt::Game game(1000, 800, "Game");
    game.Setup(4);

    game.LifeCycle();

    return 0;
}