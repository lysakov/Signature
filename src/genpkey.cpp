#include "common.hpp"

int main (int argc, char **argv)
{
    try {
        Set mode = get_mode(argc, argv);
        Elliptic_curve param_set(mode);
        std::ofstream priv_key_f;
        Private_key priv_key(param_set);
        auto names = get_files(argc, argv);
        if (names.empty() || names.size() > 1) {
            throw std::runtime_error("Incorrect command line arguments.");
        }
        priv_key_f.open(names[0]);
        priv_key_f << priv_key;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
