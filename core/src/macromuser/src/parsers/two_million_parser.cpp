#include "parsers/two_million_parser.hpp"

#include "macromuser/filepaths.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

using namespace macromuse::core;

namespace
{
// TODO:
//  - Explore compiletime build up of regex objects:
//  https://stackoverflow.com/questions/40427152/compiletime-build-up-of-stdregex
// - Explore removing the need to copy for passing input to f_segment_list_string
// - Optimize reserving the recipe vector
// - Patch Recipe IDs

/**
 * @brief This function expects a list of strings from the csv and returns a new vector containing the split strings
 * @param list_string a string consisting of a list of strings with the brackets stripped. Example:
 * `""string_1"", ""string_2"", ..., ""string_n""`
 * @return std::vector<std::string> a new vector of strings containing the strings with the `""""` stripped. Example:
 * `[string_1, string_2, ..., string_n]`
 */
std::vector<std::string> f_segment_list_string(std::string const & list_string)
{
  std::vector<std::string> output = {};

  /*
   * In order to split this line we ar going to use the following regex:
   * `\"\"([^"]*)\"\",?` - POSIX ERE flavor
   * Validated against first list with regex101
   *
   * This works by searching for two back to back quotation marks then greedily capturing all characters up until the
   * next instance of a quotation mark up to the next comma optionally
   *
   * ASSUMPTIONS:
   *  - There are NO quotation mark characters in the strings themselves, escaped or otherwise. This WILL BREAK if
   * assumption not met
   *  - There will always be a comma character at the end of a string if another follows. Only the group before the
   * first instance of no comma at the end will be captured
   *
   * There is only one group to capture here so all matches will fall in group 1 with the double quotations and commas
   * stripped.
   */
  std::regex re_posix_ex(R"~(\"\"([^"]*)\"\",?)~", std::regex::extended);

  for (auto iter = std::sregex_iterator{list_string.cbegin(), list_string.cend(), re_posix_ex}, last = std::sregex_iterator{};
       iter != last;
       ++iter) {
    std::ssub_match const & g1match = (*iter)[1];
    output.emplace_back(g1match.first, g1match.second);
  }

  return output;
}

Recipe f_extract_recipe_from_line(std::string const & line)
{
  Recipe recipe{};

  /*
   * In order to split this line we ar going to use the following regex:
   * `\"\[([^]]*)\]\",|([^,]+),?` - POSIX ERE flavor
   * Validated against first line with regex101
   *
   * This works by attempting to greedily capture all characters between the list brackets
   * then falling back to greedily capturing all characters up to the next comma optionally.
   * This means values captured by group one will need to be processed again.
   * The comma is optional to capture the last match with the newline stripped from getline
   *
   * This will capture the groups in the following order:
   * Match 1: `title` in group 2
   * Match 2: `ingredients` in group 1
   * Match 3: `directions` in group 1
   * Match 4: `link` in group 2
   * Match 5: `source` in group 2
   * Match 6: `NER` in group 1
   * Match 7: `site` in group 2
   */
  std::regex re_posix_ex(R"~(\"\[([^]]*)\]\",|([^,]+),?)~", std::regex::extended);

  auto reg_begin = std::sregex_iterator(line.begin(), line.end(), re_posix_ex);
  auto reg_end   = std::sregex_iterator();

  // Match 1: `title` in group 2
  if (std::ssub_match const & title = (*reg_begin)[2]; title.matched) {
    recipe.title = {title.first, title.second};
  }
  reg_begin++;

  // Match 2: `ingredients` in group 1
  if (std::ssub_match const & ingredients = (*reg_begin)[1]; ingredients.matched) {
    recipe.ingredients = f_segment_list_string(ingredients.str());
  }
  reg_begin++;

  // Match 3: `directions` in group 1
  if (std::ssub_match const & directions = (*reg_begin)[1]; directions.matched) {
    recipe.instructions = f_segment_list_string(directions.str());
  }
  reg_begin++;

  // Match 4: `link` in group 2
  // not used currently

  // Match 5: `source` in group 2
  // not used currently

  // Match 6: `NER` in group 1
  // not used currently

  // Match 7: `site` in group 2
  // not used currently

  return recipe;
}
}    // namespace

