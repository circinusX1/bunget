/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: marrius9876@gmail.com

*/

#include <assert.h>
#include "bungetconf.h"
#include "libbungetpriv.h"
#include "ascon.h"
#include "bu_gap.h"
#include "bu_gatt.h"
#include "hci_config.h"

using namespace bunget;

extern bool     __alive;
ContextImpl*    Ctx;


/****************************************************************************************
*/
BtCtx::BtCtx(){};
BtCtx::~BtCtx(){};

/****************************************************************************************
*/
SrvDevice::SrvDevice(ISrvProc* proc, int& hcid, const char* name, int delay):_cb_proc(proc),
                    _def(false),_hcidev(hcid)
{
    _gapp = 0;
    _gatt = 0;
    _pacl = 0;
    _running = false;
    _name = name;
    _respdelay = delay;
    _hcistatus = STATE_POWEREDOFF;
    _advstatus = 0;
    _scanrespdatastatus = 0;
    _advstatus = 0;
    _defaults = false;
    _status = eOFFLINE;

    if(_respdelay > 100 || _respdelay< 0)
        delay = 100; // nomore than 100 ms
}

/****************************************************************************************
*/
SrvDevice::~SrvDevice()
{
    _status = eOFFLINE;
    if(_hci)
        _hci->reset();
    delete _pacl;
    delete _gatt;
    delete _gapp;
    delete _hci;

    for(auto &h : _handles)
        delete h;
}

/****************************************************************************************
*/
void    SrvDevice::power_switch(bool on)
{
    /**
    TODO    add driver power on / off hci socket
    */
    if(on==false)
        _status = eOFFLINE;
}

/****************************************************************************************
*/
void SrvDevice::run()
{
    _status = eINITALISING;
#ifdef USE_UVLIB
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
#else
    _hci->start(_respdelay);
    _running = true;

    // flush socket
    for(int k=0;k<100 && __alive;k++)
    {
        __alive = _hci->pool();
        ::usleep(10000);
    }
    _status = eRUNNING;
    while(__alive)
    {
        __alive = _hci->pool();
        ::usleep(1000);
    }
    _status = eUNKNOWN;
#endif
}

/****************************************************************************************
*/
void SrvDevice::on_configure_device(int devid)
{
    char name[128];

    _status = eINITALISING;

    system("service bluetoothd stop");
    system("service bluetooth stop");
    system("sudo systemctl stop bluetooth");
    system("rfkill unblock bluetooth");
    ::sprintf(name,"hciconfig hci%d down", _hcidev);
    system(name);
    ::usleep(100000);
    ::sprintf(name,"hciconfig hci%d up", _hcidev);
    system(name);
  //  system("hciconfig hci0 sspmode 0");
 //   system("hciconfig hci0 nosecmgr");
 //   system("hciconfig hci0 noencrypt");
//    system("hciconfig hci0 noauth");
//    system("hciconfig hci0 noleadv");
//    system("hciconfig hci0 noscan");

    ::sprintf(name,"hciconfig hci%d name  %s", _hcidev, _name.c_str());
    system(name);
    ::sprintf(name,"hciconfig hci%d piscan", _hcidev);
    system(name);
    ::sprintf(name,"hciconfig hci%d leadv", _hcidev);
    system(name);
    printf("%s", "done dirty work\n");
}

/****************************************************************************************
*/
void SrvDevice::on_dev_status(bool onoff)
{
    if(onoff==true)
    {
        if(_running)
        {
            _gapp->advertise(_name.c_str(),_services, _pin);
        }
    }
    _cb_proc->onDeviceStatus(onoff);//onDeviceStatus
}

/****************************************************************************************
*/
int     SrvDevice::advertise(bool onoff)
{
    if(onoff==true)
    {
        if(0==_hci)
        {
            on_configure_device(_hcidev);

            _hci = new bu_hci(this);
            if(!_hci->init(_hcidev, false))
            {
                delete _hci;
                _hci = 0;
                _hcidev=-1;
            }
            _gapp =  new bu_gap(_hci);
            _gatt= new bu_gatt(_hci);
            _gapp->advertise(_name.c_str(), _services, _pin);
        }
    }
    else
    {
        _gapp->stop_adv();
    }
    return _hcidev;
}

/****************************************************************************************
*/
IService* SrvDevice::add_service(uint16_t srvid, const char* name)
{
    if(!_defaults){
        _defaults=true;
        add_default_service();
    }

    GattSrv* ps = new GattSrv(srvid, this);
    ps->_name=name;
    ps->_srv = this;
    _services.push_back(ps);
    ps->_hndl = add_gattel(ps);
    return ps;
}

