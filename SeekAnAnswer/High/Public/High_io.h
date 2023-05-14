#pragma once
#include <iostream>
#include <string>
#include "High_int.h"
#include "High_float.h"

std::ostream& operator<<(std::ostream& cout, High_int data);
std::istream& operator>>(std::istream& cin, High_int& data);
std::ostream& operator<<(std::ostream& cout, High_float data);
std::istream& operator>>(std::istream& cin, High_float& data);
