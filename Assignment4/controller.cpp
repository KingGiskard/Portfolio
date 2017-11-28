/* Author: Robert King, Kevin Lund
* Class that handles the input of a text file and generates a report of different food
* items and their corresponding warehouses. report shows the most popular food and
* outputs which items are well stocked
*/


#include <vector>
#include <map>
#include <string>
#include "food_template.h"
#include "warehouse.h"
#include "boost/date_time/gregorian/gregorian_types.hpp"
#include <iostream>
#include <fstream>

/*
* removes all the expired food in all warehouses
*/
void remove_expired_products(std::map<std::string, warehouse> & warehouses, boost::gregorian::date current_date, std::map<std::string, food_template> & types_of_food);
/**
  * processes the food command from the input file and creates a new food template to
  * represent the new food item
*/
food_template handle_food_command(std::string command);
/*
* handle the warehouse command and create a new warehouse that correlates to the command
*
*/
warehouse handle_warehouse_command(std::string command, std::map<std::string,food_template> types_of_food);
/*
* handles the start date command and creates a gregorian date to represent the date
*
*/
boost::gregorian::date handle_start_date_command(std::string command);
/*
* updates the date object to reflect the next_day
*/
boost::gregorian::date handle_next_day_command(boost::gregorian::date current_date);

/*
* handles a recieve command for a given warehouse. Adds the corresponding amount of food.
*
*/
void handle_receive_command(std::string command, std::map<std::string, warehouse> & warehouses, boost::gregorian::date current_date, std::map<std::string, food_template> &types_of_food);
/*
* handles a request for a food item and removes the item from the corresponding warehouse if possible.
*
*/
void handle_request_command(std::string command, std::map<std::string, warehouse> & warehouses, std::map<std::string, food_template> &types_of_food);
/*
* method goes through all the different types of food and prints the most popular food item
*/
void print_most_popular(std::map<std::string, food_template> types_of_food);

  main(int argc, char** argv)
  {
  //a vector of all food_templates that are held in the warehouses
  std::map<std::string, food_template> types_of_food;
  //maps the warehouse by name
  std::map<std::string, warehouse> warehouses;
  //current date for recieve and request actions
  boost::gregorian::date current_date;

  //get file from command line
	std::ifstream input_file;
	input_file.open(argv[1]);
  //read in the file
	std::string line_input;
	if(input_file.is_open())
	{
	  bool end_of_file = false;
	    while(std::getline(input_file,line_input) && !end_of_file)
	    {
        //read file line by line
	      std::size_t first_space  = line_input.find_first_of(' ');
	      std::string command = line_input;
        //check for the end of the file
	      if(first_space != std::string::npos)
	      {
		        command = line_input.substr(0,first_space);
	      }
	      else
	      {
		        end_of_file = true;
	      }

        //treat each command differently
	        if(command.compare("FoodItem") ==0)
		      {
		          food_template new_template = (handle_food_command(line_input));
		          types_of_food[new_template.get_upc_code()] = new_template;
		      }
	        else if(command.compare("Warehouse") == 0)
		      {
		            warehouse new_warehouse = handle_warehouse_command(line_input, types_of_food);
		              warehouses[new_warehouse.get_name()]=new_warehouse;
		      }
	        else if(command.compare("Start") == 0)
		      {
		          current_date = handle_start_date_command(line_input);
		      }
	        else if(command.compare("Next") == 0)
		      {
		          current_date = handle_next_day_command(current_date);
              //remove_expired_products after each day
		          remove_expired_products(warehouses, current_date, types_of_food);
		      }
		      else if(command.compare("Request:")== 0)
		      {
		          handle_request_command(line_input, warehouses, types_of_food);
		      }
		      else if(command.compare("Receive:")== 0)
		      {
		          handle_receive_command(line_input, warehouses, current_date, types_of_food);
		      }

	    }
	    input_file.close();
	}

  //generate the output file
  std::cout <<  "Report by Kevin Lund and Robert King" << std::endl << std::endl;
  std::cout <<  "Unstocked Products:" << std::endl;
  //iterate all of the types of food and determine which are in stock
  for(std::map<std::string, food_template>::iterator it = types_of_food.begin(); it != types_of_food.end(); it++)
  {
    if((it->second).get_stocked_warehouses() == 0){
      std::cout << it->first << " " << it->second.get_name() << std::endl;
    }

  }

  //output for Well-Stocked food
  std::cout << std::endl <<  "Well-Stocked Products:" << std::endl;
  for(std::map<std::string, food_template>::iterator it = types_of_food.begin(); it != types_of_food.end(); it++)
  {
    if((it->second).get_stocked_warehouses() > 1){
      std::cout << it->first << " " << it->second.get_name() << std::endl;
    }

  }
  print_most_popular(types_of_food);
}

/**
  * processes the food command from the input file and creates a new food template to
  * represent the new food item
*/
food_template handle_food_command(std::string command)
{
  //get upc string
  int string_start = 21;
  int length = command.find_first_of(' ',21) - 21;
  std::string upc_code = command.substr(string_start, length);

  //get the shelf life
  string_start = command.find_first_of(":", 23) + 2;
  length = command.find_first_of(' ', string_start) - string_start;
  std::string shelf_life = command.substr(string_start, length);
  int i_shelf_life = atoi(shelf_life.c_str());

  //get the name
  string_start = command.find_first_of(":", string_start) + 2;
  length = (command.size()) - string_start ;
  std::string name = command.substr(string_start, length);

  food_template food(name, i_shelf_life, upc_code);
  return food;
}

