#include "signature.hpp"

using namespace Signature;

Context::Context (const Elliptic_curve &param_set, const std::string &input_name) : 
param_set(&param_set), h_ctx(Hash::Context::HASH_512, input_name), input_name(input_name)
{
    if (this->param_set->get() == PARAM_SET_256) {
        h_ctx.set_mode(Hash::Context::HASH_256);
    }
}

std::string Context::create_name (const std::string &output_name)
{
    if (output_name.empty()) {
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
        name = "signatures/" + name + ".crt";
        return name;
    }
    return output_name;
}

void Context::update () 
{
    h_ctx.update();
}

uint512_t Context::count_hash ()
{
    while (!h_ctx.eof()) {
        update();
    }
    uint512_t result = h_ctx.finish();
    if (this->param_set->get() == PARAM_SET_256) {
        hash = extend(result.split<256>()[1]);
    } else {
        hash = result;
    }
    return hash;
}   

void Context::sign (const Private_key &priv_key, const std::string &file_name)
{
    std::ofstream output(create_name(file_name));
    count_hash();
    uint512_t e = hash % param_set->q;
    if (e == (uint512_t)0) {
        e = 1;
    }
    uint512_t k;
    do {
        k = gen_rand_seq(*param_set);
        Point c = k * Point(param_set);
        r = c.get_x() % param_set->q;
    } while (r == (uint512_t)0);
    s = addition(multiplication(r, priv_key.get(), param_set->q), multiplication(k, e, param_set->q), param_set->q);
    if (param_set->get() == PARAM_SET_256) {
        output << r.split<256>()[0] << s.split<256>()[0] << std::endl;
    } else {
        output << r << s << std::endl;
    }
}

void Context::get_crt (const std::string &file_name)
{
    std::ifstream input(create_name(file_name));;
    if (!input) {
        throw std::runtime_error("File with signature doesn't exist.");
    }
    std::string signature;
    std::getline(input, signature);
    if (signature.size() != 4 * UINT512_SIZE && signature.size() != 4 * UINT256_SIZE) {
        throw std::runtime_error("Incorrect signature format.");
    }
    size_t size = param_set->get() == PARAM_SET_256 ? 2 * UINT256_SIZE : 2 * UINT512_SIZE;
    if (signature.size() == 4 * UINT512_SIZE && param_set->get() == PARAM_SET_512) {
        r = uint512_t(uint512_t::HEX, signature.substr(0, size));
        s = uint512_t(uint512_t::HEX, signature.substr(size, signature.size()));
    } else {
        if (signature.size() == 4 * UINT256_SIZE && param_set->get() == PARAM_SET_256) {
            r = extend(uint256_t(uint256_t::HEX, signature.substr(0, size)));
            s = extend(uint256_t(uint256_t::HEX, signature.substr(size, signature.size())));
        } else {
            throw std::runtime_error("Incorrect signature format.");
        }
    }
}

bool Context::verify (const Public_key &pub_key, const std::string &signature_file)
{
    get_crt(signature_file);
    if (r > param_set->q || s > param_set->q) {
        return false;
    }
    count_hash();
    uint512_t e = hash % param_set->q;
    uint512_t v = inverse(e, param_set->q);
    uint512_t z1 = multiplication(s, v, param_set->q);
    uint512_t z2 = opposite(multiplication(r, v, param_set->q), param_set->q);
    Point c = z1 * Point(param_set) + z2 * pub_key.get();
    if (c.get_x() % param_set->q == r) {
        return true;
    } else {
        return false;
    }
}
