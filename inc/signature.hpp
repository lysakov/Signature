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
        Context (const Elliptic_curve&, const std::string&, const std::string& = "");
        void update ();
        void sign (const Private_key&);
        uint512_t count_hash ();

      private :
        const Elliptic_curve *param_set;
        std::ofstream output;
        Hash::Context h_ctx;
        uint512_t hash;
        uint512_t r;
        uint512_t s;

    };
}

#endif
