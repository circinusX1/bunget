/**
    Copyright: zirexix 2016

    This program is distributed
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This program, or portions of it cannot be used in commercial
    products without the written consent of the author: marrius9876@gmail.com

*/
#include <assert.h>
#include "bybuff.h"
#include "bu_hci.h"
#include "libbungetpriv.h"
#include <iostream>

/****************************************************************************************
*/
bu_hci::bu_hci(SrvDevice* psrv):_pev(psrv),_isDevUp(false),_chekingdev(false),_connected(false)
{
#ifdef ARM_CC
    _delay = 16384;
#else
    _delay = 2048;
#endif
    _socket = new hci_socket_ble(this);
}

/****************************************************************************************
*/
bu_hci::~bu_hci()
{
    _clear_cache();
    _socket->close();
    delete _socket;
}

/****************************************************************************************
*/
bool bu_hci::init(int& devid, bool userchannel)
{
    try
    {
        _socket->create();
        if(userchannel)
        {
            _socket->bind_user(&devid);
            reset();
        }
        else
        {
            _socket->bind_raw(&devid);
        }
    }
    catch(hexecption& e)
    {
        e.report();
        return false;
    }
    _devid = devid;
    return check_dev_state();
}

/****************************************************************************************
*/
bool bu_hci::start(int delay)
{
    _delay = delay * 1000;
    return check_dev_state();
}

/****************************************************************************************
*/
void bu_hci::_set_hci_filter()
{
    struct  _PAACK8
    {
        uint32_t    filter0;
        uint32_t    filter1;
        uint32_t    filter2;
        uint16_t    filter3;
    }  filter =
    {
        btohl((1 << HCI_EVENT_PKT)| (1 << HCI_ACLDATA_PKT)),
        btohl((1 << EVT_DISCONN_COMPLETE) | (1 << EVT_ENCRYPT_CHANGE) | (1 << EVT_CMD_COMPLETE) | (1 << EVT_CMD_STATUS)),
        btohs(1 << (EVT_LE_META_EVENT - 32)),
        //btohl(HCI_EVENT_PKT| HCI_ACLDATA_PKT),
        //btohl(EVT_DISCONN_COMPLETE|EVT_ENCRYPT_CHANGE|EVT_CMD_COMPLETE|EVT_CMD_STATUS),
        //btohs(0x30),
        0
    };
    _socket->set_filter((const uint8_t*)&filter, sizeof(filter));
}

/****************************************************************************************
*/
void bu_hci::_set_event_mask()
{
    struct  _PAACK8
    {
        hcihr   _hcihr;
        set_event_mask_cp _set_event_mask_cp;
    }
    event =
    {
        {
            HCI_COMMAND_PKT,
            CMD_OPCODE_PACK(OCF_SET_EVENT_MASK,OGF_HOST_CTL),
            uint8_t(sizeof(set_event_mask_cp)),
        },
        {
            0xff,
            0xff,
            0xfb,
            0xff,
            0x07,
            0xf8,
            0xbf,
            0x3d
        }
    };
    _write_sock(event);
}

/****************************************************************************************
*/
void bu_hci::_set_le_event_mask()
{
    struct  _PAACK8
    {
        hcihr   _hcihr;
        set_event_mask_cp _set_event_mask_cp;
    }
    event =
    {
        {
            HCI_COMMAND_PKT,
            CMD_OPCODE_PACK(OCF_SET_EVENT_MASK,OGF_LE_CTL),
            uint8_t(sizeof(set_event_mask_cp)),
        },
        {
            0x1f,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00
        }
    };
    _write_sock(event);
}

/****************************************************************************************
*/
void bu_hci::_read_version()
{
    hcihr   cmd =
    {
        HCI_COMMAND_PKT,
        CMD_OPCODE_PACK( OCF_READ_LOCAL_VERSION,OGF_INFO_PARAM),
        0
    };
    _write_sock(cmd);
}

