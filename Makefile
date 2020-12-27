# Clean cfetch binary
clean:
	rm -rf cfetch


# Build cfetch
# Every distro has its own installation command
arch:
	$(info Building...)
	gcc cfetch-arch.c -o cfetch

debian:
	$(info Building...)
	gcc cfetch-debian.c -o cfetch

elementary:
	$(info Building...)
	gcc cfetch-elementary.c -o cfetch

kisslinux:
	$(info Building...)
	gcc cfetch-kisslinux.c -o cfetch

linuxlite:
	$(info Building...)
	gcc cfetch-linuxlite.c -o cfetch

linuxmint:
	$(info Building...)
	gcc cfetch-linuxmint.c -o cfetch

popos:
	$(info Building...)
	gcc cfetch-popos.c -o cfetch

raspbian:
	$(info Building...)
	gcc cfetch-raspbian.c -o cfetch

ubuntu:
	$(info Building...)
	gcc cfetch-ubuntu.c -o cfetch


# Install cfetch
# This will copy cfetch to the /usr/bin directory
install:
	$(info Installing...)
	cp -v cfetch /usr/bin

# Uninstall cfetch
# This will delete cfetch from the /usr/bin/ directory.
uninstall:
	$(info Uninstalling...)
	sudo rm -rf /usr/bin/cfetch
