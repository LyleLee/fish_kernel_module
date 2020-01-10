#!/bin/bash
module="scull"
device="scull"
mode="644"

/sbin/insmod ./${module}.ko $* || exit 1 ## $* 代表所有命令行的参数

cat /proc/devices | grep "$device"

