#!../../bin/linux-x86_64/luciole

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase("dbd/luciole.dbd")
luciole_registerRecordDeviceDriver pdbbase

LucioleCreateDriver("tcp_ip", "10.2.101.36")

dbLoadRecords("$(TOP)/db/luciole.db", "PORT=tcp_ip, ADDR=0, TIMEOUT=1, B=MANACA, E=LUCIOLE, Ch=1")
dbLoadRecords("$(TOP)/db/luciole.db", "PORT=tcp_ip, ADDR=0, TIMEOUT=1, B=MANACA, E=LUCIOLE, Ch=2")

## Run this to trace the stages of iocInit
#traceIocInit

cd "${TOP}/iocBoot/${IOC}"
iocInit
