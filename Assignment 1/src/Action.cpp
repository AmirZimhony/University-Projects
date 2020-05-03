
#include "../include/Restaurant.h"
#include "../include/Dish.h"
#include "../include/Table.h"
#include "../include/Customer.h"
#ifndef ACTION_H_
#define ACTION_H_
#endif

#include <vector>
#include <iostream>

//***********BaseAction section
BaseAction::BaseAction() {}

void BaseAction::Syntax(std::string input) {
    syntax=input;
}

std::string BaseAction::getSyntax() const {
    return syntax;
}
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
void BaseAction::complete() {
status=COMPLETED;
}
void BaseAction::error(std::string errorMsg) {
this->errorMsg=errorMsg;
status=ERROR;
}

//probably unnecessary
std::string BaseAction::translateAction(ActionStatus A) const {
    if(status==COMPLETED)return "Completed";
    if(status==ERROR)return getErrorMsg();
}


ActionStatus BaseAction::getStatus() const {
    return status;
}

BaseAction::BaseAction(const BaseAction &action) {
    this->errorMsg=action.errorMsg;
    this->status=action.status;
    this->syntax=action.syntax;
}

//*********** OpenTable section
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList): BaseAction(),tableId(id),customers(customersList) {}

void OpenTable::act(Restaurant &restaurant) {
    if((tableId>restaurant.getNumOfTables()-1)||(restaurant.getTable(tableId)->isOpen())||(restaurant.getTable(tableId)->getCapacity()<customers.size())) {
        error("Table does not exist or is already open");
        std::cout << "Table does not exist or is already open" << std::endl;
    }
    else {
        restaurant.getTable(tableId)->openTable();
        for(int i=0;i<customers.size();i++){
            restaurant.getTable(tableId)->addCustomer(customers.at(i));
        }
        complete();
    }
}

std::string OpenTable::toString() const {
    if(getStatus()==COMPLETED)
        return getSyntax()+" Completed";
    else
        return getSyntax()+" Error:"+getErrorMsg();
}

OpenTable::~OpenTable() {//DESTRUCTOR
    for (int i = 0; i < customers.size(); i++) {
        delete customers[i];
        customers[i] = nullptr;
    }
    customers.clear();
}


OpenTable::OpenTable(const OpenTable &o):BaseAction(),tableId(o.tableId) {//COPY CONSTRUCTOR
    for (int i = 0; i < customers.size(); i++) {
        this->customers.push_back(o.customers[i]);
    }
}


OpenTable::OpenTable(OpenTable &&other):BaseAction(), tableId(other.tableId) {//MOVE CONSTRUCTOR
    for (int i = 0; i < customers.size(); i++) {
        this->customers.push_back(other.customers[i]);
    }
}

BaseAction* OpenTable::clone() {
    return new OpenTable(*this);
}








//**************Order section
Order::Order(int id):BaseAction(),tableId(id) {}

void Order::act(Restaurant &restaurant) {
    if((!restaurant.getTable(tableId)->isOpen())||(tableId>restaurant.getNumOfTables()-1)){
        std::cout<<"Table does not exist or is not open"<<std::endl;
        error("Table does not exist or is not open");
    }
    else {
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        complete();
    }
}

std::string Order::toString() const {
    if(getStatus()==COMPLETED)
        return getSyntax()+" Completed";
    else
        return getSyntax()+" Error "+getErrorMsg();
}

 BaseAction* Order::clone() {
  return new Order(*this);
}


//Move section
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTable(src),dstTable(dst),id(customerId) {}

void MoveCustomer::act(Restaurant &restaurant) {
    if ((srcTable>restaurant.getNumOfTables()-1)||(dstTable>restaurant.getNumOfTables()-1)) {
        error("Cannot move customer");//if we recieved the number of a table that doesn't exist
        std::cout<<"Cannot Move Customer"<<std::endl;
        return;
    }
    if((!restaurant.getTable(srcTable)->isOpen())||(!restaurant.getTable(dstTable)->isOpen())) {
        error("Cannot move customer");//if one of the tables is not open
        std::cout<<"Cannot Move Customer"<<std::endl;
        return;
    }
    if ((restaurant.getTable(srcTable)->getCustomer(id)== nullptr)||(restaurant.getTable(dstTable)->getCustomers().size()==restaurant.getTable(dstTable)->getCapacity())) {
        error("Cannot move customer");//if customer isn't in the source or doesn't have room in the destination
        std::cout<<"Cannot Move Customer"<<std::endl;
        return;
    }

    Customer *move=restaurant.getTable(srcTable)->getCustomer(id);
    restaurant.getTable(dstTable)->addCustomer(move);
    restaurant.getTable(srcTable)->removeCustomer(id);
    if( restaurant.getTable(srcTable)->getCustomers().size()==0)
        restaurant.getTable(srcTable)->closeTable();

    complete();
}


std::string MoveCustomer::toString() const {
    if (getStatus() == COMPLETED)
        return getSyntax() + " Completed";
    else
        return getSyntax() + " Error" + getErrorMsg();
}

