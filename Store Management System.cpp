#include <iostream>
#include <string>
#include <fstream>
#include "person.h"

using namespace std;

int main()
{
    string u, p;
    char j = ' ';
    cout << "\n\n   |============ WELCOME TO STORE MANAGEMENT ============|";
menu:
    cout << "\n\n=============================================================";
    cout << "\n\t\t   STORE MANAGEMENT SYSTEM\n";
    cout << "=============================================================";
    cout << "\n\n\t\t   1. Dealer Menu\n\n\t\t   2. Customer Menu\n\n\t\t   3. Employee Menu\n\n\t\t   4. Exit";
    cout << "\n\n=============================================================\n";
    cout << "\n Enter your choice: ";
    cin >> j;
    cin.clear();
    cin.ignore(200, '\n');

    while (j != '1' && j != '2' && j != '3' && j != '4')
    {
        cout << "\n '" << j << "' is not an option!\n";
        cout << "\n Reenter your choice: ";
        cin >> j;
        cin.clear();
        cin.ignore(200, '\n');
    }
    if (j == '1')
    {
        Dealer D;
        D.menu();
        goto menu;
    }
    else if (j == '2')
    {
        Customer C;
        C.menu();
        goto menu;
    }
    else if (j == '3')
    {
        Employee E;
        E.menu();
        goto menu;
    }
    else if (j == '4') cout << "\n\tBye bye!\n\n";
    return 0;
}
