#ifndef SIGNATURE
#define SIGNATURE

#include <fstream>
#include "block.hpp"
#include "elliptic_curve.hpp"
#include "hash.hpp"

namespace Signature
{
    class Context
    {

      public :
        Context (const Elliptic_curve&, const std::string&);
        void update ();
        void sign (const Private_key&, const std::string& = "");
        bool verify (const Public_key&, const std::string& = "");
        uint512_t count_hash ();

      private :
        const Elliptic_curve *param_set;
        Hash::Context h_ctx;
        std::string input_name;
        uint512_t hash;
        uint512_t r;
        uint512_t s;
        std::string create_name (const std::string&);
        void get_crt (const std::string&);

    };
}

#endif
