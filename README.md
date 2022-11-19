# Introduction

## Installation
### Arch
```
pacman -S avr-binutils avr-gcc avr-libc avr-gdb avrdude openocd doxygen make picocom arm-none-eabi-binutils arm-none-eabi-gcc arm-none-eabi-gdb
paru -S avarice-svn 
```

### Ubuntu
```
sudo apt install gcc-avr avr-libc avrdude libhidapi-dev libftdi-dev libftdi1-dev make picocom
cd /tmp
git clone https://git.code.sf.net/p/openocd/code openocd
cd openocd
./bootstrap
mkdir build
cd build
../configure --enable-cmsis-dap --enable-openjtag --prefix=/opt/openocd
cd ..
make
sudo make install
```
> you might need to add `/opt/openocd` to path and re-login
> Also ubuntu packages might be not correct. I've just guessed the package names

### Compilation
compile and flash
```
make flash
```

### Documentation
```
make docs
```

### Static Snalysis
```
make check
```
> Not properly set up on node 2 static analysis will try to analyze vendor code.

### Serial connection to nodes
for node 1:
```
  picocom -b 9600 --parity e --stopbits 1 --databits 8 --imap lfcrlf /dev/<DEVICE>
```
`<DEVICE>` is usually `ttySX` where `X` is a number. Just try each number from 0 and onwards. Uart in it self does not have any handshake protocol so you'll have to open each port and see if there are any data there.

for node 2:
```
  picocom -b 9600 /dev/<DEVICE> --imap lfcrlf
```
run `sudo dmesg --follow` and reconnect device that hosts the serial connection. The output will tell what device the serial connection is on.
Example output:
```
[23132.518120] usb 1-3.4: new full-speed USB device number 99 using xhci_hcd
[23132.629661] usb 1-3.4: New USB device found, idVendor=2341, idProduct=003d, bcdDevice= 0.01
[23132.629678] usb 1-3.4: New USB device strings: Mfr=1, Product=2, SerialNumber=220
[23132.629686] usb 1-3.4: Product: Arduino Due Prog. Port
[23132.629691] usb 1-3.4: Manufacturer: Arduino (www.arduino.cc)
[23132.629696] usb 1-3.4: SerialNumber: 957303339373517141E1
[23132.638326] cdc_acm 1-3.4:1.0: ttyACM0: USB ACM device
```
Here `ttyACM0` is the device name. On node 2 usb port closest to the barrel jack has the serial data. That cable needs to be attached to the computer.

## VS Code
Workspace has been set up. Open `.vscode/nodes.code-workspace` and click on Open Workspace. Should probably set up devcontainer as well so other OS can easily get started.

### Debugging
After opening the directory inside workspace just press `Ctrl+Shift+D` and select node to debug. Make sure you compile without optimization otherwise debugging will be impossible. However when compiling without optimizing for space then the binary might get to big and the code will not run at all. That means you'll most likely not be able to do e2e testing and have to only rely on manual unit tests.

# Other
## maybe useful bash snippets
print heaviest functions
```
avr-nm --print-size --size-sort --radix=x build/output.elf | tail
```

## Useful links
- [C gibberish translator](https://cdecl.org/)
- [Cheat sheet collection](https://devhints.io/)
- [Memfault blog](https://interrupt.memfault.com/blog/boostrapping-libc-with-newlib)
- [Newlib documentation](https://sourceware.org/newlib/libc.html#Syscalls)
- [AVR libc documentation](https://www.nongnu.org/avr-libc/user-manual/index.html)
