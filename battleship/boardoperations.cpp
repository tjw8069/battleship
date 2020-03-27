/*-------------------------------------------------------------------------------------------------------
 *                                           boardOperations.cpp
 *
 * these are the operations that handle parts of the game play.
 * The fillBoard function is the meat of the backend of the program. It randomizes the location of
 * each ship object on the game board. It does this by picking a random direction (down or right), a random location
 * with in a limited range of values and testing to see if all the locations that would be part of
 * the ship are = to NULL. If they aren't, it picks a new location and tests that one.
 *
 * EX.
 *
 * n = NULL, s = Ship
 *
 * BEFORE SUB
 * IS PLACED
 *   1 2 3 4 5
 * A n n n n n     1)Function is placing submarine (takes up three coordinates).
 * B n m n n n     2)Function decides on vertical orientation.
 * C n n n n n     3)So the submarine doesn't go off the board, the lowest the base can be is the C row.
 * D n n s s n     4)Fucntion chooses B3 as base, checks two locations below and finds one has a ship.
 * E n n n n n     5)D3 is occupied by one part of a destroyer.
 * AFTER           6)Funciton repeats process, new direction is horizontal, columns are limited to
 *   1 2 3 4 5       3 at the greatest.
 * A n n n n n                7)New base is B2, checks all locatations to the right.
 * B n s s s n     8)All places equal NULL so it links these board spots to the submarine object.
 * C n n n n n     9)All this pertains only to the non-readable board
 * D n n s s n     10)B2 - B4 now contain the submarine
 * E n n n n n
 *
 *
 * There is also a readBoard, which contains the human readable information.
 * printBoard iterates through the human readable board and prints it including several formatting
 * points.
 * These functions are used in the playgame.cpp file.
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * ----------------------------------------------------------------------------------------------------*/

#include <iostream>
#include "ship.h"
#include "carrier.h"
#include "battleship.h"
#include "cruiser.h"
#include "submarine.h"
#include "destroyer.h"

using namespace std;

//void fillBoard(Ship ***board, Ship *carrier, Ship *battle, Ship *cruiser, Ship *sub, Ship *des);
//void printBoard(char board[10][10], Ship* carrier, Ship* battle, Ship* crusier, Ship* sub, Ship * des,
//                int turnCount);

void fillBoard (Ship ***board, Ship* carrier, Ship *battle, Ship *cruiser, Ship *sub, Ship *des){
    //carrier
    srand(time(NULL));
    int row = 0;
    int col = 0;

    //carrier: 1 total. 5 blocks
    int dir = rand() % 2;//decided dirction
    if (dir == 0){//vertical
        do{
            col = rand() % 10;
            row = rand() % 6;
        }while(board[row][col] != NULL || board[row + 1][col] != NULL || board[row + 2][col] != NULL ||
               board[row + 3][col] != NULL || board[row + 4][col] != NULL);
        for(int i = 0; i < 5; i++){
            board[row + i][col] = carrier;
        }
    }
    else{//horizontal
        do{
            col = rand() % 6;
            row = rand() % 10;
        }while(board[row][col] != NULL || board[row][col+ 1] != NULL || board[row][col + 2] != NULL ||
               board[row][col + 3] != NULL || board[row][col + 4] != NULL);
        for (int i = 0; i < 5; i++){
            board[row][col + i] = carrier;
        }
    }

    //battleship
    dir = rand() % 2;//decided dirction
    if (dir == 0){//vertical
        do{
            col = rand() % 10;
            row = rand() % 7;
        }while(board[row][col] != NULL || board[row + 1][col] != NULL || board[row + 2][col] != NULL ||
               board[row + 3][col] != NULL);
        for(int i = 0; i < 4; i++){
            board[row + i][col] = battle;
        }
    }
    else{//horizontal
        do{
            col = rand() % 7;
            row = rand() % 10;
        }while(board[row][col] != NULL || board[row][col+ 1] != NULL || board[row][col + 2] != NULL ||
               board[row][col + 3] != NULL);
        for (int i = 0; i < 4; i++){
            board[row][col + i] = battle;
        }
    }

    //Cruisers 3 total. 3 blocks each
    dir = rand() % 2;//decided dirction
    if (dir == 0){//vertical
        do{
            col = rand() % 10;
            row = rand() % 8;
        }while(board[row][col] != NULL || board[row + 1][col] != NULL || board[row + 2][col] != NULL);
        for(int i = 0; i < 3; i++){
            board[row + i][col] = cruiser;
        }
    }
    else{//horizontal
        do{
            col = rand() % 8;
            row = rand() % 10;
        }while(board[row][col] != NULL || board[row][col+ 1] != NULL || board[row][col + 2] != NULL);
        for (int i = 0; i < 3; i++){
            board[row][col + i] = cruiser;
        }
    }

    //Submarine
    dir = rand() % 2;
    if (dir == 0){//vertical
        do{
            col = rand() % 10;
            row = rand() % 8;
        }while(board[row][col] != NULL || board[row + 1][col] != NULL || board[row + 2][col] != NULL);
        for(int i = 0; i < 3; i++){
            board[row + i][col] = sub;
        }
    }
    else{//horizontal
        do{
            col = rand() % 8;
            row = rand() % 10;
        }while(board[row][col] != NULL || board[row][col+ 1] != NULL || board[row][col + 2] != NULL);
        for (int i = 0; i < 3; i++){
            board[row][col + i] = sub;
        }
    }

    //Destroyer
    dir = rand() % 2;
    if (dir == 0){//vertical
        do{
            col = rand() % 10;
            row = rand() % 9;
        }while(board[row][col] != NULL || board[row + 1][col] != NULL);
        for(int i = 0; i < 2; i++){
            board[row + i][col] = des;
        }
    }
    else{//horizontal
        do{
            col = rand() % 9;
            row = rand() % 10;
        }while(board[row][col] != NULL || board[row][col+ 1] != NULL);
        for (int i = 0; i < 2; i++){
            board[row][col + i] = des;
        }
    }
}

void printBoard(char board[10][10], Ship *carrier, Ship *battle, Ship *crusier, Ship *sub, Ship *des,
                int turnCount){

    //formatts the board makes it into a table
    //tracks his, misses, turns used and ships that have been sunk
    cout << "Turn: " << turnCount << endl << "  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 " << endl
         << "-------------------------------------------" << endl;

    for (int i = 0; i < 10; i++){
        cout <<  (char)(i + 65) << " | ";//prints a letter, conversion from int to char
        for(int x = 0; x < 10; x++){
            cout << board[i][x] << " | ";//human readable board
        }
        cout << endl << "-------------------------------------------" << endl;
    }

    if (carrier->isSunk() || battle->isSunk() || crusier->isSunk() || sub->isSunk() || des->isSunk()){
        //if a ship is sunk, find out which one and print them out with a bullet
        //\u2002 is a bulletpoint
        cout << "Sunk:" << endl;
        if (carrier->isSunk())
            cout << "\u2022 Carrier ";

        if (battle->isSunk())
            cout << "\u2022 Battleship ";

        if (crusier->isSunk())
            cout << "\u2022 Cruiser ";

        if (sub->isSunk())
            cout << "\u2022 Submarine ";

        if (des->isSunk())
            cout << "\u2022 Destroyer ";

        cout << endl;
    }else{
        //maintains formatting even if no ships are sunk
        cout << endl << endl;
    }
}
