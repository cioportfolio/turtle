#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "web.hpp"

// SSI tags - tag length limited to 8 bytes by default
const char * ssi_tags[] = {"status", "ssid"};

u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
  size_t printed;
  switch (iIndex) {
  case 0: // status
    {
      printed = snprintf(pcInsert, iInsertLen, "Working");
    }
    break;
  case 1: // ssid
    {
      printed = snprintf(pcInsert, iInsertLen, ssid);
    }
    break;
  default:
    printed = 0;
    break;
  }

  return (u16_t)printed;
}

// Initialise the SSI handler
void ssi_init() {
  http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}
