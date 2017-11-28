/*Author: Robert King, Kevin Lund
* class to represent a warehouse object.
* food is stored in an inventory using a map that maps the upc code of the food to an
* actualy food object
*
* warehouse is manipulate by using the recieve, remove expired and request methods 
*/

#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <string>
#include <map>
#include "food.h"
#include <vector>
#include "food_template.h"
#include "boost/date_time/gregorian/gregorian_types.hpp"

class warehouse
{
  private:
  //name of the warehouse
  std::string name;
  //map of string to the type of food stored in the warehouse


  public:
  std::map<std::string,food> inventory;
  /*
  *  Constructor to create a new warehouse
  * Paramenters: string n : the name of the warehouse
  *              map list_food_types: the types of food that the warehouse keeps track of
  *
  */
  warehouse(std::string name, std::map<std::string, food_template> list_food_types);
  warehouse();
  /*
  * method that monintors when a given number of food is recieved at the warehouse
  * adds the given number of food objects to the inventory of the warehouse
  * Parameters: Date recieve date: the date when the food was added to the warehouse
  *             string food_code: the foodCode of the food recieve_date
  *             int amount_of_food: the number of food items that are recieve
  *             map types_of_food: map of upc code to the type of food
  */
  void receive(boost::gregorian::date recieve_date, std::string foodCode, int amount_of_food,  std::map<std::string, food_template> &types_of_food);
  /*
  * method that monintors when a given number of food is requested from the warehouse
  * removes the given number of food objects to the inventory of the warehouse
  * Parameters: string food_code: the foodCode of the food recieve_date
  *             int amount_of_food: the number of food items that are requested
  *             map types_of_food: map of upc code to the type of food
  */
  void request(std::string foodCode, int amount_of_food, std::map<std::string, food_template> &types_of_food);
  /*
  * method that removes all food that has expired before the given input date
  * expired food is removed from the inventory of the warehouse
  * Parameters: date expiration_date: date used to check if food is expired
  *             map types_of_food: map of upc code to the type of food
  */
  void remove_expired(boost::gregorian::date expiration_date,  std::map<std::string, food_template> &types_of_food);
  /*
  * getter method to get the name of the warehouse
  */
  std::string get_name();

};

#endif
