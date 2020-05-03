#include "../include/Restaurant.h"
#include "../include/Customer.h"//my addition

#ifndef RESTAURANT_H_
#define RESTAURANT_H_
#endif

#include "../include/Action.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cstring>
#include <bits/stdc++.h>
using namespace std;

Restaurant::Restaurant(){};


//aid function, used to get the type of the dish in the config file.
DishType getDishType(string &type2) {
    if (type2=="VEG") return VEG;
    if (type2=="ALC") return ALC;
    if (type2=="BVG") return BVG;
    if (type2=="SPC") return SPC;
}

//aid function,does the opposite of the former one
std::string  Restaurant::getDishTypeName(DishType i){
    if (i==VEG) return "VEG";
    if (i==ALC) return "ALC";
    if (i==BVG) return "BVG";
    if (i==SPC) return "SPC";
}


Restaurant::Restaurant(const std::string &configFilePath) {
    std::ifstream cFile(configFilePath);
    if (cFile.is_open()) {

        std::string line;
        while (line[0] == '#' || line.empty()) {
            getline(cFile, line);
        }
        // when we get to this line, we are at the number-of-tables lines

        int numTables = stoi(line);// holds number of tables, works. "stoi" converts string to an int.
        getline(cFile, line);
        while (line[0] == '#' || line.empty()) {
            getline(cFile, line);
        }
        //when we get to this line, we are at the line that holds the capacity of each table

        vector<string> tokens;
        stringstream check1(line);
        string intermediate;
        while (getline(check1, intermediate, ',')) {
            tokens.push_back(intermediate);// works, now we have a vector called "tokens" that has the capacity of each table.
        }
        //building tables now with the vector "tokens"-PROBLEMATIC,NOW WORKING ON THIS
        for (int i = 0; i < tokens.size(); i++) {
            Table* table=new Table(stoi(tokens.at(i)));
            tables.push_back(table);
        }



        getline(cFile, line);//works. pointing at "#Menu".
        while (line[0] == '#' || line.empty()) {
            getline(cFile, line);
        }
        //works. When we get to this line, "line" holds the line of the first dish.


        int dishCounter = 0;
        vector<string> dishInfo;
        stringstream check2(line);
        string partOfDish;
        while (getline(check2, partOfDish, ',')) {
            dishInfo.push_back(partOfDish);
        }
        string meal = dishInfo[0];
        DishType type1 = getDishType(dishInfo[1]);
        int price = stoi(dishInfo[2]);
        Dish d(dishCounter, meal, price, type1);//problematic
        menu.push_back(d);//problematic
        dishCounter++;
        // while (line!=NULL){


        while (getline(cFile, line)) {//continuing to scan the menu
            vector<string> dishInfo;
            stringstream check2(line);
            string partOfDish;
            while (getline(check2, partOfDish, ',')) {
                dishInfo.push_back(partOfDish);
            }
            string meal = dishInfo[0];
            DishType type1 = getDishType(dishInfo[1]);
            int price = stoi(dishInfo[2]);
            Dish d(dishCounter, meal, price, type1);
            menu.push_back(d);
            dishCounter++;
        }






    }
}








int Restaurant::getNumOfTables() const {
    return  int(tables.size());
}


Table* Restaurant::getTable(int ind){
    return tables.at(ind);
}


std::vector<Dish> &Restaurant::getMenu() {
    return menu;
}




