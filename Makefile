# Clean cfetch binary
clean:
	rm -rf cfetch


# Install cfetch
# This will copy cfetch to the /usr/bin directory
# Every distro has its own installation command
arch:
	$(info Installing...)
	gcc cfetch-arch.c -o cfetch && sudo cp cfetch /usr/bin

debian:
	$(info Installing...)
	gcc cfetch-debian.c -o cfetch && sudo cp cfetch /usr/bin

elementary:
	$(info Installing...)
	gcc cfetch-elementary.c -o cfetch && sudo cp cfetch /usr/bin

linuxlite:
	$(info Installing...)
	gcc cfetch-linuxlite.c -o cfetch && sudo cp cfetch /usr/bin

linuxmint:
	$(info Installing...)
	gcc cfetch-linuxmint.c -o cfetch && sudo cp cfetch /usr/bin

popos:
	$(info Installing...)
	gcc cfetch-popos.c -o cfetch && sudo cp cfetch /usr/bin

ubuntu:
	$(info Installing...)
	gcc cfetch-ubuntu.c -o cfetch && sudo cp cfetch /usr/bin


# Uninstall cfetch
# This will delete cfetch from the /usr/bin/ directory.
uninstall:
	$(info Uninstalling...)
	sudo rm -rf /usr/bin/cfetch