/****************************************************************************************
*/
void bu_hci::_read_baddr()
{
    hcihr   cmd =
    {
        HCI_COMMAND_PKT,
         CMD_OPCODE_PACK(OCF_READ_BD_ADDR , OGF_INFO_PARAM),
        0
    };
    _write_sock(cmd);
}

/****************************************************************************************
*/
void bu_hci::_write_le_host()
{
    struct  _PAACK8
    {
        hcihr                       hdr;
        write_le_host_supported_cp  str;
    } cmd =
    {
        {
            HCI_COMMAND_PKT,
            CMD_OPCODE_PACK(OCF_WRITE_LE_HOST_SUPPORTED,OGF_HOST_CTL),
            0x2
        },
        {
            0x1,
            0x0
        }
    };
    _write_sock(cmd);
};

/****************************************************************************************
*/
void bu_hci::_read_le_hosts()
{
    hcihr cmd =
    {
        HCI_COMMAND_PKT,
        CMD_OPCODE_PACK(OCF_READ_LE_HOST_SUPPORTED,OGF_HOST_CTL),
        0x0
    };
    _write_sock(cmd);
}

/****************************************************************************************
*/
void bu_hci::_set_adv_params(uint16_t mini,uint16_t maxi, uint8_t advtype/*8*/)
{
    struct  _PAACK8
    {
        hcihr   _hcihr;
        le_set_advertising_parameters_cp lead;
    } adv =
    {
        {
            HCI_COMMAND_PKT,
            (CMD_OPCODE_PACK(OCF_LE_SET_ADVERTISING_PARAMETERS,OGF_LE_CTL)),
            15
        }
        ,
        {
            btohs(mini),
            btohs(maxi),
            advtype,
            9,
            10,
            {0,0,0,0,0,0},
            17,
            18
        }
    };

    _write_sock(adv);
}

/****************************************************************************************
*/
void bu_hci::reset()
{
    hcihr    hdr =
    {
        HCI_COMMAND_PKT,
        (CMD_OPCODE_PACK(OCF_RESET, OGF_HOST_CTL)),
        0x00
    };
    _write_sock(hdr);
}

/****************************************************************************************
*/
void bu_hci::set_adv_data(const sdata& data)
{
    struct _PAACK8
    {
        hcihr    hdr;
        le_set_advertising_data_cp lea;
    }  lead =
    {
        {
            HCI_COMMAND_PKT,
            (CMD_OPCODE_PACK(OCF_LE_SET_ADVERTISING_DATA,OGF_LE_CTL)),
            32
        },
        {
            (uint8_t)data.len,
            {
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,0
            }
        }
    };
    ::memcpy(lead.lea.data,data.data,(int)data.len);
    _write_sock(lead);
}

/****************************************************************************************
*/
void bu_hci::set_sca_res_data(const sdata& data)
{
    struct  _PAACK8
    {
        hcihr    hdr;
        le_set_scan_response_data_cp lea;
    }lrspd =
    {
        {
            HCI_COMMAND_PKT,
            (CMD_OPCODE_PACK(OCF_LE_SET_SCAN_RESPONSE_DATA,OGF_LE_CTL)),
            32
        },
        {
            (uint8_t)data.len,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0
        }
    };
    ::memcpy(lrspd.lea.data,data.data,(int)data.len);
    _write_sock(lrspd);
}

/****************************************************************************************
*/
void bu_hci::enable_adv(uint8_t enable)
{
    struct  _PAACK8
    {
        hcihr   hdr;
        le_set_advertise_enable_cp lsae;

    }sae={
        {
            HCI_COMMAND_PKT,
            (CMD_OPCODE_PACK(OCF_LE_SET_ADVERTISE_ENABLE,OGF_LE_CTL)),
            0x01
        },
        enable
    };
    _write_sock(sae);
    _pev->onAdvertized(enable!=0);
}

