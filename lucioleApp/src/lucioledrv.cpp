/*____________________________________________________________________________________________________________________________________
|	Driver support for Arinax Luciole Cold Light Source developed with AsynPortDriver module from asyn/SynApps                    |
|	Brazilian Synchroton Light National Laboratory - Campinas, 05/21/2019                                                         |
|	Author: Allan S. B. Bugyi	(allan.bugyi@lnls.br)									      |
|	Version: 1.0														      |
|	Tested                                                                                                                        |
|                                                                                                                                     |
|       License:                                                                                                                      |
|        This software is distributed under the following ISC license:                                                                |
|                                                                                                                                     |
|        Copyright © 2019 BRAZILIAN SYNCHROTRON LIGHT SOURCE <sol@lnls.br>                                                            |
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
    createParam(triggerMode_ch1_binaryInValueString, asynParamInt32, &triggerMode_ch1_binaryInValue);
    createParam(triggerMode_ch2_binaryInValueString, asynParamInt32, &triggerMode_ch2_binaryInValue);
    createParam(setTrigger_ch1_binaryInValueString, asynParamInt32, &setTrigger_ch1_binaryInValue);
    createParam(setTrigger_ch2_binaryInValueString, asynParamInt32, &setTrigger_ch2_binaryInValue);
    createParam(connectionState_binaryInValueString, asynParamInt32, &connectionState_binaryInValue);
    createParam(error_mbbiValueString, asynParamInt32, &error_mbbiValue);
    
    luciole_open(ip);
}

Lucioledrv::~Lucioledrv(){
    //even thought implemented, the deconstrutor is not ever called because of the registered function methodology in EPICS
    luciole_releaseNetworkMode(1);
    luciole_releaseNetworkMode(2);
    luciole_close();
}

void Lucioledrv::luciole_open(char *strPortDesc){
    char aux[30];
    strcpy(aux, "TCP:");
    strcat(aux, strPortDesc);
    strcat(aux, ":4528");
    
    int returnedValue = cls_OpenCom(aux);
    char aux2[30];
    strcpy(aux2, strPortDesc);
    if(returnedValue>=0){
        hdiaframe = returnedValue;
        luciole_setEthernetInterface(aux2);
    }else{ //error
        cout<<"ERROR: Not able to effectively communicate with the device. Please, try to rerun the IOC.\n";
        setIntegerParam(error_mbbiValue, returnedValue);
        callParamCallbacks();
    }
}

void Lucioledrv::luciole_close(){
    cls_CloseCom(hdiaframe);
    cls_Close();
    setIntegerParam(connectionState_binaryInValue, 0);
    callParamCallbacks();
}

void Lucioledrv::luciole_releaseNetworkMode(BYTE Chx){
    int returnedValue = cls_ReleaseNetworkMode(Chx);
    if(returnedValue<0){
        setIntegerParam(error_mbbiValue, returnedValue);
        callParamCallbacks();        
    }
}

void Lucioledrv::luciole_IsConnected(){
    bool isConnected = cls_IsConnected();
    if(isConnected) setIntegerParam(connectionState_binaryInValue, 1);
    else            setIntegerParam(connectionState_binaryInValue, 0);
    callParamCallbacks();    
}

void Lucioledrv::luciole_getLightValue(BYTE Chx){
    int returnedValue = cls_GetLightValue(Chx);
    if(returnedValue>=0){
        setIntegerParam(light_ch1_longOutValue, returnedValue);
        callParamCallbacks();
    }else{ //error
        setIntegerParam(error_mbbiValue, returnedValue);
        callParamCallbacks();        
    }
}

/* currently unused
void Lucioledrv::luciole_getState(){
    char **pstrError;
    cls_GetState(pstrError);
    
}
*/

void Lucioledrv::luciole_setTrigger(BYTE Chx, int flag){
    int returnedValue = cls_SetTrigger(Chx, flag);
    if(returnedValue<0){ //error
        setIntegerParam(error_mbbiValue, returnedValue);
        callParamCallbacks();
    }
}

int Lucioledrv::luciole_getTrigger(BYTE Chx){
    int mode = cls_GetTrigger(Chx);
    return mode;
}

void Lucioledrv::luciole_setEthernetInterface(char *strIpAddr){
    int returnedValue;
    returnedValue = cls_SetEthernetInterface(strIpAddr);
    if(returnedValue<0){ //error
        setIntegerParam(error_mbbiValue, returnedValue);
        callParamCallbacks();
        return;
    }
    luciole_IsConnected();
}

void Lucioledrv::luciole_setLightValue(BYTE Chx, WORD LightVal){
    int returnedValue = cls_SetLightValue(Chx, LightVal);
    if(returnedValue<0){ //error     
        setIntegerParam(error_mbbiValue, returnedValue);
        callParamCallbacks();
    }
}

//------------ asynPortDriver extended method ------------
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
        short lightValue = (short) value;
        if(lightValue>0 && lightValue<20000){
            luciole_setLightValue(1, lightValue);
        }
        
    }
    else if(function==light_ch2_longOutValue){
        short lightValue = (short) value;
	if(lightValue>0 && lightValue<20000){
        	luciole_setLightValue(2, lightValue);
	}
    }
    else if(function==setTrigger_ch1_binaryInValue){
        luciole_setTrigger(1, 1);
    }
    else if(function==setTrigger_ch2_binaryInValue){
        luciole_setTrigger(2, 1);        
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
