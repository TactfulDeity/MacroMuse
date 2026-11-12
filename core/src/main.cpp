#include "parsers/two_million_parser.hpp"

#include <algorithm>
#include <iostream>

using namespace macromuse::core;

namespace
{
std::string f_dump_string_list(std::vector<std::string> const & stringList)
{
  std::string output = "";
  std::for_each(stringList.begin(), stringList.end(), [&output](std::string const & i_string) {
    output += ("- " + i_string + "\n");
  });
  return output;
}

void print_recipe(size_t index, std::vector<Recipe> const & recipe_list)
{
  Recipe const & selected = recipe_list[index];

  std::cout << "--------- Recipe " << index << " ---------" << std::endl;
  std::cout << "ID: " << selected.id << std::endl;
  std::cout << "Name: " << selected.title << std::endl;
  std::cout
    << "Ingredients List: "
    << "(size: "
    << selected.ingredients.size()
    << ")\n"
    << ::f_dump_string_list(selected.ingredients)
    << std::endl;
  std::cout
    << "Instructions: "
    << "(size: "
    << selected.instructions.size()
    << ")\n"
    << f_dump_string_list(selected.instructions)
    << std::endl;
  std::cout << "\n" << std::endl;
}

void print_x_recipes(size_t num_to_print, std::vector<Recipe> const & recipe_list)
{
  std::cout << "Size of recipe list: " << recipe_list.size() << std::endl;
  for (size_t i = 0; i < num_to_print; i++) {
    if (i == recipe_list.size()) {
      break;
    }

    print_recipe(i, recipe_list);
  }
}
}    // namespace

int main()
{
  std::vector<Recipe> recipe_list(0);

  // Load the data via the csv parser
  parsers::TwoMillionParser parser{};
  int result = parser.load(recipe_list);
  if (result < 0) {
    return result;
  };

  print_x_recipes(5, recipe_list);

  print_recipe(200, recipe_list);

  print_recipe(9995, recipe_list);

  // Tokenize inputs with the pipeline

  // Run the ML Engine to categorize dataset

  // Determine the nearest cluster from input data

  // Export the nearest cluster

  return 0;
}
