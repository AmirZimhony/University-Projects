#include "../include/Table.h"
#include "../include/Customer.h"
#include "../include/Restaurant.h"
#include "../include/Dish.h"
#include "../include/Action.h"
#ifndef TABLE_H_
#define TABLE_H_
#endif
#include <vector>

//problematic
Table::Table(int t_capacity) {
    capacity=t_capacity;
    std::vector<Customer*> customersList;
    std::vector<OrderPair > orderList;
    open= false;
}

int Table::getCapacity() const {
    return capacity;
}

void Table::addCustomer(Customer* customer){
    customersList.push_back(customer);
    for (int i=0;i<customer->getOrders().size();i++){
        OrderPair orderpair(customer->getId(),customer->getOrders().at(i));
        insertOrder(orderpair);
    }
}



void Table::removeCustomer(int id) {
    std::vector<OrderPair>& ref=getOrders();
    orderList.clear();
    for (int i = 0; i < customersList.size(); i++) {
        if (customersList[i]->getId() == id) {
            customersList[i]= nullptr;
        }
        for (int j=0;j<orderList.size();j++){
            if( ref.at(j).first!=id) {
                OrderPair ord(ref.at(j).first, ref.at(j).second);
                orderList.push_back(ord);
            }
        }
    }

}

Customer* Table::getCustomer(int id) {
    for (int i = 0; i < customersList.size(); i++) {
        if (customersList[i]->getId() == id) {
            return customersList[i];
        }
    }
    return nullptr;
}

std::vector<Customer*>& Table::getCustomers() {
    return customersList;
}

std::vector<OrderPair>& Table::getOrders() {
    return orderList;
}

void Table::order(const std::vector<Dish> &menu) {
    for (int i = 0; i < customersList.size(); i++) {

        std::vector<int> order(customersList[i]->order(menu));
        for (int j = 0; j < order.size(); j++) {

            OrderPair orderpair(customersList[i]->getId(),menu[order[j]]);
            orderList.push_back(orderpair);
        }

    }
}

void Table::openTable() {
    open=true;
}

void Table::closeTable() {
    for (int i = 0;i<customersList.size(); i++) {
        delete customersList[i];
        customersList[i] = nullptr;
    }
    open = false;
}

int Table::getBill(){
    int result;
    for (int i = 0; i < orderList.size(); i++) {
        result=result+orderList.at(i).second.getPrice();
    }
    return result;
}



bool Table::isOpen() {
    return open;
}


void Table::insertOrder(OrderPair o) {
    orderList.push_back(o);
}



Table::~Table(){//destructor, ok
    orderList.clear();
    customersList.clear();
    for (int i = 0; i < customersList.size(); i++) {
        delete customersList[i];
        customersList.at(i) = nullptr;
    }

}

Table::Table(const Table &T): open(T.open) {//copy constructor,not ok
        capacity = T.capacity;
        for ( int i=0;i<T.customersList.size();i++)
            customersList.push_back(T.customersList[i]);
        for ( int j=0;j<T.orderList.size();j++) {
            OrderPair O(T.orderList.at(j).first, T.orderList.at(j).second);
            orderList.push_back(O);
        }
}

Table & Table::operator=(const Table &T) {//copy assignment operator
        delete this;
        for ( int i=0;i<this->customersList.size();i++) {
            delete customersList[i];
            customersList[i] = nullptr;
        }
        this->capacity=T.capacity;
        this->open=T.open;
    for ( int i=0;i<T.customersList.size();i++)
        customersList.push_back(T.customersList[i]);
    for ( int j=0;j<T.orderList.size();j++)
        orderList.push_back(T.orderList[j]);
}

Table::Table(Table &&T) {//move constructor

    this->capacity=T.capacity;
    this->open=T.open;
    for ( int i=0;i<T.customersList.size();i++)
        customersList.push_back(T.customersList[i]);
    for ( int j=0;j<T.orderList.size();j++)
        orderList.push_back(T.orderList[j]);

    for ( int i=0;i<T.customersList.size();i++) {
        delete T.customersList[i];
        T.customersList[i]= nullptr;
    }
}

Table& Table::operator=(Table &&T) {//move assignment operator

    for ( int i=0;i<this->customersList.size();i++) {
        delete customersList[i];
        customersList[i] = nullptr;
    }
    delete this;
    this->capacity=T.capacity;
    this->open=T.open;
    for ( int i=0;i<T.customersList.size();i++)
        customersList.push_back(T.customersList[i]);
    for ( int j=0;j<T.orderList.size();j++)
        orderList.push_back(T.orderList[j]);

    for ( int i=0;i<T.customersList.size();i++) {
        delete T.customersList[i];
        T.customersList[i]= nullptr;
    }
}

Table* Table::clone() {
    return new Table(*this);
}