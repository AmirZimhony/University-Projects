#include "../include/Customer.h"
#include "../include/Restaurant.h"
#include <algorithm>

using namespace std;


Customer::Customer(string c_name, int c_id):name(c_name),id(c_id) {};

string Customer::getName() const {
    return name;
}

int Customer::getId() const{
    return id;
}

void Customer::pushDish(Dish n) {
    ordered.push_back(n);
}

 std::vector<Dish> Customer::getOrders() {
    return ordered;
}

Customer::Customer(const Customer &c):id(c.id),name(c.name) {
    for (int i=0;i<c.ordered.size();i++)
        ordered.push_back(c.ordered[i]);
}






//**********************vegetarian customer section
VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id) {}



vector<int> VegetarianCustomer::order(const vector<Dish> &menu) {

    vector<int> orderVec;
    int food=-1;


    //here we order the vegetarian dish with the smallest id
    for (int i = 0; i < menu.size(); i++) {
        if ((menu[i].getType() == 0) &&(food==-1)) {
            orderVec.push_back(i);
            food=i;
        }
    }
    //here we order the most expensive non-alcoholic beverage
    int minprice = 0;
    int idBvg = -1;
    for (int i = 0; i < menu.size(); i++) {
        if ((menu[i].getType() ==2) && (menu[i].getPrice() > minprice)) {
            idBvg = i;
            minprice = menu[i].getPrice();
        }
    }
    if (idBvg != -1) {
        orderVec.push_back(idBvg);
    }
    // the Customer orders only if both items are on the menu.
    if (orderVec.size() < 2) {
        vector<int> emptyVec;
        cout << "vegetarian customer didn't order anything" << endl;
       // bool first_order = false;  because if he didn't find what to order in the first time, he won't find the j'th time; the menu doesn't change
        return emptyVec;
    }
    else {
        cout << getName() +" ordered "+ menu.at(orderVec[0]).getName() << endl;
        cout << getName() +" ordered "+ menu.at(orderVec[1]).getName() << endl;
        cout << orderVec[0] << endl;
        cout << orderVec[1] << endl;
        //bool first_order = false; not used anywhere
        Dish food=menu.at(orderVec[0]);
        Dish drink=menu.at(orderVec[1]);
        pushDish(food);
        pushDish(drink);
        return orderVec;
    }
}


string VegetarianCustomer::toString() const {
    return std::to_string(getId())+' '+this->getName();
}


Customer* VegetarianCustomer::clone() {
    return new  VegetarianCustomer(*this);
}

//***********cheap customer section
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id) {
    first_order = true;
};



///cheap customer orders the cheapest item on the menu. and does not order more.
vector<int> CheapCustomer::order(const vector<Dish> &menu) {
    if (first_order) {
        vector<int> orderVec;
        int cheapest_dish_price = 2147483647;
        int id = -1;
        for (int i = 0; i < menu.size(); i++) {
            if (menu[i].getPrice() < cheapest_dish_price) {
                id = i;
                cheapest_dish_price = menu[i].getPrice();
            }
        }
        if (id != -1) {
            orderVec.push_back(id);
            Dish food=menu.at(id);
            pushDish(food);
            cout << getName() +" ordered "+ menu.at(id).getName() << endl;
        }
        first_order = false;
        return orderVec;
    }
    else {
        vector<int> emptyVec;
        return emptyVec;
    }
}

string CheapCustomer::toString() const {
    return std::to_string(getId())+' '+this->getName();
}


Customer* CheapCustomer::clone() {
    return new CheapCustomer(*this);
}


//*******************spicyCustomer section
SpicyCustomer::SpicyCustomer(string name, int id):Customer(name,id) {
    first_order = true;
};



vector<int> SpicyCustomer::order(const vector<Dish> &menu) {
    //spicy customer orders the most expensive dish in the first order.
    if (first_order) {
        vector<int> orderVec;
        int id = -1;
        int m_expensive_spc_dish_price = -1;
        //DishType spc(SPC);
        for (int i = 0; i < menu.size(); i++) {
            if ((menu[i].getType() == 1) && (menu[i].getPrice() > m_expensive_spc_dish_price)) {
                id = i;
                m_expensive_spc_dish_price = menu[i].getPrice();
            }
        }
        if (id != -1) {
            orderVec.push_back(id);
            Dish food=menu.at(id);
            pushDish(food);
            cout << getName() +" ordered "+ menu.at(id).getName() << endl;
        }
        first_order = false;
        return orderVec;
    } ///for further orders, he orders the cheapest non alcoholic drink:
    else {
        vector<int> orderVec;
        int id = -1;
        int cheapest_alc_bvg_price = 2147483647;
       //DishType bvg(BVG);
        for (int i = 0; i < menu.size(); i++) {
            if ((menu[i].getType() ==2) && (menu[i].getPrice() < cheapest_alc_bvg_price)) {
                id = i;
                cheapest_alc_bvg_price = menu[i].getPrice();
            }
        }
        if (id != -1) {
            orderVec.push_back(id);
            Dish drink=menu.at(id);
            pushDish(drink);
            cout << getName() +" ordered "+ menu.at(id).getName() << endl;
        }
        return orderVec;
    }
}


string SpicyCustomer::toString() const {
    return std::to_string(getId())+' '+this->getName();
}



Customer* SpicyCustomer::clone(){
    return new SpicyCustomer(*this);
}


//*****************ALCOHOLIC customer section
AlchoholicCustomer::AlchoholicCustomer(string name, int id):Customer(name,id) {
    first_order=true;
}



vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    //for the first order, alcoholic customer orders the cheapest alcoholic drink.
    if (first_order) {
        vector<int> orderVec;
        //DishType alc(ALC);
        int id = -1;
        int cheapest_alc_price = 2147483647;
        for (int i = 0; i < menu.size(); i++) {
            if ((menu[i].getType() == 3) && (menu[i].getPrice() < cheapest_alc_price)) {
                id =i;
                cheapest_alc_price=menu[i].getPrice();
            }
        }
        if (id != -1) {
            orderVec.push_back(id);
            Dish food=menu.at(id);
            pushDish(food);
            cout << getName() +" ordered "+ menu.at(id).getName() << endl;
        }
        first_order = false;
        last_order = id;
        return orderVec;
    }
    else { ///for further orders, alcoholic customer orders the next-in-line more expensive alcoholic drink.
        //let's find the item on the menu that corresponds to the id. and its price:
        //int prev_id = last_order;
        int id=last_order ;
        int prev_price = menu[last_order].getPrice();
        //DishType alc(ALC);
        //lets find the next more expensive alcoholic beverage:
        vector<int> orderVec;
        int cheapest_alc_price = 2147483647;
        for (int i = 0; i < menu.size(); i++) {
            if ((menu[i].getType() == 3) && (menu[i].getPrice() >= prev_price) && (i != last_order)&&(menu[i].getPrice()<=cheapest_alc_price)) {
                id = i;
                cheapest_alc_price=menu[i].getPrice();
            }
        }
        if (id == last_order) { // he already ordered the most expensive alcoholic drink before
            return orderVec; // we return the empty vector
        }
        else {
            orderVec.push_back(id);
            Dish drink=menu.at(id);
            pushDish(drink);
            cout << getName() +" ordered "+ menu.at(id).getName() << endl;
            last_order=id;
            return orderVec;

        }
    }
}

string AlchoholicCustomer::toString() const {
    return std::to_string(getId())+' '+this->getName();
}


Customer* AlchoholicCustomer::clone() {
    return new AlchoholicCustomer(*this);
}
