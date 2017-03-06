/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: marrius9876@gmail.com

*/

#ifndef RFCOMM_SOCKET_H
#define RFCOMM_SOCKET_H

#include "bt_socket.h"


class rfcomm_socket : public bt_socket
{
public:
     rfcomm_socket(hci_data_eater* hci):bt_socket(hci){}
     ~rfcomm_socket(){};

     void create();
     void bind(const bdaddr_t& addr, uint8_t channel, uint16_t psm=0, uint8_t cid=0);
};

#endif // RFCOMM_SOCKET_H
