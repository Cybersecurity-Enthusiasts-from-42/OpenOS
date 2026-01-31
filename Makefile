# OpenOS Root Makefile
# Builds the kernel in Kernel2.0 directory

.PHONY: all clean run help

all:
	@echo "Building OpenOS kernel..."
	$(MAKE) -C Kernel2.0

clean:
	@echo "Cleaning build artifacts..."
	$(MAKE) -C Kernel2.0 clean

run: all
	@echo "Running OpenOS in QEMU..."
	@chmod +x tools/run-qemu.sh
	@./tools/run-qemu.sh

help:
	@echo "OpenOS Build System"
	@echo "-------------------"
	@echo "make all   - Build the kernel"
	@echo "make clean - Remove build artifacts"
	@echo "make run   - Build and run in QEMU"
	@echo "make help  - Show this help message"