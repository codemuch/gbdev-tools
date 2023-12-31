/*
    fixer.c

    This tool will take whatever bytes are in tile_map, shift each byte by SHIFT_VALUE,
    and store the new bytes in dynamic_tiles. The original buffer and the new buffer will
    be printed to the screen.
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BYTES_PER_LINE  16
#define TILE_MAP_SIZE   160
#define SHIFT_VALUE     64

unsigned char dynamic_tiles[255];

const unsigned char tile_map[] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0b, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x10, 0x11, 0x08, 0x12, 0x13, 0x14, 0x15, 0x14, 0x16, 0x16, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1d, 0x1f, 0x20, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x2a, 0x00, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x00, 0x31, 0x32, 0x31, 0x32, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x34, 0x2d, 0x35, 0x2f, 0x36, 0x37, 0x38, 0x39, 0x38, 0x3a, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x42, 0x44, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4c, 0x4d, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4e, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned char new_map[TILE_MAP_SIZE];

uint8_t i = 0; // community iterator

const unsigned char _get_hi_byte(const unsigned char bytes[], const uint8_t len){
    unsigned char hi = 0;
    for (i = 0; i < len; ++i) {
        
        if (bytes[i] > hi) {
            hi = bytes[i];
        }
    }
    return hi;
}

void pk_rebase(const unsigned char cb[], unsigned char nb[], uint8_t shift, const uint8_t len) {
    const unsigned char hi_byte = _get_hi_byte(cb, len);
    bool changed = false;
    for (i = 1; i <= hi_byte; ++i) {
        for (uint8_t j = 0; j < len; ++j) {
            if (cb[j] == 0x00) {
                nb[j] = cb[j];
            } else {
                if (cb[j] == i) {
                    nb[j] = shift;
                    changed = true;
                }
            }
        }
        if (changed) {
            changed = false;
             ++shift;
        }
    }
}

void print_bytes(const unsigned char bytes[], int len) {
    uint8_t printed;
    printf("[+] Byte map:\n{\n");
    for (i = 0, printed = 0; i < len; ++i) {

        if (printed == 0) { printf("  "); }
        printf("0x%02x, ", bytes[i]);
        ++printed;

        if (printed == BYTES_PER_LINE) {
            printf("\n");
            printed = 0;
        }
    }
    printf("}\n");
}

int main(void) {
    unsigned char hi_byte = _get_hi_byte(tile_map, TILE_MAP_SIZE);
    printf("[+] Map size: %d\n", TILE_MAP_SIZE);
    print_bytes(tile_map, TILE_MAP_SIZE);
    printf("[+] Highest byte found in map: 0x%02x\n", hi_byte);

    pk_rebase(
        tile_map,
        new_map,
        SHIFT_VALUE,
        TILE_MAP_SIZE
    );

    print_bytes(new_map, TILE_MAP_SIZE);

    return 0;
}