/****************************************************************************************
*/
void bu_hci::disconnect(uint16_t handle, uint8_t reason)
{
    struct  _PAACK8
    {
        hcihr   hdr;
        disconnect_cp rh;

    }  cmd =
    {
        {
            HCI_COMMAND_PKT,
            (CMD_OPCODE_PACK(OCF_LE_SET_ADVERTISE_ENABLE,OGF_LE_CTL)),
            0x01
        },
        {
            btohs(handle),
            reason
        }
    };
    _write_sock(cmd);
}

/****************************************************************************************
*/
void bu_hci::read_rssi(uint16_t handle)
{
    struct  _PAACK8
    {
        hcihr       hdr;
        uint16_t    r;
    } cmd =
    {
        {
            HCI_COMMAND_PKT,
            (CMD_OPCODE_PACK(OCF_READ_RSSI,OGF_STATUS_PARAM)),
            0x02
        },
        btohs(handle)
    };
    _write_sock(cmd);
}

/****************************************************************************************
*/
void bu_hci::write_ack_packet(uint16_t handle, uint16_t cid, const sdata& data)
{
    struct  _PAACK8
    {
        struct  _PAACK8
        {
            uint8_t a;
            uint16_t b;
            uint16_t c;
            uint16_t len;
            uint16_t cid;
        } hdr;
        uint8_t  pck[512]; /*enough well see*/
    }
    cmd={
            {
                (uint8_t)HCI_ACLDATA_PKT,
                (uint16_t)(btohs((uint16_t)(handle | (ACL_START_NO_FLUSH << 12)))),
                (uint16_t)(data.len + 4),
                (uint16_t)(btohs((uint16_t)(data.len))),
                (uint16_t)(btohs(cid)),
            },
        {0}
    };
    ::memcpy(cmd.pck, data.data, std::min((size_t)data.len, size_t(512)));
    _write_sock(cmd, data.len+sizeof(cmd.hdr));
}

/****************************************************************************************
*/
void bu_hci::write_local_name(const char* name)
{
	change_local_name_cp cp;

	memset(&cp, 0, sizeof(cp));
	strncpy((char *) cp.name, name, sizeof(cp.name));
	send_cmd(OCF_CHANGE_LOCAL_NAME, OGF_HOST_CTL, CHANGE_LOCAL_NAME_CP_SIZE, &cp);
}

/****************************************************************************************
*/
int bu_hci::read_local_name()
{
    read_local_name_rp rp;
    send_cmd(OCF_READ_LOCAL_NAME, OGF_HOST_CTL, READ_LOCAL_NAME_RP_SIZE, &rp);
	return 0;
}

/****************************************************************************************
*/
void bu_hci::send_cmd(uint16_t ocf, uint16_t ogf, uint8_t plen, void *param)
{
    uint8_t loco[512];
	hci_command_hdr hc;

	hc.opcode   = CMD_OPCODE_PACK(ocf, ogf);
	hc.plen     = plen;

    memcpy(loco, &hc, sizeof(hc));
    if(plen)
        memcpy(loco+sizeof(hc), param, plen);

    _write_sock(loco, plen+sizeof(hc));
}

/****************************************************************************************
*/
void bu_hci::_onhci_state_chnaged(HCI_STATE state)
{
    _state = state;
}

/****************************************************************************************
*/
bool bu_hci::onSpin(bt_socket* sock) //received
{
    return _pev->onSpin();
}

/****************************************************************************************
*/
bool bu_hci::pool()
{
    bool rv  = true;
    if(check_dev_state() && _socket->valid())
    {
        if(_socket->pool()==false)
            return false;
        return true;
    }
    else
    {
        rv = _pev->onSpin();
    }
    ::usleep(10000);
    return rv;
}

/****************************************************************************************
*/
void bu_hci::_clear_cache()
{
    for(auto  &el : _cache)
    {
        no_acl_start_len_dynamic* pd = el.second;
        delete pd;
    }
    _cache.clear();
}

