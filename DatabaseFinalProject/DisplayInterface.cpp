#include "DisplayInterface.h"
#include "UserCheck.h"
#include <vector>

using namespace std;

string const connString = "host=localhost port=5432 dbname=finalproject user=postgres password=password";


/*  Helper string to prompt user */
string enterCMD = "Please enter a command, 0 to sign out.\n";

//identifies the current user
int currentUserID = 0;

//Shows a list of games that is currently within our inventory
void DisplayInterface::showGame() {
    string colNames[12] = {"SKU", "name", "release date", "date added to store", "rating", "genre", "price", "number of sales", "on Xbox", "on Playstation", "on Steam" "on Switch"};

    /* Response array format (just to provide a visual)
    [0]		[1]			[2]					[3]					[4]				[5]
    SKU		name	release date	date added to store		rating sum		rating count
    [6]			[7]				[8]			[9]				[10]			[11]			[12]			[13]
    genre	retail price	discount	number of sales		on Xbox		on Playstation		on Switch	on Steam
    */
	string query = "SELECT * FROM games";

	try {
		//Server connection
		connection conn(connString.c_str());
		work w(conn);

		result response = w.exec(query);

		//Print the column lables
		cout << left
		<< setw(5) << colNames[0]
		<< setw(30) << colNames[1]
		<< setw(11) << colNames[2]
		<< setw(11) << colNames[3]
		<< setw(4) << colNames[4]
		<< setw(15) << colNames[5]
		<< setw(10) << colNames[6]
		<< setw(7) << colNames[7]
		<< setw(7) << colNames[8]
		<< setw(7) << colNames[9]
		<< setw(7) << colNames[10]
		<< setw(7) << colNames[11] << endl;
		
		for(int i = 0; i < response.size(); i++) {
			double rating = response[i][4].as<double>() / response[i][4].as<double>();
			double price = response[i][7].as<double>() * (1 - (response[i][8].as<double>() / 100));

			cout << left
			<< setw(5) << response[i][0].as<int>()				//SKU
			<< setw(30) << response[i][1].as<string>()			//Title
			<< setw(11) << response[i][2].as<string>()			//Release date	not sure what to do with date
			<< setw(11) << response[i][3].as<string>()			//Added to store date	not sure what to do with date
			<< setw(4) << fixed << setprecision(2) << rating	//Rating
			<< setw(15) << response[i][6].as<string>()			//genre
			<< setw(10) << fixed << setprecision(2) << price	//price
			<< setw(7) << response[i][9].as<int>()				//number of sales
			<< setw(7) << response[i][10].as<bool>()			//on xbox
			<< setw(7) << response[i][11].as<bool>()			//on playstation
			<< setw(7) << response[i][12].as<bool>()			//on switch
			<< setw(7) << response[i][13].as<bool>() << endl;	//on steam
		}

		conn.close();	//close the connection
	}
	catch (exception e) {
		cout << e.what();
		exit(0);
	}
}

//Helper function that forces user to pick a value between min and max (inclusive)
double DisplayInterface::getUserChoice(double min, double max) {
    double c = -1;
    cin >> c;   //get choice

    while(c < min || c > max || !cin.good()) {  //if it's invalid, prompt user, clear, and try again.
        cout << "Error, invalid choice.\n";
        cout << "Please enter a number between " << min << " and " << max << " (inclusive).\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> c;
    }

    return c;
}

