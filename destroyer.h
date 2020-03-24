#ifndef DESTROYER_H
#define DESTROYER_H
#include "ship.h"

class Destroyer:public Ship{
public:
    Destroyer();
    int health;
    bool hit;

    int getHealth();
    void getHit();
    bool isHit();
    bool isSunk();

    ~Destroyer();
};

#endif // DESTROYER_H
