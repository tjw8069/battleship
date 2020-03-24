/*----------------------------------------------------------------------------------------------------------------------
 *                                                 Cruiser.cpp
 *
 * Defines the necessary members.
 *
 * Defines all virtual fucntions.
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/

#include "cruiser.h"

Cruiser::Cruiser(){
    health = 3;
    damage = 0;
    hit = false;
}

int Cruiser::getHealth(){return health;}
void Cruiser::getHit(){damage++;}
bool Cruiser::isHit(){return hit;}
bool Cruiser::isSunk(){
    if(health == damage){
        return true;
    }else{return false;}
}

Cruiser::~Cruiser(){}
