#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

//I think the default destructor, copy constructor and copy assignment are good here, because this class doesn't have resources.

class Customer{
public:
	Customer(std::string c_name, int c_id); //done
	virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
	virtual std::string toString() const = 0;
	std::string getName() const; //done
	int getId() const; //done
	//virtual ~Customer();
	std::vector<Dish> getOrders();//ADDITION
	void pushDish(Dish n);//ADDITION
	//virtual Customer* clone();//addition
     //virtual ~Customer();
	 Customer(const Customer &c);//copy constructor

private:
	const std::string name;
	const int id;
    std::vector<Dish> ordered;//ADDITION
};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id); //done
	std::vector<int> order(const std::vector<Dish> &menu); //done
	std::string toString() const; //done
	int getNumOfOrders();
	Customer* clone();
	//~VegetarianCustomer()= default;
private:

};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id); // done
	std::vector<int> order(const std::vector<Dish> &menu); // done
	std::string toString() const; // done
	int getNumOfOrders();
    Customer* clone();
    //~CheapCustomer()= default;
private:
	bool first_order;//addition

};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id); // done
	std::vector<int> order(const std::vector<Dish> &menu); // done
	std::string toString() const; // done
	int getNumOfOrders();
    virtual Customer* clone();

private:
	bool first_order;//addition

};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id); // done
	std::vector<int> order(const std::vector<Dish> &menu); //done
	std::string toString() const; // done
	int getNumOfOrders();
    Customer* clone();
    //~AlchoholicCustomer()=default;
private:
	bool first_order;//addition
	int last_order;//addition

};


#endif