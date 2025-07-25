 #include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>
using namespace std;

void manual(){
	cout << "\n---Welcome To Inventory Management System!---\n\n--Main Portal--\n\n1. Enter Creditionals\n0. Exit\n";
}
class Validate {
public:
	bool isString(string input) {
	    if (input.empty()) 
			return false;
	    for (char c : input) {
	        if (!isalpha(c)) 
	            return false;
	    }
	    return true;
	}
	string toLowerCase(string input) {
	    for (char &c : input) {
	        c = tolower(c);
	    }
	    return input;
	}
	string getValidatedString(string prompt) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);
            if (isString(input)) 
				return toLowerCase(input);
            else 
				cout << "Invalid input! Only letters are allowed. Even avoid spaces.\nTry again! ";
        }
    }
	bool isAlphaNum(string input) {
	    if (input.empty()) 
		return false;
	    for (char c : input) {
	        if (!isalpha(c) && !isdigit(c)) 
	            return false;
	    }
	    return true;
	}
	string getValidatedAlphaNum(string prompt) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);
            if (isAlphaNum(input))
				return toLowerCase(input);
            else 
				cout << "Invalid input! Only letters and numbers allowed.\nTry again! ";
        }
    }
	int getValidatedInt(string input) {
	    int val;
	    cout << input;
	    while (!(cin >> val)) {
	        cout << "Invalid input! only Integers(numbers) are allowed!\nTry again! ";
	        cin.clear();
	        cin.ignore(1000, '\n');
	    }
	    cin.ignore();
	    return val;
	}
	void checkReadFileOpen(ifstream &file, string filename) {
	    file.open(filename);
	    if (!file.is_open()) {
		    throw invalid_argument("Error: Unable to open file for reading: " + filename);
	    }
	}
	void checkWriteFileOpen(ofstream &file, string filename) {
		file.open(filename, ios::app);
	    if (!file.is_open()) {
	        throw invalid_argument("Error: Unable to open file for writing: " + filename);
	    }
	}
};
Validate v;
class Item {
public:
    string name, category, itemStatus;
    double price;
    int quantity, expiryDate, itemID;

