#include "common.hpp"

int main (int argc, char **argv)
{
    try {
        Set mode = get_mode(argc, argv);
        Elliptic_curve param_set(mode);
        auto names = get_files(argc, argv);
        auto pub_key = get_public_key(param_set, names);
        if (names.size() < 3) {
            names.push_back((char*)"");
        }
        Signature::Context ctx(param_set, names[1]);
        if (ctx.verify(pub_key, names[2])) {
            std::cout << "SUCCESS" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
