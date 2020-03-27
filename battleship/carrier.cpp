/*----------------------------------------------------------------------------------------------------------------------
 *                                                 Carrier.cpp
 *
 * Defines the necessary members.
 *
 * Defines all virtual fucntions.
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/

#include "carrier.h"

Carrier::Carrier(){
    health = 5;
    damage = 0;
    hit = false;
}

int Carrier::getHealth(){return health;}
void Carrier::getHit(){damage++;}
bool Carrier::isHit(){return hit;}
bool Carrier::isSunk(){
    if (health == damage){
        return true;
    }else{return false;}
}

Carrier::~Carrier(){}

