#pragma once

#include "macromuser/parsers/csv_parser.hpp"

#include <string>

namespace macromuse::core::parsers
{
class TwoMillionParser : public CSVParser
{
public:
  int load(std::vector<Recipe>& recipeList);
};
}    // namespace macromuse::core::parsers
