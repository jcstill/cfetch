# Build cfetch
# Every distro has its own installation command
Start:
	$(info Building...)
	gcc cfetch.c -o cfetch

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

# Clean cfetch binary
clean:
	rm -rf cfetch