#include "admin.h"
#include <iostream>

using namespace std;

Admin::Admin(Database *database)
{
    db = database;
}

void Admin::viewAllUsers()
{
    MYSQL *conn = db->getConnection();

    mysql_query(conn, "SELECT user_id, name, email, status FROM Users");
    MYSQL_RES *result = mysql_store_result(conn);

    cout << "\n----- ALL USERS -----\n";

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        cout << "ID: " << row[0]
             << " | Name: " << row[1]
             << " | Email: " << row[2]
             << " | Status: " << row[3] << "\n";
    }

    mysql_free_result(result);
}

void Admin::blockUser()
{
    int id;
    cout << "\nEnter User ID to Block: ";
    cin >> id;

    MYSQL *conn = db->getConnection();

    char query[256];
    snprintf(query, sizeof(query),
        "UPDATE Users SET status = 'Blocked' WHERE user_id = %d", id);

    if (mysql_query(conn, query))
    {
        cout << "\nError: " << mysql_error(conn) << endl;
        return;
    }

    cout << "\nUser Blocked Successfully.\n";
}

void Admin::unblockUser()
{
    int id;
    cout << "\nEnter User ID to Unblock: ";
    cin >> id;

    MYSQL *conn = db->getConnection();

    char query[256];
    snprintf(query, sizeof(query),
        "UPDATE Users SET status = 'Active' WHERE user_id = %d", id);

    if (mysql_query(conn, query))
    {
        cout << "\nError: " << mysql_error(conn) << endl;
        return;
    }

    cout << "\nUser Unblocked Successfully.\n";
}

void Admin::viewAllTransactions()
{
    MYSQL *conn = db->getConnection();

    mysql_query(conn,
        "SELECT transaction_id, sender_wallet_id, receiver_wallet_id, amount, transaction_type, transaction_time "
        "FROM Transactions ORDER BY transaction_time DESC");

    MYSQL_RES *result = mysql_store_result(conn);

    cout << "\n----- ALL TRANSACTIONS -----\n";

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        cout << "ID: " << row[0]
             << " | Sender Wallet: " << row[1]
             << " | Receiver Wallet: " << row[2]
             << " | Amount: Rs " << row[3]
             << " | Type: " << row[4]
             << " | Time: " << row[5] << "\n";
    }

    mysql_free_result(result);
}