/****************************************************************************************
*/
IService* SrvDevice::add_service(const bt_uuid_t& srvid, const char* name)
{
    if(!_defaults){
        add_default_service();
        _defaults=true;
    }

    GattSrv* ps = new GattSrv(srvid, this);
    ps->_name = name;
    ps->_srv = this;
    _services.push_back(ps);
    ps->_hndl = add_gattel(ps);
    return ps;
}

/****************************************************************************************
*/
/****************************************************************************************
*/
int SrvDevice::adv_beacon(const char* suid, uint16_t minor,
                          uint16_t major, int8_t power, uint16_t manid,
                          const uint8_t* data, uint8_t length)
{

    Cguid cuid = Cguid::from_string(suid);

    TRACE(cuid.to_string());

    if(0==_hci)
    {
        on_configure_device(_hcidev);

        _hci = new bu_hci(this);
        if(!_hci->init(_hcidev, false))
        {
            delete _hci;
            _hci = 0;
            _hcidev=-1;
        }
        _gapp =  new bu_gap(_hci);
        _gatt= new bu_gatt(_hci);
    }
    length = std::min(uint8_t(20), length);

    bybuff by (data, length);
   _gapp->adv_beacon(cuid.as128(), minor, major, power, manid, by);

    return _hcidev;
}

/****************************************************************************************
*/
IService* SrvDevice::get_service(const bt_uuid_t& srvid)
{
    for(const auto& s : _services)
    {
        if(::memcmp(&s->get_uid(),&srvid,sizeof(bt_uuid_t)==0))
            return s;
    }
    return 0;
}

/****************************************************************************************
*/
IService* SrvDevice::get_service(uint16_t srvid)
{
    for(const auto& s : _services)
    {
        if(::memcmp(&s->get_uid(),&srvid,sizeof(bt_uuid_t)==0))
            return s;
    }
    return 0;
}

/**
    device name
    appearance
*/
/****************************************************************************************
*/
void SrvDevice::add_default_service()
{
    uint8_t apear[]={0x80,0x00};
    IService* ps = add_service(0x1800,"name");

    ps->add_charact(0x2a00, PROPERTY_READ,0,0,_name.length(),(uint8_t*)_name.c_str());
    ps->add_charact(0x2a01, PROPERTY_READ,0,0,2,apear);

    uint8_t apear1[]={0x00,0x00,0x00,0x00};
    ps = add_service(0x1801,"appear");
    ps->add_charact(0x2a05, PROPERTY_INDICATE,0,0,4,apear1);
}

/****************************************************************************************
*/
void SrvDevice::on_encrypt_chnage(const evt_encrypt_change* pecc)
{
    if(_pacl && _handle==pecc->handle)
    {
        _pacl->set_he(pecc->handle, pecc->encrypt);
    }
}

/****************************************************************************************
*/
void SrvDevice::on_acl_packet(uint16_t handle, uint16_t cid, const sdata& data)
{
    if(_pacl)
    {
        _pacl->push(cid, data);
    }
}

/****************************************************************************************
*/
void SrvDevice::get_version(uint8_t& hciver, uint16_t& hcirev, uint8_t& lmpver, uint16_t& man, uint16_t& lmpsubver)
{
    hciver=_hciver;
    lmpver=_lmpver;
    man=_man;
    lmpsubver=_lmpsubver;
}

/****************************************************************************************
*/
void SrvDevice::on_read_version(uint8_t hciver, uint16_t hcirev, uint8_t lmpver, uint16_t man, uint16_t lmpsubver)
{
    _hciver=hciver;
    _lmpver=lmpver;
    _man=man;
    _lmpsubver=lmpsubver;
}

/****************************************************************************************
*/
const bdaddr_t& SrvDevice::get_baddr()const
{
    return _baddr;
}

/****************************************************************************************
*/
void SrvDevice::on_mac_change(const bdaddr_t& addr)
{
    ::memcpy(&_baddr,&addr,sizeof(addr));
}

/****************************************************************************************
*/
void SrvDevice::on_adv_status(HCI_STATE status)
{
    _hcistatus = status; //STATE_POWEREDOFF
}

/****************************************************************************************
*/
void SrvDevice::on_adv_data_status(uint8_t status)
{
    _advstatus = status;
}

/****************************************************************************************
*/
void SrvDevice::on_scan_resp_datat_status(uint8_t status)
{
    _scanrespdatastatus =status;
}

/****************************************************************************************
*/
void SrvDevice::on_adv_enable(uint8_t status)
{
    _advstatus = status;
}

