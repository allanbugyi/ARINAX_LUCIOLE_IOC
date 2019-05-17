#ifndef LUCIOLEDRV_H
#define LUCIOLEDRV_H

#define light_ch1_longOutValueString		"LIGHT_CH1_LONGOUT_VAL"
#define light_ch2_longOutValueString            "LIGHT_CH2_LONGOUT_VAL"
#define lightState_ch1_longOutValueString	"LIGHTSTATE_CH1_STRINGIN_VAL"
#define lightState_ch2_longOutValueString	"LIGHTSTATE_CH2_STRINGIN_VAL"
#define triggerMode_binaryOutValueString	"TRIGGERMODE_BINARYOUT_VAL"
#define connectionState_binaryInValueString     "CONNECTIONSTATE_BINARYIN_VAL"
#define channelIndx_binaryOutValueString        "CHANNELINDX_BINARYOUT_VAL"

#define NUM_LONG_OUT		4
#define NUM_BINARY_OUT		1
#define NUM_BINARY_IN		2

#define NUM_PARAMS		7
#define MAX_SIGNALS		1


class Lucioledrv: public asynPortDriver{
    public:
        Lucioledrv(const char *portName, char *ip);

        //AsynPortDriver methods extended
        virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
        //virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual);

        //Luciole's library related
        void luciole_open(char *strPortDesc);
        void luciole_close();
        void luciole_IsConnected();
        int luciole_getLightValue(BYTE Chx);
        void luciole_getState(char **pstrError);
        int luciole_getTrigger(BYTE Chx);
        int luciole_setEthernetInterface(char *strIpAddr);
        int luciole_setLightValue(BYTE Chx, WORD LightVal);
        int luciole_setTrigger(BYTE Chx, int flag);
            
    protected:
        int light_ch1_longOutValue;
        int light_ch2_longOutValue;
        int lightState_ch1_longOutValue;
        int lightState_ch2_longOutValue;
        int triggerMode_binaryOutValue;
        int connectionState_binaryInValue;
        int channelIndx_binaryOutValue;
};

#endif /* LUCIOLEDRV_H */

