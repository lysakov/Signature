#include "common.hpp"

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
