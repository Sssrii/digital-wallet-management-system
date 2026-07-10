#ifndef ADMIN_H
#define ADMIN_H

#include "database.h"

class Admin
{
private:
    Database *db;

public:
    Admin(Database *database);

    void viewAllUsers();
    void blockUser();
    void unblockUser();
    void viewAllTransactions();
};

#endif