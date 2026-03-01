# OpenOS - New Features Implementation

## Overview

This document describes the newly implemented advanced features for OpenOS, making it a more complete and modern operating system.

## Implemented Features

### 1. IPC Mechanisms (Inter-Process Communication)

**Location**: `include/ipc.h`, `kernel/ipc.c`

OpenOS now includes two essential IPC mechanisms:

#### Pipes
- Circular buffer-based implementation
- 4KB buffer size per pipe
- Support for reader/writer PIDs
- Non-blocking read/write operations

**API:**
```c
pipe_t* pipe_create(uint32_t reader_pid, uint32_t writer_pid);
int pipe_write(pipe_t* pipe, const void* data, size_t size);
int pipe_read(pipe_t* pipe, void* buffer, size_t size);
void pipe_close(pipe_t* pipe);
```

#### Message Queues
- Fixed-size message queue (16 messages)
- Maximum message size: 256 bytes
- FIFO message delivery
- Type-based message filtering support

**API:**
```c
msg_queue_t* msgqueue_create(uint32_t owner_pid);
int msgqueue_send(msg_queue_t* queue, uint32_t sender_pid, uint32_t type, const void* data, size_t size);
int msgqueue_receive(msg_queue_t* queue, message_t* msg);
void msgqueue_close(msg_queue_t* queue);
```

**Testing:**
```
OpenOS> test_ipc
```

### 2. Multi-core SMP Support (Symmetric Multi-Processing)

**Location**: `include/smp.h`, `kernel/smp.c`

Provides detection and management of multiple CPU cores:

**Features:**
- CPUID-based CPU detection
- Support for up to 16 CPUs
- Per-CPU data structures
- CPU state management (ONLINE, OFFLINE, HALTED)
- Application Processor (AP) boot support

**API:**
```c
void smp_init(void);
uint32_t smp_get_cpu_count(void);
uint32_t smp_get_current_cpu(void);
cpu_info_t* smp_get_cpu_info(uint32_t cpu_id);
int smp_boot_ap(uint32_t cpu_id);
```

**Testing:**
```
OpenOS> test_smp
```

### 3. GUI/Windowing System

**Location**: `include/gui.h`, `kernel/gui.c`

Basic graphical user interface framework with window management:

**Features:**
- 800x600x32 framebuffer support
- Window creation and management
- Drawing primitives (pixels, rectangles, lines, text)
- Window states (visible, hidden, minimized)
- Simple window rendering with title bars

**API:**
```c
void gui_init(void);
window_t* gui_create_window(int x, int y, int width, int height, const char* title);
void gui_show_window(window_t* window);
void gui_hide_window(window_t* window);
void gui_render_window(window_t* window);
void gui_draw_pixel(int x, int y, uint32_t color);
void gui_draw_rect(rect_t* rect, uint32_t color);
void gui_draw_filled_rect(rect_t* rect, uint32_t color);
```

**Testing:**
```
OpenOS> test_gui
```

### 4. Networking Stack

**Location**: `include/network.h`, `kernel/network.c`

Basic TCP/IP networking implementation:

**Features:**
- Ethernet frame handling
- IP header structure
- TCP and UDP protocol support
- Socket interface (create, bind, connect, send, receive)
- Network device abstraction
- Checksum calculation

**Protocols Supported:**
- ICMP (protocol 1)
- TCP (protocol 6)
- UDP (protocol 17)

**API:**
```c
void net_init(void);
socket_t* net_socket_create(uint8_t protocol);
int net_socket_bind(socket_t* socket, uint16_t port);
int net_socket_connect(socket_t* socket, ip_addr_t* ip, uint16_t port);
int net_socket_send(socket_t* socket, const void* data, size_t size);
int net_socket_recv(socket_t* socket, void* buffer, size_t size);
```

**Testing:**
```
OpenOS> test_net
```

### 5. Shell Scripting

**Location**: `include/script.h`, `kernel/script.c`

Enhanced shell with scripting capabilities:

**Features:**
- Variable storage and management (64 variables)
- Variable substitution
- Control flow structures (if, while, for)
- I/O redirection support
- Pipe creation between commands
- Script execution from strings

**API:**
```c
void script_init(void);
int script_execute(const char* script);
int script_set_var(const char* name, const char* value);
const char* script_get_var(const char* name);
int script_parse_if(const char* condition);
int script_create_pipe(const char* cmd1, const char* cmd2);
```

**Default Variables:**
- `PATH`: /bin:/usr/bin
- `HOME`: /home
- `PS1`: OpenOS>

**Testing:**
```
OpenOS> test_script
```

## Build Instructions

All features are integrated into the main build system:

```bash
make clean
make
make run
```

## Boot Sequence

The kernel now initializes all subsystems in the following order:

1. Console initialization
2. IDT (Interrupt Descriptor Table)
3. Exception handlers
4. PIC (Programmable Interrupt Controller)
5. Timer (100 Hz)
6. Keyboard
7. Filesystem (VFS)
8. IPC mechanisms
9. SMP support
10. GUI framework
11. Networking stack
12. Shell scripting

## Testing Commands

Five new test commands are available:

1. **test_ipc** - Tests pipes and message queues
2. **test_smp** - Shows CPU detection and status
3. **test_gui** - Demonstrates window creation and management
4. **test_net** - Tests network device and socket operations
5. **test_script** - Demonstrates variable and script execution

## Architecture

All new features follow the existing modular architecture:

```
OpenOS/
├── include/           # New feature headers
│   ├── ipc.h
│   ├── smp.h
│   ├── gui.h
│   ├── network.h
│   └── script.h
└── kernel/            # New feature implementations
    ├── ipc.c
    ├── smp.c
    ├── gui.c
    ├── network.c
    └── script.c
```

## Future Enhancements

While these features provide a solid foundation, several areas can be expanded:

### IPC
- Add semaphores and mutexes
- Implement shared memory
- Add signal support

### SMP
- Implement APIC (Advanced Programmable Interrupt Controller)
- Add per-CPU kernel stacks
- Implement CPU affinity for processes

### GUI
- Add actual bitmap font rendering
- Implement mouse support
- Add GUI widgets (buttons, text boxes, etc.)
- VBE/VESA mode detection and switching

### Networking
- Complete TCP state machine
- Add ARP protocol
- Implement DHCP client
- Add packet queuing

### Scripting
- Expand parser for complex expressions
- Add function definitions
- Implement background jobs
- Add command history

## License

MIT License - See LICENSE file for details.
