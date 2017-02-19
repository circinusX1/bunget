/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: zyraxes.comatgmaildotcom.

*/

#include "sco_socket.h"

/****************************************************************************************
*/
void sco_socket::create()
{

    _sock = ::socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_SCO);
    if(_sock<=0) _THROW(0);

}

/****************************************************************************************
*/
void sco_socket::bind(const bdaddr_t& src, uint8_t srctype, uint16_t psm, uint8_t cid)
{
    struct sockaddr_sco addr;

    memset(&addr, 0, sizeof(addr));
    addr.sco_family = AF_BLUETOOTH;
    bacpy(&addr.sco_bdaddr, &src);

    if (::bind(_sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        _THROW(0);
    }
}
