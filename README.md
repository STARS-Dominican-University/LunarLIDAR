# LunarLIDAR
|![sphere-inner](https://github.com/STARS-Dominican-University/LunarLIDAR/assets/70508631/360bafb3-9f83-45ac-8c4d-96d9d9fa406c)|![sphere](https://github.com/STARS-Dominican-University/LunarLIDAR/assets/70508631/5f7e52f7-a878-4508-8db9-0e96b83b7972)|
|:--:|:--:|
|Inner Point Cloud|Outer Point Cloud|


## Table of Contents

1. [Introduction](#introduction)
2. [File Hierarchy](#file-hierarchy)
3. [Arduino](#arduino)
4. [Python](#python)
5. [Running Model](#running-model)
6. [Finding USB Port Number](#finding-usb-port-number)

### Introduction
LunarLidar is an `arduino-python` based project that uses LIDAR sensors to create a point cloud in a given lunar enviornment. This project is funded by the _National Aeronautics and Space Administration_ subdivision _NASA MINDS_. 

### File Hierarchy
- **Data**
  - *data.csv*
- **Source**
  - **Arduino**
    - *receiver*
      - *receiver-ino*
    - *transmitter*
      - *transmitter-ino*
  - **Notebook**
    - *cloud-mapping*
  - **Python**
    - *computers*
    - *main*
    - *math-model*
- index
- README


### Arduino

### Python

### Running Model

## Finding USB Port Number

To find your USB port number, run the following command in your terminal:

```bash
ls /dev/cu.*
```

You will see an output similar to this:

```bash
/dev/cu.BLTH			
/dev/cu.Bluetooth-Incoming-Port	
/dev/cu.usbmodem1422101
```

Use the final one, `/dev/cu.usbmodem1422101`, as your USB port number.
_the usb number will not be the same_