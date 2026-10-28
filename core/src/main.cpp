#include "parsers/two_million_parser.hpp"

#include <algorithm>
#include <iostream>

using namespace macromuse::core;
namespace {
std::string f_dump_ingredients(const std::vector<std::string>& ingredientsList)
{
  std::string output = "";
  std::for_each(ingredientsList.begin(), ingredientsList.end(), [&output](const std::string& i_string) {
    output += i_string;
  });
  return output;
}

void print_x_recipes(size_t num_to_print, const std::vector<Recipe>& recipeList)
{
  std::cout << "Size of recipe list: " << recipeList.size() << std::endl;
  for(size_t i = 0; i < num_to_print; i++)
  {
    if(i == recipeList.size())
    {
      break;
    }

    std::cout << "--------- Recipe " << i << " ---------" << std::endl;
    std::cout << "ID: " << recipeList[i].id << std::endl;
    std::cout << "Name: " << recipeList[i].title << std::endl;
    std::cout << "Ingredients List: " << ::f_dump_ingredients(recipeList[i].ingredients) << std::endl;
    std::cout << "Instructions: " << recipeList[i].instructions << std::endl;
    std::cout << "\n" << std::endl;
  }
}
}

int main() {

  std::vector<Recipe> recipeList(0);

  // Load the data via the csv parser
  parsers::TwoMillionParser parser{};
  int result = parser.load(recipeList);
  if(result < 0) {
    return result;
  };

  print_x_recipes(5, recipeList);

  // Tokenize inputs with the pipeline

  // Run the ML Engine to categorize dataset

  // Determine the nearest cluster from input data

  // Export the nearest cluster

  return 0;
}