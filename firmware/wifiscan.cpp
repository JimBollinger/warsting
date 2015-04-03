
//#include "WifiScan.h"
#include "application.h"
#include "wlan.h"

//Wifiscan.h
#pragma once

#include "application.h"

// scan result taken from http://e2e.ti.com/support/wireless_connectivity/f/851/t/166684.aspx?pi310978=4
struct WifiScanResults_t {
    uint32_t networks;
    uint32_t status;
    uint8_t valid:1;
    uint8_t rssi:7;
    uint8_t security:2;  // 0 - open 
    uint8_t ssidlen:6;
    uint16_t time;
    uint8_t ssid[32];
    uint8_t bssid[6];
};

//wifiscan.cpp
class WifiScan {
    public:
        int startScan();
        bool next(WifiScanResults_t& result);        
};



int WifiScan::startScan() {
   const unsigned long intervalTime[16] = { 2000, 2000, 2000, 2000,  2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000 };

    wlan_ioctl_set_scan_params(4000, 20, 100, 5, 0x7FF, -120, 0, 300, 
        (unsigned long * ) &intervalTime);

    return 0;
}

/**
 * Determine if a given region of memory is empty - i.e. all zeros.
 */
bool is_empty(void* data, unsigned len) {
    for (unsigned i=0; i<len; i++)
        if (((uint8_t*)data)[i])
            return false;
    return true;
}

bool WifiScan::next(WifiScanResults_t& scanResult) {
    // clear the result
    memset(&scanResult, 0, sizeof(scanResult));
    long err = wlan_ioctl_get_scan_results(0, (uint8_t* ) &scanResult);
    return (!err && scanResult.valid && !is_empty(scanResult.bssid, sizeof(scanResult.bssid)));
}

