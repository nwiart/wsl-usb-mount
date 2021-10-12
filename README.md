# wsl-usb-mount
Simple utility for mounting / unmounting USB storage devices in a WSL environment.

## How do I compile this project?
1. Install `g++` and `make` on your WSL distribution using its package manager.
2. Clone this project using `git clone git@github.com:nwiart/wsl-usb-mount.git`.
3. Navigate to the newly created directory and launch make using `make`.
4. There should now be an executable file named `usbmount`.
5. Have fun! Type `usbmount --help` for info, and don't forget to execute it with sudo when mounting or unmounting!

## How do I use it?
- Display help :
	`./usbmount [--help]`
- Mounting a drive :
	`sudo ./usbmount <windows_drive_letter>`
	This creates a new directory at `/mnt/<windows_drive_letter>` and mounts the USB device on it.
- Unmounting a drive :
	`sudo ./usbmount -u <windows_drive_letter>`
	This unmounts the USB device, but does not destroy the mounted directory.
