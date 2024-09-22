#ifndef GAME_H
#define GAME_H

class City;
class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nTooters);
    ~Game();

        // Mutators
    void play();

  private:
    City* m_city;
};
#endif // GRID_INCLUDED
