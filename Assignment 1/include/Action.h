#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "../include/Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    void Syntax(std::string input);//my addition
    std::string getSyntax() const;//my addition
    std::string translateAction(ActionStatus A) const;//my addition
    //rule of 5 additions for open table
     //~BaseAction();//destructor
    BaseAction(const BaseAction& action);//copy constructor
  //  BaseAction(BaseAction &&A);//move constructor
    //virtual BaseAction* clone();
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string syntax;//MY ADDITION
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;
    //additions for rule of five(without assignment operators)
    virtual ~OpenTable();
    OpenTable(const OpenTable &o);//copy constructor
    OpenTable(OpenTable &&other);//move constructor
     BaseAction* clone();
private:
	const int tableId;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone();
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone();
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone();
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone();
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone();
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone();
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone();
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone();
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BaseAction* clone();

};


#endif