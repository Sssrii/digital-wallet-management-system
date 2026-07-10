#include <iostream>
#include "database.h"
#include "user.h"
#include "wallet.h"
#include "transaction.h"
#include "admin.h"

using namespace std;

int main()
{
    cout << "=========================================\n";
    cout << "     DIGITAL WALLET MANAGEMENT SYSTEM\n";
    cout << "=========================================\n\n";

    Database db;
    if (!db.connect())
    {
        cout << "\nUnable to connect to database.\n";
        return 1;
    }

    User user(&db);

    int choice;
    cout << "\n1. Register\n2. Login\n3. Admin Login\n4. Exit\nEnter choice: ";
    cin >> choice;

    if (choice == 1)
    {
        user.registerUser();
    }
    else if (choice == 2)
    {
        if (user.login())
        {
            Wallet wallet(&db, user.userId);
            Transaction txn(&db, user.userId);

            int dashChoice;
            do
            {
                cout << "\n===== USER DASHBOARD =====\n";
                cout << "1. View Profile\n";
                cout << "2. Check Balance\n";
                cout << "3. Add Money\n";
                cout << "4. Withdraw Money\n";
                cout << "5. Transfer Money\n";
                cout << "6. Transaction History\n";
               cout << "7. View Notifications\n";
                cout << "8. Logout\n";
                cout << "Enter choice: ";
                cin >> dashChoice;

                switch (dashChoice)
                {
                    case 1: user.viewProfile(); break;
                    case 2: wallet.checkBalance(); break;
                    case 3: wallet.addMoney(); break;
                    case 4: wallet.withdrawMoney(); break;
                    case 5: txn.transferMoney(); break;
                    case 6: txn.viewHistory(); break;
                    case 7: user.viewNotifications(); break;
                    case 8: cout << "\nLogged out.\n"; break;
                    default: cout << "\nInvalid choice.\n";
                }
            } while (dashChoice != 8);
        }
    }
    else if (choice == 3)
    {
        string adminUser, adminPass;
        cout << "\nAdmin Username: ";
        cin >> adminUser;
        cout << "Admin Password: ";
        cin >> adminPass;

        if (adminUser == "admin" && adminPass == "admin123")
        {
            Admin admin(&db);

            int adminChoice;
            do
            {
                cout << "\n===== ADMIN DASHBOARD =====\n";
                cout << "1. View All Users\n";
                cout << "2. Block User\n";
                cout << "3. Unblock User\n";
                cout << "4. View All Transactions\n";
                cout << "5. Logout\n";
                cout << "Enter choice: ";
                cin >> adminChoice;

                switch (adminChoice)
                {
                    case 1: admin.viewAllUsers(); break;
                    case 2: admin.blockUser(); break;
                    case 3: admin.unblockUser(); break;
                    case 4: admin.viewAllTransactions(); break;
                    case 5: cout << "\nLogged out.\n"; break;
                    default: cout << "\nInvalid choice.\n";
                }
            } while (adminChoice != 5);
        }
        else
        {
            cout << "\nInvalid Admin Credentials.\n";
        }
    }

    db.disconnect();
    return 0;
}