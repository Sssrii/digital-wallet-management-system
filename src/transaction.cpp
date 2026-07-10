#include "transaction.h"
#include <iostream>

using namespace std;

Transaction::Transaction(Database *database, int uid)
{
    db = database;
    userId = uid;
}

void Transaction::transferMoney()
{
    string receiverEmail;
    double amount;

    cout << "\n----- TRANSFER MONEY -----\n";
    cout << "Enter Receiver's Email: ";
    cin.ignore();
    getline(cin, receiverEmail);

    cout << "Enter Amount to Transfer: Rs ";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "\nInvalid amount.\n";
        return;
    }

    MYSQL *conn = db->getConnection();

    // Step 1: Find receiver's user_id from their email
    char e_email[201];
    mysql_real_escape_string(conn, e_email, receiverEmail.c_str(), receiverEmail.length());

    char findQuery[256];
    snprintf(findQuery, sizeof(findQuery),
        "SELECT user_id FROM Users WHERE email='%s'", e_email);

    mysql_query(conn, findQuery);
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);

    if (!row)
    {
        cout << "\nReceiver not found.\n";
        mysql_free_result(result);
        return;
    }

    int receiverUserId = atoi(row[0]);
    mysql_free_result(result);

    if (receiverUserId == userId)
    {
        cout << "\nYou cannot transfer money to yourself.\n";
        return;
    }

    // Step 2: Check sender's balance
    char balanceQuery[256];
    snprintf(balanceQuery, sizeof(balanceQuery),
        "SELECT balance FROM Wallets WHERE user_id = %d", userId);

    mysql_query(conn, balanceQuery);
    result = mysql_store_result(conn);
    row = mysql_fetch_row(result);

    double senderBalance = atof(row[0]);
    mysql_free_result(result);

    if (amount > senderBalance)
    {
        cout << "\nInsufficient balance. Current balance: Rs " << senderBalance << "\n";
        return;
    }

    // Step 3: Deduct from sender
    char deductQuery[256];
    snprintf(deductQuery, sizeof(deductQuery),
        "UPDATE Wallets SET balance = balance - %.2f WHERE user_id = %d",
        amount, userId);
    mysql_query(conn, deductQuery);

    // Step 4: Add to receiver
    char addQuery[256];
    snprintf(addQuery, sizeof(addQuery),
        "UPDATE Wallets SET balance = balance + %.2f WHERE user_id = %d",
        amount, receiverUserId);
    mysql_query(conn, addQuery);

    // Step 5: Get wallet_ids for both, to log the transaction
    char senderWalletQuery[256];
    snprintf(senderWalletQuery, sizeof(senderWalletQuery),
        "SELECT wallet_id FROM Wallets WHERE user_id = %d", userId);
    mysql_query(conn, senderWalletQuery);
    result = mysql_store_result(conn);
    row = mysql_fetch_row(result);
    int senderWalletId = atoi(row[0]);
    mysql_free_result(result);

    char receiverWalletQuery[256];
    snprintf(receiverWalletQuery, sizeof(receiverWalletQuery),
        "SELECT wallet_id FROM Wallets WHERE user_id = %d", receiverUserId);
    mysql_query(conn, receiverWalletQuery);
    result = mysql_store_result(conn);
    row = mysql_fetch_row(result);
    int receiverWalletId = atoi(row[0]);
    mysql_free_result(result);

    // Step 6: Insert transaction record
    char insertQuery[512];
    snprintf(insertQuery, sizeof(insertQuery),
        "INSERT INTO Transactions (sender_wallet_id, receiver_wallet_id, amount, transaction_type, status) "
        "VALUES (%d, %d, %.2f, 'Transfer', 'Success')",
        senderWalletId, receiverWalletId, amount);
   mysql_query(conn, insertQuery);

    // Add notifications for both sender and receiver
    char notifySender[300];
    snprintf(notifySender, sizeof(notifySender),
        "INSERT INTO Notifications (user_id, message) VALUES (%d, 'You sent Rs %.2f')",
        userId, amount);
    mysql_query(conn, notifySender);

    char notifyReceiver[300];
    snprintf(notifyReceiver, sizeof(notifyReceiver),
        "INSERT INTO Notifications (user_id, message) VALUES (%d, 'You received Rs %.2f')",
        receiverUserId, amount);
    mysql_query(conn, notifyReceiver);

    cout << "\nRs " << amount << " transferred successfully!\n";
}

void Transaction::viewHistory()
{
    MYSQL *conn = db->getConnection();

    // Get this user's wallet_id first
    char walletQuery[256];
    snprintf(walletQuery, sizeof(walletQuery),
        "SELECT wallet_id FROM Wallets WHERE user_id = %d", userId);
    mysql_query(conn, walletQuery);
    MYSQL_RES *walletResult = mysql_store_result(conn);
    MYSQL_ROW walletRow = mysql_fetch_row(walletResult);
    int myWalletId = atoi(walletRow[0]);
    mysql_free_result(walletResult);

    // Get all transactions where I'm sender or receiver
    char query[512];
    snprintf(query, sizeof(query),
        "SELECT transaction_id, sender_wallet_id, receiver_wallet_id, amount, transaction_type, transaction_time "
        "FROM Transactions WHERE sender_wallet_id = %d OR receiver_wallet_id = %d "
        "ORDER BY transaction_time DESC",
        myWalletId, myWalletId);

    mysql_query(conn, query);
    MYSQL_RES *result = mysql_store_result(conn);

    cout << "\n----- TRANSACTION HISTORY -----\n";

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        int senderWallet = atoi(row[1]);
        string direction = (senderWallet == myWalletId) ? "Sent" : "Received";

        cout << "ID: " << row[0]
             << " | " << direction
             << " | Amount: Rs " << row[3]
             << " | Type: " << row[4]
             << " | Time: " << row[5] << "\n";
    }

    mysql_free_result(result);
}