#ifndef WALLET_H
#define WALLET_H

#include "database.h"

class Wallet
{
private:
    Database *db;
    int userId;

public:
    Wallet(Database *database, int uid);

    void checkBalance();
    void addMoney();
    void withdrawMoney();
};

#endif