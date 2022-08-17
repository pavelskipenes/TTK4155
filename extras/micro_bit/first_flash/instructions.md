To program the micro::bit with nrfjprog we have to upload the BBC micro::bit J-Link OB Software to it. This is a hex-file that can be downloaded from [https://www.segger.com/downloads/jlink#BBC_microbit](https://www.segger.com/downloads/jlink#BBC_microbit). This is a one time thing, and does not have to be repeated unless using the micro::bit device abstraction layer (DAL) in between. 
To upload the file to the `micro::bit`, do the following:
- Hold the reset button while connecting it to the computer.
- The micro::bit should now appear as "MAINTENANCE" in the file system. Release the reset button
- Copy the hex-file to the "MAINTENANCE" folder
- The micro::bit will reeboot and appear as "MICROBIT"
- If the micro::bit still appears as "MAINTENANCE", unplug the micro::bit and plug it in again
- Run the command nrfjprog -f nrf51 -e from the terminal. 
- The led matrix should de-illuminate.
