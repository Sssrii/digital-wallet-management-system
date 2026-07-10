#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "database.h"

class Transaction
{
private:
    Database *db;
    int userId;

public:
    Transaction(Database *database, int uid);

    void transferMoney();
    void viewHistory();
};

#endif