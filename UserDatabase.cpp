#include "UserDatabase.h"
#include "User.h"
#include <iostream>
#include <fstream>

#include <string>
#include <vector>
using namespace std;

UserDatabase::UserDatabase()
    : first_time(true)
{
    // Replace this line with correct code.
}

bool UserDatabase::load(const string& filename)
{
	if (!first_time)
		return false;

	ifstream infile(filename);    
	if (!infile)		        
	{
		cerr << "Error: Cannot open data.txt!" << endl;
		return false;
	}
	
	int lineNumber = 1;
	string name = "";
	string email = "";
	vector<string> watch_history;

	string line;
	while (getline(infile, line))
	{
		if (!line.empty())
		{
			if (lineNumber == 1) { name = line; }

			else if (lineNumber == 2) { email = line; }

			else if (lineNumber == 3) { ; } // number of movies in watch history, no need to save 

			else { watch_history.push_back(line); }

			lineNumber++;
		}
		else // seperator
		{
			User entry(name, email, watch_history);
			m_user_tree.insert(email, entry);

			// reset line number and watch history vector
			lineNumber = 1;
			watch_history.clear();
		}
	}

	// to account for the text file not ending with an empty line
	// this will ensure the last movie got added to the tree
	if (lineNumber != 1)
	{
		User entry(name, email, watch_history);
		m_user_tree.insert(email, entry);

		// reset line number and watch history vector
		lineNumber = 1;
		watch_history.clear();
	}

	first_time = false;
	return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
	TreeMultimap<std::string, User>::Iterator it = m_user_tree.find(email);
	if (it.is_valid())
	{
		User& found_user = it.get_value();
		return &found_user;
	}
	else
	{
		return nullptr;
	}
}
