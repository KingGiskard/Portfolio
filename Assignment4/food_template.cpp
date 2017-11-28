/*
 * Kevin Lund and Robert King
 * 2/14/17
 *
 * Objects of this class are used to represent the layout of food objects and make
 * them available to the controller. food_templates must be initialized with a shelf life,
 * a upc code, and a name.
 *
 * * this class also monitors the number of warehouses that has this item stocked_warehouses
  * this class also monitors the number of times this type of food was requested
 */

#include "food_template.h"
#include <iostream>
/*
 * Constructor for food_template objects. Intializes the fields of the template to
 * the passed values.
 *
 * Parameters:
 *      string _name - The name of the food
 *      int _shelf_life - The shelf life of the food
 *      int _uoc_code - The upc code of the food
 */
food_template::food_template(std::string _name, int _shelf_life, std::string _upc_code){
  name = _name;
  shelf_life = _shelf_life;
  upc_code = _upc_code;
  stocked_warehouses = 0;
  times_requested = 0;
}
//empty Constructor is necessary for the food template to work in a map
//Constructor is never used. only used for the syntax of a map object
food_template::food_template()
{
}

/*
 * Getter method for name field of food_template
 */
std::string food_template::get_name(){
  return name;
}

  /*
  * increase the number of warehouses that have this food item stocked by 1
  */
void food_template::add_warehouse()
{
  stocked_warehouses++;
}

/*
* reduces the number of warehouses that have this food item stocked by 1
*/
void food_template::remove_warehouse()
{
  stocked_warehouses--;
}

/*
 * Getter method for shelf_life field of food_template
 */
int food_template::get_shelf_life(){
  return shelf_life;
}

/*
 * Getter method for upc_code field of food_template
 */
std::string food_template::get_upc_code(){
  return upc_code;
}

/*
* getter method for the number of warehouses that have this food item stocked
*/
int food_template::get_stocked_warehouses(){
  return stocked_warehouses;
}

/*
* increases the number of times this food was request by input count
* used to monitor how popular this food is
*/
void food_template::add_requests(int count){
  times_requested += count;
}

/*
* getter method for the number of times this food was requested
*/
int food_template::get_times_requested(){
  return times_requested;
}