void Restaurant::start() {
    std::cout << "Restaurant is now open!" << std::endl;
    open = true;
    numOfCust=0;
    string input;//ok
    getline(cin,input);//ok
    //consider parsing now and checking first word of string
    while( input!="closeall") {//ok
        vector<string> partsOfSent;
        stringstream check2(input);
        string part;
        while (getline(check2, part, ' ')) {
            partsOfSent.push_back(part);
        }
        std::cout << partsOfSent.at(0)<< std::endl;// check first word of input
        if (partsOfSent.at(0)=="open"){
            int numOfTable=stoi(partsOfSent.at(1));
            vector<Customer*> localList;
            for(int j=2;j<partsOfSent.size();j++){//part where we make the customerlist vector
                vector<string> twoParts;
                stringstream check11(partsOfSent.at(j));
                string temp;
                while (getline(check11, temp, ',')) {
                    twoParts.push_back(temp);
                }
                string name=twoParts.at(0);
                string type=twoParts.at(1);

                if(type=="veg") {
                    Customer *v = new VegetarianCustomer(name, numOfCust);
                    numOfCust++;
                    localList.push_back(v);
                }

                if(type=="chp") {
                    Customer *c=new CheapCustomer(name, numOfCust);
                    numOfCust++;
                    localList.push_back(c);
                }

                if(type=="spc"){
                    Customer *s=new SpicyCustomer(name, numOfCust);
                    numOfCust++;
                    localList.push_back(s);
                }

                if(type=="alc"){
                    Customer *a=new AlchoholicCustomer(name, numOfCust);
                    numOfCust++;
                    localList.push_back(a);
                }

            }
            BaseAction *open=new OpenTable(numOfTable,localList);
            open->Syntax(input);
            open->act(*this);
            actionsLog.push_back(open);
        }
        if (partsOfSent.at(0)=="move"){
            BaseAction *move=new MoveCustomer(stoi(partsOfSent.at(1)),stoi(partsOfSent.at(2)),stoi(partsOfSent.at(3)));
            move->Syntax(input);
            move->act(*this);
            actionsLog.push_back(move);
        }
        if (partsOfSent.at(0)=="status"){
            BaseAction *stat=new PrintTableStatus(stoi(partsOfSent.at(1)));
            stat->Syntax(input);
            stat->act(*this);
            actionsLog.push_back(stat);
        }
        if (partsOfSent.at(0)=="order"){
            BaseAction *ord=new Order(stoi(partsOfSent.at(1)));
            ord->Syntax(input);
            ord->act(*this);
            actionsLog.push_back(ord);
        }
        if (partsOfSent.at(0)=="menu") {//ok, for PrintMenu action
            BaseAction *printMe=new PrintMenu;
            printMe->Syntax(input);
            printMe->act(*this);
            actionsLog.push_back(printMe);
        }
        if (partsOfSent.at(0)=="log"){
            BaseAction *print=new PrintActionsLog;
            print->act(*this);
            actionsLog.push_back(print);
        }
        if( partsOfSent.at(0)=="close"){
            BaseAction *close=new Close(stoi(partsOfSent.at(1)));
            close->Syntax(input);
            close->act(*this);
            actionsLog.push_back(close);
        }
        if (partsOfSent.at(0)=="backup"){
            BaseAction *bu=new BackupRestaurant();
            bu->act(*this);
            actionsLog.push_back(bu);
        }
        if (partsOfSent.at(0)=="restore"){
            BaseAction *re=new RestoreResturant();
            re->act(*this);
            actionsLog.push_back(re);
        }




        getline(cin, input);//ok
        //for every action, there should be an if statement that would tell us what action to operate.
    }
    BaseAction *end=new CloseAll;
    end->act(*this);
    actionsLog.push_back(end);
}



const std::vector<BaseAction*>& Restaurant::getActionsLog() const {
    return actionsLog;
}


//rule of 5 additions

///copy assignment operator
Restaurant &Restaurant::operator=(const Restaurant &other) {
    for (int i = 0; i < tables.size(); i++) {
        delete tables[i];
        tables[i] = nullptr;
    }
    tables.clear();

    menu.clear();
    actionsLog.clear();
    for (int i = 0; i < actionsLog.size(); i++) {
        delete actionsLog[i]; // need to define it. now it's the default destructor.
        actionsLog[i] = nullptr;
    }

    this->open = other.open;


    for (int i = 0; i < other.tables.size(); i++) {
        this->tables.push_back(new Table(*other.tables[i]));
    }
    //copying menu
    for (int i = 0; i<other.menu.size(); i++) {
        this->menu.push_back(other.menu[i]);
    }
    //copying actions log

    for (int i = 0; i< other.actionsLog.size(); i++) {
        this->actionsLog.push_back(other.actionsLog[i]);
    }

}


//destructor
Restaurant::~Restaurant() {
    //open = 0;
    //menu doesn't have any resources and is on the stack, so allio is coolio.
    for (int i = 0; i < tables.size(); i++) {
        delete tables[i];
        tables[i] = nullptr;
    }
    tables.clear();


    for (int i = 0; i < actionsLog.size(); i++) {
        delete actionsLog[i]; // need to define it. now it's the default destructor.
        actionsLog[i] = nullptr;
    }
    actionsLog.clear();


    menu.clear();

}

///move assignment operator
Restaurant &Restaurant::operator=(Restaurant &&other) {
    /// first, we move all our data to backup
    this->open = other.open;
    this->menu = other.menu;
    this->actionsLog = other.actionsLog;
    this->tables = other.tables;

}

///copy constructor
Restaurant::Restaurant(const Restaurant &other): open(other.open){
    //copying menu
    for (int i=0; i<other.menu.size(); i++) {
        this->menu.push_back(other.menu[i]);
    }
    //copying actionslog
    for (int i=0; i<other.actionsLog.size(); i++) {
        BaseAction *A(other.actionsLog[i]);
        this->actionsLog.push_back(A);
    }
    //copying tables
    for (int i=0; i<other.tables.size(); i++) {
        this->tables.push_back(new Table(*other.tables[i]));
    }
    numOfCust=other.numOfCust;

}

///move constructor
Restaurant::Restaurant(Restaurant &&other): open(other.open), menu(other.menu), actionsLog(other.actionsLog), tables(other.tables)  {
    for (int i=0; i<other.tables.size(); i++) {
        delete other.tables[i];
        other.tables[i]= nullptr;
    }
    for (int i=0; i<other.actionsLog.size(); i++) {
        delete other.actionsLog[i];
        actionsLog[i]= nullptr;
    }
}