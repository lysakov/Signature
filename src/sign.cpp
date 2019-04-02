#include "common.hpp"

int main (int argc, char **argv)
{
    try {
        Set mode = get_mode(argc, argv);
        Elliptic_curve param_set(mode);
        auto names = get_files(argc, argv);
        auto priv_key = get_priv_key(param_set, names);
        if (names.size() < 3) {
            names.push_back((char*)"");
        }
        Signature::Context ctx(param_set, names[1]);
        ctx.sign(priv_key, names[2]);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
