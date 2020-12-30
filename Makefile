# Clean cfetch binary
clean:
	@echo "Cleaning..."
	rm -rf cfetch
	@echo "Finished!";


# Build cfetch
arch:
	@echo "Compiling cfetch..."
	gcc cfetch-arch.c -o cfetch
	@echo "Compilation finised!"

debian:
	@echo "Compiling cfetch..."
	gcc cfetch-debian.c -o cfetch
	@echo "Compilation finised!"

devuan:
	@echo "Compiling cfetch..."
	gcc cfetch-devuan.c -o cfetch
	@echo "Compilation finised!"

elementary:
	@echo "Compiling cfetch..."
	gcc cfetch-elementary.c -o cfetch
	@echo "Compilation finised!"

fedora:
	@echo "Compiling cfetch..."
	gcc cfetch-fedora.c -o cfetch
	@echo "Compilation finised!"

gentoo:
	@echo "Compiling cfetch..."
	gcc cfetch-gentoo.c -o cfetch
	@echo "Compilation finised!"

kisslinux:
	@echo "Compiling cfetch..."
	gcc cfetch-kisslinux.c -o cfetch
	@echo "Compilation finised!"

linuxlite:
	@echo "Compiling cfetch..."
	gcc cfetch-linuxlite.c -o cfetch
	@echo "Compilation finised!"

linuxmint:
	@echo "Compiling cfetch..."
	gcc cfetch-linuxmint.c -o cfetch
	@echo "Compilation finised!"

manjaro:
	@echo "Compiling cfetch..."
	gcc cfetch-manjaro.c -o cfetch
	@echo "Compilation finised!"

mxlinux:
	@echo "Compiling cfetch..."
	gcc cfetch-mxlinux.c -o cfetch
	@echo "Compilation finised!"

popos:
	@echo "Compiling cfetch..."
	gcc cfetch-popos.c -o cfetch
	@echo "Compilation finised!"

pureos:
	@echo "Compiling cfetch..."
	gcc cfetch-pureos.c -o cfetch
	@echo "Compilation finised!"

raspbian:
	@echo "Compiling cfetch..."
	gcc cfetch-raspbian.c -o cfetch
	@echo "Compilation finised!"

ubuntu:
	@echo "Compiling cfetch..."
	gcc cfetch-ubuntu.c -o cfetch
	@echo "Compilation finised!"


# Install cfetch
# This will copy cfetch to /usr/bin directory.
install:
	@echo "Installing cfetch..."
	sudo cp -v cfetch /usr/bin
	@echo "Installation finished!"


# Uninstall cfetch
# This will delete cfetch from the /usr/bin/ directory.
uninstall:
	@echo "Uninstalling cfetch..."
	sudo rm -rf /usr/bin/cfetch
	@echo "Uninstalling finished!"
