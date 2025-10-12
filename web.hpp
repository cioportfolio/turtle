// Based upon https://github.com/LearnEmbeddedSystems/pico-w-webserver-template and official pico_w access point example
#ifndef WEB_H
#define WEB_H

#include "dhcpserver.h"
#include "dnsserver.h"
#include "lwip/apps/httpd.h"
#include "lwipopts.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

// WIFI Credentials - take care if pushing to github!
#include "secrets.hpp"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
//#define STA //uncomment to connect to existing Wifi using ssid and password instead of creating an access point

dhcp_server_t dhcp_server;
dns_server_t dns_server;

void sta_init() {
    printf("Enabling STA mode...\n");
    cyw43_arch_enable_sta_mode();
    // Connect to the WiFI network
    while (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000)!=0) {
        printf("Attempting to connect...\n");
    }
    printf("Connected.\n");
}

void server_init() {
    // Initialise web server
    printf("Starting HTTP server...\n");

    httpd_init();
    printf("HTTP server initialised\n");
}

void ap_init() {
    printf("Enabling AP mode...\n");

    cyw43_arch_enable_ap_mode(ssid, password, CYW43_AUTH_WPA2_AES_PSK);
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

void web_init() {
    stdio_init_all();

    cyw43_arch_init();

#ifdef STA
    sta_init();
#else
    ap_init();
#endif
    server_init();
}
#endif