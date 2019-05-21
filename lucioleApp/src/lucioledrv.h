#ifndef LUCIOLEDRV_H
#define LUCIOLEDRV_H

#define light_ch1_longOutValueString		"LIGHT_CH1_LONGOUT_VAL"
#define light_ch2_longOutValueString            "LIGHT_CH2_LONGOUT_VAL"
#define deviceState_stringInValueString         "DEVICESTATE_STRINGIN_VAL"
#define triggerMode_ch1_binaryInValueString     "TRIGGERMODE_CH1_BINARYIN_VAL"
#define triggerMode_ch2_binaryInValueString     "TRIGGERMODE_CH2_BINARYIN_VAL"
#define setTrigger_ch1_binaryInValueString      "SETTRIGGER_CH1_BINARYOUT_VAL"
#define setTrigger_ch2_binaryInValueString      "SETTRIGGER_CH2_BINARYOUT_VAL"
#define connectionState_binaryInValueString     "CONNECTIONSTATE_BINARYIN_VAL"
#define error_mbbiValueString                   "ERROR_MBBI_VAL"

#define NUM_PARAMS		9
#define MAX_SIGNALS		1


class Lucioledrv: public asynPortDriver{
    public:
        Lucioledrv(const char *portName, char *ip);
        ~Lucioledrv();

        //AsynPortDriver methods extended
        virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
        
        //Luciole's library related
        void luciole_open(char *strPortDesc);
        void luciole_close();
        void luciole_releaseNetworkMode(BYTE Chx);
        void luciole_IsConnected();
        void luciole_getLightValue(BYTE Chx);
        //void luciole_getState(); //currently unused
        void luciole_setTrigger(BYTE Chx, int Flag);
        int luciole_getTrigger(BYTE Chx);
        void luciole_setEthernetInterface(char *strIpAddr);
        void luciole_setLightValue(BYTE Chx, WORD LightVal);
            
    protected:
        int light_ch1_longOutValue;
        int light_ch2_longOutValue;
        int deviceState_stringInValue;
        int triggerMode_ch1_binaryInValue;
        int triggerMode_ch2_binaryInValue;
        int setTrigger_ch1_binaryInValue;
        int setTrigger_ch2_binaryInValue;
        int connectionState_binaryInValue;
        int error_mbbiValue;
        
    private:
        h_TinyDiaFrame hdiaframe; //handle for communication
};

#endif /* LUCIOLEDRV_H */

