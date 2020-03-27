/*------------------------------------------------------------------------------------------------------------------------
 *                                                   Carrier Class
 *
 * derived from the ship class, has 5 health, meaning it needs to be hit 5 times to be sunk).
 *
 *  Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/
#ifndef CARRIER_H
#define CARRIER_H
#include "ship.h"

class Carrier: public Ship{
public:
    Carrier();
    int health;
    bool hit;

    int getHealth();
    void getHit();
    bool isHit();
    bool isSunk();

    ~Carrier();
};

#endif // CARRIER_H