/****************************************************************************************
*/
void SrvDevice::on_rssi(uint16_t handle, uint8_t rssi)
{
//    _handle = handle;
    _rssi = rssi;
}

/****************************************************************************************
*/
void SrvDevice::le_ltk_neg_reply(uint16_t handle)
{
    if(_pacl)
    {
        _pacl->set_ltk_r();
    }
}

/****************************************************************************************
*/
void SrvDevice::onAdvertized(bool onoff)
{
    return _cb_proc->onAdvertized(onoff);
}

/****************************************************************************************
*/
bool SrvDevice::onSpin()
{
    if(_status == eRUNNING)
    {
        return _cb_proc->onSpin(this);
    }
    return true;
}

/****************************************************************************************
*/
void SrvDevice::on_disconnect(const evt_disconn_complete* evdc)
{
    for(auto &h : _handles)
    {
        if(h->_internal & INTERN_SUBSCRIBED)
        {
            _cb_proc->onSubscribesNotify(h, false);
        }
        h->_internal=0;
    }

    _gatt->reset();
    delete _pacl;
    _pacl = 0;
    this->data_unsubscribe(_gatt);
    _gapp->restart_adv();
    _cb_proc->onConnect(0);
    _eaters.clear();
}

/****************************************************************************************
*/
void SrvDevice::on_le_connected(uint8_t status, uint16_t handle, uint8_t role,
                                HCI_ADDRTYPE addressType, const bdaddr_t& address,
                                uint16_t interval, uint16_t latency,
                                uint16_t  supervisionTimeout, uint8_t masterClockAccuracy)
{
    memcpy(&_address, &address,sizeof(bdaddr_t));
    _handle = handle;
    _gatt->reset();
    assert(_pacl==0);
    if(_pacl!=0)
    {
        delete _pacl;
    }
    _pacl = new bu_asc(_hci,
                        handle,
                       _hci->_address,
                       _hci->_addrtype,
                       address,
                       addressType);
    _gatt->setAclPtr(_pacl);
    this->data_subscribe(_gatt);
    char bas[32];
    ba2str(&_address, bas);
    _remote._mac = bas;
    _remote._name = "*";
    _remote._props = addressType;
    _cb_proc->onConnect(&_remote);
}


/****************************************************************************************
*/
void SrvDevice::on_le_conn_update_complette_shit(uint8_t status, uint16_t handle, uint16_t interval,
                                            uint16_t latency, uint16_t supervisionTimeout)
{
}

/****************************************************************************************
*/
int    SrvDevice::feed_them(uint8_t opt,const sdata& data)
{
    int processed;
    for(auto &e : _eaters)
    {
        processed = e->on_sock_data(opt, data);
        if(processed)
            return processed;
    }
    return -1; // all processed
}

/****************************************************************************************
*/
void SrvDevice::refresh()
{
    if(_hci)
        _hci->read_baddr();
}

/****************************************************************************************
*/
uint16_t SrvDevice::add(GHandler* g)
{
    _handles.push_back(g);
    return (uint16_t)_handles.size();
}

/****************************************************************************************
*/
int SrvDevice::write_charact(GHandler* ph)
{
    if(_status != eRUNNING)
    {
        ERROR("E: cannot write durring initialisation");
        return -1;
    }

    if(_gatt)
    {
        return _gatt->writeCharaterisitc(ph);
    }
    return 0;
}

/****************************************************************************************
*/
int SrvDevice::write_descr(GHandler* ph)
{
    if(_status != eRUNNING)
    {
        ERROR("E: cannot write durring initialisation");
        return -1;
    }

    if(_gatt)
    {
        return _gatt->write_descr(ph);
    }
    return 0;
}

/****************************************************************************************
*/
void SrvDevice::stop()
{
    if(_hci)
        _hci->disconnect(0, 0);
    on_disconnect(0);
    delete _hci;
    _hci = 0;
}

/****************************************************************************************
*/
int SrvDevice::set_name(const char* name)
{
    if(_gapp){
        _gapp->set_btname(name);
        return 0;
    }
    return -1;
}


/****************************************************************************************
*/
BtCtx* BtCtx::instance()
{
    if(0==Ctx)
        Ctx = new ContextImpl();
    return Ctx;
}

/****************************************************************************************
*/
IServer* ContextImpl::new_server(ISrvProc* proc, int hcidev, const char* name, int tweak_delay)
{
    if(_adapters.find(hcidev) == _adapters.end())
    {
        SrvDevice* p = new SrvDevice(proc, hcidev, name, tweak_delay);
        if(hcidev<0){
            delete p;
            return 0;
        }

        _adapters[hcidev] = p;
    }
    return  _adapters[hcidev];
}