//Forces the user to pick a SKU number that exists in the data base.
//Used when user has to select a game to delete/purchase/rate/etc
//Modes:
//0 -   Asks user for input until they give a SKU# that the user owns ex: rating a game
//1 -   Asks user for input until they give a SKU# that is present in the games database ex: deleting a game, buying a game
//2 -   Asks user for input until they give a SKU# that is NOT present in the ganes database ex: creating a game
int DisplayInterface::getValidSKU(int mode) {
    int SKU = -1;
    vector<int> dbSKUs;
    
    bool validInput;
    string query;
    result response;

    //creates connection to database
    connection conn(connString.c_str());
    work w(conn);

    //get all SKU numbers in database that are options
    if(mode == 0) { //want owned games only
        //string query = "SELECT SKU FROM purchased_games WHERE ID = " + to_string(currentUserID);
        query = "SELECT games.SKU, name, platform FROM games INNER JOIN purchased_games ON (games.SKU = purchased_games.SKU) WHERE ID = " + to_string(currentUserID);
        response = w.exec(query);
        
        if (response.size() == 0) {
            cout << "Error, user doesn't own any games.\n";
            return -1;
        }

        //prints games user owns
        for (int i = 0; i < response.size(); i++) {
            cout << "SKU: " + response[i][0].as<string>() + ", title: " + response[i][1].as<string>() + ", platform: " + response[i][2].as<string>() << endl;
        }
    }
    else {  //not owned, can have any SKU# in the database
        query = "SELECT SKU FROM games";
        response = w.exec(query);

        if (response.size() == 0) {
            cout << "Error, no games exist in the database.\n";
            return -1;
        }

        //prints all games
        showGame(); //todo (skip for now) implement a new show game function with not as much info
    }

    for (int i = 0; i < response.size(); i++) {
        dbSKUs.push_back(response[i][0].as<int>());
    }
    
    validInput = false;
    while (!validInput) {
        if(mode == 2)
            cout << "Please enter a SKU # that is not currently present.\n";
        else
            cout << "Please select a SKU #.\n";

        SKU = getUserChoice(0, 999999); //SKU is a positive int with up to 6 digits. So up to 999,999

        //Check if SKU exists in the vector
        if (find(dbSKUs.begin(), dbSKUs.end(), SKU) != dbSKUs.end()) {   //SKU exists in the database
            if (mode != 2)   //if mode is not 2, we have found a valid SKU#
                validInput = true;
            else
                cout << "Error: SKU number exist in the database. Please enter a new SKU number.\n";
        }
        else {
            if(mode == 2)
                validInput = true;
            else
                cout << "Error: SKU number does not exist in the database. Please enter a new SKU number.\n";
        }
    }

    return SKU;
}

//Add game to be used by employees and managers
void DisplayInterface::addGame() {
    string gName, genre, retail_price, on_xbox, on_playstation, on_nswitch, on_steam;
    try { //Server connection
		connection conn(connString.c_str());
		work w(conn);

        //ask for info to add
        int SKU = getValidSKU(2);   //prompts user for a SKU# until they select one that is NOT present

        string query = "SELECT SKU FROM games WHERE SKU = " + to_string(SKU);

        result response = w.exec(query);

        if (response.size() != 0) {
            cout << "This SKU already exists in the system.\n";
            //throw;
        }
        else {//does not exist
            //gets game info with input validation to ensure queries are not passed as parameters
            cout << "Enter the name of the game: ";
            cin.ignore();
            getline(cin, gName);
            gName = w.quote(gName);

            cout << "Enter the genre: ";
            getline(cin, genre);
            genre = w.quote(genre);

            cout << "Enter the retail price: ";
            getline(cin, retail_price);
            retail_price = w.quote(retail_price);

            cout << "Is the game on Xbox? (true/false): ";
            getline(cin, on_xbox);
            on_xbox = w.quote(on_xbox);

            cout << "Is the game on Playstation? (true/false): ";
            getline(cin, on_playstation);
            on_playstation = w.quote(on_playstation);

            cout << "Is the game on Nintendo Switch? (true/false): ";
            getline(cin, on_nswitch);
            on_nswitch = w.quote(on_nswitch);

            cout << "Is the game on Steam? (true/false): ";
            getline(cin, on_steam);
            on_steam = w.quote(on_steam);

            query = "INSERT INTO games(SKU, name, genre, retail_price, on_Xbox, on_Playstation, on_Steam, on_NSwitch) VALUES(" + to_string(SKU) + ", " + gName + ", " + genre + ", " + to_string(retail_price) + ", " + to_string(on_xbox) + ", " + to_string(on_playstation) + ", " + to_string(on_steam) + ", " + to_string(on_nswitch) + ")";
            w.exec(query);
            w.commit();

            conn.close();	//close the connection
        }
    }
	catch (exception e) {
		cout << e.what();
		exit(0);
    }
}

//print response
void DisplayInterface::printResponse(result response) {
    for (int i = 0; i < response.size(); i++) {
        cout << "ID: " << response[i][0].as<string>() << ", ";
        for (int j = 1; j < response[i].size() - 1; j++) {
            if(!response[i][j].is_null())
                cout << response[i][j].as<string>() << ", ";
        }
        if(!response[i][response[i].size() - 1].is_null())
            cout << response[i][response[i].size() - 1].as<string>() << ".\n";
        else
            cout << ".\n";
    }
}

