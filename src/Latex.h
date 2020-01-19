#pragma once
#include "color.h"

#include <string>
#include <vector>

std::string GetLatex(const std::string& title, 
					 const std::vector<std::string>& problems, 
					 int boardSize);