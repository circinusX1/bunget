/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: marrius9876@gmail.com

*/

#ifndef CRYPTOS_H
#define CRYPTOS_H

#include "bybuff.h"

class cryptos
{
public:
    cryptos();
    virtual ~cryptos();
    void gen_random(int ammount, bybuff&  dst);
    void s1(bybuff& tk, bybuff& r1, bybuff& r2, bybuff& stk);
    void c1(bybuff& k, bybuff& r, bybuff& pres, bybuff& preq, bybuff& iat, bybuff& ia, bybuff& rat, bybuff& ra,  bybuff&  dst) ;

private:
    void _eee(bybuff& key, bybuff& data, bybuff& out);
    void _oxor(const bybuff& src1, const bybuff& src2, bybuff& dst);
    void _sha_encrypt(const bybuff& key, const bybuff& data, bybuff& out);
    void _sha_decrypt(const bybuff& key, const bybuff& data, bybuff& out);
};

#endif // CRYPTOS_H
