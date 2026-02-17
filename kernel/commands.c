/*
 * OpenOS - Built-in Shell Commands Implementation
 */

#include "commands.h"
#include "shell.h"
#include "string.h"
#include "kernel.h"
#include "../drivers/console.h"
#include "../drivers/timer.h"
#include "../arch/x86/ports.h"
#include "../fs/vfs.h"

/* Forward declaration for accessing command table */
const shell_command_t* shell_get_commands(int* count);

/*
 * Register all built-in commands
 */
void commands_register_all(void) {
    shell_register_command("help", "Display list of available commands", cmd_help);
    shell_register_command("clear", "Clear the console screen", cmd_clear);
    shell_register_command("echo", "Print text to console", cmd_echo);
    shell_register_command("uname", "Display OS name and version", cmd_uname);
    shell_register_command("uptime", "Show system uptime", cmd_uptime);
    shell_register_command("pwd", "Print current working directory", cmd_pwd);
    shell_register_command("ls", "List directory contents", cmd_ls);
    shell_register_command("cd", "Change directory", cmd_cd);
    shell_register_command("cat", "Display file contents", cmd_cat);
    shell_register_command("reboot", "Reboot the system", cmd_reboot);
}

/*
 * Help command - Display list of available commands
 */
void cmd_help(int argc, char** argv) {
    (void)argc;  /* Unused parameter */
    (void)argv;  /* Unused parameter */
    
    console_write("\nAvailable Commands:\n");
    console_write("===================\n\n");
    
    int count;
    const shell_command_t* commands = shell_get_commands(&count);
    
    for (int i = 0; i < count; i++) {
        console_write("  ");
        console_write(commands[i].name);
        
        /* Add padding */
        size_t name_len = string_length(commands[i].name);
        for (size_t j = name_len; j < 12; j++) {
            console_write(" ");
        }
        
        console_write("- ");
        console_write(commands[i].description);
        console_write("\n");
    }
    
    console_write("\n");
}

/*
 * Clear command - Clear the console screen
 */
void cmd_clear(int argc, char** argv) {
    (void)argc;  /* Unused parameter */
    (void)argv;  /* Unused parameter */
    
    console_clear();
}

/*
 * Echo command - Print text to console
 */
void cmd_echo(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (i > 1) {
            console_write(" ");
        }
        console_write(argv[i]);
    }
    console_write("\n");
}

/*
 * Uname command - Display OS name and version
 */
void cmd_uname(int argc, char** argv) {
    (void)argc;  /* Unused parameter */
    (void)argv;  /* Unused parameter */
    
    console_write("OpenOS version 0.1.0\n");
    console_write("Architecture: i386 (32-bit x86)\n");
    console_write("Kernel: Educational Operating System\n");
}

/*
 * Helper function to print a decimal number
 */
static void print_number(uint32_t value) {
    if (value == 0) {
        console_put_char('0');
        return;
    }
    
    char buffer[11];  /* Max 10 digits + null terminator */
    int i = 10;
    buffer[i] = '\0';
    
    while (value > 0 && i > 0) {
        i--;
        buffer[i] = '0' + (value % 10);
        value /= 10;
    }
    
    console_write(&buffer[i]);
}

/*
 * Uptime command - Show system uptime
 */
void cmd_uptime(int argc, char** argv) {
    (void)argc;  /* Unused parameter */
    (void)argv;  /* Unused parameter */
    
    uint64_t uptime_ms = timer_get_uptime_ms();
    
    /*
     * Use 32-bit arithmetic to avoid 64-bit division library dependency.
     * NOTE: This will wrap after ~49.7 days (2^32 milliseconds).
     * This is acceptable for an educational OS but should be addressed
     * if the system needs to track longer uptimes.
     */
    uint32_t uptime_ms_low = (uint32_t)uptime_ms;
    
    /* Calculate time components */
    uint32_t total_seconds = uptime_ms_low / 1000;
    uint32_t milliseconds = uptime_ms_low % 1000;
    uint32_t seconds = total_seconds % 60;
    uint32_t minutes = (total_seconds / 60) % 60;
    uint32_t hours = (total_seconds / 3600) % 24;
    uint32_t days = total_seconds / 86400;
    
    console_write("System uptime: ");
    
    if (days > 0) {
        print_number(days);
        console_write(" day");
        if (days > 1) console_write("s");
        console_write(", ");
    }
    
    if (hours > 0 || days > 0) {
        print_number(hours);
        console_write(" hour");
        if (hours != 1) console_write("s");
        console_write(", ");
    }
    
    if (minutes > 0 || hours > 0 || days > 0) {
        print_number(minutes);
        console_write(" minute");
        if (minutes != 1) console_write("s");
        console_write(", ");
    }
    
    print_number(seconds);
    console_write(".");
    
    /* Print milliseconds with leading zeros */
    if (milliseconds < 100) console_put_char('0');
    if (milliseconds < 10) console_put_char('0');
    print_number(milliseconds);
    
    console_write(" seconds\n");
}

