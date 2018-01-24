/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: marrius9876@gmail.com

*/

#ifndef _LEBINENDS_
#define _LEBINENDS_

#include "include/bluetooth.h"

inline int is_little_endian()
{
    short a = 1;
    return *((char*)&a) & 1;
}









#endif //_LEBINENDS_