/****************************************************************************************
*/
int bu_hci::on_sock_data(uint8_t code, const sdata& buffer) //received
{
    uint8_t  eventType = buffer.data[0];
    uint16_t blen = buffer.len;
    std::string scase="NOT HANDLED ";

    bybuff  trace(buffer.data, buffer.len);
    TRACE("-->["<< int(buffer.len) <<"]"<< trace.to_string());

    if (HCI_EVENT_PKT == eventType)
    {
        switch(buffer.data[1])
        {
            case EVT_DISCONN_COMPLETE:
                scase="EVT_DISCONN_COMPLETE";
                {
                    evt_disconn_complete* pdc = (evt_disconn_complete*)(buffer.data+4);
                    pdc->handle=htobs(pdc->handle);
                    memcpy(&_dcached, pdc, sizeof(_dcached));
                    _pev->on_disconnect(pdc);
                    _clear_cache();
                    _connected=false;
                }
                break;
            case EVT_ENCRYPT_CHANGE:
                scase="EVT_ENCRYPT_CHANGE";
                {
                    evt_encrypt_change* pec = (evt_encrypt_change*)(buffer.data+4);
                    pec->handle=htobs(pec->handle);
                    _pev->on_encrypt_chnage(pec);
                }
                break;
            case EVT_CMD_COMPLETE:
                scase="EVT_CMD_COMPLETE";
                {
                    no_evt_cmd_complete* necc = (no_evt_cmd_complete*)(buffer.data+4);
                    necc->cmd=htobs(necc->cmd);
                    this->_oncmd_complette(necc);
                }
                break;
            case EVT_LE_META_EVENT:
                scase="EVT_LE_META_EVENT";

                {
                    no_evt_le_meta_event* pev = (no_evt_le_meta_event*)(buffer.data+3);
                    this->_onmeta(pev);
                }
                break;
            case EVT_CONN_REQUEST:
                scase="EVT_CONN_REQUEST";
                {
                    evt_conn_request* preq= (evt_conn_request*)(buffer.data+4);
                    bdaddr_t dest;
                    baswap(&dest, &preq->bdaddr);
                    memcpy(&preq->bdaddr,&dest,sizeof(dest));
                }
                break;
            case  EVT_CMD_STATUS:
                scase="EVT_CMD_STATUS";
                {
                    evt_cmd_status* pevs = (evt_cmd_status*)(buffer.data+4);
                    size_t elll = 3 + sizeof(evt_cmd_status);
                }
                break;
            case EVT_REMOTE_NAME_REQ_COMPLETE:
                scase="EVT_REMOTE_NAME_REQ_COMPLETE";
                {
                    evt_remote_name_req_complete* pnc = (evt_remote_name_req_complete*)(buffer.data+4);
                    TRACE("remote name: " << pnc->name);
                }
                break;
            default:
                break;
        }//switch
    }
    else if (HCI_ACLDATA_PKT == eventType)
    {
        uint16_t  val =  oa2t<uint16_t>(buffer.data,1);
        uint16_t flags =  val >> 12;
        uint16_t handle = val & 0x0FFF;

        if(ACL_START_NO_FLUSH == flags)
        {
            ///_clear_cache();  TODO
            scase="ACL_START_NO_FLUSH";
            flags = ACL_START;
        }
        if (ACL_START == flags)// || ACL_START_NO_FLUSH==flags)
        {
            uint16_t cid  = oa2t<uint16_t>(buffer.data,7);
            uint16_t expectedlen = oa2t<uint16_t>(buffer.data,5);
            uint16_t chunklen = blen-9;
            sdata sd;

            scase="ACL_START";

            sd.len = expectedlen;
            sd.data = buffer.data + 9;
            if (expectedlen == chunklen)
            {
                _pev->on_acl_packet(handle,cid, sd);
            }
            else
            {
                /// assert(_cache.find(handle) == _cache.end());
                //accumulate data bt handler
                if(_cache.find(handle) == _cache.end())
                {
                    no_acl_start_len_dynamic* pd = new(no_acl_start_len_dynamic);
                    pd->cit = (cid);
                    pd->len = (chunklen);
                    pd->expectedlen = (expectedlen);
                    pd->byarr.insert(pd->byarr.end(), sd.data, sd.data + sd.len);
                    _cache[handle] = pd;
                }
                else
                {
                    TRACE("Error: We should not be getting this!. \n");
                }
            }
        }
        else if (ACL_CONT == flags)
        {
            /// keep accumulating, or discard
            uint16_t chunklen = blen-9;
            sdata sd;

            scase="ACL_CONT";

            auto el = _cache.find(handle);
            if (el == _cache.end())
                return true;

            no_acl_start_len_dynamic* pd = el->second;
            pd->len += chunklen;
            pd->byarr.insert(pd->byarr.end(), sd.data, sd.data + sd.len);
            //assert(pd->expectedlen == expectedlen);

            if(pd->expectedlen == pd->len )
            {
                sd.len = pd->len;
                sd.data = &pd->byarr[0];
                _pev->on_acl_packet(handle, pd->cit, sd);
                delete pd;
                _cache.erase(handle);
            }
        }
        else
        {
            TRACE("!!! HCI_ACLDATA_PKT  unknown flag: " << std::hex << int(flags) << std::dec);
        }
    }
    else{
       TRACE("!!!  NO KNOWN on_sock_data EVENTTYPE " << std::hex << int(eventType) << std::dec );
    }
    TRACE("HCI: " << scase);
    return true;
}