/*
 * PWD command - Print current working directory
 */
void cmd_pwd(int argc, char** argv) {
    (void)argc;  /* Unused parameter */
    (void)argv;  /* Unused parameter */
    
    vfs_node_t* current = kernel_get_current_directory();
    if (!current) {
        console_write("/\n");
        return;
    }
    
    /* Build path by walking up to root */
    char path[256];
    char temp[256];
    path[0] = '\0';
    
    vfs_node_t* node = current;
    while (node && node->parent != node) {  /* Stop at root (root is its own parent) */
        /* Prepend current node name */
        string_copy(temp, "/");
        string_concat(temp, node->name);
        string_concat(temp, path);
        string_copy(path, temp);
        
        node = node->parent;
    }
    
    /* If path is empty, we're at root */
    if (path[0] == '\0') {
        console_write("/\n");
    } else {
        console_write(path);
        console_write("\n");
    }
}

/*
 * LS command - List directory contents
 */
void cmd_ls(int argc, char** argv) {
    vfs_node_t* dir;
    
    if (argc < 2) {
        /* List current directory */
        dir = kernel_get_current_directory();
    } else {
        /* List specified directory */
        if (argv[1][0] == '/') {
            /* Absolute path */
            dir = vfs_resolve_path(argv[1]);
        } else {
            /* Relative path - build absolute path */
            char abs_path[256];
            vfs_node_t* current = kernel_get_current_directory();
            
            /* Build current path first */
            char current_path[256];
            current_path[0] = '\0';
            
            if (current && current->parent != current) {
                vfs_node_t* node = current;
                char temp[256];
                while (node && node->parent != node) {
                    string_copy(temp, "/");
                    string_concat(temp, node->name);
                    string_concat(temp, current_path);
                    string_copy(current_path, temp);
                    node = node->parent;
                }
            }
            
            if (current_path[0] == '\0') {
                string_copy(abs_path, "/");
            } else {
                string_copy(abs_path, current_path);
            }
            
            string_concat(abs_path, "/");
            string_concat(abs_path, argv[1]);
            
            dir = vfs_resolve_path(abs_path);
        }
        
        if (!dir) {
            console_write("ls: cannot access '");
            console_write(argv[1]);
            console_write("': No such file or directory\n");
            return;
        }
    }
    
    if (!dir) {
        console_write("ls: error accessing directory\n");
        return;
    }
    
    if (dir->type != NODE_DIRECTORY) {
        console_write("ls: '");
        console_write(argv[1]);
        console_write("': Not a directory\n");
        return;
    }
    
    /* List directory contents */
    for (uint32_t i = 0; i < dir->child_count; i++) {
        vfs_node_t* child = dir->children[i];
        if (child) {
            console_write(child->name);
            if (child->type == NODE_DIRECTORY) {
                console_write("/");
            }
            console_write(" ");
        }
    }
    console_write("\n");
}

/*
 * CD command - Change directory
 */
