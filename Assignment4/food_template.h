/*
 * Kevin Lund and Robert King
 * 2/14/17
 *
 * Objects of this class are used to represent the layout of food objects and make
 * them available to the controller. food_templates must be initialized with a shelf life,
 * a upc code, and a name.
 *
 * this class also monitors the number of warehouses that has this item stocked_warehouses
 * this class also monitors the number of times this type of food was requested
 */

#ifndef _FOOD_TEMPLATE_H_
#define _FOOD_TEMPLATE_H_

#include <string>

class food_template {
 private:
  // Name of food
  std::string name;
  // Number of days food lasts on shelf
  int shelf_life;
  // upc code for food
  std::string upc_code;
  int stocked_warehouses;
  int times_requested;

 public:
/*
 * Constructor for food_template objects. Intializes the fields of the template to
 * the passed values.
 *
 * Parameters:
 *      string _name - The name of the food
 *      int _shelf_life - The shelf life of the food
 *      int _uoc_code - The upc code of the food
 */
  food_template(std::string _name, int _shelf_life, std::string _upc_code);
  food_template();

  /*
  * increase the number of warehouses that have this food item stocked by 1
  */
  void add_warehouse();

  /*
  * reduces the number of warehouses that have this food item stocked by 1
  */
  void remove_warehouse();

  /*
  * increases the number of times this food was request by input count
  * used to monitor how popular this food is
  */
  void add_requests(int count);

  int get_times_requested();

/*
 * Getter method for name field of food_template
 */
  std::string get_name();

/*
 * Getter method for shelf_life field of food_template
 */
  int get_shelf_life();
/*
 * Getter method for upc_code field of food_template
 */
  std::string get_upc_code();

  /*
  * getter method for the number of warehouses that have this food item stocked
  */
  int get_stocked_warehouses();
};

#endif
