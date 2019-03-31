#include "elliptic_curve.hpp"

extern "C" int rand_bytes (uint8_t*, int);

Elliptic_curve::Elliptic_curve (Set set) : set(set)
{
    if (set == PARAM_SET_512) {
        p = uint512_t(uint512_t::HEX, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC7");
        a = uint512_t(uint512_t::HEX, "DC9203E514A721875485A529D2C722FB187BC8980EB866644DE41C68E143064546E861C0E2C9EDD92ADE71F46FCF50FF2AD97F951FDA9F2A2EB6546F39689BD3");
        b = uint512_t(uint512_t::HEX, "B4C4EE28CEBC6C2C8AC12952CF37F16AC7EFB6A9F69F4B57FFDA2E4F0DE5ADE038CBC2FFF719D2C18DE0284B8BFEF3B52B8CC7A5F5BF0A3C8D2319A5312557E1");
        e = 1;
        d = uint512_t(uint512_t::HEX, "9E4F5D8C017D8D9F13A5CF3CDF5BFE4DAB402D54198E31EBDE28A0621050439CA6B39E0A515C06B304E2CE43E79E369E91A0CFC2BC2A22B4CA302DBB33EE7550");
        m = uint512_t(uint512_t::HEX, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF26336E91941AAC0130CEA7FD451D40B323B6A79E9DA6849A5188F3BD1FC08FB4");
        q = uint512_t(uint512_t::HEX, "3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC98CDBA46506AB004C33A9FF5147502CC8EDA9E7A769A12694623CEF47F023ED");
        x = uint512_t(uint512_t::HEX, "E2E31EDFC23DE7BDEBE241CE593EF5DE2295B7A9CBAEF021D385F7074CEA043AA27272A7AE602BF2A7B9033DB9ED3610C6FB85487EAE97AAC5BC7928C1950148");
        y = uint512_t(uint512_t::HEX, "F5CE40D95B5EB899ABBCCFF5911CB8577939804D6527378B8C108C3D2090FF9BE18E2D33E3021ED2EF32D85822423B6304F726AA854BAE07D0396E9A9ADDC40F");
        u = 0x12; 
        v = uint512_t(uint512_t::HEX, "469AF79D1FB1F5E16B99592B77A01E2A0FDFB0D01794368D9A56117F7B38669522DD4B650CF789EEBF068C5D139732F0905622C04B2BAAE7600303EE73001A3D");
    } else {
        p = extend(uint256_t(uint256_t::HEX, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97"));
        a = extend(uint256_t(uint256_t::HEX, "C2173F1513981673AF4892C23035A27CE25E2013BF95AA33B22C656F277E7335"));
        b = extend(uint256_t(uint256_t::HEX, "295F9BAE7428ED9CCC20E7C359A9D41A22FCCD9108E17BF7BA9337A6F8AE9513"));
        e = 1; 
        d = extend(uint256_t(uint256_t::HEX, "0605F6B7C183FA81578BC39CFAD518132B9DF62897009AF7E522C32D6DC7BFFB"));
        m = uint512_t(uint512_t::HEX, "0000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000003F63377F21ED98D70456BD55B0D8319C");
        q = extend(uint256_t(uint256_t::HEX, "400000000000000000000000000000000FD8CDDFC87B6635C115AF556C360C67"));
        x = extend(uint256_t(uint256_t::HEX, "91E38443A5E82C0D880923425712B2BB658B9196932E02C78B2582FE742DAA28"));
        y = extend(uint256_t(uint256_t::HEX, "32879423AB1A0375895786C4BB46E9565FDE0B5344766740AF268ADB32322E5C"));
        u = 0x0D; 
        v = extend(uint256_t(uint256_t::HEX, "60CA1E32AA475B348488C38FAB07649CE7EF8DBE87F22E81F92B2592DBA300E7"));
    }
}

Set Elliptic_curve::get () const
{
    return set;
}

std::ostream& operator << (std::ostream &str, const Elliptic_curve &param_set)
{
    str << "p = " << param_set.p << std::endl;
    str << "a = " << param_set.a << std::endl;
    str << "b = " << param_set.b << std::endl;
    str << "e = " << param_set.e << std::endl;
    str << "d = " << param_set.d << std::endl;
    str << "m = " << param_set.m << std::endl;
    str << "q = " << param_set.q << std::endl;
    str << "x = " << param_set.x << std::endl;
    str << "y = " << param_set.y << std::endl;
    str << "u = " << param_set.u << std::endl;
    str << "v = " << param_set.v;
    return str;
}

Point::Point () : param_set(nullptr)
{}

Point::Point (const Elliptic_curve *param_set) : param_set(param_set)
{
    x = param_set->u;
    y = param_set->v;
}

Point::Point (const Elliptic_curve *param_set, const uint512_t &x, const uint512_t &y) : Point(param_set)
{
    this->x = x;
    this->y = y;
}

Point Point::operator + (const Point &point)
{
    auto p = param_set->p;
    auto x1y2_x2y1 = addition(multiplication(x, point.y, p), multiplication(point.x, y, p), p);
    auto y1y2ax1x2 = substraction(multiplication(y, point.y, p), multiplication(param_set->e, multiplication(x, point.x, p), p), p);
    auto dx1x2y1y2 = multiplication(param_set->d, multiplication(x, multiplication(point.x, multiplication(y, point.y, p), p), p), p);
    auto new_x = multiplication(x1y2_x2y1, inverse(addition(1, dx1x2y1y2, p), p), p);
    auto new_y = multiplication(y1y2ax1x2, inverse(substraction(1, dx1x2y1y2, p), p), p);
    return Point(param_set, new_x, new_y);
}

std::ostream& operator << (std::ostream &str, const Point &point)
{
    if (point.param_set->get() == PARAM_SET_512) {
        str << point.x << std::endl;
        str << point.y;
    } else {
        str << point.x.split<256>()[0] << std::endl;
        str << point.y.split<256>()[0];
    } 
    return str;
}

Point& Point::operator ++ ()
{
    *this = *this + *this;
    return *this;
}

Point Point::convert ()
{
    auto p = param_set->p;
    auto s = multiplication(substraction(param_set->e, param_set->d, p), inverse(4, p), p);
    auto t = multiplication(addition(param_set->e, param_set->d, p), inverse(6, p), p);
    auto s_v = multiplication(s, addition(1, y, p), p);
    auto new_x = addition(t, multiplication(s_v, inverse(substraction(1, y, p), p), p), p);
    auto new_y = multiplication(s_v, inverse(multiplication(x, substraction(1, y, p), p), p), p);
    return Point(nullptr, new_x, new_y);
}

uint512_t Point::get_x () const noexcept
{
    return x;
}

uint512_t Point::get_y () const noexcept
{
    return y;
}

Point operator * (const uint512_t &k, const Point &p)
{
    Point pow[8 * UINT512_SIZE];
    pow[0] = p;
    Point result(p.param_set, 0, 1);
    for (size_t i = 1; i < 8 * UINT512_SIZE; ++i) {
        pow[i] = pow[i - 1] + pow[i - 1];
    }
    for (size_t i = 0; i < 8 * UINT512_SIZE; ++i) {
        if (k[i]) {
            result = result + pow[i];
        }
    }
    return result;
}

uint512_t gen_rand_seq (const Elliptic_curve &param_set)
{
    uint512_t seq;
    uint8_t seq_arr[UINT512_SIZE] = {0};
    int arr_size = param_set.get() == PARAM_SET_512 ? UINT512_SIZE : UINT256_SIZE;
    do {
        rand_bytes(seq_arr, arr_size);
        seq = uint512_t(seq_arr, UINT512_SIZE);
    } while (seq == (uint512_t)0 || (seq > param_set.q));
    return seq;
}

Private_key::Private_key (const Elliptic_curve &param_set) : param_set(&param_set)
{
    key = gen_rand_seq(param_set);
}

Private_key::Private_key (const Elliptic_curve &param_set, const uint512_t &key) : param_set(&param_set), key(key)
{}
    
uint512_t Private_key::get () const
{
    return key;
}

std::ostream& operator << (std::ostream &str, const Private_key &priv_key)
{
    if (priv_key.param_set->get() == PARAM_SET_512) {
        str << priv_key.key;
    } else {
        str << priv_key.key.split<256>()[0];
    }
    return str;
}

Public_key::Public_key (const Elliptic_curve &param_set, const Private_key &priv_key) : key(&param_set, 0, 1)
{
    key = priv_key.get() * Point(&param_set);
}

Public_key::Public_key (const Point &point) : key(point)
{}

std::ostream& operator << (std::ostream &str, const Public_key &pub_key)
{
    str << pub_key.key;
    return str;
}
