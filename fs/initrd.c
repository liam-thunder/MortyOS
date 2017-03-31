#include "initrd.h"
#include "mem/heap.h"
#include "common/string.h"

fs_node_t *initrd_root;
fs_node_t *initrd_dev;

fs_node_t *root_nodes;
uint32_t root_nodes_num;

super_block_t* vsb;

const uint32_t vfs_root_idx = 2;

struct dirent dirent;

static void read_root_dir(super_block_t* vsb);
static uint32_t file_read(super_block_t* vsb, char* filename, char* buffer);
static uint32_t initrd_read(fs_node_t* node, uint32_t offset, uint32_t size, char* buffer);
static struct dirent *initrd_readdir(fs_node_t *node, uint32_t index);
static fs_node_t *initrd_finddir(fs_node_t *node, char *name);


fs_node_t *init_initrd(uint32_t s_addr) {
	vsb = init_superblock(s_addr);

	initrd_root = (fs_node_t*)kmalloc(sizeof(fs_node_t));
	strcpy(initrd_root->name, "initrd");
	initrd_root->file_type = FS_DIR;
	initrd_root->file_len = 0;

	initrd_root->read = 0;
	initrd_root->write = 0;
	initrd_root->open = 0;
	initrd_root->close = 0;
	initrd_root->readdir = &initrd_readdir;
	initrd_root->finddir = &initrd_finddir;
	initrd_root->ptr = 0;

	initrd_dev = (fs_node_t*)kmalloc(sizeof(fs_node_t));
	strcpy(initrd_dev->name, "dev");
	initrd_dev->file_type = FS_DIR;
	initrd_dev->file_len = 0;

	initrd_dev->read = 0;
	initrd_dev->write = 0;
	initrd_dev->open = 0;
	initrd_dev->close = 0;
	initrd_dev->readdir = &initrd_readdir;
	initrd_dev->finddir = &initrd_finddir;
	initrd_dev->ptr = 0;

	root_nodes_num = 0;

	read_root_dir(vsb);

	return initrd_root;
}

// read info of files contained in root dir into root_nodes
static void read_root_dir(super_block_t* vsb) {
	inode_t* root_inode = get_inode_ptr(vsb, vfs_root_idx);
    block_t* root_block = get_block_ptr(vsb, root_inode->block_idx[0]);
    root_nodes_num = root_block->file_num - 2;
    root_nodes = (fs_node_t*)kmalloc(sizeof(fs_node_t) * root_nodes_num);
    for (uint32_t i = 0, file_num = 0; i < root_block->file_num; i++) {
        if (strcmp(root_block->file_name[i], ".") == 0 || strcmp(root_block->file_name[i], "..") == 0) continue;
        strcpy(root_nodes[file_num].name, root_block->file_name[i]);
        inode_t* file_inode = get_inode_ptr(vsb, root_block->file_inode[i]);
        root_nodes[file_num].file_len = file_inode->file_size;
        root_nodes[file_num].file_type = FS_FILE;
        root_nodes[file_num].read = &initrd_read;
        root_nodes[file_num].write = 0;
        root_nodes[file_num].readdir = 0;
        root_nodes[file_num].finddir = 0;
        root_nodes[file_num].open = 0;
        root_nodes[file_num++].close = 0;
    }
}

// read file contents into buffer
static uint32_t file_read(super_block_t* vsb, char* filename, char* buffer) {
    inode_t* root_inode = get_inode_ptr(vsb, vfs_root_idx);
    block_t* root_block = get_block_ptr(vsb, root_inode->block_idx[0]);
    inode_t* file_inode = NULL;
    char* buffer_ptr = buffer;
    uint32_t ret_size = 0;
    for (uint32_t i = 0; i < root_block->file_num; i++) {
        if (strcmp(filename, root_block->file_name[i]) != 0) continue;
        file_inode = get_inode_ptr(vsb, root_block->file_inode[i]);
        break;
    }
    if (!file_inode) return 0;
    for (uint32_t i = 0; i < file_inode->block_num; i++) {
        void* data = get_block_data_ptr(vsb, file_inode->block_idx[i]);
        block_t* block = get_block_ptr(vsb, file_inode->block_idx[i]);
        strncpy(buffer_ptr, (char*)data, block->block_size);
        buffer_ptr += block->block_size;
        ret_size += block->block_size;
    }
    return ret_size;
}

static uint32_t initrd_read(fs_node_t* node, uint32_t offset, uint32_t size, char* buffer) {
	return file_read(vsb, node->name, buffer);
}

static struct dirent *initrd_readdir(fs_node_t *node, uint32_t index) {
	if(node ==  initrd_root && index == 0) {
		strcpy(dirent.name, "dev");
		dirent.name[3] = 0;
		dirent.ino = 0;
		return &dirent;
	}
	if(index -1 >= root_nodes_num) return 0;
	strcpy(dirent.name, root_nodes[index-1].name);
	dirent.name[strlen(root_nodes[index-1].name)] = 0; // Make sure the string is NULL-terminated.
    return &dirent;
} 

static fs_node_t *initrd_finddir(fs_node_t *node, char *name) {
	if(node == initrd_root && !strcmp(name, "dev")) return initrd_dev;
	for(int i = 0; i < root_nodes_num; i++)
			if(!strcmp(name, root_nodes[i].name))
				return &root_nodes[i];
	return 0;
}