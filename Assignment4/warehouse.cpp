/*Author: Robert King, Kevin Lund
* class to represent a warehouse object.
* food is stored in an inventory using a map that maps the upc code of the food to an
* actualy food object
*
* warehouse is manipulate by using the recieve, remove expired and request methods
*/

#include "warehouse.h"
#include "food.h"
#include <iostream>


 /*
 *  Constructor to create a new warehouse
 * Paramenters: string n : the name of the warehouse
 *              map list_food_types: the types of food that the warehouse keeps track of
 *
 */
warehouse::warehouse(std::string n, std::map<std::string, food_template> list_food_types)
{
  name = n;

  for(std::map<std::string,food_template>::iterator it = list_food_types.begin(); it != list_food_types.end(); ++it)
  {
    std::string food_id = it->first;
    food newFood(it->second);
    inventory[food_id] = newFood;
  }
}
warehouse::warehouse()
{
}

/*
* method that monintors when a given number of food is recieved at the warehouse
* adds the given number of food objects to the inventory of the warehouse
* Parameters: Date recieve date: the date when the food was added to the warehouse
*             string food_code: the foodCode of the food recieve_date
*             int amount_of_food: the number of food items that are recieve
*             map types_of_food: map of upc code to the type of food
*/
void warehouse::receive(boost::gregorian::date recieve_date, std::string foodCode, int amount_of_food,  std::map<std::string, food_template> &types_of_food)
{
   inventory[foodCode].add(recieve_date, amount_of_food, types_of_food, foodCode);
   return;
}

/*
* method that monintors when a given number of food is requested from the warehouse
* removes the given number of food objects to the inventory of the warehouse
* Parameters: string food_code: the foodCode of the food recieve_date
*             int amount_of_food: the number of food items that are requested
*             map types_of_food: map of upc code to the type of food
*/
void warehouse::request(std::string foodCode, int amount_of_food,  std::map<std::string, food_template> &types_of_food)
{
  inventory[foodCode].remove(amount_of_food, types_of_food, foodCode);
  return;

}

/*
* method that removes all food that has expired before the given input date
* expired food is removed from the inventory of the warehouse
* Parameters: date expiration_date: date used to check if food is expired
*             map types_of_food: map of upc code to the type of food
*/
void warehouse::remove_expired(boost::gregorian::date expiration_date,  std::map<std::string, food_template> &types_of_food)
{
  for(std::map<std::string,food>::iterator it = inventory.begin(); it != inventory.end(); ++it)
  {
    (it->second).remove_expired(expiration_date, types_of_food, it->first);
  }
  return;

}

/*
* getter method to get the name of the warehouse
*/
std::string warehouse::get_name()
{
  return name;
}
