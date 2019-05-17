/*____________________________________________________________________________________________________________________________________
|	Driver support for Arinax Luciole Cold Light Source developed with AsynPortDriver module from asyn/SynApps                    |
|	Brazilian Synchroton Light National Laboratory - Campinas, 04/  /2019                                                         |
|	Author: Allan S. B. Bugyi	(allan.bugyi@lnls.br)									      |
|	Version: 1.0														      |
|	Tested                                                                                                                        |
|                                                                                                                                     |
|       License:                                                                                                                      |
|        This software is distributed under the following ISC license:                                                                |
|                                                                                                                                     |
|        Copyright © 2018 BRAZILIAN SYNCHROTRON LIGHT SOURCE <sol@lnls.br>                                                            |
|                                                                                                                                     |
|        Permission to use, copy, modify, and/or distribute this software for any                                                     |
|        purpose with or without fee is hereby granted, provided that the above                                                       |
|        copyright notice and this permission notice appear in all copies.                                                            |
|                                                                                                                                     |
|        THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES                                                     |
|        WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF                                                             |
|        MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR                                                     |
|        ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES                                                       |
|        WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION                                                 |
|        OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN                                                       |
|        CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.                                                                     |
|_____________________________________________________________________________________________________________________________________|*/


#include <cstdlib>
#include <iostream>
#include <string.h>
#include <array>

//EPICS's includes
#include <iocsh.h>
#include <epicsExport.h>
#include <registryFunction.h>

//AsynPortDriver's includes
#include <asynPortDriver.h>

//Luciole's includes
#include "Lucioldll.h"
#include "lucioledrv.h"

using namespace std;
static const char *driverName = "Lucioledriver";

Lucioledrv::Lucioledrv(const char *portName, char *ip)
        : asynPortDriver(portName, MAX_SIGNALS, NUM_PARAMS,
                            asynInt32Mask | asynFloat64Mask | asynOctetMask | asynDrvUserMask, // Interfaces that we implement
                            asynInt32Mask | asynFloat64Mask | asynOctetMask, // Interfaces that do callbacks
                            ASYN_MULTIDEVICE | ASYN_CANBLOCK, 1, /* ASYN_CANBLOCK=1, ASYN_MULTIDEVICE=1, autoConnect=1 */
                            0, 0) /* Default priority and stack size */
{
    createParam(light_ch1_longOutValueString, asynParamInt32, &light_ch1_longOutValue);
    createParam(light_ch2_longOutValueString, asynParamInt32, &light_ch2_longOutValue);
    createParam(lightState_ch1_longOutValueString, asynParamOctet, &lightState_ch1_longOutValue);
    createParam(lightState_ch2_longOutValueString, asynParamOctet, &lightState_ch2_longOutValue);
    createParam(triggerMode_binaryOutValueString, asynParamInt32, &triggerMode_binaryOutValue);
    createParam(connectionState_binaryInValueString, asynParamInt32, &connectionState_binaryInValue);
    createParam(channelIndx_binaryOutValueString, asynParamInt32, &channelIndx_binaryOutValue);
    
    luciole_open(ip);
}

void Lucioledrv::luciole_open(char *strPortDesc){
    char aux[30];
    strcpy(aux, "TCP:");
    strcat(aux, strPortDesc);
    strcat(aux, ":4528");
    
    int error = cls_OpenCom(aux);
    char aux2[30];
    strcpy(aux2, strPortDesc);
    if(error>0)luciole_setEthernetInterface(aux2);
}

void Lucioledrv::luciole_close(){
    return;
}

void Lucioledrv::luciole_IsConnected(){
    return;
}

int Lucioledrv::luciole_getLightValue(BYTE Chx){
    int lightValue = cls_GetLightValue(Chx);
    if(lightValue>0){
        setIntegerParam(light_ch1_longOutValue, lightValue);
        callParamCallbacks();
    }
    //cls. lightValue
    return 0;
}

void Lucioledrv::luciole_getState(char **pstrError){
    return;
}

int Lucioledrv::luciole_getTrigger(BYTE Chx){
    return 0;    
}

int Lucioledrv::luciole_setEthernetInterface(char *strIpAddr){
    int error = cls_SetEthernetInterface(strIpAddr);
    return 0;    
}

int Lucioledrv::luciole_setLightValue(BYTE Chx, WORD LightVal){
    int error = cls_SetLightValue(Chx, LightVal);
    cout<<error;
    return 0;
}

int Lucioledrv::luciole_setTrigger(BYTE Chx, int flag){
    return 0;
}

//------------ asynPortDriver extended methods ------------
asynStatus Lucioledrv::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int function = pasynUser->reason;
        const char *paramName;
        getParamName(function, &paramName);
    int addr=0;
    asynStatus status = asynSuccess;
    const char* functionName = "writeInt32";

    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

    if(function==light_ch1_longOutValue){
        char lightValue = (char) value;
        luciole_setLightValue(1, lightValue);
    }
    else if(function==light_ch2_longOutValue){
        char lightValue = (char) value;
        luciole_setLightValue(2, lightValue);
    }
    
    /* Set the parameter in the parameter library. */
    status = (asynStatus) setIntegerParam(addr, function, value);

    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks(addr, addr);
    
    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, name=%s, value=%d", 
                  driverName, functionName, status, function, paramName, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, name=%s, value=%d\n", 
              driverName, functionName, function, paramName, value);
    return status;
}
//--------------------------------------------------------

extern "C" int LucioleCreateDriver(const char *portName, char *ip){
    Lucioledrv *lucioledrv = new Lucioledrv(portName, ip);
    lucioledrv = NULL;
    return(asynSuccess);
}

static const iocshArg portNameArg = { "Port name", iocshArgString};
static const iocshArg ipArg = { "IP", iocshArgString};
static const iocshArg * const createDriverArgs[] = {&portNameArg, &ipArg};
static const iocshFuncDef createDriverFuncDef = {"LucioleCreateDriver", 2, createDriverArgs};

static void createDriverCallFunc(const iocshArgBuf *args){
    LucioleCreateDriver(args[0].sval, args[1].sval);
}

void drvLucioleRegister(void){
    iocshRegister(&createDriverFuncDef,createDriverCallFunc);
}

extern "C" {
    epicsExportRegistrar(drvLucioleRegister);
}