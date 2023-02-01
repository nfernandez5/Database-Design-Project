#include "UserCheck.h"
#include "DisplayInterface.h"

using namespace std;
using namespace pqxx;

//string for connection to database with credentials
string const connString = "host=localhost port=5432 dbname=finalproject user=postgres password=password";

/* function will handle whether the user wants to sign in or sign up, then check to see if they are in the database and output the proper interface */
void UserCheck::signInOrSignUp(string signInSignUp) {
    int ID = 0;
    string fname, lname, address, phone_num, email, username, password;

    for_each(signInSignUp.begin(), signInSignUp.end(), [](char& c) {
        c = ::tolower(c);
    });


    bool isUser = false;
    bool properPassword = false;
    if (signInSignUp == "sign up") {
        //code to send username and password to the database and create the user
        try {
            createNewUser();
            
            system("pause");
            system("cls");
            exit(1);
        }
        catch (exception e) {
            cout << e.what();
            exit(0);
        }
    }
    else if (signInSignUp == "sign in") {
        bool isUser = false;
        cout << "Please sign in." << endl;

        try {
            /* creates connection to the database */
            connection conn(connString.c_str());
            work w(conn);

            while (!isUser) {
                cout << "Enter your username: ";
                cin.ignore();
                getline(cin, username);
                username = w.quote(username);
                
                cout << "Enter your password: ";
                getline(cin, password);

                string query = "SELECT ID, password FROM people WHERE username = " + username;

                result res = w.exec(query);

                //res[0][1].as<string>() == password
                if (res.size() > 0 && res[0][1].as<string>().compare(password) == 0) {
                    isUser = true;

                    //gets user ID
                    int userID = res[0][0].as<int>();

                    //check if user is normal user, manager, or employee
                    query = "SELECT * FROM employees WHERE ID = " + to_string(userID);

                    result empCheck = w.exec(query);
                    if(empCheck.size() != 0) {
                        //user exists in employee table, check if they are a manager
                        row user = w.exec1(query);
                        if (user[6].as<bool>() == true) { //manager
                            DisplayInterface::displayMenu(userID, 2);
                        }
                        else if (user[6].as<bool>() == false) { //employee
                            DisplayInterface::displayMenu(userID, 1);
                        }
                    }
                    else {  //User is a customer
                        DisplayInterface::displayMenu(userID, 0);
                    }
                }
                else {
                    cout << "Please Enter Proper Credentials.\n";
                    system("pause");
                    system("cls");
                }
            }
        }
        catch (exception e) {
              cout << e.what();
              exit(0);
        }
    }
}

/* ensures that the user is entering a string password when creating an account */
bool UserCheck::checkStrongPassword(string& password) {
    auto n = password.length();

    // Checking lower alphabet in string
    bool hasLower = false, hasUpper = false, hasDigit = false, specialChar = false;
    string normalChars = "abcdefghijklmnopqrstu" "vwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";

    //checks the password character by character
    for (int i = 0; i < n; i++) {
        if (islower(password[i]))
            hasLower = true;
        if (isupper(password[i]))
            hasUpper = true;
        if (isdigit(password[i]))
            hasDigit = true;
        size_t special = password.find_first_not_of(normalChars);
        if (special != string::npos)
            specialChar = true;
    }

    //if the password is a strong password, true, otherwise false
    if (hasLower && hasUpper && hasDigit && specialChar)
        return true;
    else
        return false;
}

//creates a new user
void UserCheck::createNewUser() {
    string fname, lname, address, email, username, password;
    int ID;
    double phone_num;

    connection conn(connString.c_str());
    work w(conn);
    
    cout << "Please enter a username and password of your choice: " << endl;

    //Get username
    cout << "Username: ";
    cin.ignore();
    getline(cin, username);
    username = w.quote(username);   //Sanitize input

    //Get password
    cout << "Please Enter a Strong Password: ";
    getline(cin, password);
    password = w.quote(password);

    while (!UserCheck::checkStrongPassword(password)) {
        cout << "Your password must contain at least one digit, one special character, and one upper case letter.\n";
        system("pause");
        system("cls");
        cout << "Enter a strong password: ";
        getline(cin, password);
        password = w.quote(password);
    }

    cout << "Enter your first name, last name, phone number, desired ID (up to 4 numbers), address, and email\n";

    cout << "First name: ";
    cin.ignore();
    getline(cin, fname);
    fname = w.quote(fname);   //Sanitize input

    cout << "Last name: ";
    cin.clear();
    getline(cin, lname);
    lname = w.quote(lname);   //Sanitize input

    cout << "PhoneNumber: ";
    cin.clear();
    phone_num = DisplayInterface::getUserChoice(1000000000, 9999999999);

    cout << "ID: ";
    cin.clear();
    ID = DisplayInterface::getUserChoice(0, 999999);

    cout << "Address: ";
    cin.ignore();
    getline(cin, address);
    address = w.quote(address);   //Sanitize input

    cout << "Email: ";
    cin.clear();
    getline(cin, email);
    email = w.quote(email); //Sanitize input

    string query = "INSERT INTO people(ID, username, password, fname, lname, address, phone_num, email) VALUES("
        + to_string(ID) + ", " + username + ", " + to_string(password) + ", " + fname + ", " + lname + ", " + address + ", " + to_string(phone_num) + ", " + email + ");";

    result res = w.exec(query);
    w.commit();

    cout << "You are now signed up!\n";
    conn.close();
}