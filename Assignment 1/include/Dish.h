#ifndef DISH_H_
#define DISH_H_

#include <string>

enum DishType{
    VEG, SPC, BVG, ALC
};

class Dish{
public:
    Dish(int d_id, std::string d_name, int d_price, DishType d_type);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    DishType getType() const;
    //RULE OF 3 ADDITIONS
    virtual ~Dish();
    Dish(const Dish &d);
    Dish& operator=(const Dish &d);

private:
	const int id;
    const std::string name;
    const int price;
    const DishType type;
};





#endif