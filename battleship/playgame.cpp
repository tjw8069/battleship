/*------------------------------------------------------------------------------------------------------
 *                                          playgame.cpp
 *
 * This file handles all information related to game play.
 * It creates the game board--a triple Ship pointer.
 * I creates the ship object game peices.
 * It calls fill board to randomize the ships' locations.
 *
 * It goes on to prompt the user until all ships have been sunk. On every iteration,
 * the function gets the coordinate from the user, parses out the information, checks if there is
 * a ship at that location, then updates the backend board as well as the humman readable board.
 * The function prints this infromation using the printBord function.
 *
 * In the end, it returns an integer value of the number of turns that were used to sink all the
 * ships, this information may be stored in the user file if it is the lowest number of terms
 * (best score) the user has achieved.
 *
 * Author: Tyler Winship tjw8069@g.rit.edu
 * ---------------------------------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <cstdlib>
#include "ship.h"
#include "carrier.h"
#include "battleship.h"
#include "cruiser.h"
#include "submarine.h"
#include "destroyer.h"

using namespace std;

//prototypes
void fillBoard(Ship ***board, Ship *carrier, Ship *battle, Ship *cruiser, Ship *sub, Ship *des);
void printBoard(char board[10][10], Ship* carrier, Ship* battle, Ship* crusier, Ship* sub, Ship * des,
                int turnCount);

int playGame(){
    //make board
    //board is a triple Ship pointer containing 100 possible locations for the ships.
    Ship*** board;
    board = new Ship**[10];
    for(int i = 0; i < 10 ; i++){
        board[i] = new Ship*[10];
        //makes pointers NULL, this will be used to see if a ship has been hit
        for(int x = 0; x < 10; x++){
            board[i][x] = NULL;
        }
    }

    //create objects
    Ship* carrier = new Carrier;
    Ship* battle = new Battleship;
    Ship* cruiser = new Cruiser;
    Ship* sub = new Submarine;
    Ship* des = new Destroyer;

    //fill board: randomizes the locaiton of all the ships on the board
    //(see fill board function for specific implementation)
    fillBoard(board, carrier, battle, cruiser, sub, des);

    //this is the human readable board. it contains 100 characters
    // ~ = water
    // O = Miss
    // X = Hit
    char readBoard[10][10];
    for (int i = 0; i < 10; i++){
        for(int x = 0; x < 10; x++){
            //sets all board values to water
            readBoard[i][x] = '~';
        }
    }

    int turnCount = 1;
    cout << endl;
    //first printboard. shows orignal board
    printBoard(readBoard,carrier,battle,cruiser,sub,des,turnCount);

    //process repeats as long as any of the ships are not sunk
    while(!(carrier->isSunk() && battle->isSunk() && cruiser->isSunk() && sub->isSunk() && des->isSunk())){
        //prompts for the coordinate
        cout << "Enter a cordinate letter|number (no spaces): ";
        string input;
        //scans into a string
        cin >> input;
        bool err = false;//used to make sure the false infromation is not printed
        int row = 0;
        int col = 0;

        //handles coordinates a1 - j10. Not case sensitive
        //makes sure inputs are valid
        if(input.size() == 2 && !((input[0] > 'J' && input[0] < 'a') || input[0] > 'j')){
            for(string::iterator it = input.begin() ; it != input.end(); it++){
                //makes sure input is valid
                if(*it >= 'a' && *it <= 'j'){
                    row = (int) (*it - 97); //converts from character to integer
                }
                else if(*it >= 'A' && *it <= 'J'){
                    row = (int) (*it - 65);//converts from character to integer
                }
                else if(*it >= '1' && *it <= '9'){
                    col = (int) (*it - 49);//converts from character to integer
                }
           }
        }
        //handles corrdinates in column 10 which will always have a length of 3 as opposed to 2
        //makes sure inputs are valid
        else if(input.size() == 3 && input[1] == '1' && input[2] == '0' &&
                !((input[0] > 'J' && input[0] < 'a') || input[0] > 'j')){
            //column is always the same
            col = 9;
            //determines the row
            //checks for valid input, not case sensitive
            for(string::iterator it = input.begin() ; it != input.end(); it++){
                if(*it >= 'a' && *it <= 'j'){
                    row = (int) (*it - 97);
                    cout << row << endl;
                }
                else if(*it >= 'A' && *it <= 'J'){
                    row = (int) (*it - 65);
                }
            }
        }
        else{//bad input for coordinates
            //these manipulate the colsole to clear it then print from the top left
            cout << "\033[2J" << "\033[1;1H";
            cerr << "Enter a valid letter and number without any spaces between them" << endl;
            err = true; //makes the bad row and col don't have an effect on the game
        }

        if((!err) && readBoard[row][col] == '~'){
            //if there is a valid input
            //screen is cleared
            cout << "\033[2J" << "\033[1;1H";

            //if the coordinate has a ship (is not NULL)
            if(board[row][col] != NULL){
                //deal damage to ship
                board[row][col]->getHit();
                //chage location to character x for hit
                readBoard[row][col] = 'X';
                if(board[row][col]->isSunk()){
                    //if the ship has been sunk, say that
                    cout << "SUNK!" << endl;
                }
                else{
                    //if its a hit but not sunk yet
                    cout << "HIT!" << endl;
                }
            }else{
                //a miss
                cout << "Miss!" << endl;
                //change positon to O from ~
                readBoard[row][col] = 'O';
            }
            //turn is only incremented if the coordinate input is valid
            turnCount++;
        }else if((!err) && readBoard[row][col] != '~'){
            //makes sure the same location isn't picked twice.
            //this ensures ships will not have extra damage delt to them
            //and the turns are accurate

            //clears screen again
            cout << "\033[2J" << "\033[1;1H";
            cerr << "Select a tile that hasn't been chosen yet." << endl;
        }
        //prints the updated board (see boardoperations.cpp)
        printBoard(readBoard,carrier,battle,cruiser,sub,des,turnCount);
    }

    //delete the objects
    delete carrier;

    delete battle;

    delete cruiser;

    delete sub;

    delete des;

    //deleteing the board
    for(int i = 0; i < 10; i++){
        for (int x = 0; x < 10; x++){
            //setts all points to NULL becasue objects have already been deleted
            board[i][x] = NULL;
            //delete the lowest level pointers
            delete [] board[i][x];
        }
        delete [] board[i];//deletes double pointers
    }
    delete [] board;//deletes the triple pointers
    return turnCount;//returns the number of turns used to win the game
}
