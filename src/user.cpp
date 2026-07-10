#include "user.h"
#include <iostream>
#include <cstring>

using namespace std;

User::User(Database *database)
{
    db = database;
}

void User::registerUser()
{
    string name, email, phone, password, dob;

    cout << "\n----- REGISTER NEW USER -----\n";
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter Email: ";
    getline(cin, email);

    cout << "Enter Phone: ";
    getline(cin, phone);

    cout << "Enter Password: ";
    getline(cin, password);

    cout << "Enter DOB (YYYY-MM-DD): ";
    getline(cin, dob);

    MYSQL *conn = db->getConnection();

    char e_name[201], e_email[201], e_phone[101], e_password[201], e_dob[51];

    mysql_real_escape_string(conn, e_name, name.c_str(), name.length());
    mysql_real_escape_string(conn, e_email, email.c_str(), email.length());
    mysql_real_escape_string(conn, e_phone, phone.c_str(), phone.length());
    mysql_real_escape_string(conn, e_password, password.c_str(), password.length());
    mysql_real_escape_string(conn, e_dob, dob.c_str(), dob.length());

    char query[1024];
    snprintf(query, sizeof(query),
        "INSERT INTO Users (name, email, phone, password, dob) "
        "VALUES ('%s', '%s', '%s', '%s', '%s')",
        e_name, e_email, e_phone, e_password, e_dob);

    if (mysql_query(conn, query))
    {
        cout << "\nRegistration Failed: " << mysql_error(conn) << endl;
        return;
    }

    // Get the new user's ID and create their wallet
    int newUserId = (int)mysql_insert_id(conn);

    char walletQuery[256];
    snprintf(walletQuery, sizeof(walletQuery),
        "INSERT INTO Wallets (user_id, balance, wallet_status) VALUES (%d, 0.00, 'Active')",
        newUserId);

    if (mysql_query(conn, walletQuery))
    {
        cout << "\nWallet Creation Failed: " << mysql_error(conn) << endl;
        return;
    }

    cout << "\nAccount Created Successfully! Your wallet is ready.\n";
}

bool User::login()
{
    string email, password;

    cout << "\n----- LOGIN -----\n";
    cout << "Enter Email: ";
    cin.ignore();
    getline(cin, email);

    cout << "Enter Password: ";
    getline(cin, password);

    MYSQL *conn = db->getConnection();

    char e_email[201], e_password[201];
    mysql_real_escape_string(conn, e_email, email.c_str(), email.length());
    mysql_real_escape_string(conn, e_password, password.c_str(), password.length());

    char query[512];
    snprintf(query, sizeof(query),
        "SELECT user_id, name, status FROM Users WHERE email='%s' AND password='%s'",
        e_email, e_password);

    if (mysql_query(conn, query))
    {
        cout << "\nLogin Failed: " << mysql_error(conn) << endl;
        return false;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == nullptr || mysql_num_rows(result) == 0)
    {
        cout << "\nInvalid Email or Password.\n";
        if (result) mysql_free_result(result);

        // Log failed attempt (we don't know user_id, so we skip logging if user not found)
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int foundUserId = atoi(row[0]);
    string status = row[2];

    if (status == "Blocked")
    {
        cout << "\nYour account is blocked. Contact admin.\n";
        mysql_free_result(result);

        // Log failed login (blocked)
        char logQuery[256];
        snprintf(logQuery, sizeof(logQuery),
            "INSERT INTO Login_History (user_id, ip_address, status) VALUES (%d, '127.0.0.1', 'Failed')",
            foundUserId);
        mysql_query(conn, logQuery);

        return false;
    }

    userId = foundUserId;
    userName = row[1];

    cout << "\nLogin Successful! Welcome, " << userName << "!\n";

    mysql_free_result(result);

    // Log successful login
    char logQuery[256];
    snprintf(logQuery, sizeof(logQuery),
        "INSERT INTO Login_History (user_id, ip_address, status) VALUES (%d, '127.0.0.1', 'Success')",
        userId);
    mysql_query(conn, logQuery);

    return true;
}
void User::viewProfile()
{
    MYSQL *conn = db->getConnection();

    char query[256];
    snprintf(query, sizeof(query),
        "SELECT name, email, phone, dob, status FROM Users WHERE user_id = %d", userId);

    mysql_query(conn, query);
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);

    if (row)
    {
        cout << "\n----- MY PROFILE -----\n";
        cout << "Name: " << row[0] << "\n";
        cout << "Email: " << row[1] << "\n";
        cout << "Phone: " << row[2] << "\n";
        cout << "DOB: " << row[3] << "\n";
        cout << "Status: " << row[4] << "\n";
    }

    mysql_free_result(result);
}
void User::viewNotifications()
{
    MYSQL *conn = db->getConnection();

    char query[256];
    snprintf(query, sizeof(query),
"SELECT message, notification_date FROM Notifications WHERE user_id = %d ORDER BY notification_date DESC",        userId);

    if (mysql_query(conn, query))
    {
        cout << "\nError: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == nullptr)
    {
        cout << "\nError fetching notifications: " << mysql_error(conn) << endl;
        return;
    }

    cout << "\n----- NOTIFICATIONS -----\n";

    if (mysql_num_rows(result) == 0)
    {
        cout << "No notifications yet.\n";
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        cout << row[0] << " (" << row[1] << ")\n";
    }

    mysql_free_result(result);
}