/*----------------------------------------------------------------------------------------------------------------------
 *                                                 Battleship.cpp
 *
 * Defines the necessary members.
 *
 * Defines all virtual fucntions.
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/

#include "battleship.h"

Battleship::Battleship(){
    health = 4;
    damage = 0;
    hit = false;
}

int Battleship::getHealth(){return health;}
void Battleship::getHit(){damage++;}
bool Battleship::isHit(){return hit;}
bool Battleship::isSunk(){
    if(health == damage){
        return true;
    }else{return false;}
}

Battleship::~Battleship(){}
