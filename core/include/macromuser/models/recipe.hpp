#pragma once

#include "macromuser/models/macronutrients.hpp"

#include <string>
#include <vector>

namespace macromuse::core
{
struct Recipe
{
  size_t id;
  std::string title;
  std::string instructions;
  std::vector<std::string> ingredients;

  Macronutrients nutrition;
};
}    // namespace macromuse::core
