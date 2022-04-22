#include <stdio.h>
#include "rational.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

static Rational rtnl_simplify(Rational rtnl)
{
    assert(rtnl.denom != 0);
    int a = abs(rtnl.num);
    int b = abs(rtnl.denom);

    while(b!=0)
    {
        int tmp = b;
        b = a % b;
        a = tmp;
    }

    rtnl.num = rtnl.num / a;
    rtnl.denom = rtnl.denom / a;
    if(rtnl.denom < 0)
    {
        rtnl.num *= -1;
        rtnl.denom *= -1;
    }

    return rtnl;
}

extern Rational rtnl_add(Rational rtnl0, Rational rtnl1)
{
    Rational rtnlF;

    rtnlF.num = (rtnl0.num * rtnl1.denom) + (rtnl1.num * rtnl0.denom);
    rtnlF.denom = (rtnl0.denom * rtnl1.denom);

    rtnlF = rtnl_simplify(rtnlF);
    return rtnlF;
}
extern Rational rtnl_sub(Rational rtnl0, Rational rtnl1)
{
    Rational rtnlF;

    rtnlF.num = (rtnl0.num * rtnl1.denom) - (rtnl1.num * rtnl0.denom);
    rtnlF.denom = (rtnl0.denom * rtnl1.denom);

    rtnlF = rtnl_simplify(rtnlF);
    return rtnlF;
}
extern Rational rtnl_mul(Rational rtnl0, Rational rtnl1)
{
    Rational rtnlF;

    rtnlF.num = (rtnl0.num * rtnl1.num);
    rtnlF.denom = (rtnl0.denom * rtnl1.denom);

    rtnlF = rtnl_simplify(rtnlF);
    return rtnlF;
}
extern Rational rtnl_div(Rational rtnl0, Rational rtnl1)
{
    Rational rtnlF;

    rtnlF.num = (rtnl0.num * rtnl1.denom);
    rtnlF.denom = (rtnl0.denom * rtnl1.num);

    rtnlF = rtnl_simplify(rtnlF);
    return rtnlF;
}

extern Rational rtnl_init(int num, int denom)
{
    Rational rtnl;
    rtnl.num = num;
    rtnl.denom = denom;

    rtnl = rtnl_simplify(rtnl);
    return rtnl;
}

extern Rational rtnl_ipow(Rational rtnl, int ipow)
{

    if(ipow == 0)
    {
        Rational rtnlF;
        rtnlF.num = 1;
        rtnlF.denom = 1;
        return rtnlF;
    }
    else if(ipow>0)
    {
        Rational rtnlF = rtnl;
        for(int i=1; i < ipow; i++)
        {
            rtnlF = rtnl_mul(rtnlF, rtnl);
        }
        return rtnlF;
    }
    else
    {
        Rational rtnlI;
        rtnlI.num = rtnl.denom;
        rtnlI.denom = rtnl.num;
        Rational rtnlF = rtnlI;
        for(int i=-1; i > ipow; i--)
        {
            rtnlF = rtnl_mul(rtnlF, rtnlI);
        }
        return rtnlF;
    }
    
}

extern char *rtnl_asStr(Rational rtnl, char buf[RTNL_BUF_SIZE])
{
    sprintf(buf, "(%d/%d)", rtnl.num, rtnl.denom);
    // j += sprintf(buf, "/%d)", rtnl.denom);
    return buf;
}
