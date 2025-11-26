# OpenOS

OpenOS is an educational, open-source operating system built from scratch for the x86 architecture.

The goal is to build a small, understandable OS **from zero**, inspired by hobby OS projects like MyraOS, xv6, and OSDev examples — but implemented with **our own code**, fully documented, and open for community contribution.

## 🎯 Mission
To create a collaborative OS development environment where students, beginners, and low-level enthusiasts can learn:

- How CPUs boot an OS  
- What a kernel actually does  
- How memory, interrupts, and drivers work  
- How processes and syscalls operate  
- How filesystems and user programs work  

All with clean, simple, modern C + Assembly code.

## 🚀 Features (planned)
- Multiboot-compatible 32-bit kernel  
- VGA text output  
- Interrupt handling (IDT, ISRs, IRQs)  
- Physical & virtual memory management (paging)  
- Kernel heap allocator  
- Basic drivers (keyboard, PIT, screen)  
- Process abstraction + simple scheduler  
- Syscalls + userland programs  
- (Later) Toy filesystem + maybe a GUI  

## 🛠️ Build & Run

### Requirements
- gcc (32-bit)
- nasm
- make
- qemu-system-i386
- grub-mkrescue

### Build
```
make
```

### Run
```
./tools/run-qemu.sh
```

## 🤝 Contributing
OpenOS is open for contributions at all levels:
- Code  
- Documentation  
- Bug fixes  
- New features  
- Roadmap proposals  

Everything is welcome.

## 📄 License
MIT License — free to use, modify, and contribute.
