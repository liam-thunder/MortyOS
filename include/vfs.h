#ifndef VFS_H
#define VFS_H
#include "types.h"

#define FS_FILE 0x01
#define FS_DIR  0x02
#define FS_CHARDEVICE 0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE 0x05
#define FS_SYMLINK 0x06
#define FS_MOUNTPOINT 0x08

typedef uint32_t (*read_type_t)(fs_node*, uint32_t, uint32_t, uint8_t);
typedef uint32_t (*write_type_t)(fs_node*, uint32_t, uint32_t, uint8_t);
typedef void (*open_type_t)(fs_node*);
typedef void (*close_type_t)(fs_node*);
typedef dirent* (*readdir_type_t)(fs_node*, uint32_t);
typedef fs_node* (*finddir_type_t)(fs_node*, char* name);

typedef struct fs_node
{
	char name[128];
	uint32_t flags;
	uint32_t length;
	uint32_t inode;

	read_type_t read;
	write_type_t write;

	readdir_type_t readdir;
	finddir_type_t finddir;

	fs_node *ptr;

} fs_node_t;

struct dirent
{
	char name[128];
	uint32_t ino;
};


extern fs_node_t *fs_root;

uint32_t readFS(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t writeFS(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void openFS(fs_node_t *node, uint8_t read, uint32_t write);
void closeFS(fs_node_t *node);
dirent* readdirFS(fs_node_t *node, uint32_t index);
fs_node_t* finddir_fs(fs_node_t *node, char *name);

#endif