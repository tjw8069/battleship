/*----------------------------------------------------------------------------------------------------------------------
 *                                                 Submarine.cpp
 *
 * Defines the necessary members.
 *
 * Defines all virtual fucntions.
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/

#include "submarine.h"

Submarine::Submarine(){
    health = 3;
    damage = 0;
    hit = false;
}

int Submarine::getHealth(){return health;}
void Submarine::getHit(){damage++;}
bool Submarine::isHit(){return hit;}
bool Submarine::isSunk(){
    if(health == damage){
        return true;
    }else{return false;}
}

Submarine::~Submarine(){}
