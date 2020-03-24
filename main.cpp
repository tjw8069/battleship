/*----------------------------------------------------------------------------------------------------------------
 *                                                   Main()
 *
 * Welcomes user, tells them what the game is.
 * Checks file for past users/captains.
 * Displays their name and previous best scores.
 * Starts game (see playgame.cpp).
 * Compares the score of the game to the best previous score for the user.
 * If the score is lower, it is saved as the new best score and written into the file with the other users.
 *
 * File format for in/out file streams will always be
 * [number of users]
 * [user1]
 * [user2]
 * [user3]...
 * [user number of users]
 *
 * The number of users will always be scanned in first and will determine how many users are scanned in.
 *
 * Author: Tyler Winship tjw8060@g.rit.edu
 * Date: 8 DEC 2017
 * -------------------------------------------------------------------------------------------------------------*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include "ship.h"
#include "user.h"
#include "carrier.h"
#include "battleship.h"
#include "cruiser.h"
#include "submarine.h"
#include "destroyer.h"


using namespace std;

//prototype fuction. Definition in playgame.cpp
int playGame();

int main()
{
    //clears console screen starts printing from the top left (non portable solution)
    cout << "\033[2J" << "\033[1;1H";

    //Welcomes player, Tells them the goal of the game
    cout << "Welcome to Battleship!" << endl<< "Your goal is to eliminate the enemy ships in the fewest turns "
            "possible." << endl << "Good luck, Captain!" << endl <<
            endl;

    //declares variables
    fstream usersFile;
    vector <User*> users;
    int* numUsers = new int;
    User* currentUsr = NULL;
    int selection = 0;

    //opens the user file with infromation from previous games saved
    usersFile.open("battleShipUsers.txt", std::fstream::in | std::ios::binary);
    usersFile.seekg(0, usersFile.beg);
    if(usersFile.is_open()){//checks if file can be opened and has infromation in it

        //the file has information which will be read into the program.
        //tells the program the number of users saved in the file.
        usersFile.read((char*)numUsers, sizeof(int));

        //allocates memory for the Users in the vector users
        for (int i = 0; i < *numUsers; i++){
            users.push_back(new User);
        }

        //reads in data to the previously allocated memory
        for (int i = 0; i < *numUsers; i++){
            usersFile.read((char*) users[i], sizeof(User));
        }

        //This process allows the user to choose the right user, create users, and delete users
        while(currentUsr == NULL){//if currentUsr == NULL, no user has been selected yet

            //prompting user to select or alter the available users
            cout << "Which Captain are you?" << endl;
            for (int i = 0; i< *numUsers; i++){
                cout << i + 1 << ") User: " << users[i]->name << endl;
            }
            cout << "Enter the number before desired Captain to select it." << endl <<
                    "Enter -1 to add a Captain. Enter -2 to delete a Captain." << endl << endl <<
                    "Enter number: ";

            //scans in the options
            cin >> selection;
            if (selection <= *numUsers && selection > 0){
                //selects the correct user from the file, displays improtant information about it
                cout << endl << "You selected user number " << selection << "." << endl <<
                        "Capatain name: " << users[selection - 1]->name << endl <<
                        "Lowest number of turns: " << users[selection - 1]->leastTurns << endl;

                //sets the correct user from the file to the current user
                currentUsr = users[selection - 1];
            }

            //creating new user
            else if(selection == -1){
                cout << endl <<"Apointing a Captain: " << endl << endl <<
                        "Enter a name with no spaces that is less than 20 characters: ";
                string name;
                //allocating memory
                User* newUser = new User;
                cin >> name;

                //takes the input string and puts it in the name data member
                int i = 0;
                for (string::iterator it = name.begin(); it != name.end(), i < 20; it++, i++){
                    newUser->name[i] = *it;
                }
                cout << endl << "Captain Appointed:" << endl <<
                        "Name: " << newUser->name << endl;

                //puts new user into the vector so it can be written back with the others later
                users.push_back(newUser);

                selection = *numUsers; //for file writing. Need to set selection to new value

                //so the program will write the correct number of users back to the file later
                (*numUsers)++;

                //sets current user to the new user
                currentUsr = newUser;
            }

            //deleteing a usr
            else if(selection == -2){
                bool error = false;
                do{
                    int deleteUser = 0;

                    //prompts the user to be deleted
                    cout << "Enter the number of the user you would like to delete: ";
                    cin >> deleteUser;
                    if(deleteUser <= (*numUsers) && deleteUser > 0){//if the selection is valid

                        //deletes the selected data
                        delete users[deleteUser - 1];

                        //alters the vector to accomodate the change
                        users.erase(users.begin() + (deleteUser - 1));

                        //removing one from count
                        (*numUsers)--;
                    }
                    else{
                        //means the deleteUser value was bad
                        cerr << "Enter a valid number" << endl;

                        //makes the delete portion of the code loop through again after displaying the error
                        error = true;
                    }
                }while (error == true);
            }
            else{
                //bad selection value
                cerr << "Enter a valid number" << endl;
            }
        }
    }
    else{
        //if the file could not be opened or didn't have any information. This will happen on the first run.
        //Allows the user to make a new captain. This data will be saved and can be accesed later.
        //Works the same as the add user portion above
        cout << "Appointing a Captain: " << endl << endl <<
                "Enter a name with no spaces that is less than 20 characters: ";
        string name;
        User* newUser = new User;
        cin >> name;
        int i = 0;
        for (string::iterator it = name.begin(); it != name.end(), i < 20; it++, i++){
            newUser->name[i] = *it;
        }
        cout << endl << "Captain Appointed:" << endl <<
                "Name: " << newUser->name << endl;
        currentUsr = newUser;
        users.push_back(newUser);
        (*numUsers)++;
    }
    usersFile.close();//closes file all reading is done

    cout << endl <<
            "5 enemy ships are present in the grid. Sink them all as quickly as " << endl <<
            "possible! On the board, \"~\" is water, \"O\" is a miss, and \"X\" is a" << endl <<
            "hit! The 5 ships are a carrier (5 spaces), a battleship (4 spaces)," << endl <<
            "a cruiser (3 spaces), a submarine (3 spaces), and a destroyer (2 spaces)" << endl
            << endl <<
            "Good luck!" << endl;

    //plays the game, returns the number of turns required to win
    int turns = playGame();

    //if the new score is lower
    if(turns < currentUsr->leastTurns){
        //says the good new, changes current users value and the vector value of current user to write
        //it into the file later
        cout << "New best score! " << turns << " turns!" << endl << endl;
        currentUsr->leastTurns = turns;
        if(selection != 0){//if the user file is never opened, selection will equal zero
            //the opening was successfull so currentUsr is being copied over its originall self in
            //the vector to be written back later
            users[selection - 1]->leastTurns = currentUsr->leastTurns;
        }else{
            //if the file didn't open, there will only be one user and selection will not be accurate
            //becasue it was never used
            users[0]->leastTurns = currentUsr->leastTurns;
        }
    }else{
        //if the user didn't beat their previous score
        cout << "Best Score: " << currentUsr->leastTurns << endl <<
                "Turns Used: " << turns << endl << endl;
    }

    cout << "Thanks for Playing!" << endl << endl;

    //writes information back to file
    usersFile.open("battleShipUsers.txt", std::fstream::out | std::fstream::binary);
    if(usersFile.is_open()){
        usersFile.clear();
        usersFile.seekp(0, usersFile.beg);

        usersFile.write((char*) numUsers, sizeof(int));
        for (int i = 0; i < *numUsers; i++){
            usersFile.write((char*) users[i], sizeof(User));
        }
        usersFile.close();
    }else{
        cerr << "Data could not be saved" << endl;
    }

    //deletes allocated memory
    delete currentUsr;
    for(int i = 0; i < *numUsers; i++){
        delete users[i];
    }
    delete numUsers;
}