void cmd_cd(int argc, char** argv) {
    if (argc < 2) {
        console_write("Usage: cd <directory>\n");
        return;
    }
    
    vfs_node_t* target;
    
    /* Handle special cases */
    if (string_compare(argv[1], "/") == 0) {
        /* Go to root */
        target = vfs_get_root();
    } else if (string_compare(argv[1], ".") == 0) {
        /* Stay in current directory */
        return;
    } else if (string_compare(argv[1], "..") == 0) {
        /* Go to parent directory */
        vfs_node_t* current = kernel_get_current_directory();
        if (current && current->parent) {
            target = current->parent;
        } else {
            target = vfs_get_root();
        }
    } else if (argv[1][0] == '/') {
        /* Absolute path */
        target = vfs_resolve_path(argv[1]);
    } else {
        /* Relative path - build absolute path */
        char abs_path[256];
        vfs_node_t* current = kernel_get_current_directory();
        
        /* Build current path first */
        char current_path[256];
        current_path[0] = '\0';
        
        if (current && current->parent != current) {
            vfs_node_t* node = current;
            char temp[256];
            while (node && node->parent != node) {
                string_copy(temp, "/");
                string_concat(temp, node->name);
                string_concat(temp, current_path);
                string_copy(current_path, temp);
                node = node->parent;
            }
        }
        
        if (current_path[0] == '\0') {
            string_copy(abs_path, "/");
        } else {
            string_copy(abs_path, current_path);
        }
        
        string_concat(abs_path, "/");
        string_concat(abs_path, argv[1]);
        
        target = vfs_resolve_path(abs_path);
    }
    
    if (!target) {
        console_write("cd: ");
        console_write(argv[1]);
        console_write(": No such file or directory\n");
        return;
    }
    
    if (target->type != NODE_DIRECTORY) {
        console_write("cd: ");
        console_write(argv[1]);
        console_write(": Not a directory\n");
        return;
    }
    
    kernel_set_current_directory(target);
}

/*
 * CAT command - Display file contents
 */
void cmd_cat(int argc, char** argv) {
    if (argc < 2) {
        console_write("Usage: cat <filename>\n");
        return;
    }
    
    vfs_node_t* file;
    
    if (argv[1][0] == '/') {
        /* Absolute path */
        file = vfs_resolve_path(argv[1]);
    } else {
        /* Relative path - build absolute path */
        char abs_path[256];
        vfs_node_t* current = kernel_get_current_directory();
        
        /* Build current path first */
        char current_path[256];
        current_path[0] = '\0';
        
        if (current && current->parent != current) {
            vfs_node_t* node = current;
            char temp[256];
            while (node && node->parent != node) {
                string_copy(temp, "/");
                string_concat(temp, node->name);
                string_concat(temp, current_path);
                string_copy(current_path, temp);
                node = node->parent;
            }
        }
        
        if (current_path[0] == '\0') {
            string_copy(abs_path, "/");
        } else {
            string_copy(abs_path, current_path);
        }
        
        string_concat(abs_path, "/");
        string_concat(abs_path, argv[1]);
        
        file = vfs_resolve_path(abs_path);
    }
    
    if (!file) {
        console_write("cat: ");
        console_write(argv[1]);
        console_write(": No such file or directory\n");
        return;
    }
    
    if (file->type != NODE_FILE) {
        console_write("cat: ");
        console_write(argv[1]);
        console_write(": Is a directory\n");
        return;
    }
    
    /* Read and display file contents */
    uint8_t buffer[VFS_MAX_FILE_SIZE];
    ssize_t bytes_read = vfs_read(file, 0, file->length, buffer);
    
    if (bytes_read < 0) {
        console_write("cat: error reading file\n");
        return;
    }
    
    /* Display contents */
    for (ssize_t i = 0; i < bytes_read; i++) {
        console_put_char((char)buffer[i]);
    }
}

/*
 * Reboot command - Reboot the system
 */
void cmd_reboot(int argc, char** argv) {
    (void)argc;  /* Unused parameter */
    (void)argv;  /* Unused parameter */
    
    console_write("Rebooting system...\n");
    
    /*
     * Give time for message to display.
     * NOTE: Busy-wait loop timing is CPU-speed dependent.
     * On typical systems this provides ~100-500ms delay.
     */
    for (volatile int i = 0; i < 10000000; i++) {
        /* Busy wait */
    }
    
    /* Method 1: Use keyboard controller to reset */
    uint8_t good = 0x02;
    while (good & 0x02) {
        good = inb(0x64);
    }
    outb(0x64, 0xFE);
    
    /* Method 2: Triple fault (fallback) */
    /* Create an invalid IDT descriptor */
    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) invalid_idt = {0, 0};
    
    __asm__ __volatile__("cli");  /* Disable interrupts */
    __asm__ __volatile__("lidt %0" : : "m"(invalid_idt));  /* Load invalid IDT */
    __asm__ __volatile__("int $0x03");  /* Trigger interrupt */
    
    /* If we get here, the reboot failed */
    console_write("Reboot failed!\n");
}
