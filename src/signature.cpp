#include "signature.hpp"

using namespace Signature;

Context::Context (const Elliptic_curve &param_set, const std::string &input_name, const std::string &output_name) : 
param_set(&param_set), h_ctx(Hash::Context::HASH_512, input_name)
{
    if (this->param_set->get() == PARAM_SET_256) {
        h_ctx.set_mode(Hash::Context::HASH_256);
    }
    if (output_name.empty()) {
        std::string name = input_name;
        auto dot_pos = input_name.rfind(".");
        name.erase(dot_pos, input_name.size());
        auto dir_pos = name.find("/");
        while (dir_pos != std::string::npos) {
            name.erase(0, name.find("/") + 1);
            dir_pos = name.find("/");
        }
        name = "signatures/" + name + ".crt";
        output.open(name);
    } else {
        output.open(output_name);
    }
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

void Context::sign (const Private_key &priv_key)
{
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
