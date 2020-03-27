/*------------------------------------------------------------------------------------------------------------------------
 *                                                 Battleship class
 *
 * Has 4 health, meaning it needs to be hit four times to be sunk.
 * derived from ship class
 *
 *  Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/
#ifndef BATTLESHIP_H
#define BATTLESHIP_H
#include "ship.h"

class Battleship: public Ship{
public:
    Battleship();
    int health;
    bool hit;

    int getHealth();
    void getHit();
    bool isHit();
    bool isSunk();

    ~Battleship();
};

#endif // BATTLESHIP_H
