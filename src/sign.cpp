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

std::string convert_name (const std::string &input_name)
{
    std::string name = input_name;
    auto dot_pos = input_name.rfind(".");
    name.erase(dot_pos, input_name.size());
    name.erase(0, name.find("/"));
    name += ".crt";
    return name;
}

Private_key get_priv_key (const Elliptic_curve &param_set, const std::vector<char*> names)
{
    if (names.size() < 2 || names.size() > 3) {
        throw std::runtime_error("Incorrect command line arguments.");
    }
    std::ifstream priv_key_f;
    priv_key_f.open(names[0]);
    if (!priv_key_f) {
        throw std::runtime_error("File with private key not found.");
    }
    std::string priv_key_s;
    std::getline(priv_key_f, priv_key_s);
    while (priv_key_s.size() < UINT512_SIZE * 2) {
        priv_key_s = "0" + priv_key_s;
    }
    uint512_t priv_key_num(uint512_t::HEX, priv_key_s);
    return Private_key(param_set, priv_key_num);
}

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
        Signature::Context ctx(param_set, names[1], names[2]);
        ctx.sign(priv_key);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
