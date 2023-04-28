#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include "treemm.h"

class User;

class UserDatabase
{
  public:
    UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:
      bool first_time; 
      TreeMultimap<std::string, User> m_user_tree;
      
};

#endif // USERDATABASE_INCLUDED
