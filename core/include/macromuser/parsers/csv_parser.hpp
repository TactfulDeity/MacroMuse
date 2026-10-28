#pragma once

#include "macromuser/models/recipe.hpp"

namespace macromuse::core {
class CSVParser {
public:
  virtual int load(std::vector<Recipe>& recipeList) = 0;
};
}