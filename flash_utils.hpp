#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pico/flash.h"
#include "hardware/flash.h"

extern uint32_t ADDR_PERSISTENT[];

/* From Jan Cumps at Element14 Community https://community.element14.com/products/raspberry-pi/b/blog/posts/raspberry-pico-c-sdk-reserve-a-flash-memory-block-for-persistent-storage */
inline char *getAddressPersistent() {
    return reinterpret_cast<char *>(ADDR_PERSISTENT);
}

inline uint32_t getOffsetPersistent() {
    return reinterpret_cast<uint32_t>(ADDR_PERSISTENT)-XIP_BASE;
}

static void call_flash_range_erase(void *param) {
    flash_range_erase(getOffsetPersistent(), FLASH_SECTOR_SIZE);
}

static void call_flash_range_program(void *param) {
    flash_range_program(getOffsetPersistent(), static_cast<const uint8_t *>(param), FLASH_PAGE_SIZE);
}

bool writePersistent(char * data, size_t len) {
    printf("Erase...\n");
    int ret = flash_safe_execute(call_flash_range_erase, NULL, UINT32_MAX);
    if (ret!=PICO_OK) {
        printf("Cannot erase persistent storages. Error: %d\n",ret);
        return false;
    }
    printf("Erased. Writing...\n");
    char buffer[FLASH_PAGE_SIZE];
    memcpy(buffer, data, len<FLASH_PAGE_SIZE?len:FLASH_PAGE_SIZE);
    ret = flash_safe_execute(call_flash_range_program, static_cast<void *>(buffer), UINT32_MAX);
    if (ret!=PICO_OK) {
        printf("Cannot write persistent storages. Error: %d\n",ret);
        return false;
    }
    printf("Written.\n");
    return true;
}

void readPersistent(char * buffer, size_t len) {
    printf("Symbol: 0x%08X\n", ADDR_PERSISTENT);
    printf("Address read: 0x%08X\n", getAddressPersistent());
    printf("XIP_BASE: 0x%08X Offset: 0x%08X\n", XIP_BASE, getOffsetPersistent());
    memcpy(buffer, getAddressPersistent(), len<FLASH_PAGE_SIZE?len:FLASH_PAGE_SIZE);
}