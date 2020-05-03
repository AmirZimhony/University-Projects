#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "../include/Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);//constructor
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    void insertOrder(OrderPair o);
    //Rule of 5 additions from here on
    virtual ~Table();//destructor
    Table(const Table &T);//copy constructor
    Table & operator=(const Table &T);//copy assignment operator
    Table(Table &&T);//move constructor
    Table&operator=(Table &&T);//MOVE ASSIGNMENT OPERATOR
    Table* clone();




private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif