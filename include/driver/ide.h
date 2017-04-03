#ifndef IDE_H
#define IDE_H

#define SECTOR_SIZE 512
#define IDE_BSY 0x80
#define IDE_DRDY 0x40
#define IDE_DF 0x20
#define IDE_ERR 0x1

#define IDE_CMD_READ 0x20
#define IDE_CMD_WRITE 0x30
#define IDE_CMD_IDENTIFY 0xEC

#define IO_BASE 0x1F0

void ide_init();
#endif