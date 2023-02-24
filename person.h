#pragma once
#include <string>
#include <stack>

using namespace std;

typedef struct product
{
	string name;
	int quantity;
	float price;
} product;

class Person
{
public:
	friend bool login(string, string, ifstream&);
	friend void Display();
};

class Dealer : public Person
{
public:
	friend void add_item(string, int, float);
	friend void refill(string, int);
	friend void remove(string);
	void menu();
};

class Employee : public Person
{
public:
	friend void refill(string, int);
	void menu();
};

class Customer : public Person
{
	stack<product> Bill;
public:
	void purchase();
	void Display_Bill(stack<product>);
	void menu();
};
