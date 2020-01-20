#!/bin/bash

module="scull"

/sbin/rmmod $module && echo "unload success"

rm -f /dev/${module}[0-3]
