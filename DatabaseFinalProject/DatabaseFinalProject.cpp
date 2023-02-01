// DatabaseProjectInterface.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <pqxx/pqxx>
#include "UserCheck.h"
#include "DisplayInterface.h"

using namespace std;
using namespace pqxx;

int main()
{
    string connString = "host=localhost port=5432 dbname=finalproject user=postgres password=password";
    string username = "";
    string password = "";
    string signInSignUp = "";
    int choice = -1;

    UserCheck user;

    //sign up or sign in
    cout << "Welcome to the Game Store!\n";
    cout << "Would you like to sign in or sign up?\n" << "1: sign in\n" << "2: sign up\n";
    choice = DisplayInterface::getUserChoice(1, 2);

    if(choice == 1)
        user.signInOrSignUp("sign in");
    else
        user.signInOrSignUp("sign up");


    //todo: implement this into the menu so that it will loop
}