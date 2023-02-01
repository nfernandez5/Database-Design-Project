#pragma once
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

//class handles all user related methods and variables
class UserCheck
{
private: //Login Information
	string firstName = "";
	string lastName = "";
	string username = "";
	string password = "";
	string email = "";
	string address = "";
	int phoneNum = 0;
	bool login = true;
	int userID = 0;

public:
	//default constructor
	UserCheck() {};
	//deconstructor
	~UserCheck() {};

	//paramterized constructor
	UserCheck(string username, string password) {
		this->username = username;
		this->password = password;
	}

	//getters
	string getF_Name() { return firstName; };
	string getL_Name() { return lastName; };
	string getUser_Name() { return username; };
	string getPassword() { return password; };
	string getEmail() { return email; };
	string getAddress() { return address; };
	int getPhone_Num() { return phoneNum; };
	int getUserID() { return userID; }

	//setters
	void setF_Name(string firstName) { this->firstName = firstName; };
	void setL_Name(string lastName) { this->lastName = lastName; };
	void setUser_Name(string username) { this->username = username; };
	void setPassword(int password) { this->password = password; };
	void setEmail(string email) { this->email = email; };
	void setAddress(string address) { this->address = address; };
	void setPhone_Num(int phoneNum) { this->phoneNum = phoneNum; };
	void setUserID(int userID) { this->userID = userID; };

	static void signInOrSignUp(string signInOrSignUp);

	//bool checkUser(string username, string password);

	static bool checkStrongPassword(string& password);

	static void createNewUser();
};

