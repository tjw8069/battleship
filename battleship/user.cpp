/*---------------------------------------------------------------------------------------------------
 *                                                 User.cpp
 *
 * Defines the necessary members.
 *
 * Overloads the "<<" operator
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * ------------------------------------------------------------------------------------------------*/

#include "user.h"
#include <climits>
#include <iostream>

using namespace std;

User::User()
{
    for(int i = 0; i < 20; i++){//initializes to zero
        *(name + i) = 0;
    }
    leastTurns = INT_MAX;//from climits
}

void User::print(){
    cout << "Captain name: " << name << endl <<
            "Lowest number of turns: " << leastTurns << endl;
}

void User::print(int numUsers){//overloaded funciton
    if (numUsers == 0){
        cout << "Captain Appointed:" << endl <<
                "Name: " << name << endl;
    }else{
        cout << "Captain name: " << name << endl <<
                "Lowest number of turns: " << leastTurns << endl;
    }
}

std::ostream  &operator  << (std::ostream &out, User &user)//overloaded operator
{
    out << "Capatain name: " << user.name << endl <<
           "Lowest number of turns: " << user.leastTurns << "turns" << endl;
    return out;
}
