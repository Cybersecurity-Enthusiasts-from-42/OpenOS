/*
 * OpenOS - Virtual File System
 * 
 * Provides a unified interface for file operations across different filesystems.
 */

#ifndef OPENOS_FS_VFS_H
#define OPENOS_FS_VFS_H

#include <stdint.h>
#include <stddef.h>

/* Define ssize_t if not available */
#ifndef SSIZE_MAX
typedef int32_t ssize_t;
#endif

/* Maximum limits for static allocation */
#define VFS_MAX_NODES       128
#define VFS_MAX_CHILDREN    32
#define VFS_MAX_FILE_SIZE   4096
#define VFS_NAME_LENGTH     128
#define VFS_MAX_PATH_LENGTH 256

/* Node types */
typedef enum {
    NODE_FILE,
    NODE_DIRECTORY
} vfs_node_type_t;

/* Forward declarations */
struct vfs_node;
struct vfs_dirent;

/* Directory entry structure */
typedef struct vfs_dirent {
    char name[VFS_NAME_LENGTH];
    uint32_t inode;
} vfs_dirent_t;

/* VFS node structure */
typedef struct vfs_node {
    char name[VFS_NAME_LENGTH];
    vfs_node_type_t type;
    uint32_t inode;
    uint32_t length;
    uint32_t flags;
    struct vfs_node* parent;
    struct vfs_node* children[VFS_MAX_CHILDREN];
    uint32_t child_count;
    uint8_t content[VFS_MAX_FILE_SIZE];
    
    /* Function pointers for operations */
    ssize_t (*read)(struct vfs_node*, uint32_t, uint32_t, uint8_t*);
    ssize_t (*write)(struct vfs_node*, uint32_t, uint32_t, const uint8_t*);
    void (*open)(struct vfs_node*);
    void (*close)(struct vfs_node*);
    struct vfs_dirent* (*readdir)(struct vfs_node*, uint32_t);
} vfs_node_t;

/* VFS initialization */
void vfs_init(void);

/* Core VFS functions */
vfs_node_t* vfs_get_root(void);
vfs_node_t* vfs_create_node(const char* name, vfs_node_type_t type);
vfs_node_t* vfs_find_node(vfs_node_t* parent, const char* name);
int vfs_add_child(vfs_node_t* parent, vfs_node_t* child);
int vfs_remove_child(vfs_node_t* parent, const char* name);
vfs_node_t* vfs_resolve_path(const char* path);

/* File operations */
ssize_t vfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);
ssize_t vfs_write(vfs_node_t* node, uint32_t offset, uint32_t size, const uint8_t* buffer);
vfs_dirent_t* vfs_readdir(vfs_node_t* node, uint32_t index);

/* Utility functions */
void vfs_list_directory(vfs_node_t* dir);

#endif /* OPENOS_FS_VFS_H */
