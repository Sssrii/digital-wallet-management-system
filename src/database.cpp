#include "database.h"

Database::Database()
{
    conn = nullptr;
}

Database::~Database()
{
    disconnect();
}

bool Database::connect()
{
    conn = mysql_init(nullptr);

    if (conn == nullptr)
    {
        std::cout << "MySQL Initialization Failed!\n";
        return false;
    }

    MYSQL *temp = mysql_real_connect(
        conn,
        "localhost",
        "wallet_user",
        "wallet123",
        "DigitalWallet",
        3306,
        nullptr,
        0);

    if (temp == nullptr)
    {
        std::cout << "Database Connection Failed!\n";
        std::cout << mysql_error(conn) << std::endl;
        mysql_close(conn);
        conn = nullptr;
        return false;
    }

    std::cout << "Database Connected Successfully!\n";
    return true;
}

void Database::disconnect()
{
    if (conn != nullptr)
    {
        mysql_close(conn);
        conn = nullptr;
    }
}

MYSQL *Database::getConnection()
{
    return conn;
}