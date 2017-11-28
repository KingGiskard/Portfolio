/*
 * Kevin Lund and Robert King
 * 2/14/17
 *
 * Objects of this class represent food inventory within a warehouse.
 * Food objects are aware of their name, the shelf life of the item,
 * and a list of dates corresponding to the expiration dates of each
 * food item of this type within a warehouse. A single food object is
 * used to represent any number of actual food items, each of which are
 * represented by their expiration date, which is stored in a queue. These
 * items must be added chronologically, which forces an invariance where
 * the items which expire soonest are removed first.
 */

#include "food.h"
#include <iostream>

/*
 * Constructor for food objects. Takes a food template and uses it to
 * the values of the food object.
 *
 * Parameters:
 *      food_template food_def - The Food template object whose values
 * correspond to those of the food object.
 */
food::food(food_template food_def){
  name = food_def.get_name();
  shelf_life = boost::gregorian::date_duration((long)food_def.get_shelf_life());
  is_counted = false;
  date_list;

}

/*
   * This constructor exists in order to satisfy the needs of std::map.
   *This constructor will not be used in the project
   */
food::food()
{
}

/*
 * Gets the number of food items that are represented by this food
 * object.
 */
int food::get_count(){
  // The size of date_list is the number of food items
  //  std::cout << "Size: " << date_list.size() << std::endl;
  return date_list.size();
}

/*
 * Getter method for name field of food.
 */
std::string food::get_name(){
  return name;
}

/*
 * Removes all food items from object whose expiration date comes
 * chronologically after the date passed.
 *
 * Parameters:
 *      date current_date - The date for which food items will be
 * removed if their expiration date is chronologically after it.
 */
void food::remove_expired(boost::gregorian::date current_date,  std::map<std::string, food_template> &types_of_food, std::string upc_code){
  // std::cout << "entered expired " << name<<  std::endl;
  if(get_count()==0)
    {
      is_counted = false;
      return;
    }

  // Due to queue invariance, once a date is found which is less than
  // the passed date, no more dates must be checked.
  while(get_count()!=0 && current_date == date_list.front()){
    date_list.pop();
  }
  //all stock expired
  if(get_count() == 0)
  {
    is_counted = false;
     types_of_food[upc_code].remove_warehouse();
    return;
  }
  else
  {
    is_counted = true;
    return;
  }
}

/*
 * Adds a passed number of food items to the food object. Each item
 * will have an expiration date of current_date + shelf_life.
 *
 * Parameters:
 *      date current_date - The current date
 *      int count - The number of food items to be added.
 */
void food::add(boost::gregorian::date current_date, int count,  std::map<std::string, food_template> &types_of_food, std::string upc_code){
  for(int i = 0; i < count; i++){
    date_list.push(current_date + shelf_life);
  }
  if(count != 0 && !is_counted)
  {
      is_counted = true;
      types_of_food[upc_code].add_warehouse();
      return;
  }
  return;

}

/*
 * Removes a specified number of food items. If less items exist
 * then are to be removed, the queue is cleared.
 *
 * return true when remove method gets rid of all the previous stock of the food
 *return false otherwise
 * Parameter:
 *      int count - The number of food items to be removed.
 */
void food::remove(int count,  std::map<std::string, food_template> &types_of_food, std::string upc_code){
  types_of_food[upc_code].add_requests(count);
  if(get_count()==0)
    {
      is_counted = false;
      return;
    }

  // If less items exist than need to be removed, simply remove all items
  if(get_count()!=0 && count >= date_list.size())
  {
    while(!date_list.empty()){
      date_list.pop();
    }
    is_counted = false;
     types_of_food[upc_code].remove_warehouse();
    return;
  }
  // Otherwise remove the specified number of items
  for(int i = 0; i < count; i++){
    date_list.pop();
  }
  is_counted = true;
  return;
}
