/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: marrius9876@gmail.com

*/

#ifndef ASYNCCONNLESS_H
#define ASYNCCONNLESS_H

#include <stdint.h>
#include "include/bluetooth.h"
#include "secmanp.h"
#include "bu_hci.h"
#include "bybuff.h"

class bu_asc
{
public:
    bu_asc(bu_hci* hci, uint16_t handle, const bdaddr_t& local, int ltyp,  const bdaddr_t& remote, int rtyp);
    virtual ~bu_asc();
    void write(uint16_t cid, const bybuff& buffer);
    void push(uint16_t cid, const sdata& buffer);
    void set_he(uint16_t handle, uint8_t encrypt);
    void set_ltk_r();
    bool is_encrypted()const{return _encrypted;}
private:
    uint16_t    _handle;
    bu_hci*     _hci;
    bool        _encrypted;
    secmanp*    _secman;
};

#endif // ASYNCCONNLESS_H
