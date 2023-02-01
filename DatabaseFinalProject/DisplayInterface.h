#pragma once
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <pqxx/pqxx>
//#include "UserCheck.h"

using namespace std;
using namespace pqxx;

class DisplayInterface
{
public:
	//default constructor
	DisplayInterface() {}

	//displays interface for customer, employee, and manager
	static void displayMenu(int userID, int menuID);

	//chooses option for manager menu
	static void managerChoice(int choice);

	//chooses option for customer menu
	static void customerChoice(int choice);

	//chooses option for employee menu
	static void employeeChoice(int choice);

	//gets a valid number from the user between min and max
	static double getUserChoice(double min, double max);

	//gets and checks for a valid SKU number
	//what is "valid" depends on mode
	static int getValidSKU(int mode);

	//adds a video game to the database
	static void addGame();

	//deletes a video game to the database
	static void deleteGame();

	//bans a customer by dropping them from the table (trigger will insert them into banned customer table)
	static void banCustomer();

	//function to show all video games
    static void showGame();

	//prints contents of response from database
	static void printResponse(result response);
};

