/*----------------------------------------------------------------------------------------------------------------------
 *                                                 Submarine Class
 *
 * Also has three health.
 * Derived from ship class
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/

#ifndef SUBMARINE_H
#define SUBMARINE_H
#include "ship.h"

class Submarine: public Ship{
public:
    Submarine();
    int health;
    bool hit;

    int getHealth();
    void getHit();
    bool isHit();
    bool isSunk();

    ~Submarine();
};

#endif // SUBMARINE_H
