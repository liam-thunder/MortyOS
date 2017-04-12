#include "driver/ide.h"
#include "common/common.h"
#include "driver/buf.h"

static int32_t ide_wait(int32_t is_err_check);
static int32_t ide_start_request(struct buf* b);

void init_ide() {
    int32_t tmp = ide_wait(0);
    return;
}


static int32_t ide_wait(int32_t is_err_check) {
    int32_t r;
    // Command Block Registers Address 0x1F7 = Command/status
    while(((r = inb(0x1F7)) & (IDE_BSY | IDE_DRDY)) != IDE_DRDY) {
        // do nothing...
    }
    if(is_err_check && (r & (IDE_DF|IDE_ERR)) != 0)
        return -1;
    return 0;
}

static int32_t ide_start_request(struct buf* b) {
    // TODO...
    return 0;
}