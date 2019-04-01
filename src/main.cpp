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
    std::cout << argv[argc - 1] << '\n';
    try {
        Elliptic_curve param_set(PARAM_SET_256);
        Private_key d(param_set);
        Public_key Q(param_set, d);
        std::cout << Q << '\n';
        Signature::Context ctx(param_set, "bin/main.o");
        //std::cout << argv[argc - 1] << " : " << ctx.count_hash() << std::endl;
        //ctx.sign(d); 
        if (ctx.verify(Q, "signatures/main.crt")) {
            std::cout << "TRUE" << std::endl;
        } else {
            std::cout << "FALSE" << std::endl;
        } 
        //std::cout << d << '\n';
        //Public_key q(param_set, Private_key(param_set, param_set.q));
        //std::cout << q << '\n';
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
