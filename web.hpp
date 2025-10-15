// Based upon https://github.com/LearnEmbeddedSystems/pico-w-webserver-template and official pico_w access point example
#ifndef WEB_H
#define WEB_H

#include "dhcpserver.h"
#include "dnsserver.h"
#include "lwipopts.h"
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "flash_utils.hpp"

// WIFI Credentials - take care if pushing to github!
#include "secrets.hpp"
#define WIFI_RETRY 5
const char *apssid = WIFI_SSID;
const char *appassword = WIFI_PASSWORD;

char ssid[33]={'\0'};
char pwd[33]={'\0'};

dhcp_server_t dhcp_server;
dns_server_t dns_server;

int sta_init() {
    printf("Enabling STA mode...\n");
    cyw43_arch_enable_sta_mode();
    // Connect to the WiFI network

    int ret=-1;
    for (int i=0; i<WIFI_RETRY && ret!=0; i++) {
        printf("Trying to connect to %s...\n", ssid);
        ret=cyw43_arch_wifi_connect_timeout_ms(ssid, pwd, CYW43_AUTH_WPA2_AES_PSK, 30000);
    }
    return ret;
}

void server_init() {
    // Initialise web server
    printf("Starting HTTP server...\n");

    httpd_init();
    printf("HTTP server initialised\n");
}

void ap_init() {
    printf("Enabling AP mode...\n");

    cyw43_arch_enable_ap_mode(apssid, appassword, CYW43_AUTH_WPA2_AES_PSK);
    printf("AP mode enabled...\n");

    ip4_addr_t gw, mask;
    gw.addr = PP_HTONL(CYW43_DEFAULT_IP_AP_ADDRESS);
    mask.addr = PP_HTONL(CYW43_DEFAULT_IP_MASK);

    // Start the dhcp server
    printf("Starting DHCP...\n");

    dhcp_server_init(&dhcp_server, &gw, &mask);
    printf("DHCP started. Starting DNS...\n");

    // Start the dns server
    dns_server_init(&dns_server, &gw);
    printf("DNS started...\n");
}

void readSavedWifi() {
    char buffer[66]="";
    readPersistent(buffer, 66);
                buffer[32]='\0';
                    buffer[65]='\0';
    strncpy(ssid, buffer, 32);
    strncpy(pwd, &(buffer[33]),32);
}

void saveWifi() {
    char buffer[66]="";
    strncpy(buffer, ssid,32);
    buffer[32]='\0';
    strncpy(&(buffer[33]), pwd,32);
    buffer[65]='\0';
    writePersistent(buffer, 66);
}

void web_init() {
    stdio_init_all();

    cyw43_arch_init();

    readSavedWifi();
    if (sta_init()!=0) {
        ap_init();
        ssid[0]='\0';
    }
    server_init();
}
#endif