/*-----------------------------------------------------------------------------------------------------------------------
 *                                                       Cruiser Class
 *
 * has 3 health, must be hit three times to be sunk
 * derived from ship class
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/

#ifndef CRUISER_H
#define CRUISER_H
#include "ship.h"

class Cruiser: public Ship{
public:
    Cruiser();
    int health;
    bool hit;

    int getHealth();
    void getHit();
    bool isHit();
    bool isSunk();

    ~Cruiser();
};

#endif // CRUISER_H
