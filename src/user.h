#ifndef USER_H
#define USER_H

#include "database.h"
#include <string>

using namespace std;

class User
{
private:
    Database *db;

public:
    int userId;
    string userName;

    User(Database *database);

    void registerUser();
    bool login();
    void viewProfile();
    void viewNotifications();
};

#endif