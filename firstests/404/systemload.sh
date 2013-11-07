#!/bin/bash
#probing systemload 31.10.13

#f = open('/proc/stat/','r')    
for l in open("/proc/stat/"):
    l = l.split()
    if l[0] == 'procs_running':
       result = int(l[1])

print result
