#include <iostream>
#include <mysql.h>

using namespace std;

int main()
{
    MYSQL *conn = mysql_init(nullptr);

    if (conn == nullptr)
    {
        cout << "MySQL Initialization Failed!" << endl;
        return 1;
    }

    cout << "MySQL Header and Library Found Successfully!" << endl;

    mysql_close(conn);

    return 0;
}