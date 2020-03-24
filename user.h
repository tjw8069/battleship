/*----------------------------------------------------------------------------------------------------------------------
 *                                                 User Class
 *
 * Contains the infromation that will be written to the file to track the people who have played the game
 *
 * name: username for the player
 * leastTurns: the best score the player has achieve in any game they have played
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * -------------------------------------------------------------------------------------------------------------------*/

#ifndef USER_H
#define USER_H

#include <iomanip>
#include <fstream>

class User
{
public:
    User();
    char name[20];
    int leastTurns;
    void print();
    void print(int numUsers); //overloaded function
    friend std::ostream &operator << (std::ostream &out, User &user); //overloaded operator
};

#endif // USER_H