//Delete game; to be used by employees and managers
void DisplayInterface::deleteGame() {
    try { //Server connection
        connection conn(connString.c_str());
        work w(conn);

        //get a valid sku number (in this case, valid is any SKU# that exists in the database)
        int SKU = getValidSKU(1);

        //Make the query, execute, commit
        string query = "DELETE FROM games WHERE SKU = " + to_string(SKU);
        w.exec(query);
        w.commit();
        conn.close();	//close the connection
    }
	catch (exception e) {
		cout << e.what();
		exit(0);
    }
}

/* bans customer from the peoples table */
void DisplayInterface::banCustomer() {
    int ID;
    string query;
    result response;

    try { //Server connection
        connection conn(connString.c_str());
        work w(conn);
        
        cout << "Enter a customer ID you want to ban:\n";
        ID = getUserChoice(0, 999999);

        query = "SELECT ID FROM people WHERE ID = " + to_string(ID);
        response = w.exec(query);

        //checks to see if customer does not exist
        //todo, while loop for this stuff
        if (response.size() == 0) {
            cout << "Customer does not exist.\n";
            return;
        }

        //drop customer from people table
        query = "DELETE FROM people WHERE ID = " + to_string(ID);
        
        w.exec(query);
        w.commit();
        
        cout << "Customer has been banned!\n";
        conn.close();	//close the connection
    }
	catch (exception e) {
		cout << e.what();
		exit(0);
    }
}

/* UI */
//menuID == 0: customer, 1: employee, 2: manager
void DisplayInterface::displayMenu(int userID, int menuID) {
    currentUserID = userID; //current user is a global variable for the class

    int choice = -1;
    bool done = false;
    while (choice != 0) {
        switch(menuID) {
            case 0: //Customer
                cout << "**************************************************\n"
                << +"*      Welcome to the Game Store, Customer!      *\n"
                << +"**************************************************\n"
                << +"              1. Buy a game                       \n"
                << +"           2. Rate an owned game                  \n";
            //  << +"           3. View account info                   \n";
                cout << enterCMD;
                choice = getUserChoice(0, 2);
                customerChoice(choice);
                break;
            case 1: //Employee
                cout << "**************************************************\n"
                << +"*      Welcome to the Game Store, Employee       *\n"
                << +"**************************************************\n"
                << +"              1. Add Game                         \n"
                << +"              2. Delete Game                      \n"
                << +"              3. View Inventory                   \n"
                << +"              4. Ban Customer                     \n"
                << +"              5. View Customer Info               \n";
                cout << enterCMD;

                choice = getUserChoice(0, 5);
                employeeChoice(choice);
                break;
            case 2: //Manager
                cout << "**************************************************\n"
                << +"*      Welcome to the Game Store, Manager!       *\n"
                << +"**************************************************\n"
                << +"              1. Add Game                         \n"
                << +"              2. Delete Game                      \n"
                << +"              3. View Inventory                   \n"
                << +"              4. Ban Customer                     \n"
                << +"              5. Manage Employees                 \n"
                << +"              6. View Employee/Customer Info      \n";
                cout << enterCMD;

                choice = getUserChoice(0, 6);
                managerChoice(choice);
                break;
            default:
                cout << "This should never appear :( displayMenu default\n";
                exit(1);
        }
    }
}

