#include <iostream>
#include <string>
#include <utility>
#include <sstream>
#include "passserver.h"
using namespace std;
using namespace cop4530;

void Menu();

int main(int argc, const char * argv[]){

string size, choice, username, password, newpassword;
pair<string, string> kv;

string filename;
cout << "Enter preferred hash table capacity: ";
getline(cin, size); 
cout << '\n' << size << '\n';
PassServer server(atoi(size.c_str()));

Menu();

while (getline(cin, choice))
{

	if (choice == "x") 
		break;
	if (choice == "l")
	{
		cout << "Enter file name to load from: ";
		cin >> filename;
		const char* cstr = filename.c_str();
		if (!server.load(cstr))
			cout << "Error: could not load from " << filename << '.';
		cin.ignore();
	}

	else if (choice == "a")
	{
		cout << "Enter username: ";
		cin >> username; 
		cout << "Enter password: ";
		cin >> password;
		kv = make_pair(username, password);

		if(server.addUser(kv))
			cout << "\nUser " << username << " added";
		else 
			cout << "Error: user already exists.";
		cin.ignore();
	}

	else if (choice == "r")
	{
		cout << "Enter username: ";
		cin >> username;
		
		if(server.removeUser(username))
			cout << "\nUser " << username << " removed";
		else
			cout << "Error: user does not exist.";
		cin.ignore();
	}

	else if (choice == "c")
	{
		cout << "Enter username: ";
		cin >> username;
		cout << "Enter password: ";
		cin >> password; 
		cout << "Enter new password: ";
		cin >> newpassword;
		kv = make_pair(username, password);

		if (server.changePassword(kv, newpassword))	
			cout << '\n' << username << "'s password has been changed.";
		else
			cout << "Error: could not change" << username << "'s password";
		cin.ignore();
	}

	else if (choice == "f")
	{
		cout << "Enter username: ";
		cin >> username;

		if (server.find(username))
			cout << "\nUser " << username << " found";
		else
			cout << "Error: user not found";
		cin.ignore();
	}

	else if (choice == "d")
	{
		cout << '\n';
		server.dump();
	}

	else if (choice == "s")
	{
		cout << "Size of hash table is " << server.size();
	}

	else if (choice == "w")
	{
		cout << "Enter file name to write to: ";
		cin >> filename;
		const char* cstr = filename.c_str();
		if(!server.write_to_file(cstr))
			cout << "Error: could not right to file " << filename;
		cin.ignore();
	}
	
	else
		cout << "Error: invalid menu entry";
	
	
	Menu();
}

	return 0; 
}


void Menu()
{
  cout << "\n\n";
  cout << "l - Load From File" << endl;
  cout << "a - Add User" << endl;
  cout << "r - Remove User" << endl;
  cout << "c - Change User Password" << endl;
  cout << "f - Find User" << endl;
  cout << "d - Dump HashTable" << endl;
  cout << "s - HashTable Size" << endl;
  cout << "w - Write to Password File" << endl;
  cout << "x - Exit program" << endl;
  cout << "\nEnter choice : ";
}