/*
* handle the warehouse command and create a new warehouse that correlates to the command
*
*/
warehouse handle_warehouse_command(std::string command, std::map<std::string,food_template> types_of_food)
{
  int string_start = 12;
  int length = (command.size()) - string_start;
  std::string warehouse_name = command.substr(string_start, length);

  warehouse new_warehouse(warehouse_name, types_of_food);
  return new_warehouse;
}

/*
* handles the start date command and creates a gregorian date to represent the date
*
*/
boost::gregorian::date handle_start_date_command(std::string command)
{
  //get the month
  int string_start = 12;
  int length = command.find_first_of('/') - string_start;
  std::string input_month  = command.substr(string_start, length);
  boost::gregorian::greg_month month(atoi(input_month.c_str()));

  //get the day
  string_start = string_start+length+1;
  length = command.find_first_of('/',string_start) - string_start;
  std::string input_day  = command.substr(string_start, length);
  boost::gregorian::greg_day day(atoi(input_day.c_str()));

  //get the year
  string_start = string_start+length+1;
  length = (command.size()) - string_start;
  std::string input_year  = command.substr(string_start, length);
  boost::gregorian::greg_year year(atoi(input_year.c_str()));


  boost::gregorian::date current_date(year,month,day);

  return current_date;
}

/*
* updates the date object to reflect the next_day
*/
boost::gregorian::date handle_next_day_command(boost::gregorian::date current_date)
{
  boost::gregorian::date_duration next_day(1);
  return current_date + next_day;
}

/*
* handles a request for a food item and removes the item from the corresponding warehouse if possible.
*
*/
void handle_request_command(std::string command, std::map<std::string, warehouse> & warehouses, std::map<std::string, food_template> &types_of_food)
{
  //get upc code of food item to remove
  int string_start = 9;
  int length = command.find_first_of(' ',string_start) - string_start;
  std::string upc_code = command.substr(string_start, length);

  //get number of food items that are requested
  string_start = string_start + length + 1;
  length = command.find_first_of(' ',string_start) - string_start;
  std::string amount = command.substr(string_start, length);
  int quantity = atoi(amount.c_str());

  //get the warehouse where the request is preformed
  string_start = string_start + length + 1;
  length = (command.size()) - string_start;
  std::string warehouse = command.substr(string_start, length);

  warehouses[warehouse].request(upc_code, quantity, types_of_food);

}

/*
* removes all the expired food in all warehouses
*     Parameters:
*                 map warehouses: all warehouses stored
*                 date current_date: date used to check for expired Products
*                 map types_of_food: all the types of food to check
*/
void remove_expired_products(std::map<std::string, warehouse> & warehouses, boost::gregorian::date current_date, std::map<std::string, food_template> &types_of_food)
{
  for(std::map<std::string, warehouse>::iterator it = warehouses.begin(); it != warehouses.end(); it++)
  {
    (it->second).remove_expired(current_date, types_of_food);

  }
}

/*
* handles a recieve command for a given warehouse. Adds the corresponding amount of food.
*     Parameters:
*                 map warehouses: all warehouses stored
*                 date current_date: date used to for food objects added
*                 map types_of_food: all the types of food to check
*/
void handle_receive_command(std::string command, std::map<std::string, warehouse> & warehouses, boost::gregorian::date current_date, std::map<std::string, food_template> &types_of_food)
{
  //get upc code
  int string_start = 9;
  int length = command.find_first_of(' ',string_start) - string_start;
  std::string upc_code = command.substr(string_start, length);

  //get quantity of food that is recieved
  string_start = string_start + length + 1;
  length = command.find_first_of(' ',string_start) - string_start;
  std::string amount = command.substr(string_start, length);
  int quantity = atoi(amount.c_str());

  //get the warehouse where the food is recieved
  string_start = string_start + length + 1;
  length = ((command.size()) - string_start);
  std::string warehouse = command.substr(string_start, length);

  warehouses[warehouse].receive(current_date, upc_code, quantity, types_of_food);

}

/*
* method goes through all the different types of food and prints the most popular food item
*/
void print_most_popular(std::map<std::string, food_template> types_of_food){
  std::cout << std::endl <<  "Most Popular Products:" << std::endl;
  //if no food items exist
  for(int i = 0; i < 3; i++){
    if(types_of_food.size() == 0){
      return;
    }

    std::map<std::string, food_template>::iterator it1 = types_of_food.begin();
    food_template max = it1->second;

   for(std::map<std::string, food_template>::iterator it = types_of_food.begin(); it != types_of_food.end(); it++){
     if(it->second.get_times_requested() > max.get_times_requested()){
       max = it->second;
     }
  }
   types_of_food.erase(max.get_upc_code());
   std::cout << max.get_upc_code() << " " << max.get_name() << std::endl;

  }

}
