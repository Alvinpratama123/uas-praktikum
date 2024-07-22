#include <iostream>
#include <mysql/mysql.h>
#include <sstream>
#include <string>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "cpp_crud";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Connected to the database successfully." << endl;
        } else {
            cerr << "Connection failed: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init failed" << endl;
    }
    return conn;
}

void create_product(const string& name, const int price, const int size, const string& color) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO product (name, price, size, color) VALUES ('" << name << "', " << price << ", " << size << ", '" << color << "')";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Product successfully added." << endl;
        }
        mysql_close(conn);
    }
}

void get_products() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM product")) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Name: " << row[1] << ", Price: " << row[2] << ", Size: " << row[3] << ", Color: " << row[4] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void update_product(int productid, const string& name, const int price, const int size, const string& color) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE product SET name = '" << name << "', price = " << price << ", size = " << size << ", color = '" << color << "' WHERE id = " << productid;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Product successfully updated." << endl;
        }
        mysql_close(conn);
    }
}

void delete_product(int productid) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM product WHERE id = " << productid;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Product successfully deleted." << endl;
        }
        mysql_close(conn);
    }
}

bool login() {
    int userType;
    string username, password;

    cout << "\nMenu:\n";
    cout << "1. Admin\n";
    cout << "2. User\n";
    cout << "Pilihan: ";
    cin >> userType;
    cin.ignore();

    if (userType == 1) {
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        if (username == "Alvin" && password == "123") {
            system("CLS");
            return true;
        } else {
            cout << "Invalid username or password." << endl;
            return false;
        }
    } else if (userType == 2) {
        system("CLS");
        return true;
    } else {
        cout << "Invalid choice." << endl;
        return false;
    }
}

int main() {
    if (!login()) {
        return 0;
    }

    int choice;
    int userType;

    cout << "\nMenu:\n";
    cout << "1. Admin\n";
    cout << "2. User\n";
    cout << "Pilihan: ";
    cin >> userType;

    if (userType == 1) {
        while (true) {
            cout << "\nMenu:\n";
            cout << "1. Add Product\n";
            cout << "2. Show All Products\n";
            cout << "3. Update Product\n";
            cout << "4. Delete Product\n";
            cout << "5. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                string name, color;
                int price, size;
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter price: ";
                cin >> price;
                cout << "Enter size: ";
                cin >> size;
                cout << "Enter color: ";
                cin.ignore();
                getline(cin, color);
                create_product(name, price, size, color);
            } else if (choice == 2) {
                get_products();
            } else if (choice == 3) {
                int productid;
                string name, color;
                int price, size;
                cout << "Enter product ID to update: ";
                cin >> productid;
                cin.ignore();
                cout << "Enter new name: ";
                getline(cin, name);
                cout << "Enter new price: ";
                cin >> price;
                cout << "Enter new size: ";
                cin >> size;
                cout << "Enter new color: ";
                cin.ignore();
                getline(cin, color);
                update_product(productid, name, price, size, color);
            } else if (choice == 4) {
                int productid;
                cout << "Enter product ID to delete: ";
                cin >> productid;
                delete_product(productid);
            } else if (choice == 5) {
                break;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    } else if (userType == 2) {
        while (true) {
            cout << "\nMenu:\n";
            cout << "1. Show All Products\n";
            cout << "2. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                get_products();
            } else if (choice == 2) {
                break;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    } else {
        cout << "Invalid choice." << endl;
    }

    return 0;
}