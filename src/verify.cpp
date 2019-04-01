#include "block.hpp"
#include "signature.hpp"

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

Public_key get_public_key (const Elliptic_curve &param_set, const std::vector<char*> names)
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
