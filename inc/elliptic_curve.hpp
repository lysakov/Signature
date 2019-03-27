#ifndef ELLIPTIC_CURVE
#define ELLIPTIC_CURVE

#include "block.hpp"
#include "modular_arithmetic.hpp"

enum Set {
    PARAM_SET_512,
    PARAM_SET_256
};

class Elliptic_curve
{

  public :
    uint512_t p;
    uint512_t a;
    uint512_t b;
    uint512_t e;
    uint512_t d;
    uint512_t m;
    uint512_t q;
    uint512_t x;
    uint512_t y;
    uint512_t u;
    uint512_t v;
    Elliptic_curve (Set set = PARAM_SET_512);
    Set get () const;

  private :
    Set set;

};  

std::ostream& operator << (std::ostream&, const Elliptic_curve&);

class Point
{

  public :
    Point ();
    Point (const Elliptic_curve*);
    Point (const Elliptic_curve*, const uint512_t&, const uint512_t&);
    Point operator + (const Point&);
    Point& operator ++ ();
    Point convert ();
    friend std::ostream& operator << (std::ostream&, const Point&);

  private :
    const Elliptic_curve *param_set;
    uint512_t x;
    uint512_t y;

};

class Private_key
{

  public :
    Private_key (const Elliptic_curve&);
    Private_key (const Elliptic_curve&, const uint512_t&);
    uint512_t get () const;
    friend std::ostream& operator << (std::ostream&, const Private_key&);

  private :
    const Elliptic_curve *param_set;
    uint512_t key;

};

class Public_key
{

  public :
    Public_key (const Elliptic_curve&, const Private_key&);
    Public_key (const Point&);
    friend std::ostream& operator << (std::ostream&, const Public_key&);

  private :
    Point key;

};

#endif
