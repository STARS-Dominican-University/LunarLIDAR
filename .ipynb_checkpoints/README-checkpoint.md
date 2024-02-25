# LunarLIDAR

## Finding USB Port number
To find your USB port number, run the following command:
``bash
ls /dev/cu.*
```

in your terminal. You will find something like this spit out :

``bash
/dev/cu.BLTH			
/dev/cu.Bluetooth-Incoming-Port	
/dev/cu.usbmodem1422101
```

use the final one.
