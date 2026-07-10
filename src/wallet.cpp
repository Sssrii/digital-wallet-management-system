#include "wallet.h"
#include <iostream>

using namespace std;

Wallet::Wallet(Database *database, int uid)
{
    db = database;
    userId = uid;
}

void Wallet::checkBalance()
{
    MYSQL *conn = db->getConnection();

    char query[256];
    snprintf(query, sizeof(query),
        "SELECT balance FROM Wallets WHERE user_id = %d", userId);

    if (mysql_query(conn, query))
    {
        cout << "\nError: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);

    if (row)
    {
        cout << "\nCurrent Balance: Rs " << row[0] << "\n";
    }
    else
    {
        cout << "\nWallet not found.\n";
    }

    mysql_free_result(result);
}

void Wallet::addMoney()
{
    double amount;
    cout << "\nEnter amount to add: Rs ";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "\nInvalid amount.\n";
        return;
    }

    MYSQL *conn = db->getConnection();

    char query[256];
    snprintf(query, sizeof(query),
        "UPDATE Wallets SET balance = balance + %.2f WHERE user_id = %d",
        amount, userId);

    if (mysql_query(conn, query))
    {
        cout << "\nError: " << mysql_error(conn) << endl;
        return;
    }

    // Add notification
    char notifyQuery[300];
    snprintf(notifyQuery, sizeof(notifyQuery),
        "INSERT INTO Notifications (user_id, message) VALUES (%d, 'Rs %.2f added to your wallet')",
        userId, amount);
    mysql_query(conn, notifyQuery);

    cout << "\nRs " << amount << " added successfully!\n";
    checkBalance();
}

void Wallet::withdrawMoney()
{
    double amount;
    cout << "\nEnter amount to withdraw: Rs ";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "\nInvalid amount.\n";
        return;
    }

    MYSQL *conn = db->getConnection();

    char checkQuery[256];
    snprintf(checkQuery, sizeof(checkQuery),
        "SELECT balance FROM Wallets WHERE user_id = %d", userId);

    if (mysql_query(conn, checkQuery))
    {
        cout << "\nError: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);

    if (!row)
    {
        cout << "\nWallet not found.\n";
        mysql_free_result(result);
        return;
    }

    double currentBalance = atof(row[0]);
    mysql_free_result(result);

    if (amount > currentBalance)
    {
        cout << "\nInsufficient balance. Current balance: Rs " << currentBalance << "\n";
        return;
    }

    char updateQuery[256];
    snprintf(updateQuery, sizeof(updateQuery),
        "UPDATE Wallets SET balance = balance - %.2f WHERE user_id = %d",
        amount, userId);

    if (mysql_query(conn, updateQuery))
    {
        cout << "\nError: " << mysql_error(conn) << endl;
        return;
    }

    cout << "\nRs " << amount << " withdrawn successfully!\n";
    checkBalance();
}