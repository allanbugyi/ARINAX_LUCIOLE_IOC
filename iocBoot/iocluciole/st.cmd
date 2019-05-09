#!../../bin/linux-x86_64/luciole

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase("dbd/luciole.dbd")
luciole_registerRecordDeviceDriver pdbbase

LucioleCreateDriver("tcp_ip", "0.0.0.0")

dbLoadRecords()

## Run this to trace the stages of iocInit
#traceIocInit

cd "${TOP}/iocBoot/${IOC}"
iocInit
