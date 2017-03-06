/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: zyraxes.comatgmaildotcom.

*/

/*
    This program should run as root.

    Every time a characterisitc/service is chnaged,
    turn off and on on mobile the BT to clear the cached LE's.

    http://plugable.com/2014/06/23/plugable-usb-bluetooth-adapter-solving-hfphsp-profile-issues-on-linux
    Newer Kernel Versions (3.16 and later)
    wget https://s3.amazonaws.com/plugable/bin/fw-0a5c_21e8.hcd
    sudo mkdir /lib/firmware/brcm
    sudo mv fw-0a5c_21e8.hcd /lib/firmware/brcm/BCM20702A0-0a5c-21e8.hcd

    This demo adds 1 service 0x123F with 3 characteristis.
        0x3400  control a GPIO pin, we connect a LED, on GPIO 17 '/sys/class/gpio/gpio17/value'
        0x3401
        0x3402

*/

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <libbunget.h>


using namespace std;
using namespace bunget;

bool __alive = true;

/****************************************************************************************
*/
int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, 0x541B, &bytesWaiting);
    return bytesWaiting;
}

/****************************************************************************************
*/
class my_proc : public ISrvProc
{
public:
    my_proc();
    void onServicesDiscovered(std::vector<IHandler*>& els);
    void onReadRequest(IHandler* pc);
    int onSubscribesNotify(IHandler* pc, bool b);
    void onIndicate(IHandler* pc);
    void onWriteRequest(IHandler* pc);
    void onWriteDescriptor(IHandler* pc, IHandler* pd);
    void onAdvertized(bool onoff);
    void onDeviceStatus(bool onoff);
    void onConnect(const HciDev* connected);
    bool onSpin(IServer* ps);

private:
    void        _prepare_gpio17();
    const char* _get_time();
    float       _get_temp();
    const char* _get_temp_s();
    uint8_t     _get_gpio();
    void        _send_value(IHandler* pc);

public:
    char        _some[20];
    bool        _subscribed;
    IHandler*   LedChr;       // RW
    IHandler*   TimeChr;      // NIR
    IHandler*   Temp1Chr;     // NIR
};

/****************************************************************************************
*/
#define UID_GPIO    0x3400
#define UID_TIME    0x3401
#define UID_TEMP    0x3403

/****************************************************************************************
*/
int main(int n, char* v[])
{
    if(n==1)
    {
        std::cout << "bunget hcidev# <b>    [ b for beacon mode demo] \n";
        return -1;
    }


    BtCtx*      ctx = BtCtx::instance();
    my_proc     procedure;

    int dev = ::atoi(v[1]);
    try{
        IServer*    BS =  ctx->new_server(&procedure, dev, "advname", 9);
        //BS->set_name("advname"); // this is the bt name.

        if(n==3 && v[2][0]=='b') //beacon mode
        {
            //99999999-9999-9999-9999-999999999999
            BS->adv_beacon("11111111-1111-1111-1111-111111111111", 1, 10, -10, 0x004C, (const uint8_t*)"MARIUS", 7);
        }
        else
        {

            IService*   ps = BS->add_service(0x123F,"demo");
            procedure.LedChr = ps->add_charact(UID_GPIO,PROPERTY_WRITE|PROPERTY_INDICATE|PROPERTY_NOTIFY,
                                     0,
                                     FORMAT_RAW, 1); // 1 / 0

            procedure.TimeChr = ps->add_charact(UID_TIME, PROPERTY_READ|PROPERTY_NOTIFY,
                                     0,
                                     FORMAT_RAW, 20); // we send it as string

            procedure.Temp1Chr = ps->add_charact(UID_TEMP, PROPERTY_NOTIFY|PROPERTY_INDICATE,
                                      0,
                                      FORMAT_FLOAT, FORMAT_FLOAT_LEN); // we send it as float
            BS->advertise(true);
        }

        BS->run();
        BS->stop();

    }
    catch(bunget::hexecption& ex)
    {

        ERROR (ex.report());
    }

    return 0;
}

/****************************************************************************************
*/
my_proc::my_proc()
{
    _subscribed=false;
    Temp1Chr = 0;
    _prepare_gpio17();
}


/****************************************************************************************
*/
bool my_proc::onSpin(IServer* ps)
{
    static int inawhile=0;

    if(_kbhit()){
        if(getchar()=='q')
        return false;
    }

    // notification
    if(inawhile++%800==0)
    {
        if(_subscribed)
        {
            _send_value(TimeChr);
            _send_value(Temp1Chr);
        }
    }
    return true;
}
/**

}


*/

/****************************************************************************************
*/
void my_proc::onServicesDiscovered(std::vector<IHandler*>& els)
{
    TRACE("my_proc event: onServicesDiscovered");
}

/****************************************************************************************
*/
/// remote readsd pc characteritcis
void my_proc::onReadRequest(IHandler* pc)
{
    TRACE("my_proc event:  onReadRequest:" <<  std::hex<< pc->get_16uid() << std::dec);
    _send_value(pc);
}

/****************************************************************************************
*/
int my_proc::onSubscribesNotify(IHandler* pc, bool b)
{
    TRACE("my_proc event: onSubscribesNotify:" << std::hex<< pc->get_16uid() << "="<<(int)b<< std::dec);
    _subscribed = b;
    return 0 ;
}

