#include <fstream>
#include <vector>
#include "block.hpp"
#include "elliptic_curve.hpp"

Set get_mode (int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-s") {
            return PARAM_SET_256;
        }
    }
    return PARAM_SET_512;
}

std::vector<char*> get_files (int argc, char **argv)
{
    std::vector<char*> names;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) != "-s") {
            names.push_back(argv[i]);
        }
    }
    return names;
}

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
