# Clean cfetch binary
clean:
	@echo "Cleaning..."
	rm -rf cfetch
	@echo "Finished!";


# Build cfetch
compile:
	@echo "Compiling cfetch..."
	gcc cfetch.c -o cfetch
	@echo "Compilation finished!"


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