/****************************************************************************************
*/
void bu_hci::on_error(const hci_error& error)
{
    if (error.message == "network-error")
    {
        this->_onhci_state_chnaged(STATE_NETWORK_DOWN);
    }
}

/****************************************************************************************
*/
void bu_hci::_oncmd_complette(const no_evt_cmd_complete* nevcc)
{
    uint16_t    handle;

    switch(nevcc->cmd)
    {
        case RESET_CMD:
             TRACE("RESET_CMD");
            _reconfigure();
            break;
        case READ_LE_HOST_SUPPORTED_CMD:
            {
                TRACE("READ_LE_HOST_SUPPORTED_CMD");
                uint8_t le = nevcc->data[0];
                uint8_t simul = nevcc->data[1];
            }
            break;
        case READ_LOCAL_VERSION_CMD:
            {
                TRACE("READ_LOCAL_VERSION_CMD");
                uint8_t  hciVer = nevcc->data[0];
                if (hciVer < 0x06)
                {
                    this->_onhci_state_chnaged(STATE_UNSUPORTED);
                }
                uint16_t hciRev = oa2t<uint16_t>(nevcc->data,1);
                uint8_t lmpVer = nevcc->data[3];
                uint16_t m1 = oa2t<uint16_t>(nevcc->data,4);
                uint16_t lmpSubVer = oa2t<uint16_t>(nevcc->data,6);

                _pev->on_read_version(hciVer, hciRev, lmpVer, m1, lmpSubVer);
                if (_state != STATE_POWEREDON)
                {
                    enable_adv(0);
                    _set_adv_params();
                }
            }
            break;
        case READ_BD_ADDR_CMD:
            {
                TRACE("READ_BD_ADDR_CMD");
                memcpy(&_address, nevcc->data, sizeof(bdaddr_t));
                _addrtype = ADDR_PUBLIC;
                _pev->on_mac_change(_address);
                char out[32];
                ba2str(&_address, out);
            }
            break;
        case LE_SET_ADVERTISING_PARAMETERS_CMD:
            TRACE("LE_SET_ADVERTISING_PARAMETERS_CMD");
            _onhci_state_chnaged(STATE_POWEREDON);
            _pev->on_adv_status(_state);
            break;
        case LE_SET_ADVERTISING_DATA_CMD:
            TRACE("LE_SET_ADVERTISING_DATA_CMD");
            _pev->on_adv_data_status(nevcc->status);
            break;
        case LE_SET_SCAN_RESPONSE_DATA_CMD:
            TRACE("LE_SET_SCAN_RESPONSE_DATA_CMD");
            _pev->on_scan_resp_datat_status(nevcc->status);
            break;
        case LE_SET_ADVERTISE_ENABLE_CMD:
            TRACE("LE_SET_ADVERTISE_ENABLE_CMD");
            _pev->on_adv_enable(nevcc->status);
            break;
        case READ_RSSI_CMD:
            {
                TRACE("READ_RSSI_CMD");
                handle =  oa2t<uint16_t>(nevcc->data,0); //result.readUInt16LE(0);
                uint8_t rssi = nevcc->data[2];
                _pev->on_rssi(handle, rssi);
            }
            break;
        case LE_LTK_NEG_REPLY_CMD:
            TRACE("LE_LTK_NEG_REPLY_CMD");
            handle =  oa2t<uint16_t>(nevcc->data,0);
            _pev->le_ltk_neg_reply(handle);
            break;
        case CMD_OPCODE_PACK(OCF_HOLD_MODE,OGF_LINK_POLICY):
            {
                TRACE("CMD_OPCODE_PACK(OCF_HOLD_MODE,OGF_LINK_POLICY)");
                hold_mode_cp* hmcp = (hold_mode_cp*)nevcc->data;
            }
            break;
        case CMD_OPCODE_PACK(OCF_INQUIRY,OGF_LINK_CTL):
            {
                TRACE("CMD_OPCODE_PACK(OCF_INQUIRY,OGF_LINK_CTL)");
            }
            break;
        default:
            {
                TRACE("default");
                uint16_t ogf = CMD_OPCODE_OGF(nevcc->cmd);
                uint16_t ocf = CMD_OPCODE_OCF(nevcc->cmd);
                TRACE("UNK command: OCF=" << std::hex <<
                                                int(ocf) <<
                                                ", OGF=" << int(ogf) << std::dec);
            }
            break;
    }
}

