/*----------------------------------------------------------------------------------------------------------------------
 *                                                 Destroyer.cpp
 *
 * Defines the necessary members.
 *
 * Defines all virtual fucntions.
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/

#include "destroyer.h"

Destroyer::Destroyer(){
    health = 2;
    damage = 0;
    hit = false;
}

int Destroyer::getHealth(){return health;}
void Destroyer::getHit(){damage++;}
bool Destroyer::isHit(){return false;}
bool Destroyer::isSunk(){
    if(health == damage){
        return true;
    }else{return false;}
}

Destroyer::~Destroyer(){}

