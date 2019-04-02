#include "common.hpp"

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

Private_key get_priv_key (const Elliptic_curve &param_set, const std::vector<char*> &names)
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

Public_key get_public_key (const Elliptic_curve &param_set, const std::vector<char*> &names)
{
    uint512_t x, y;
    if (names.size() < 2 || names.size() > 3) {
        throw std::runtime_error("Incorrect command line arguments.");
    }
    std::ifstream pub_key_f;
    pub_key_f.open(names[0]);
    if (!pub_key_f) {
        throw std::runtime_error("File with private key not found.");
    }
    std::string pub_key_s_x;
    std::string pub_key_s_y;
    std::getline(pub_key_f, pub_key_s_x);
    std::getline(pub_key_f, pub_key_s_y);
    if (pub_key_s_x.size() != pub_key_s_y.size()) {
        throw std::runtime_error("Incorrect public key.");
    }
    if (pub_key_s_x.size() == 2 * UINT512_SIZE && param_set.get() == PARAM_SET_512) {
        x = uint512_t(uint512_t::HEX, pub_key_s_x);
        y = uint512_t(uint512_t::HEX, pub_key_s_y);
    } else {
        if (pub_key_s_x.size() == 2 * UINT256_SIZE && param_set.get() == PARAM_SET_256) {
            x = extend(uint256_t(uint256_t::HEX, pub_key_s_x));
            y = extend(uint256_t(uint256_t::HEX, pub_key_s_y));
        } else {
            throw std::runtime_error("Incorrect public key.");
        }
    }
    return Public_key(Point(&param_set, x, y));
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
