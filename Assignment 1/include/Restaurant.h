#ifndef RESTAURANT_H_
#define RESTAURANT_H_
#endif
#include <vector>
#include <string>
#include "../include/Dish.h"
#include "../include/Table.h"
#include "../include/Action.h"

extern Restaurant *backup;

class Restaurant{		
public:
	Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
	std::vector<Dish>& getMenu();
    std::string getDishTypeName(DishType i);//my addition
    //rule of 5 additions
    Restaurant& operator=(const Restaurant &other);//copy assignment operator
    virtual ~Restaurant();//destructor
    Restaurant& operator=(Restaurant &&other);//move assignment operator
    Restaurant(const Restaurant &other);//copy constructor
    Restaurant(Restaurant &&other);//MOVE CONSTRUCTOR

private:
        bool open;
        std::vector<Table*> tables;
        std::vector<Dish> menu;// we have this
        std::vector<BaseAction*> actionsLog;
        int numOfCust;
};

