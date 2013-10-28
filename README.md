the start of a linux CLI for the evic electronic cigarette


# to compile
```
./autogen.sh
./configure
make
```





# to run
plug in your evic - and look in your dmesg for the SG number and run
```
./src/freevic /dev/sg2 # where SG2 is your device handle
```



# Sample Output
```
Current Evic Settings:
First Name: Your-FirstName
Last Name: Your-LastName
Battery Life: 99%
Evic Mode: 0 - Volt Mode (value: 3.60)
Atomizer Resistance 2.00 ohm
```

feel free to contribute - use at your own risk!
license: GPLv3