BaseAction* MoveCustomer::clone() {
    return new MoveCustomer(*this);
}

//***********PrintMenu section
PrintMenu::PrintMenu(): BaseAction(){}


void PrintMenu::act(Restaurant &restaurant) {
        for(int i=0;i<restaurant.getMenu().size();i++)
            std::cout<< restaurant.getMenu().at(i).getName()<<" "<<restaurant.getDishTypeName(restaurant.getMenu().at(i).getType())<<" "<<restaurant.getMenu().at(i).getPrice()<< "NIS" <<std::endl;
}

std::string PrintMenu::toString() const {
    return getSyntax()+" "+"Completed";
}

BaseAction* PrintMenu::clone() {
    return new PrintMenu(*this);
}

//**********Close section
Close::Close(int id):BaseAction(),tableId(id) {}


void Close::act(Restaurant &restaurant) {
    if((tableId>restaurant.getNumOfTables())||(!restaurant.getTable(tableId)->isOpen()))
    {
        error("Table does not exist or is not open");
        std::cout << "Table does not exist or is not open" << std::endl;
    }
    else{
        std::cout << "Table " << tableId << " was closed. Bill " << restaurant.getTable(tableId)->getBill() << "NIS"
                  << std::endl;
        restaurant.getTable(tableId)->closeTable();
        complete();
    }
}

std::string Close::toString() const {
    if(getStatus()==COMPLETED ) {
        return getSyntax()+" Completed";
    }
    else
        return getSyntax()+" Error: "+this->getErrorMsg();
}

BaseAction* Close::clone() {
    return new Close(*this);
}


//***********Closeall section
CloseAll::CloseAll(): BaseAction(){}

void CloseAll::act(Restaurant &restaurant) {
    for(int i=0;i<restaurant.getNumOfTables();i++)
        if(restaurant.getTable(i)->isOpen()) {
            std::cout << "Table" << i << "was closed. Bill " << restaurant.getTable(i)->getBill() << "NIS" << std::endl;
            restaurant.getTable(i)->closeTable();
        }
}

std::string CloseAll::toString() const {
    return " ";
}

BaseAction* CloseAll::clone() {
    return new CloseAll(*this);
}

//***********Print table status section
PrintTableStatus::PrintTableStatus(int id):BaseAction(),tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant) {
    if (!restaurant.getTable(tableId)->isOpen())
        std::cout<<"Table "+std::to_string(tableId)+" status: closed"<<std::endl;
    else{
        Table *t=restaurant.getTable(tableId);
        Customer *C=restaurant.getTable(tableId)->getCustomer(0);
        std::vector <Customer*> cust=t->getCustomers();
        std::cout<<"Table "+std::to_string(tableId)+" status: open"<<std::endl;
        std::cout<<"Customers:"<<std::endl;
        for (int i=0;i<t->getCustomers().size();i++){
            std::cout<<std::to_string(cust[i]->getId())+" "+cust[i]->getName()<<std::endl;
        }
        std::cout<<"Orders:"<<std::endl;
        for (int j=0;j<restaurant.getTable(tableId)->getOrders().size();j++){
            std::cout<<restaurant.getTable(tableId)->getOrders().at(j).second.getName()+" "+std::to_string(restaurant.getTable(tableId)->getOrders().at(j).second.getPrice())+"NIS "+std::to_string(restaurant.getTable(tableId)->getOrders().at(j).first)<<std::endl;
        }
        std::cout<<"Current bill: "+std::to_string(restaurant.getTable(tableId)->getBill())+"NIS"<<std::endl;
    }
}

std::string PrintTableStatus::toString() const {
    return getSyntax()+" Completed";
}

BaseAction* PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
}

//************PrintAction section
PrintActionsLog::PrintActionsLog(): BaseAction() {}

void PrintActionsLog::act(Restaurant &restaurant) {
    for(int i=0;i<restaurant.getActionsLog().size();i++){
        std::cout<<restaurant.getActionsLog().at(i)->toString()<<std::endl;
    }
}

std::string PrintActionsLog::toString() const {
    return "log Completed ";
}

BaseAction* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

//************Backup section
BackupRestaurant::BackupRestaurant(): BaseAction() {}

void BackupRestaurant::act(Restaurant &restaurant) {
     if (backup== nullptr)
         backup=new Restaurant(restaurant);
     else
         *backup=restaurant;
}

std::string BackupRestaurant::toString() const {
    return "backup Completed";
}


BaseAction* BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
}


//************Restore section
RestoreResturant ::RestoreResturant():BaseAction() {}

void RestoreResturant::act(Restaurant &restaurant) {
    if(backup== nullptr) {
        error("No backup available");
        std::cout<<"No backup available"<<std::endl;
    }
    else
    restaurant=*backup;
    complete();
}

std::string RestoreResturant::toString() const {
    if (getStatus()==COMPLETED)
        return "restore Completed";
    else
        return "Error: "+getErrorMsg();
}

BaseAction* RestoreResturant::clone() {
    return new RestoreResturant(*this);
}