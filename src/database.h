#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <mysql.h>

class Database
{
private:
    MYSQL *conn;

public:
    Database();
    ~Database();

    bool connect();

    void disconnect();

    MYSQL *getConnection();
};

#endif