/* gets choice for customer
Ideas:
Buy game (asks for game title, then platform, then checks funds before adding to purchased games table and incrementing library size)
Rate game (shows owned games and asks to pick one then set rating, update rating count/sum of game accordingly) (skip)
Add funds (just adds funds, assume payment processing is handled somewhere else) todo
*/
void DisplayInterface::customerChoice(int choice) {
    int SKU;
    string platform = "";
    string query = "";
    result response;
    int userSelection = 0;

    bool xbox;
    bool pstation;
    bool nswitch;
    bool steam;
    bool validInput;
    double price;
    int rating;

	try {
		//Server connection
		connection conn(connString.c_str());
		work w(conn);

        row wantedGame;

        switch (choice) {
            case 1: //Buy game
                SKU = getValidSKU(1);   //get a valid sku number
                
                query = "SELECT * FROM games WHERE SKU = " + to_string(SKU);

                //get info for the game
                wantedGame = w.exec1(query);

                //Have the user pick a platform
                xbox = wantedGame[10].as<bool>();;
                pstation = wantedGame[11].as<bool>();;
                nswitch = wantedGame[12].as<bool>();;
                steam = wantedGame[13].as<bool>();

                cout << "Please select a platform to purchase this game on.\n";
                cout << "Valid platforms:\n";
                //todo (skipped) insert the valid choices into a vector, then print i: vec.at(i). That way getUserChoice works better with it
                if(xbox)
                    cout << "1: Xbox\n";
                if(pstation)
                    cout << "2: Playstation\n";
                if(nswitch)
                    cout << "3: Nintendo Switch\n";
                if(steam)
                    cout << "4: Steam\n";

                validInput = false;
                while(!validInput) {
                    validInput = true;
                    userSelection = getUserChoice(1, 4);

                    if(userSelection == 1 && xbox)
                        platform = "xbox";
                    else if(userSelection == 2 && pstation)
                        platform = "playstation";
                    else if(userSelection == 3 && nswitch)
                        platform = "nswitch";
                    else if(userSelection == 4 && steam)
                        platform = "steam";
                    else {
                        cout << "Error, invalid selection.\n";
                        validInput = false;
                    }
                }

                query = "SELECT * FROM purchased_games WHERE SKU = " + to_string(SKU) + " AND ID = " + to_string(currentUserID) + "AND platform = '" + to_string(platform) + "'";
                response = w.exec(query);

                if(response.size() != 0) {
                    cout << "Error, you already own this game for that platform!\n";
                    break;
                }
                
                //todo: check if user has sufficient funds      (skip for now)
                //wg[7] is the retail price, wg[8] is the discount. 
                //Since wg[8] should be a %, divide it by 100. Then subtract that from 1 to get a decimal that represents the actual price.
                //Then multiply
                price = wantedGame[7].as<double>() * (1 - (wantedGame[8].as<double>() / 100));
                
                //todo: insert game into purchased_games table
                query = "INSERT INTO purchased_games(ID, SKU, platform, price_paid) VALUES(" + to_string(currentUserID) + ", " + to_string(SKU) + ", '" + to_string(platform) + "', " + to_string(price) + ")";
                w.exec(query);
                w.commit();
                
                cout << "Thank you for purchasing the game!\n";
                //todo: decrease user funds (skip)
                //todo: increment user library size (skip)
                break;
            case 2: //Rate game
                //get a valid sku number from user
                SKU = getValidSKU(1);

                //get platform from user
                //todo (skip) make getting platform a function
                query = "SELECT * FROM games WHERE SKU = " + to_string(SKU);

                //get info for the game
                wantedGame = w.exec1(query);
                xbox = wantedGame[10].as<bool>();;
                pstation = wantedGame[11].as<bool>();;
                nswitch = wantedGame[12].as<bool>();;
                steam = wantedGame[13].as<bool>();

                cout << "Please select a platform to rate this game on.\n";
                cout << "Valid platforms:\n";
                //todo (skipped) insert the valid choices into a vector, then print i: vec.at(i). That way getUserChoice works better with it
                if(xbox)
                    cout << "1: Xbox\n";
                if(pstation)
                    cout << "2: Playstation\n";
                if(nswitch)
                    cout << "3: Nintendo Switch\n";
                if(steam)
                    cout << "4: Steam\n";

                validInput = false;
                while(!validInput) {
                    validInput = true;
                    userSelection = getUserChoice(1, 4);

                    if(userSelection == 1 && xbox)
                        platform = "xbox";
                    else if(userSelection == 2 && pstation)
                        platform = "pstation";
                    else if(userSelection == 3 && nswitch)
                        platform = "nswitch";
                    else if(userSelection == 4 && steam)
                        platform = "steam";
                    else {
                        cout << "Error, invalid selection.\n";
                        validInput = false;
                    }
                }
                
                //get rating from user
                cout << "Please enter your rating, 0 to 5.\n";
                rating = getUserChoice(0, 5);

                //Finally, update the rating
                query = "UPDATE purchased_games SET rating = " + to_string(rating) + " WHERE SKU = " + to_string(SKU) + " AND ID = " + to_string(currentUserID) + " AND platform = '" + platform + "'";
                w.exec(query);
                w.commit();

                //todo: (skip) edit rating values in the game entry in games
                break;
           // case 3: //funds, skip
             //   break;
            case 0:
                cout << "Signing out...\n";
                exit(0);
                break;
            default:
                cout << "Please pick a valid option from 0 through todo\n";
                break;
        }

		conn.close();	//close the connection
    }
	catch (exception e) {
		cout << e.what();
		exit(0);
	}
}