/****************************************************************************************
*/
void my_proc::onIndicate(IHandler* pc)
{
    TRACE("my_proc event:  onIndicate:" <<  std::hex<< pc->get_16uid() << std::dec);
    _send_value(pc);
}

/****************************************************************************************
*/
void my_proc::onWriteRequest(IHandler* pc)
{
    TRACE("my_proc event:  onWriteRequest:" <<  std::hex<< pc->get_16uid() << std::dec);
    std::string     ret;
    const uint8_t*  value = pc->get_value();
    char            by[4];
    int             i=0;

    for(;i<pc->get_length();i++)
    {
        ::sprintf(by,"%02X:",value[i]);
        ret.append(by);
    }
    TRACE("Remote data:" << ret);
    if(pc->get_16uid() == UID_GPIO)
    {
        if(::access("/sys/class/gpio/gpio17/value",0)==0)
        {
            if(value[0]==0)
                system("echo 0 > /sys/class/gpio/gpio17/value");
            else
                system("echo 1 > /sys/class/gpio/gpio17/value");
        }
    }
}

/****************************************************************************************
*/
//descriptor chnaged of the charact
void my_proc::onWriteDescriptor(IHandler* pc, IHandler* pd)
{
    TRACE("my_proc event:  onWriteDescriptor:" << int(*((int*)(pd->get_value()))));
}

/****************************************************************************************
*/
void my_proc::onAdvertized(bool onoff)
{
    TRACE("my_proc event:  onAdvertized:" << onoff);
}

/****************************************************************************************
*/
void my_proc::onDeviceStatus(bool onoff)
{
    TRACE("my_proc event:  onDeviceStatus:" << onoff);
    if(onoff==false)
    {
        _subscribed = false;
    }
}

/****************************************************************************************
*/
void my_proc::onConnect(const HciDev* device)
{
    if(device == 0)
    {
        _subscribed = false;
        TRACE("my_proc event: onConnect: disconnected");
    }
    else
    {
        TRACE("accepted: " << device->_mac <<","<< device->_name);
    }
}

/****************************************************************************************
*/
void my_proc::_prepare_gpio17()
{
    system ("chmod 777 /sys/class/gpio/export");
    system ("echo 17 > /sys/class/gpio/export");
    system ("chmod 777 /sys/class/gpio/gpio17/*");
    system ("sync");
}

/****************************************************************************************
*/
const char*  my_proc::_get_time()
{
    time_t secs = time(0);
    struct tm *local = localtime(&secs);
    sprintf(_some, "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);
    return _some;
}

/****************************************************************************************
*/
float my_proc::_get_temp()
{
    float ftamp=0.0;
#ifdef ARM_CC
    ::system("/opt/vc/bin/vcgencmd measure_temp > /tmp/bunget");
    std::ifstream ifs("/tmp/bunget");
    std::string temp( (std::istreambuf_iterator<char>(ifs) ),(std::istreambuf_iterator<char>()));
    temp = temp.substr(5);
    ftamp =::atof(temp.c_str());

#else //fake it
    std::string temp = "temp=32.5";
    temp = temp.substr(5);
    ftamp =::atof(temp.c_str());
    ftamp += rand()%15;
#endif
    return ftamp;
}

/****************************************************************************************
*/
const char* my_proc::_get_temp_s()
{
   float ftamp=0.0;
#ifdef ARM_CC
    ::system("/opt/vc/bin/vcgencmd measure_temp > /tmp/bunget");
    std::ifstream ifs("/tmp/bunget");
    std::string temp( (std::istreambuf_iterator<char>(ifs) ),(std::istreambuf_iterator<char>()));
    ::strcpy(_some,temp.c_str());
#else //fake it
    ::sprintf(_some,"temp=%d.%d", rand()%40,rand()%100);
#endif
    return _some;
}

/****************************************************************************************
*/
uint8_t my_proc::_get_gpio()
{
    if(::access("/sys/class/gpio/gpio17/value",0)==0)
    {
        std::ifstream ifs("/sys/class/gpio/gpio17/value");
        std::string temp( (std::istreambuf_iterator<char>(ifs) ),(std::istreambuf_iterator<char>()));
        return uint8_t(::atoi(temp.c_str()));
    }

    return 0;
}

/****************************************************************************************
*/
void my_proc::_send_value(IHandler* pc)
{
    uint16_t uid = pc->get_16uid();
    switch(uid)
    {
        case  UID_GPIO:
            {
                uint8_t gp = _get_gpio();
                // pc->put_value((uint8_t*)&gp,1);
                GattRw(pc).write(gp);
            }
            break;
        case  UID_TIME:
            {
                const char* t = _get_time();
                pc->put_value((uint8_t*)t,::strlen(t));
            }
            break;
        case  UID_TEMP:
            {
                float ft = _get_temp();
                pc->put_value((uint8_t*)&ft,sizeof(float));
                //const char* fts = _get_temp_s();
                //pc->put_value((uint8_t*)fts,::strlen(fts));
            }
            break;
        default:
            break;
    }
}
