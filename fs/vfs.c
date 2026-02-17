/*
 * OpenOS - Virtual File System Implementation
 * 
 * RAM-based filesystem (ramfs) implementation with static memory allocation.
 */

#include "vfs.h"
#include "../kernel/string.h"
#include "../drivers/console.h"

/* Static memory pool for VFS nodes */
static vfs_node_t node_pool[VFS_MAX_NODES];
static int node_pool_bitmap[VFS_MAX_NODES];  /* 0 = free, 1 = used */
static uint32_t next_inode = 1;

/* Root directory pointer */
static vfs_node_t* vfs_root = 0;

/* Static dirent for readdir operations */
static vfs_dirent_t static_dirent;

/* Forward declarations of operation functions */
static ssize_t ramfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);
static ssize_t ramfs_write(vfs_node_t* node, uint32_t offset, uint32_t size, const uint8_t* buffer);
static void ramfs_open(vfs_node_t* node);
static void ramfs_close(vfs_node_t* node);
static vfs_dirent_t* ramfs_readdir(vfs_node_t* node, uint32_t index);

/*
 * Allocate a node from the static pool
 */
static vfs_node_t* allocate_node(void) {
    for (int i = 0; i < VFS_MAX_NODES; i++) {
        if (node_pool_bitmap[i] == 0) {
            node_pool_bitmap[i] = 1;
            
            /* Initialize the node */
            vfs_node_t* node = &node_pool[i];
            for (int j = 0; j < VFS_NAME_LENGTH; j++) {
                node->name[j] = 0;
            }
            node->type = NODE_FILE;
            node->inode = next_inode++;
            node->length = 0;
            node->flags = 0;
            node->parent = 0;
            node->child_count = 0;
            for (int j = 0; j < VFS_MAX_CHILDREN; j++) {
                node->children[j] = 0;
            }
            for (int j = 0; j < VFS_MAX_FILE_SIZE; j++) {
                node->content[j] = 0;
            }
            
            /* Set up operation function pointers */
            node->read = ramfs_read;
            node->write = ramfs_write;
            node->open = ramfs_open;
            node->close = ramfs_close;
            node->readdir = ramfs_readdir;
            
            return node;
        }
    }
    return 0;  /* Out of nodes */
}

/*
 * Free a node back to the pool
 */
static void free_node(vfs_node_t* node) {
    for (int i = 0; i < VFS_MAX_NODES; i++) {
        if (&node_pool[i] == node) {
            node_pool_bitmap[i] = 0;
            return;
        }
    }
}

/*
 * Read from a file node
 */
static ssize_t ramfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
    if (!node || node->type != NODE_FILE) {
        return -1;
    }
    
    if (offset >= node->length) {
        return 0;  /* EOF */
    }
    
    uint32_t bytes_to_read = size;
    if (offset + bytes_to_read > node->length) {
        bytes_to_read = node->length - offset;
    }
    
    for (uint32_t i = 0; i < bytes_to_read; i++) {
        buffer[i] = node->content[offset + i];
    }
    
    return bytes_to_read;
}

/*
 * Write to a file node
 */
static ssize_t ramfs_write(vfs_node_t* node, uint32_t offset, uint32_t size, const uint8_t* buffer) {
    if (!node || node->type != NODE_FILE) {
        return -1;
    }
    
    if (offset + size > VFS_MAX_FILE_SIZE) {
        size = VFS_MAX_FILE_SIZE - offset;
    }
    
    for (uint32_t i = 0; i < size; i++) {
        node->content[offset + i] = buffer[i];
    }
    
    if (offset + size > node->length) {
        node->length = offset + size;
    }
    
    return size;
}

/*
 * Open a file (placeholder)
 */
static void ramfs_open(vfs_node_t* node) {
    (void)node;
    /* Nothing to do for ramfs */
}

/*
 * Close a file (placeholder)
 */
static void ramfs_close(vfs_node_t* node) {
    (void)node;
    /* Nothing to do for ramfs */
}

/*
 * Read directory entry
 */
static vfs_dirent_t* ramfs_readdir(vfs_node_t* node, uint32_t index) {
    if (!node || node->type != NODE_DIRECTORY) {
        return 0;
    }
    
    if (index >= node->child_count) {
        return 0;
    }
    
    vfs_node_t* child = node->children[index];
    if (!child) {
        return 0;
    }
    
    string_copy(static_dirent.name, child->name);
    static_dirent.inode = child->inode;
    
    return &static_dirent;
}

/*
 * Initialize the VFS
 */