    void set_Item() {
    	itemID = v.getValidatedInt("Enter Item ID: ");
    	name = v.getValidatedString("Enter Item Name: ");
    	category = v.getValidatedString("Enter Category: ");
        cout << "Enter Price: "; 
		while(!(cin>>price || price < 0)){
    		cout << "Invalid! Enter validiate (double or int) Item Price: ";
    		cin.clear();
			cin.ignore(1000, '\n');
		}
		cin.ignore();
		quantity = v.getValidatedInt("Enter Quantity: ");
        cout << "Enter Expiry Date (YYYYMMDD or '0' if N/A): "; 
	    while(!(cin >> expiryDate) || expiryDate < 00000000 || expiryDate > 30001231){
    		cout << "Invalid! Enter validiate (int) Item expiryDate (YYYYMMDD): ";
    		cin.clear();
			cin.ignore(1000, '\n');
		}
		cin.ignore();
	    update_itemStatus();
	}
	void update_itemStatus(){
		if (quantity > 0){
			itemStatus =  "InStock"; 
		} else {
			itemStatus = "OutOfStock";
		}
	}
	void display() {
    	cout << "\nItem Details\n";
		cout << "Item Name : " << name;
		cout << "\nItem ID: " << itemID;
		cout << "\nCategory: " << category;
		cout << "\nItem Price: " << price;
		cout << "\nQuantity in Stock: " << quantity;
		cout << "\nExpiry Date: " << expiryDate; 
		cout << "\nItem's Status: " << itemStatus << "\n";
    }  
};
class Inventory : public Item {
public:
	string iname, icategory, status;
	double iprice;
    int iquantity, iexpiry, id;	
	void loadfromstring(string line){
		stringstream ss(line);
	    char sep;
		ss >> iname >> sep >> id >> sep >> icategory >> sep >> iprice >> sep >> iquantity >> sep >> iexpiry >> sep >> status;
	}
	void displaystring(){
	    cout << "Item Name           |   " << iname << "\n";
		cout << "Item ID             |   " << id << "\n";
        cout << "Category            |   " << icategory << "\n";
        cout << "Item Price          |   " << iprice << "\n";
        cout << "Quantity in Stock   |   " << iquantity << "\n";
        cout << "Expiry Date         |   " << iexpiry << "\n";
        cout << "Item's Status       |   " << status << "\n";
		cout << "\n====================================================================\n";
	}
	void addItem(){
		set_Item();
		ifstream checkfile;
		v.checkReadFileOpen(checkfile, "Inventory.txt");
	    string data;
	    bool duplicateFound = false;

	    while (getline(checkfile, data)) {
	    	loadfromstring(data);
	    	if (id == itemID || iname == name){
	    		duplicateFound = true;
	            break;
			}
		}
		checkfile.close();
		if (duplicateFound) {
	    	int choice;
	        cout << "\nItem already exists with same Name or ID.\n";
	        cout << "Do you want to update it instead? (1-yes)/(0-no)): ";
	        choice = v.getValidatedInt("\n\nEnter Choice: ");
			if (choice == 1) {
		        updateItem(name);
		    } else {
		        cout << "Item not added.\n";
	        }
	        return;
		}
		try {
	        ofstream myfile;
			v.checkWriteFileOpen(myfile, "Inventory.txt");
	        myfile << name <<" | "<< itemID <<" | " << category << " | " << price <<" | " << quantity << " | " << expiryDate << " | " << itemStatus << "\n";
	        myfile.close();
	        cout << "Item Added!\n";
		} catch (invalid_argument e) {
	        cout << e.what() << endl;
	   	}
	}
	void displayInventory(){
		try {
			int n=1;
		    ifstream myfile;
			v.checkReadFileOpen(myfile, "Inventory.txt");
		    string data;
		    cout << "\n\n----- Listing All the Items in Inventory -----\n\n";
		    cout << "\n====================================================================\n";
		    while(getline(myfile, data)){
		        loadfromstring(data);
		        cout << "\n=====Item " << n << " Details=====\t\n";
		        displaystring();
		        n++;
			}
			myfile.close();
		} catch (invalid_argument e){
			cerr << e.what() << endl;
		}
	}
	void searchitem(string itemName){
		bool found = false;
		string data;
		try {
        ifstream myfile;
        v.checkReadFileOpen(myfile, "Inventory.txt");
        while(getline(myfile, data)){
        	loadfromstring(data);
        	if(iname == itemName){
				cout << "\n=======Item Named " << itemName << " found in stock==========\n\n";
		        displaystring();
		        found = true;
			}
		}
		if (!found){
			cout << "Item not founn in stock! Double check your entry-!\n\n";
		}
		} catch (invalid_argument e){
			cerr << e.what() << endl;
		}
	}
	void updateItem(string itemName) {
	    bool found = false;
		string line;		
		int choice;
	try{
		ifstream myfile;
		v.checkReadFileOpen(myfile, "Inventory.txt");
        ofstream shifting;
        v.checkWriteFileOpen(shifting, "Inventory_check.txt");
        while (getline(myfile, line)){
        	loadfromstring(line);
        	if(iname == itemName){
				found = true;
	            cout << "\nItem Found: " << itemName << "\n";
	            cout << "0. Exit\n";
	            cout << "1. Update Quantity\n";
	            cout << "2. Update Price\n";
	            cout << "3. Update Expiry Date\n";
	            cout << "4. Update All\n";
	            cout << "Enter your choice: ";
	            while(true){
	            	choice = v.getValidatedInt("Enter your choice: ");
					if (choice == 0){
						cout << "We are out without Updating\n";
						break;
					}
					if (choice == 1 || choice == 4) {
		                iquantity = v.getValidatedInt("Enter new quantity: ");
	            	}
	            	if (choice == 2 || choice == 4) {
		                cout << "Enter new price: ";
		                while (!(cin >> price)) {
		                    cout << "Invalid input! Enter a valid +ve (double | int )price: ";
		                    cin.clear();
		                    cin.ignore(1000, '\n');
		                }
		            }
		            if (choice == 3 || choice == 4) {
		                cout << "Enter new expiry date (YYYYMMDD): ";
		                while (!(cin >> iexpiry) || iexpiry < 00000000 || iexpiry > 30001231){
		                	cout << "Invalid! Please enter a valid expiry date in YYYYMMDD format: ";
		                    cin.clear();
		                    cin.ignore(1000, '\n');
						}
					}
					if (choice >= 1 && choice <= 4) {
				        break; 
				    } else {
				        cout << "Invalid choice!\nTry again!\n";
				    }
				}
				if(iquantity>0) 
					status =  "InStock"; 
				else 
					status = "OutOfStock";
				cout << "Item updated successfully!\n";
			}
		shifting  << iname << " | " << id << " | " << icategory << " | " << iprice << " | " << iquantity << " | " << iexpiry << " | " << status << "\n";
		}
		myfile.close();
		shifting.close();
		remove("Inventory.txt");
		rename("Inventory_check.txt", "Inventory.txt");
		if (!found){
			cout << "\nItem Not found!\n";
		}
	} catch (invalid_argument e){
			cerr << e.what() << endl;
		}
	}
	void removeItem(string itemName){
		bool found = false;
		string data;
		try {
	        ifstream myfile;
			v.checkReadFileOpen(myfile, "Inventory.txt");
	        ofstream shifting;
	        v.checkWriteFileOpen(shifting, "Inventory_check.txt");
	        while(getline(myfile, data)){
	        	loadfromstring(data);
	        	if(iname != itemName){
					shifting << iname << " | " << id << " | " << icategory << " | " << iprice << " | " << iquantity << " | " << iexpiry << " | " << status << "\n";
				}
				else{
					found = true;
					cout << "\nItem named "<< itemName <<" Removed from stock.\n";
				}
			}
			myfile.close();
			shifting.close();
			remove("Inventory.txt");
			rename("Inventory_check.txt", "Inventory.txt");
			if (!found){
				cout << "\nItem Not found!\n";
			}
		} catch (invalid_argument e){
				cerr << e.what() << endl;
		}
	}
	void removeItem(int currentDate){
		bool found = false;
		string data;
		try {
        	ifstream myfile;
			v.checkReadFileOpen(myfile, "Inventory.txt");
	        ofstream shifting;
	        v.checkWriteFileOpen(shifting, "Inventory_check.txt");
	        while(getline(myfile, data)){
	        	loadfromstring(data);
				if((iexpiry > currentDate) || (iexpiry == 0)){
					shifting << iname << " | " << id << " | " << icategory << " | " << iprice << " | " << iquantity << " | " << iexpiry << " | " << status << "\n";
				}
				else{
					found = true;
					cout << "\nItems named "<< iname <<" with expiry date "<< iexpiry <<" on or before " << currentDate << " have been removed from stock.\n";
				}
			}
			myfile.close();
			shifting.close();
			remove("Inventory.txt");
			rename("Inventory_check.txt", "Inventory.txt");
			if (!found) {
	            cout << "\nNo expired items found!\n";
	        }
		} catch (invalid_argument e){
			cerr << e.what() << endl;
		}
	}
};
class Management{
	Validate v;
public:
	string userName, userPassword, userRole;
	string n, p, r;
	void loadtostring(string line){
		stringstream s(line);
		char a;
		s >> r >> a >> n >> a >> p;
	}
	void set_user(){
		userName = v.getValidatedString("Enter User Name: ");
		userPassword = v.getValidatedAlphaNum("Enter password(alphanumeric): ");
	    userRole = v.getValidatedString("Enter Role: ");
	}
	void registerUser(){
		try{
			set_user();
			ofstream myfile;
			v.checkWriteFileOpen(myfile, "User.txt");
			myfile << userRole << " : " << userName << " : " << userPassword << "\n";
			cout << "\nUser Registered Successfully!\n";
		} catch (invalid_argument e){
			cerr << e.what() << endl;
		} 
	}
	void displayUsers(){
		try {
        ifstream myfile;
        v.checkReadFileOpen(myfile, "User.txt");
        string data;
        cout << "\n\n-----Listing All the Users of Inventory Management!-----\n\n";
		cout << "Role | Name | Password\n\n";
        while(getline(myfile, data)){
        	cout << data << endl;
		}
		cout << "\n====================================================================\n";
		myfile.close();
		} catch (invalid_argument e){
			cerr << e.what() << endl;
		}
	}
	void removeUser(string name){
	try{
		bool found = false;
		ifstream myfile;
        v.checkReadFileOpen(myfile, "User.txt");
        ofstream shifting;
        v.checkWriteFileOpen(shifting, "User_check.txt");
        string data;
        while(getline(myfile, data)){
			loadtostring(data);
			if(n != name){
				shifting << data << "\n";
			}
			else{
				found = true;
				cout << "User named " << name << " removed!" << endl;
			}
		}
		myfile.close();
		shifting.close();
		remove("User.txt");
		rename("User_check.txt", "User.txt");
		if (!found) {
		    cout << "\nNo User named " << name << " found!\n";
	    }
	} catch (invalid_argument e){
		cerr << e.what() << endl;
		}
	}
	string verifyuser(string userName, string password){
	try{
		ifstream myfile;
        v.checkReadFileOpen(myfile, "User.txt");
		string data;
		while(getline(myfile, data)){
			loadtostring(data);
			if((n == userName) && (p == password)){
				cout << "Role: " << r << "\nName: " << n << "\nPassword: " << p << endl;
				return r;
			}
		}
		return "";
	} catch (invalid_argument e){
			cerr << e.what() << endl;
		}
	}
	void admin_manual(){
		cout << "\n--- Admin Portal ---";
		cout << "\n\n--Select choice for respective CRUD Operation--\n\n";
		cout << "0. Exit from Portal\n";
		cout << "1. Add Item To Inventory\n";
		cout << "2. Remove Specific Item from Inventory\n";
		cout << "3. Search Specific Item\n";
		cout << "4. Remove Expired Items from Inventory with CurrentDate(DDMMYYYY)\n";
		cout << "5. Update Specific Item's Price & Quantity or Expiry Date\n";
		cout << "6. List of All Items in Inventory\n";
		cout << "\n\n-------Staff Management------\n";
		cout << "7. Register New Staff User\n";
		cout << "8. Remove Staff User\n";
		cout << "9. Listing all the User's Details\n";
	}
	void User_manual(){
		cout << "\n--- Staff Portal ---";
		cout << "\n--select choice for respective operation--\n\n";
		cout << "0. Exist Program\n";
		cout << "1. Search Item in Inventory\n";
		cout << "2. List All Inventory\n";
	}	
};
int main(){
	Inventory it;
	Management m;
	Validate v;
	string Item_Name, Item_ID, username, Passcode;
	int choice, ch, date;
	
	while(true){
	    manual();
        ch = v.getValidatedInt("\n\nEnter Choice: ");
        cout << "\n\n";
		if (ch == 0) {
            cout << "Exiting Program!\n";
            break;
        }
        else if(ch == 1){
       	username = v.getValidatedString("Enter your name: ");
        Passcode = v.getValidatedAlphaNum("Enter your password(alphanumeric): ");

	    string usertype = m.verifyuser(username, Passcode);
	    if(usertype == "admin"){
			while(true){
				m.admin_manual();
				choice = v.getValidatedInt("\n\nEnter Choice: ");
				cout << "\n\n";
				if (choice == 0) {
					cout << "Existing Program!\n\n";
			        break;
			    }
			    else{
			        switch(choice){
					case 1:	
						it.addItem();
						break;
					case 2:
						Item_Name = v.getValidatedString("enter item name you want to remove: ");
					    it.removeItem(Item_Name);
					    break;
					case 3:
						Item_Name = v.getValidatedString("enter item name to be searched: ");
					    it.searchitem(Item_Name);
					    break;
					case 4:
						date = v.getValidatedInt("Enter Current Date (YYYYMMDD) to remove Expire Items: ");
						it.removeItem(date);
						break;
					case 5:
						Item_Name = v.getValidatedString("enter item name to be Updated: ");
					    it.updateItem(Item_Name);
					    break;
						break;
					case 6:
						it.displayInventory();
						break;
					case 7:
						m.registerUser();
						break;
					case 8:
						username = v.getValidatedString("Enter User name to remove: ");
					    m.removeUser(username);
					    break;
					case 9:
						m.displayUsers();
						break;
					default:
						cout << "\nInvalid Choice!\n\n";
						break;
		}  }	}	}
		else if(usertype == "staff"){
				while(true){
					m.User_manual();
					choice = v.getValidatedInt("\n\nEnter Choice: ");
				if (choice == 0) {
						cout << "Existing Program!\n\n";
						break;
				    }
					switch(choice){
						case 1:
							Item_Name = v.getValidatedString("enter item name to be searched: ");
						    it.searchitem(Item_Name);
						    break;
						case 2:
							it.displayInventory();
							break;
						default:
							cout << "\nInvalid Choice!\n\n";
							break;
				}	}	}
				else {
					cout << "\nWrong Creditionals! \n--Access Denied--\n";
				}
		}
        else {
        	cout << "\nInvalid Choice! Select between(0-1)\n";
		}
	}
	return 0;
}