/****************************************************************************************
*/
void bu_hci::_onmeta(const no_evt_le_meta_event* leme)
{
    if (leme->leMetaEventType == EVT_LE_CONN_COMPLETE)
    {
        TRACE("EVT_LE_CONN_COMPLETE" );
        if(_connected)
        {
            _pev->on_disconnect(&_dcached);
            _clear_cache();
        }
        this->_onle_complette(leme);
        _connected=true;
    }
    else if (leme->leMetaEventType == EVT_LE_CONN_UPDATE_COMPLETE)
    {
        TRACE("EVT_LE_CONN_UPDATE_COMPLETE" );
        this->_onle_con_update_complette(leme);
    }
    else if(leme->leMetaEventType == EVT_LE_ADVERTISING_REPORT)
    {
        le_advertising_info* lead = (le_advertising_info*)leme->data;
        char bsttr[32];

        TRACE("EVT_LE_ADVERTISING_REPORT" );
        ba2str(&lead->bdaddr, bsttr);
        TRACE("EV TYPE = " << (int)lead->evt_type);
        TRACE("BADDR_TYPE = " << (int)lead->bdaddr_type);
        TRACE("ADDR TYPE = " << (int)lead->bdaddr_type);
        TRACE("MAC = " << (const char*)bsttr);
        TRACE("LENGTH = " << (int)lead->length);
    }
    else if(leme->leMetaEventType == EVT_LE_LTK_REQUEST)
    {
        TRACE("EVT_LE_LTK_REQUEST" );
    }
    else if(leme->leMetaEventType == EVT_LE_READ_REMOTE_USED_FEATURES_COMPLETE)
    {
#define EVT_LE_READ_REMOTE_USED_FEATURES_COMPLETE	0x04
typedef struct {
	uint8_t		status;
	uint16_t	handle;
	uint8_t		features[8];
} __attribute__ ((packed)) evt_le_read_remote_used_features_complete;
#define EVT_LE_READ_REMOTE_USED_FEATURES_COMPLETE_SIZE 11
    }
    else
        TRACE("UNKNOWN META EVENT " << std::hex <<leme->leMetaEventType );
}

