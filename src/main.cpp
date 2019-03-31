#include <fstream>
#include <iostream>
#include "hash.hpp"
#include "modular_arithmetic.hpp"
#include "elliptic_curve.hpp"
#include "signature.hpp"

using namespace Block;
using namespace Hash;

int main (int argc, char **argv)
{
    try {
        Elliptic_curve param_set(PARAM_SET_256);
        Private_key d(param_set);
        Signature::Context ctx(param_set, argv[argc - 1]);
        //std::cout << argv[argc - 1] << " : " << ctx.count_hash() << std::endl;
        ctx.sign(d); 
        //std::cout << d << '\n';
        //Public_key q(param_set, Private_key(param_set, param_set.q));
        //std::cout << q << '\n';
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
