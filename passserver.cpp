#include "passserver.h"
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <crypt.h>
#include <cstring>
#include <algorithm>
#include <functional>
#include <utility>
#include <unistd.h>


//1 param constructor
PassServer::PassServer(size_t size) : server(size)
{}

// destructor
PassServer::~PassServer()
{
	server.clear();
}

// loads content from a file into a hash table
bool PassServer::load(const char* filename)
{
	std::ifstream is;
	string username, password;
	is.clear();

	is.open(filename);
	
	if(!is)
		return false;

	while(!is.eof())
	{
		is >> username >> password;

		std::pair<string, string> kv(username, encrypt(password));

		if(!is.fail())
			server.insert(kv);
	}
	
	is.close();

	return true;
}


// adds a user element to the hash table
bool PassServer::addUser(std::pair<string, string>& kv)
{
	std::pair<string, string> temp(kv.first, encrypt(kv.second));

	return (server.insert(temp));
}

// move version of adding a user
bool PassServer::addUser(std::pair<string, string>&& kv)
{
	std::pair<string, string> temp(std::move(kv.first), encrypt(kv.second));

	return (server.insert(temp));
}

//removes a user element from the hash table 
bool PassServer::removeUser(const string& k)
{
	return (server.remove(k));
}

// changes an existing users password
bool PassServer::changePassword(const pair<string, string>& p, const string& newpassword)
{
	std::pair<string, string> temp(p.first, encrypt(p.second));
	
	if (server.match(temp) && encrypt(p.second) != encrypt(newpassword))
	{
		std::pair<string, string> temp(p.first, encrypt(newpassword));
		server.insert(temp);
		return true;
	}

	return false;

}

// finds a user element if it exists in the hash table 
bool PassServer::find(const string& user)
{
	return (server.contains(user));
}

// displays hash table elements 
void PassServer::dump()
{
	server.dump();
}

// returns the size of the hash table
size_t PassServer::size()
{
	return(server.size());
}

//writes data to a file
bool PassServer::write_to_file(const char* filename)
{
	return (server.write_to_file(filename));
}

//encrypts a users passwords 
string PassServer::encrypt(const string& str)
{
	char salt[] = "$1$########";
	char *password = new char [100];
	strcpy( password, crypt(str.c_str(), salt));
	string np(password);
	return np.substr(12, 22);
}
