/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: zyraxes.comatgmaildotcom.

*/
#include "uguid.h"
#include "bu_hci.h"
#include "hci_socket.h"
#include "l2cap_socket.h"
#include "include/libbunget.h"


/****************************************************************************************
*/
hci_socket_ble::~hci_socket_ble()
{
#ifdef USE_UVLIB
    ::uv_close((uv_handle_t*)&this->_pollHandle, (uv_close_cb)hci_socket_ble::uv_this_cb_close);
#endif //
}

/****************************************************************************************
*/
void hci_socket_ble::stop()
{
#ifdef USE_UVLIB
    ::uv_poll_stop(&this->_pollHandle);
#endif //
}

/****************************************************************************************
*/
void hci_socket_ble::create()
{
    _checkfreq = 0;
    _sock = ::socket(AF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC, BTPROTO_HCI);
    if(_sock<=0) _THROW(0);
#ifdef USE_UVLIB
    ::uv_poll_init(uv_default_loop(), &this->_pollHandle, this->_sock);
    this->_pollHandle.data = this;
#endif //
}

/****************************************************************************************
*/
void hci_socket_ble::create_bind(int dev_id)
{
	if (dev_id < 0)
	{
		_THROW(0);
	}

	_sock= socket(AF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC, BTPROTO_HCI);
 	if(_sock<=0) _THROW(0);

	struct sockaddr_hci a;
	::memset(&a, 0, sizeof(a));
	a.hci_family = AF_BLUETOOTH;
	a.hci_dev = dev_id;
	if (::bind(_sock, (struct sockaddr *) &a, sizeof(a)) < 0)
	{
        close();
        _THROW(0);
	}
}

/****************************************************************************************
*/

#ifdef USE_UVLIB
void hci_socket_ble::uv_this_cb_close(uv_poll_t* handle)
{
    delete handle;
}


/****************************************************************************************
*/
void hci_socket_ble::uv_this_cb(uv_poll_t* handle, int status, int events)
{
    hci_socket_ble *p = (hci_socket_ble*)handle->data;
    p->_notify_read();
}
#endif //

/****************************************************************************************
*/
int hci_socket_ble::bind_raw(int* devId)
{
    struct sockaddr_hci a;

    ::memset(&a, 0, sizeof(a));
    a.hci_family = AF_BLUETOOTH;
    a.hci_dev = this->_resolve_devid(devId, true);
    a.hci_channel = HCI_CHANNEL_RAW;
    this->_devId = a.hci_dev;
    this->_mode = HCI_CHANNEL_RAW;

    if(::bind(this->_sock, (struct sockaddr *) &a, sizeof(a))<0)
    {
        if(_loops==0)
        {
            _THROW("Device unplugged !!");
        }
        else
        {
            hci_error e;
            e.nerror = errno;
            e.message="unplugged";
            _hci->on_error(e);
        }
    }
    _loops++;
    return this->_devId;
}

/****************************************************************************************
*/
int hci_socket_ble::bind_user(int* devId)
{
    struct sockaddr_hci a;

    ::memset(&a, 0, sizeof(a));
    a.hci_family = AF_BLUETOOTH;
    a.hci_dev = this->_resolve_devid(devId, false);
    a.hci_channel = HCI_CHANNEL_USER;
    this->_devId = a.hci_dev;
    this->_mode = HCI_CHANNEL_USER;
    if(::bind(this->_sock, (struct sockaddr *) &a, sizeof(a))<0)
    {
        _THROW(0);
    }
    return this->_devId;
}

/****************************************************************************************
*/
void hci_socket_ble::bind_ctrl()
{
    struct sockaddr_hci a;

    ::memset(&a, 0, sizeof(a));
    a.hci_family = AF_BLUETOOTH;
    a.hci_dev = HCI_DEV_NONE;
    a.hci_channel = HCI_CHANNEL_CONTROL;
    this->_mode = HCI_CHANNEL_CONTROL;
    if(::bind(this->_sock, (struct sockaddr *) &a, sizeof(a))<0)
    {
        _THROW(0);
    }
}

/****************************************************************************************
*/
bool hci_socket_ble::is_up()
{
    struct hci_dev_info di;
    bool cacheup = false;

    ::memset(&di, 0x00, sizeof(di));
    di.dev_id = this->_devId;
    if (::ioctl(this->_sock, HCIGETDEVINFO, (void *)&di) > -1)
    {
        cacheup = (di.flags & (1 << HCI_UP)) != 0;
    }
    return cacheup;
}

/****************************************************************************************
*/
void hci_socket_ble::set_filter(const uint8_t* data, int length)
{
    bybuff buffer(data, length);
    if (::setsockopt(this->_sock, SOL_HCI, HCI_FILTER, data, length) < 0)
    {
        _THROW(0);
    }
}

/****************************************************************************************
*/
int hci_socket_ble::read(uint8_t* buffer, int sizeb)
{
    int length = bt_socket::read(buffer, sizeb);
    if (length > 0 && this->_mode == HCI_CHANNEL_RAW)
    {
        /// TODO deal with random disconnects ?!?
    }
    return length;
}

/****************************************************************************************
*/
int hci_socket_ble::_resolve_devid(int* pDevId, bool isUp)
{
    int devId = 0; // default

    if (*pDevId == -1)/* pick first one*/
    {
        struct hci_dev_list_req *dl;
        struct hci_dev_req *dr;

        dl = (hci_dev_list_req*)calloc(HCI_MAX_DEV * sizeof(*dr) + sizeof(*dl), 1);
        dr = dl->dev_req;

        dl->dev_num = HCI_MAX_DEV;

        if (::ioctl(this->_sock, HCIGETDEVLIST, dl) > -1)
        {
            for (int i = 0; i < dl->dev_num; i++, dr++)
            {
                bool devUp = dr->dev_opt & (1 << HCI_UP);
                bool match = isUp ? devUp : !devUp;

                if (match)
                {
                    // choose the first device that is match
                    // later on, it would be good to also HCIGETDEVINFO and check the HCI_RAW flag
                    devId = dr->dev_id;
                    *pDevId = devId;
                    break;
                }
            }
        }

        free(dl);
    }
    else
    {
        devId = *pDevId;
    }
    return devId;
}

/****************************************************************************************
*/
bool hci_socket_ble::pool(int* pbytes)
{
    fd_set          read;
    struct timeval  to;

    if(pbytes){
        *pbytes=0;
    }

    FD_ZERO (&read);
    FD_SET (_sock, &read);

    to.tv_sec = 0;
    to.tv_usec = 2000;
    int rv = ::select(_sock+1,&read,0,0,&to);
    if(rv < 0)
    {
        hci_error e;
        e.nerror = errno;
        e.message="network-error";
        _hci->on_error(e);
        return false;
    }
    if(rv==0)
    {
        ::usleep(100);
        return _hci->onSpin(this);
    }
    if(FD_ISSET(_sock, &read))
    {
        _notify_read();
        FD_CLR (_sock, &read);
    }
    if(pbytes){
        *pbytes=rv;
    }
    return true;
}

/****************************************************************************************
*/
void hci_socket_ble::_notify_read()
{
    uint8_t  data[512] = {0};
    sdata    packed;

    int len = this->read(data, sizeof(data));
    if(len <= 0)
    {
        hci_error e;
        e.nerror = errno;
        e.message="netdown";
        _hci->on_error(e);
    }
    else
    {
        packed.data = data;
        packed.len = (uint16_t)len;
        _hci->on_sock_data(0, packed);
    }
}
