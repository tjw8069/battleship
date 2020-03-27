/*-------------------------------------------------------------------------------------------------------------------------
 *                                                     Ship class
 *
 * Abstract class that is the base of all the ships in the game.
 * virtual fucntions allow Ship pointers to access data in the derived classes.
 * This means all the pointers can be ship pointers
 *
 *  Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/

#ifndef SHIPS_H
#define SHIPS_H


class Ship
{
public:
    int damage;
    virtual int getHealth() = 0;
    virtual void getHit() = 0;
    virtual bool isHit() = 0;
    virtual bool isSunk() = 0;

    virtual ~Ship() = 0;
};

#endif // SHIPS_H
