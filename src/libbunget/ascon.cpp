/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: marrius9876@gmail.com

*/

#include "bu_hci.h"
#include "ascon.h"
#include "libbungetpriv.h"
using namespace bunget;

/****************************************************************************************
*/
bu_asc::bu_asc(bu_hci* hci, uint16_t handle,
                            const bdaddr_t& l,
                            int ltyp,
                            const bdaddr_t& r,
                            int rtyp):_handle(handle),_hci(hci)
{
    _secman = new secmanp(this, hci, l, ltyp, r, rtyp);
}

/****************************************************************************************
*/
bu_asc::~bu_asc()
{
    delete _secman;
}

/****************************************************************************************
*/
void bu_asc::write(uint16_t cid, const bybuff& data)
{
    sdata sd;
    sd.data = data.buffer();
    sd.len = uint16_t(data.length());
#ifdef ONGOING
    _hci->enque_acl(_handle, cid, sd);
#else
    _hci->write_ack_packet(_handle, cid, sd);
#endif

}

/****************************************************************************************
*/
void bu_asc::push(uint16_t cid,  const sdata& data)
{
    if (data.len)
    {
        _hci->srv()->feed_them(cid, data);
    }
}

/****************************************************************************************
*/
void  bu_asc::set_he(uint16_t handle, uint8_t encrypt)
{
    _encrypted = encrypt ? 1 : 0;
    _secman->on_encryption_changed(_encrypted);
};

/****************************************************************************************
*/
void bu_asc::set_ltk_r()
{
    _secman->on_ltk_neg();
}
