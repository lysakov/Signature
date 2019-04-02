#ifndef COMMON
#define COMMON

#include <fstream>
#include <vector>
#include "block.hpp"
#include "elliptic_curve.hpp"
#include "signature.hpp"

Set get_mode (int, char**);
std::vector<char*> get_files (int, char**);
Private_key get_priv_key (const Elliptic_curve&, const std::vector<char*>&);
Public_key get_public_key (const Elliptic_curve&, const std::vector<char*>&);
std::string convert_name (const std::string&);

#endif