/****************************************************************************************
*/
void bu_hci::_onle_complette(const no_evt_le_meta_event* leme)
{
    uint16_t handle = oa2t<uint16_t>(leme->data,0);
    uint8_t role = leme->data[2];
    HCI_ADDRTYPE addressType = leme->data[3]== 0x01 ? ADDR_RANDOM : ADDR_PUBLIC;
    bdaddr_t addr = *((bdaddr_t*)&leme->data[4]);

    uint16_t interval = oa2t<uint16_t>(leme->data, 10) * 1.25;
    uint16_t latency = oa2t<uint16_t>(leme->data,12); // TODO: multiplier?
    uint16_t supervisionTimeout = oa2t<uint16_t>(leme->data, 14) * 10;
    uint8_t  masterClockAccuracy = leme->data[16]; // TODO: multiplier?

    char bsttr[32];
    ba2str(&addr, bsttr);

    TRACE("HANDLE = " << (int)handle);
    TRACE("role = " << (int)role);
    TRACE("address type = " << (int)addressType);
    TRACE("address = " << (const char*)bsttr);
    TRACE("interval = " << (int)interval);
    TRACE("latency = " << (int)latency);
    TRACE("supervision timeout = " << (int)supervisionTimeout);
    TRACE("master clock accuracy = " << (int)masterClockAccuracy);
    _pev->on_le_connected(leme->leMetaEventStatus, handle, role, addressType, addr,
                            interval, latency, supervisionTimeout, masterClockAccuracy);
}

/****************************************************************************************
*/
void bu_hci::_onle_con_update_complette(const no_evt_le_meta_event* leme)
{
    uint16_t handle = oa2t<uint16_t>(leme->data,0);
    uint16_t interval = oa2t<uint16_t>(leme->data,2) * 1.25;
    uint16_t latency = oa2t<uint16_t>(leme->data,4); // TODO: multiplier?
    uint16_t supervisionTimeout = oa2t<uint16_t>(leme->data,6) * 10;

    TRACE("handle = " << handle);
    TRACE("interval = " << interval);
    TRACE("latency = " << latency);
    TRACE("supervision timeout = " << supervisionTimeout);
    _pev->on_le_conn_update_complette_shit(leme->leMetaEventStatus,handle, interval, latency, supervisionTimeout);
}

/****************************************************************************************
*/
bool bu_hci::check_dev_state()
{
    if(_chekingdev){
        return true;
    }
    static int everysec=0;

    _chekingdev=true;
    if(everysec==0)
    {
        bool is_up = _socket->is_up();
        if (_isDevUp != is_up)
        {
            if (is_up)
            {
                try{
                    _reconfigure();
                }catch(...)
                {
                    is_up=false;
                }
            }
            _pev->on_dev_status(is_up);
            _isDevUp = is_up;
        }

        if(_isDevUp==false)
        {
            if(!_recreate_sock())
            {
                _THROW("");
            }
        }
    }
    if(++everysec>400)everysec=0;
    _chekingdev=false;
    return _isDevUp;
}

/****************************************************************************************
*/
bool bu_hci::_recreate_sock()
{
    _socket->close();
    _pev->on_configure_device(_devid);
     try
    {
        _socket->create();
        _socket->bind_raw(&_devid);
    }
    catch(hexecption& e)
    {
        e.report();
        return false;
    }
    return true;
}

/****************************************************************************************
*/
void bu_hci::_reconfigure()
{
    this->_set_hci_filter();
    this->_set_event_mask();
    this->_set_le_event_mask();
    this->_read_version();
    //_spinpool();
    this->_write_le_host();
    this->_read_le_hosts();
    this->_read_baddr();
    _spinpool();
}

/****************************************************************************************
*/
void bu_hci::read_baddr()
{
    this->_read_baddr();
    _spinpool();
}

/****************************************************************************************
*/
void bu_hci::_spinpool(int lops)
{
    int i = lops;
    int pbytes;
    while(--i>0){
        _socket->pool(&pbytes);
        if(pbytes)
        {
            i += 4;
        }
        ::usleep(0xFF);
        if(i>8912)// time-out
        {
            _socket->stop();
            break;
        }
    }
}

