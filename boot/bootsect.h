#pragma once

#include "types.h"

extern uint8_t boot_drive;

void halt(char const*);

uint16_t read_lba_sectors(char *buf, uint8_t drive,
                         uint32_t lba, uint16_t count);
