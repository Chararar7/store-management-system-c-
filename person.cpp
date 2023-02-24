#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <iomanip>
#include <vector>
#include <cctype>
#include <sstream>
#include <algorithm>
#include "person.h"

using namespace std;

bool login(string uname, string pass, ifstream& login_file)
{
    string username, password;
    while (!login_file.eof())
    {
        login_file >> username >> password;
        if (username == uname && password == pass)
        {
            cout << "\n Congrats! Access Granted!\n\n";
            return 1;
        }
    }
    cout << "\n\n Sorry! Access Denied!\n\n";
    return 0;
}

string get_pass()
{
    string p = "";
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch < 32)
        {
            cout << endl;
            return p;
        }
        p.push_back(ch);
        cout << '*';
    }
}

bool is_int(string& str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool is_float(string& str)
{
    bool dot_found = false;
    for (int i = 0; i < str.length(); i++)
    {
        if (!isdigit(str[i]) && str[i] != '.') return false;
        if (str[i] == '.' && dot_found) return false;
        if (str[i] == '.') dot_found = true;
    }
    return true;
}

int search(string& name, int& q, float& p)
{
    ifstream stock_file("Stock.txt");
    string n;
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    do
    {
        stock_file >> n >> q >> p;
        transform(n.begin(), n.end(), n.begin(), ::tolower);
    } while (n != name && !stock_file.eof());
    stock_file.close();
    if (n == name)
    {
        name.at(0) = toupper(name.at(0));
        return 1;
    }
    else return 0;
}

void Display()
{
    ifstream stock_file("Stock.txt");
    product p;
    cout << "\n\n\t=============================================" << endl;
    cout << "\t" << left << setw(20) << "Product" << left << setw(20) << "Quantity" << "Price" << endl;
    cout << "\t=============================================" << endl;
    while (!stock_file.eof())
    {
        stock_file >> p.name >> p.quantity >> p.price;
        cout << "\t" << left << setw(20) << p.name << left << setw(20) << p.quantity << p.price << endl;
    }
    stock_file.close();
}

void Customer::Display_Bill(stack<product> bill)
{
    float T = 0;
    if (!bill.empty())
    {
        cout << "\n\n\t=============================================" << endl;
        cout << "\t\t\t     BILL";
        cout << "\n\t=============================================" << endl;
        cout << "\t" << left << setw(20) << "Product" << left << setw(20) << "Quantity" << "Price" << endl;
        cout << "\t=============================================" << endl;
        while (!bill.empty())
        {
            product current = bill.top();
            T += bill.top().price * bill.top().quantity;
            bill.pop();
            cout << "\t" << left << setw(20) << current.name << left << setw(20) << current.quantity << current.price << endl;
        }
        cout << "\t=============================================" << endl;
        cout << "\t" << left << setw(40) << "Total  " << T << "$" << endl;
    }
}

void add_item(string name, int quantity, float price)
{
    ofstream Stock_file("Stock.txt", ios::app);
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    name.at(0) = toupper(name.at(0));
    Stock_file << "\n" << name << " " << quantity << " " << price;
    Stock_file.close();
}

void remove(string name)
{
    fstream stock_file("Stock.txt", ios::in | ios::out);
    product p;
    vector<product> products;
    while (!stock_file.eof())
    {
        stock_file >> p.name >> p.quantity >> p.price;
        if (p.name != name) products.push_back(p);
    }
    stock_file.close();

    ofstream Stock_file("Stock.txt");
    for (product p : products)
    {
        Stock_file << p.name << " " << p.quantity << " " << p.price;
        if (p.name != products.at(products.size() - 1).name) Stock_file << endl;
    }
    Stock_file.close();
}

void refill(string name, int quantity)
{
    string n;
    int q;
    float p;
    if (!search(name, q, p)) return;
    else
    {
        remove(name);
        add_item(name, q + quantity, p);
    }
}

void Customer::purchase()
{
    int q, quantity;
    char i = ' ';
    string name, qty;
    float total = 0, price;
    while (i)
    {
        cout << "\n Actions:\n 1. Buy products\n 2. Exit\n" << endl;
        cout << " Enter your choice: ";
        cin >> i;
        cin.clear();
        cin.ignore(200, '\n');

        while (i != '1' && i != '2')
        {
            cout << "\n '" << i << "' is not an option!\n";
            cout << "\n Reenter your choice: ";
            cin >> i;
            cin.clear();
            cin.ignore(200, '\n');
        }

        if (i == '1')
        {
            Display();

        enter_name:

            cout << "\n Enter a product's name: ";
            cin >> name;
            cin.clear();
            cin.ignore(200, '\n');

            if (!search(name, q, price))
            {
                cout << "\n There's no product with this name!" << endl;
                goto enter_name;
            }

            do
            {
                do
                {
                    cout << "\n Enter quantity: ";
                    cin >> qty;
                    cin.clear();
                    cin.ignore(200, '\n');
                } while (!is_int(qty));
                quantity = stoi(qty);

                if (quantity > q) cout << "\n There isn't enough units available!" << endl;
            } while (quantity < 0 || quantity > q);
            product p{ name, quantity, price };
            Bill.push(p);
            total += quantity * price;
            refill(name, -quantity);
            cout << "\n The total amount to be paid is: " << total << "$" << endl;
        }
        else if (i == '2') return;
    }
}

void Dealer::menu()
{
    string u, p;
    char j;

login:
    cout << "\n Enter your username: ";
    cin >> u;
    cout << " Enter your password: ";
    p = get_pass();

    ifstream db("Dealers.txt");

    int i = login(u, p, db);

menu:
    if (i)
    {
        cout << "\n=============================================================";
        cout << "\n\n\t\t       DEALER MENU\n\n\t\t    1. Add new product\n\n\t\t    2. Display stock\n\n\t\t    3. Refill\n\n\t\t    4. Remove an item\n\n\t\t    5. Exit";
        cout << "\n\n======================= END OF MENU =========================";
        cout << "\n\n Enter your choice : ";
        cin >> j;
        cin.clear();
        cin.ignore(200, '\n');
        string name;

        while (j != '1' && j != '2' && j != '3' && j != '4' && j != '5')
        {
            cout << "\n '" << j << "' is not an option!\n";
            cout << "\n Reenter your choice: ";
            cin >> j;
            cin.clear();
            cin.ignore(200, '\n');
        }
        if (j == '1')
        {
            int quantity;
            float price;
            string qty, prc;

        enter_name:

            cout << "\n Enter the product's name: ";
            cin >> name;
            cin.clear();
            cin.ignore(200, '\n');

            if (search(name, quantity, price))
            {
                cout << " This product is already in the stock.\n ";
                goto enter_name;
            }

            do
            {
                cout << "\n Enter quantity: ";
                cin >> qty;
                cin.clear();
                cin.ignore(200, '\n');
            } while (!is_int(qty));
            quantity = stoi(qty);

            do
            {
                cout << "\n Enter price: ";
                cin >> prc;
                cin.clear();
                cin.ignore(200, '\n');
            } while (!is_float(prc) && !is_int(prc));
            price = stof(prc);

            add_item(name, quantity, price);
            cout << "\n";
            goto menu;
        }
        else if (j == '2')
        {
            Display();
            cout << "\n";
            goto menu;
        }
        else if (j == '3')
        {
            int quantity;
            string qty;
            cout << "\n Enter the product's name: ";
            cin >> name;
            cin.clear();
            cin.ignore(200, '\n');

            do
            {
                cout << "\n Enter quantity: ";
                cin >> qty;
                cin.clear();
                cin.ignore(200, '\n');
            } while (!is_int(qty));
            quantity = stoi(qty);

            refill(name, quantity);
            cout << "\n";
            goto menu;
        }
        else if (j == '4')
        {
            float temp1;
            int temp2;

        enter_name2:

            cout << "\n Enter the product's name: ";
            cin >> name;
            if (!search(name, temp2, temp1))
            {
                cout << " There's no product with this name!" << endl;
                goto enter_name2;
            }
            remove(name);
            cout << "\n";
            goto menu;
        }
        else if (j == '5') return;
    }
    else
    {
        cout << "\n Reenter your login information:\n" << endl;
        goto login;
    }
    db.close();
}

void Employee::menu()
{
    string u, p;
    char j;

login:
    cout << "\n Enter your username: ";
    cin >> u;
    cout << " Enter your password: ";
    p = get_pass();

    ifstream db("Employees.txt");

    int  i = login(u, p, db);

menu:
    if (i)
    {
        cout << "\n=============================================================";
        cout << "\n\n\t\t    EMPLOYEE MENU\n\n\t\t   1. Display stock\n\n\t\t   2. Refill\n\n\t\t   3. Exit";
        cout << "\n\n======================= END OF MENU =========================";
        cout << "\n\n Enter your choice: ";
        cin >> j;
        cin.clear();
        cin.ignore(200, '\n');
        while (j != '1' && j != '2' && j != '3')
        {
            cout << "\n '" << j << "' is not an option!\n";
            cout << "\n Reenter your choice: ";
            cin >> j;
            cin.clear();
            cin.ignore(200, '\n');
        }
        if (j == '1')
        {
            Display();
            cout << "\n";
            goto menu;
        }
        else if (j == '2')
        {
            string name, qty;
            int quantity, temp2;
            float temp1;

         enter_name:
            cout << "\n Enter the product's name: ";
            cin >> name;
            cin.clear();
            cin.ignore(200, '\n');
            
            if (!search(name, temp2, temp1))
            {
                cout << " No product with this name!" << endl;
                goto enter_name;
            }

            do
            {
                cout << "\n Enter quantity: ";
                cin >> qty;
                cin.clear();
                cin.ignore(200, '\n');
            } while (!is_int(qty));
            quantity = stoi(qty);

            refill(name, quantity);
            cout << "\n";
            goto menu;
        }
        else if (j == '3') return;
    }
    else
    {
        cout << "\n Reenter your login information:\n" << endl;
        goto login;
    }
}

void Customer::menu()
{
    string u, p;
    char j;
login:
    cout << "\n Enter your username: ";
    cin >> u;
    cout << " Enter your password: ";
    p = get_pass();

    ifstream db("Customers.txt");

    int i = login(u, p, db);

menu:
    if (i)
    {
        cout << "\n=============================================================";
        cout << "\n\n\t\t      CUSTOMER MENU\n\n\t\t     1. Purchase\n\n\t\t     2. Display stock\n\n\t\t     3. Exit";
        cout << "\n\n======================= END OF MENU =========================";
        cout << "\n\n Enter your choice : ";
        cin >> j;
        cin.clear();
        cin.ignore(200, '\n');

        while (j != '1' && j != '2' && j != '3')
        {
            cout << "\n '" << j << "' is not an option!\n";
            cout << "\n Reenter your choice: ";
            cin >> j;
            cin.clear();
            cin.ignore(200, '\n');
        }
        if (j == '1')
        {
            purchase();
            cout << "\n";
            goto menu;
        }
        else if (j == '2')
        {
            Display();
            cout << "\n";
            goto menu;
        }
        else if (j == '3')
        {
            Display_Bill(Bill);
            return;
        }
    }
    else
    {
        cout << "\n Reenter your login information:\n" << endl;
        goto login;
    }
}