namespace macromuse::core::parsers
{
std::string k_two_million_csv = "recipe-dataset-over-2m/dataset/recipes_data.csv";

int TwoMillionParser::load(std::vector<Recipe>& recipe_list)
{
  std::string targetFile = k_data_dir + "/" + k_two_million_csv;

  std::ifstream instrm(targetFile);

  if (!instrm.is_open()) {
    std::cout << targetFile << " does not exist!";
    return -1;
  }

  // find the size of the file
  size_t existing_recipe_count = recipe_list.size();
  // recipe_list.reserve(
  //   existing_recipe_count + std::count(std::istreambuf_iterator<char>(instrm), std::istreambuf_iterator<char>(), '\n'));

  /*
   * The file uses the following structuring:
   * `title,ingredients,directions,link,source,NER,site`
   *
   * Descibed in the readme they represent as follows:
   * `title` represents the title of the food dish
   * `ingredients` contains ingredients as scraped (raw)
   * `directions` contains instructions as scraped (raw)
   * `link` not mentioned in the docs but is the link to the source?
   * Possibly link against images folder. Could be interesting to integrate
   * down the line to expose.
   * `source` not mentioned in the docs but is a string representing
   * an enum of where it was obtained? Scrape data for more details later.
   * `NER` this seems to be the ingredients stripped of amounts and other data.
   * `site` this seems to be the host web address scraped from
   *
   * Example Line:
   * `No-Bake Nut Cookies,"[""1 c. firmly packed brown sugar"", ""1/2 c. evaporated milk"", ""1/2 tsp. vanilla"", ""1/2
   * c. broken nuts (pecans)"", ""2 Tbsp. butter or margarine"", ""3 1/2 c. bite size shredded rice biscuits""]","[""In
   * a heavy 2-quart saucepan, mix brown sugar, nuts, evaporated milk and butter or margarine."", ""Stir over medium
   * heat until mixture bubbles all over top."", ""Boil and stir 5 minutes more. Take off heat."", ""Stir in vanilla and
   * cereal; mix well."", ""Using 2 teaspoons, drop and shape into 30 clusters on wax paper."", ""Let stand until firm,
   * about 30 minutes.""]",www.cookbooks.com/Recipe-Details.aspx?id=44874,Gathered,"[""bite size shredded rice
   * biscuits"", ""vanilla"", ""brown sugar"", ""nuts"", ""milk"", ""butter""]",www.cookbooks.com`
   *
   * Tokens:
   * `No-Bake Nut Cookies`
   * `""1 c. firmly packed brown sugar"", ""1/2 c. evaporated milk"", ""1/2 tsp. vanilla"", ""1/2 c. broken nuts
   * (pecans)"", ""2 Tbsp. butter or margarine"", ""3 1/2 c. bite size shredded rice biscuits""`
   * `""In a heavy 2-quart saucepan, mix brown sugar, nuts, evaporated milk and butter or margarine."", ""Stir over
   * medium heat until mixture bubbles all over top."", ""Boil and stir 5 minutes more. Take off heat."", ""Stir in
   * vanilla and cereal; mix well."", ""Using 2 teaspoons, drop and shape into 30 clusters on wax paper."", ""Let stand
   * until firm, about 30 minutes.""` `www.cookbooks.com/Recipe-Details.aspx?id=44874` `Gathered`
   * `""bite size shredded rice biscuits"", ""vanilla"", ""brown sugar"", ""nuts"", ""milk"", ""butter""`
   * `www.cookbooks.com`
   */

  std::string line = "";

  // Rip headers
  std::getline(instrm, line, '\n');

  // REMOVE LATER
  size_t TEMPCAP = 10000;
  size_t start   = 0;

  // start reading main file
  while (std::getline(instrm, line, '\n')) {
    recipe_list.emplace_back(::f_extract_recipe_from_line(line));
    if (start++ >= TEMPCAP) {
      break;
    }
  }

  instrm.close();

  return 0;
}
}    // namespace macromuse::core::parsers
