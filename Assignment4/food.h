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
 *
 */

#ifndef _FOOD_H_
#define _FOOD_H_

#include <string>
#include <queue>
#include "boost/date_time/gregorian/gregorian_types.hpp"
#include "food_template.h"

class food{
 private:
  // Name of food
  std::string name;
  // Number of days food lasts on shelf
  boost::gregorian::date_duration shelf_life;
  // Queue of dates corresponding to food items
  std::queue<boost::gregorian::date> date_list;
  //keeps track whether or not the food item is counted as stocked in its warehouse
  bool is_counted;

 public: 
  /*
   * This constructor exists in order to satisfy the needs of std::map. 
   * This constructor will not be used in the project
   */
  food();
/*
 * Constructor for food objects. Takes a food template and uses it to
 * the values of the food object.
 *
 * Parameters:
 *      food_template food_def - The Food template object whose values
 * correspond to those of the food object.
 */
  food(food_template food_def);

/*
 * Gets the number of food items that are represented by this food
 * object.
 */
  int get_count();

/*
 * Getter method for name field of food.
 */
  std::string get_name();
/*
 * Removes all food items from object whose expiration date comes
 * chronologically after the date passed.
 * 
 * Parameters:
 *      date current_date - The date for which food items will be 
 * removed if their expiration date is chronologically after it.
 */
  void remove_expired(boost::gregorian::date current_date, std::map<std::string, food_template> &types_of_food, std::string upc_code);

/*
 * Adds a passed number of food items to the food object. Each item
 * will have an expiration date of current_date + shelf_life.
 * 
 * Parameters:
 *      date current_date - The current date
 *      int count - The number of food items to be added.
 */
  void add(boost::gregorian::date current_date, int count,  std::map<std::string, food_template> &types_of_food, std::string upc_code);

/*
 * Removes a specified number of food items
 *
 * Parameter:
 *      int count - The number of food items to be removed.
 */
  void remove(int count,  std::map<std::string, food_template> &types_of_food, std::string upc_code);
};

#endif
