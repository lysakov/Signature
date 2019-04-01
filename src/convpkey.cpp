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

Private_key get_priv_key (const Elliptic_curve &param_set, const std::vector<char*> names)
{
    uint512_t priv_key_num;
    if (names.empty() || names.size() > 2) {
        throw std::runtime_error("Incorrect command line arguments.");
    }
    std::ifstream priv_key_f;
    priv_key_f.open(names[0]);
    if (!priv_key_f) {
        throw std::runtime_error("File with private key not found.");
    }
    std::string priv_key_s;
    std::getline(priv_key_f, priv_key_s);
    if (priv_key_s.size() == 2 * UINT256_SIZE && param_set.get() == PARAM_SET_256) {
        priv_key_num = extend(uint256_t(uint256_t::HEX, priv_key_s));
    } else {
        if (priv_key_s.size() == 2 * UINT512_SIZE && param_set.get() == PARAM_SET_512) {
            priv_key_num = uint512_t(uint512_t::HEX, priv_key_s);
        } else {
            throw std::runtime_error("Incorrect private key.");
        }
    }
    return Private_key(param_set, priv_key_num);
}

std::string convert_name (const std::string &input_name)
{
    std::string name = input_name;
    auto dot_pos = input_name.rfind(".");
    if (dot_pos != std::string::npos) {
        name.erase(dot_pos, input_name.size());
    }
    auto dir_pos = name.find("/");
    while (dir_pos != std::string::npos) {
        name.erase(0, name.find("/") + 1);
        dir_pos = name.find("/");
    }
    name = "keys/" + name;
    name += ".pub";
    return name;
}

int main (int argc, char **argv)
{
    try {
        Set mode = get_mode(argc, argv);
        Elliptic_curve param_set(mode);
        auto names = get_files(argc, argv);
        Private_key priv_key = get_priv_key(param_set, names);
        std::ofstream pub_key_f;
        if (names.size() > 1) {
            pub_key_f.open(names[1]);
        } else {
            pub_key_f.open(convert_name(names[0]));
        }
        Public_key pub_key(param_set, priv_key);
        pub_key_f << pub_key;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
