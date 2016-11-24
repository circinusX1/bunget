# BTLE For Linux,  Raspberry PI
## Raspberry PI BTLE C++ Library
##
###      BUNGET
##### *beta-version_plus* 
####### (Bunget, read 'ge' as in George)

#### Build lib bunget and demo application

### Prerequisites 
* Linux
  * sudo apt-get install uuid-dev
  * sudo apt-get install cmake
  * sudo apt-get install build-essential
  * sudo apt-get install libcrypto++-dev
  


### Description:

BUNGET is a C++ library for Linux which help to write GAP/GATT servers (Bluetooth 4.0). It provides abstract classes and methods to create GATT services, characteristics (READ/WRITE/NOTIFICATIONS). The library does not require the Bluez, but it requires some bluetooth utilities. The library was written from scratch. The idea of writing 'yet another BTLE implemetation' came after I spend days and days trying Bluez and Bleno. I could not get Blues working programatically and easy. Bleno is terible slow and tremendous resource eater, also brings entire java enciclopedia along with. 
So: here is a simple C++ library which uses just a HCI socket and GAP/GATT protocol helping to write GATT services. 

* Server only: GATT services characteristics, descriptors or beacons.
* Characteristics: R W I N + Descriptors. 
* Requires: hciconfig tool from blues package in /usr/bin
* (iii).    Bunget lib will stop the bluetooth service.
* (iiii) Read all documents.
* Latest version is at: https://github.com/zirexix


## Usage

* Turn device running Linux system in to a GATT server. 
   * Soon all specific dev platforms for embedded custom chips will be replaced by Linux, pretty soon...
* Debug HCI clients

## Block Diagram:

![alt text](https://github.com/comarius/bunget/blob/master/docs/bungblk.png "bunget-lib")


#### Demo video. If the image does not work use this link: http://209.177.145.195/share/bunget.mov

 [![Ipad BTLE demo video](http://209.177.145.195/share/bungetpi.png)](http://209.177.145.195/share/bunget.mov)


## Credits:
Bluez, Bleno, StCube/BlueNrg, Bluetooth 4.0 Specs.


##License:
Free, for non commercial products, otherwise you need a written personalised license from the Author.


### SDK:

- See libbunget.h in the source tree
- See main.cpp in the source tree

* For services use 16 bit UUID's. Maximum 8 servics / 8 characteristics.
* Do not mix UUIDS of 16 bytes with UUIDS of 128 bytes on the same running unit.
* The GATT (services/charact./descr.) was tested only with 16 byte UUIDS.
* Beacon advertising, needs 128 bit UUID's only.


### Demo R-PI console (left). ArduUiPush APK Right 

* ArduUiPush;No screens scanned and loaded for advertised name.

![alt text](https://bitbucket.org/repo/4kLXA4/images/3493780247-gatt.png "bunget-demo")

Check also: ArduUiPush demo: https://www.youtube.com/watch?v=Fw4yCe2ejNw

## Class Diagram

![alt text](https://github.com/zyraxes-com/bunget/blob/master/docs/bungetl.png "bunget-lib")


## Issues Tweaks 

* On different BTLE dongles the timing between receiving HCI event and sending back data disrupts the functionality.
  * Build with DEBUG enabled. If after connection the progress stops around this TRACE:
  ```javascript
HCI: ACL_START 
-->[16]0240200B0007000400100100FFFF0028 
GATT DATA:ATT_OP_READ_BY_GROUP_REQ  
my_proc event: onServicesDiscovered 
  ```
  * Tweak the receive<delay>send value at main line 127. . 
  * My Broadcom dongle works with >16 ms delay on R-PI and with >2 ms on 4 quad HP Intel PC.
  
```javascript
       IServer*    BS =  ctx->new_server(&procedure, dev, "advname", 0/* tweak delay*/);
```
* Descriptors for data type not suppotred yet. Use GATT registerred UUIDS or know your type on both ends.

#### Code excerpt

```javascript

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <uuid.h>
#include <libbunget.h>

using namespace std;
using namespace bunget;


#define UID_GPIO    0x3400
#define UID_TIME    0x3401
#define UID_TEMP    0x3403

bool __alive = true;
class my_proc : public ISrvProc
{
public:
    my_proc();
    void onServicesDiscovered(std::vector<IHandler*>& els); // called when remote discover services
    void onReadRequest(IHandler* pc);                       // called when remote reads pc's value charact.
    int evSubscribesNotify(IHandler* pc, bool b);           // called when remote writes notification descriptor !=0
    void onIndicate(IHandler* pc);                          // called when remote wants pc's indicator value
    void onWriteRequest(IHandler* pc);                      // called when remote writes pc charateristic
    void onWriteDescriptor(IHandler* pc, IHandler* pd);     // called when remote writes pc's charact descriptor pd
    void evAdvertise(bool onoff);                           // called when advertising turns on/off
    void evDeviceState(bool onoff);                         //called when device dongle is plugged / unplugged (buggy still)
    void evClientState(bool connected);                     // called when a client connects / disconnetcs 
    bool evLoop(IServer* ps);                               //called when the hci-socket idles. Be short!. 
                                                            // return false to break the run() blocking call.    

private:
    void        _prepare_gpio17();  // RPI specific, to control LEDS for the purpose of the demo
    const char* _get_time();        // reads local time, and returns a string
    float       _get_temp();        // reads temp,  on RPI. 
    const char* _get_temp_s();      // same as above but returns a string
    uint8_t     _get_gpio();        // reads GPIO status on RPI
    void        _send_value(IHandler* pc);  // sends a value to GATT

public:
    char        _some[20];
    bool        _subscribed;
    IHandler*   LedChr;       // RW
    IHandler*   TimeChr;      // NIR
    IHandler*   Temp1Chr;     // NIR
};



int main(int n, char* v[])
{
    BtCtx*      ctx = BtCtx::instance();        // get singleton Bunget instance
    my_proc     procedure;                      // callback  instance of above class

    IServer*    BS =  ctx->new_server(&procedure, 0, "rpi-bunget", 0);      // one GATT server per hci device
    IService*   ps = BS->add_service(0x123F,"demo");                        // add a service to the GATT server
    
    //
    // add some characterisitcs
    //
    procedure.LedChr = ps->add_charact(UID_GPIO,PROPERTY_WRITE|PROPERTY_INDICATE|PROPERTY_NOTIFY,
                             0,
                             FORMAT_RAW, 1); // 1 / 0  // many per service

    procedure.TimeChr = ps->add_charact(UID_TIME, PROPERTY_READ|PROPERTY_NOTIFY,
                             0,
                             FORMAT_RAW, 20); // we send it as string

    procedure.Temp1Chr = ps->add_charact(UID_TEMP, PROPERTY_NOTIFY|PROPERTY_INDICATE,
                              0,
                              FORMAT_RAW, 20); // we send it as string 

    try{
        BS->advertise(true);
        BS->run();
    }
    catch(hexecption& ex)
    {
        TRACE(ex.report());
    }
    return 0;
}

my_proc::my_proc()
{
    _subscribed=false;
    _prepare_gpio17();
}

bool my_proc::evLoop(IServer* ps)
{
    static int inawhile=0;

    // notification
    if(_subscribed && inawhile++%600==0)
    {
        _send_value(TimeChr);
        _send_value(Temp1Chr);
    }
    return true;
}

//  ... see main.cpp for the rest of the callback implenentation

```
#######################################################################
#######################################################################

H O W     TO   B U I L D 

#######################################################################
```javascript
cmake .
make
cd bin
sudo ./bunget 0  (hci device number)
```

#########################################################################
### Raspberry PI Build (check compiler version )
  - The C compiler identification is GNU 4.9.2
  - Board: RPI-2
  - Fresh distribution: Linux minibian 4.1.18-v7+ #846 SMP Thu Feb 25 14:22:53 GMT 2016 armv7l GNU/Linux
  - Login as root/raspberry

```javascript
#
# Prerequisites. Also required for new system
#
apt-get update
apt-get install bluez # (needed for hciconfig utility for now)
apt-get install cmake
apt-get install  g++
apt-get install  rfkill
apt-get install libcrypto++-dev
service bluetooth stop   # not mandatory
update-rc.d -f  bluetooth remove   # not mandatory


git clone https://github.com/comarius/bunget
cd bunget
cmake .
make
cd bin

#
# check if the device is detected
#
root@minibian:~/bunget/src/bin# hciconfig
hci0:	Type: BR/EDR  Bus: USB
	BD Address: 5C:F3:70:76:B2:B2  ACL MTU: 1021:8  SCO MTU: 64:1
	DOWN 
	RX bytes:616 acl:0 sco:0 events:34 errors:0
	TX bytes:380 acl:0 sco:0 commands:34 errors:0

# argument for hci0 is 0, for hci1 is 1 and so on
# therefore we run our GATT server as

./bunget 0


root@minibian:~/bunget/src/bin# ./bunget 0
sh: echo: I/O error
Failed to stop bluetoothd.service: Unit bluetoothd.service not loaded.
my_proc event:  onAdvertized:0 
my_proc event:  onDeviceStatus:1 
my_proc event:  onAdvertized:1 
#
# here I connected the BTLE scanner phone and read all the characteristics
#
accepted: 5F:C9:32:F3:5D:F6,* 
my_proc event: onServicesDiscovered 
my_proc event:  onWriteDescriptor:1 
my_proc event: onSubscribesNotify:3400=1 
my_proc event:  onWriteDescriptor:1 
my_proc event: onSubscribesNotify:3401=1 
my_proc event:  onWriteDescriptor:1 
my_proc event: onSubscribesNotify:3403=1 


```

  
### Issues
- If nothing happen tweak the timout [0-64] in main ctx->new_server(&procedure, dev, hostname, <timeout>);

### Tested on

####   R-PI 3
  - Linux minibian 4.4.17-v7+ #901 SMP Fri Aug 12 17:57:27 BST 2016 armv7l GNU/Linux
  - gcc (Raspbian 4.9.2-10) 4.9.2
  - g++ (Raspbian 4.9.2-10) 4.9.2
  
#### Ubuntu 16.04
  - Linux X540LA (ACER) 4.4.0-47-generic #68-Ubuntu SMP Wed Oct 26 19:39:52 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux
  - gcc (Ubuntu 4.8.5-4ubuntu2) 4.8.5
  - g++ (Ubuntu 4.8.5-4ubuntu2) 4.8.5
  
#### Linux Mint
  - Linux hp 3.19.0-32-generic #37~14.04.1-Ubuntu SMP Thu Oct 22 09:41:40 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux
  - g++ (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4
  - gcc (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4



#### Dongle USB used
```
[33544.074470] usb 4-1.2: new full-speed USB device number 5 using ehci-pci
[33544.171100] usb 4-1.2: New USB device found, idVendor=0a5c, idProduct=21e8
[33544.171108] usb 4-1.2: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[33544.171114] usb 4-1.2: Product: BCM20702A0
[33544.171118] usb 4-1.2: Manufacturer: Broadcom Corp
[33544.171123] usb 4-1.2: SerialNumber: 5CF3706B72D6
[33544.172183] bluetooth hci1: Direct firmware load for brcm/BCM20702A0-0a5c-21e8.hcd failed with error -2
[33544.172193] Bluetooth: hci1: BCM: patch brcm/BCM20702A0-0a5c-21e8.hcd not found
```



#######################################################################

Questions, Bugs, Commercial usage: Contact by eMail for any questions: marrius9876@ g m a i   l . c  o  m

