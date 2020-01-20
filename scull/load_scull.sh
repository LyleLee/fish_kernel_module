#!/bin/bash
module="scull"
mode="644"

if grep -q '^staff:' /etc/group; then
    group="staff"
else
    group="wheel"
fi


/sbin/insmod ./${module}.ko $* || exit 1 ## $* 代表所有命令行的参数

cat /proc/devices | grep "$module"
major=$(awk '$2=="scull" {print $1}' /proc/devices)

rm -f /dev/${module}[0-3]

mknod /dev/${module}0 c $major 0
mknod /dev/${module}1 c $major 1
mknod /dev/${module}2 c $major 2
mknod /dev/${module}3 c $major 3

chgrp $group /dev/${module}[0-3]
chmod $mode /dev/${module}[0-3]

