#ifndef VFS_H
#define VFS_H
#include "types.h"

// define file type
#define FS_FILE 0x1
#define FS_DIR  0x2
#define FS_CHARDEVICE 0x3
#define FS_BLOCKDEVICE 0x4
#define FS_PIPE 0x5
#define FS_SYMLINK 0x6

#define FS_MOUNTPOINT 0x8

struct fs_node;

struct dirent {
	char name[128];
	uint32_t ino;
};


typedef uint32_t (*read_type_t)(struct fs_node*, uint32_t, uint32_t, char*);
typedef uint32_t (*write_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent* (*readdir_type_t)(struct fs_node*, uint32_t);
typedef struct fs_node* (*finddir_type_t)(struct fs_node*, char* name);


typedef struct fs_node {
	char name[128];
	uint32_t file_type;
	uint32_t file_len;

	read_type_t read;
	write_type_t write;
	open_type_t open;
	close_type_t close;
	readdir_type_t readdir;
	finddir_type_t finddir;

	struct fs_node *ptr;
} fs_node_t;

extern fs_node_t *fs_root;

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, char *buffer);
uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_fs(fs_node_t *node, uint8_t read, uint32_t write);
void close_fs(fs_node_t *node);
struct dirent* readdir_fs(fs_node_t *node, uint32_t index);
fs_node_t* finddir_fs(fs_node_t *node, char *name);

#endif