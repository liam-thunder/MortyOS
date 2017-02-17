#include "vfs.h"

fs_node_t *fs_root = 0;

uint32_t readFS(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	if(node->read != 0) return node->read(node, offset, size, buffer);
	else return 0;
}

uint32_t writeFS(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	if(node->write != 0) return node->write(node, offset, size, buffer);
	else return 0;
}

void openFS(fs_node_t *node, uint8_t read, uint32_t write) {
	if(node->open != 0) return node->open(node, read, write);
	else return 0;
}