/* gets choice for employee */
void DisplayInterface::employeeChoice(int choice) {
    //int employeeChoice;
    int SKU, ID;
    string gName, genre, retail_price, on_xbox, on_playstation, on_nswitch, on_steam;
    string query = "";
    result response;

    try {
        connection conn(connString.c_str());    //connection
        work w(conn);

        switch (choice) {
        case 1: //add a game to the games database
            addGame();
            break;
        case 2: //remove a game from the games database
            deleteGame();
            break;
        case 3:
            //view games in inventory
            showGame();
            break;
        case 4:
            banCustomer();  //ban customer by dropping them from the table
            break;
        case 5:
            query = "SELECT * FROM EmployeeView";
            response = w.exec(query);

            printResponse(response);
            break;
        case 0:
            cout << "Signing out...\n";

            conn.close();
            exit(0);
            break;
        default:
            break;
        }

        conn.close();
    }
    catch (exception e){
        cout << e.what() << endl;
        exit(0);
    }
}

void DisplayInterface::managerChoice(int choice) {
    string gName, genre, retail_price, on_xbox, on_playstation, on_nswitch, on_steam;
    int SKU;

    string fname, lname, address, phone_num, email;
    //int ID;

    string query = "";

    result response;
    int userChoice = 0;
    int ID, SSN, newOrCurrent;
    string is_manager;
    bool isCorrect = false;

    try {
		connection conn(connString.c_str()); //Server connection
		work w(conn);

        switch (choice) {
        case 1: //add game to the table
            addGame();
            break;
        case 2: //delete game from the table
            deleteGame();
            break;
        case 3: //show a list of the games in the table
            showGame();
            break;
        case 4: //ban customer (drop them from the table, should be added to banned customer table)
            banCustomer();
            break;
        case 5:
            //manage employees
            cout << "Would you like to remove or add an employee? (Enter 0 to remove, 1 to add): ";   
            userChoice = getUserChoice(0, 1);
        
            if(userChoice== 0) { //remove
                cout << "Enter the Employee ID you want to remove: ";
                cin.clear();
                ID = getUserChoice(0, 999999);
                
                query = "SELECT ID FROM employees WHERE ID = " + to_string(ID);
                response = w.exec(query); 

                if (response.size() == 0) {
                    cout << "Employee not found.\n";
                    break;
                }

                query = "DELETE FROM employees WHERE ID = " + to_string(ID);
                w.exec(query);
                w.commit();
            } else if (userChoice == 1){ //add
                cout << "Are they a current or new user?: (enter 0 for current or 1 for new)";
                newOrCurrent = getUserChoice(0, 1);

                //if current user
                if (newOrCurrent == 0) {
                    query = "SELECT * FROM ManagerView";
                    response = w.exec(query);

                    printResponse(response);

                    while (!isCorrect) {
                        cout << "Enter the ID of the person you wish to make an employee: ";
                        ID = getUserChoice(0, 999999);
                        
                        query = "SELECT ID FROM people WHERE ID = " + to_string(ID);
                        response = w.exec(query);

                        if (response.size() == 0) {
                            cout << "This ID is not in the table.\n";
                        }
                        else {
                            isCorrect = true;
                        }
                    }
                }
                else { //not current user
                    //get people information
                    UserCheck::createNewUser();

                    //re-asks for employee
                    while (!isCorrect) {
                        cout << "Enter the ID you have entered before: ";
                        ID = getUserChoice(0, 999999);

                        query = "SELECT ID FROM people WHERE ID = " + to_string(ID);
                        response = w.exec(query);

                        if (response.size() == 0) {
                            cout << "This ID is not in the table.\n";
                        }
                        else {
                            isCorrect = true;
                        }
                    }
                }

                //ask for employee info and insert and stuff
                cout << "Enter the SSN: ";
                SSN = getUserChoice(0, 999999999);
                
                query = "INSERT INTO employees(ID, SSN) VALUES(" + to_string(ID) +", " + to_string(SSN) + ")";
                w.exec(query);
                w.commit();
            }
            break;
        case 6:
            query = "SELECT * FROM ManagerView"; 
            response = w.exec(query);

            printResponse(response);
            break;
        case 0:
            cout << "Signing out...\n";
            conn.close();
            exit(0);
            break;
        default:
            break;
        }
        conn.close();	//close the connection
    }
	catch (exception e) {
		cout << e.what();
		exit(0);
	}
}