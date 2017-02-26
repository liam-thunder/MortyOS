#ifndef INITRD_H
#define INITRD_H

#include "types.h"
#include "vsfs.h"

fs_node_t *init_initrd(uint32_t s_addr);

#endif