void vfs_init(void) {
    /* Initialize node pool */
    for (int i = 0; i < VFS_MAX_NODES; i++) {
        node_pool_bitmap[i] = 0;
    }
    
    /* Create root directory */
    vfs_root = allocate_node();
    if (!vfs_root) {
        console_write("ERROR: Failed to allocate root directory\n");
        return;
    }
    
    string_copy(vfs_root->name, "/");
    vfs_root->type = NODE_DIRECTORY;
    vfs_root->parent = vfs_root;  /* Root is its own parent */
    
    /* Create initial directory structure */
    vfs_node_t* bin = vfs_create_node("bin", NODE_DIRECTORY);
    vfs_node_t* etc = vfs_create_node("etc", NODE_DIRECTORY);
    vfs_node_t* home = vfs_create_node("home", NODE_DIRECTORY);
    vfs_node_t* tmp = vfs_create_node("tmp", NODE_DIRECTORY);
    
    if (bin) vfs_add_child(vfs_root, bin);
    if (etc) vfs_add_child(vfs_root, etc);
    if (home) vfs_add_child(vfs_root, home);
    if (tmp) vfs_add_child(vfs_root, tmp);
    
    /* Create sample file in /etc */
    if (etc) {
        vfs_node_t* motd = vfs_create_node("motd.txt", NODE_FILE);
        if (motd) {
            const char* content = "Welcome to OpenOS!\nThis is a test file in the filesystem.\n";
            uint32_t len = string_length(content);
            vfs_write(motd, 0, len, (const uint8_t*)content);
            vfs_add_child(etc, motd);
        }
    }
}

/*
 * Get the root directory
 */
vfs_node_t* vfs_get_root(void) {
    return vfs_root;
}

/*
 * Create a new VFS node
 */
vfs_node_t* vfs_create_node(const char* name, vfs_node_type_t type) {
    vfs_node_t* node = allocate_node();
    if (!node) {
        return 0;
    }
    
    string_copy(node->name, name);
    node->type = type;
    
    return node;
}

/*
 * Find a child node by name
 */
vfs_node_t* vfs_find_node(vfs_node_t* parent, const char* name) {
    if (!parent || parent->type != NODE_DIRECTORY) {
        return 0;
    }
    
    for (uint32_t i = 0; i < parent->child_count; i++) {
        if (parent->children[i] && string_compare(parent->children[i]->name, name) == 0) {
            return parent->children[i];
        }
    }
    
    return 0;
}

/*
 * Add a child node to a directory
 */
int vfs_add_child(vfs_node_t* parent, vfs_node_t* child) {
    if (!parent || !child || parent->type != NODE_DIRECTORY) {
        return -1;
    }
    
    if (parent->child_count >= VFS_MAX_CHILDREN) {
        return -1;  /* Directory full */
    }
    
    parent->children[parent->child_count] = child;
    parent->child_count++;
    child->parent = parent;
    
    return 0;
}

/*
 * Remove a child node from a directory
 */
int vfs_remove_child(vfs_node_t* parent, const char* name) {
    if (!parent || parent->type != NODE_DIRECTORY) {
        return -1;
    }
    
    for (uint32_t i = 0; i < parent->child_count; i++) {
        if (parent->children[i] && string_compare(parent->children[i]->name, name) == 0) {
            /* Free the node */
            free_node(parent->children[i]);
            
            /* Shift remaining children */
            for (uint32_t j = i; j < parent->child_count - 1; j++) {
                parent->children[j] = parent->children[j + 1];
            }
            parent->children[parent->child_count - 1] = 0;
            parent->child_count--;
            
            return 0;
        }
    }
    
    return -1;  /* Child not found */
}

/*
 * Resolve a path to a node
 */
vfs_node_t* vfs_resolve_path(const char* path) {
    if (!path || path[0] == '\0') {
        return 0;
    }
    
    /* Start from root if absolute path */
    vfs_node_t* current = vfs_root;
    if (path[0] != '/') {
        return 0;  /* Only absolute paths supported for now */
    }
    
    /* Root directory */
    if (path[1] == '\0') {
        return vfs_root;
    }
    
    /* Parse path components */
    char path_copy[VFS_MAX_PATH_LENGTH];
    string_copy(path_copy, path + 1);  /* Skip leading slash */
    
    char* token = string_tokenize(path_copy, "/");
    while (token) {
        /* Handle special directories */
        if (string_compare(token, ".") == 0) {
            /* Current directory - do nothing */
        } else if (string_compare(token, "..") == 0) {
            /* Parent directory */
            if (current->parent) {
                current = current->parent;
            }
        } else {
            /* Regular directory/file name */
            vfs_node_t* next = vfs_find_node(current, token);
            if (!next) {
                return 0;  /* Path component not found */
            }
            current = next;
        }
        
        token = string_tokenize(0, "/");
    }
    
    return current;
}

/*
 * Read from a file
 */
ssize_t vfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
    if (!node || !node->read) {
        return -1;
    }
    
    return node->read(node, offset, size, buffer);
}

/*
 * Write to a file
 */
ssize_t vfs_write(vfs_node_t* node, uint32_t offset, uint32_t size, const uint8_t* buffer) {
    if (!node || !node->write) {
        return -1;
    }
    
    return node->write(node, offset, size, buffer);
}

/*
 * Read directory entry
 */
vfs_dirent_t* vfs_readdir(vfs_node_t* node, uint32_t index) {
    if (!node || !node->readdir) {
        return 0;
    }
    
    return node->readdir(node, index);
}

/*
 * List directory contents (utility function)
 */
void vfs_list_directory(vfs_node_t* dir) {
    if (!dir || dir->type != NODE_DIRECTORY) {
        console_write("Not a directory\n");
        return;
    }
    
    for (uint32_t i = 0; i < dir->child_count; i++) {
        vfs_node_t* child = dir->children[i];
        if (child) {
            console_write(child->name);
            if (child->type == NODE_DIRECTORY) {
                console_write("/");
            }
            console_write("\n");
        }
    }
}
