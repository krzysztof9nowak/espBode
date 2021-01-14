#include "esp_config.h"
#include <string.h>

#if AWG == FY6900
#warning Compiling for FY6900

#include "esp_fy6900.h"

volatile SDeviceState gDeviceState;

bool fy6900_write(char* data, uint8_t len)
{
    uint32_t timeout = 0;
    Serial.write((uint8_t*)data, len);
    telnet.println("[");
    telnet.print(data);
    telnet.println("]");
    while(!Serial.available())
    {
        delay(1);
        if(timeout++>1000) return false;
    }
    bool ok = false;
    ok = (Serial.read() == 0x0a); // 0x0a == \n

    if(!ok){
      telnet.println("Invalid response for command");      
    }
    return ok;
}

void setCh1Wave(EWaveType wave)
{
    char command[] = "WMW00\n";
    snprintf(command, 7, "WMW%02u\n", wave);
    gDeviceState.ch1Wave = wave;
    fy6900_write(command, 6);
}

void setCh2Wave(EWaveType wave)
{
    char command[] = "WFW00\n";
    snprintf(command, 7, "WFW%02u\n", wave);
    gDeviceState.ch2Wave = wave;
    fy6900_write(command, 6);
}

void setCh1Output(uint32_t output)
{
  gDeviceState.ch1Output = output;
  fy6900_write((char*)(output ? "WMN1\n" : "WMN0\n"), 5);
}

void setCh2Output(uint32_t output)
{
  gDeviceState.ch2Output = output;
  fy6900_write((char*)(output ? "WFN1\n" : "WFN0\n"), 5);
}

/* Set frequency in Hz */
void setCh1Freq(uint32_t frequency)
{
    char command[] = "WMF00000000000000\n";
    snprintf(command, 19, "WMF%08lu000000\n", frequency);
    gDeviceState.ch1Freq = frequency;
    fy6900_write(command, 18);
}

/* Set frequency in Hz */
void setCh2Freq(uint32_t frequency)
{
    char command[] = "WFF00000000000000\n";
    snprintf(command, 19, "WFF%08lu000000\n", frequency);
    gDeviceState.ch2Freq = frequency;
    fy6900_write(command, 18);
}

/* Ampl is in mV: 12.345V = 12345 */
void setCh1Ampl(uint32_t ampl)
{
    char command[] = "WMA00.000\n";
    snprintf(command, 11, "WMA%02u.%03u\n", ampl/1000, ampl%1000);
    gDeviceState.ch1Ampl = ampl;
    fy6900_write(command, 10);
}

void setCh2Ampl(uint32_t ampl)
{
    char command[] = "WFA00.000\n";
    snprintf(command, 11, "WFA%02u.%03u\n", ampl/1000, ampl%1000);
    gDeviceState.ch2Ampl = ampl;
    fy6900_write(command, 10);
}

/* Phase is in 0.1deg: 12.5deg = 125 */
void setCh1Phase(uint32_t phase)
{
    char command[] = "WMP000.000\n";
    snprintf(command, 12, "WMP%03u.%03u\n", phase/1000, (phase%1000)/100);
    gDeviceState.ch1Phase = phase;
    fy6900_write(command, 11);
}

void setCh2Phase(uint32_t phase)
{
    char command[] = "WFP000.000\n";
    snprintf(command, 12, "WFP%03u.%03u\n", phase/1000, (phase%1000)/100);
    gDeviceState.ch2Phase = phase;
    fy6900_write(command, 11);
}

void setCh1Offset(int32_t offset)
{
    char command[] = "WMO00.00\n";
    gDeviceState.ch1Offset = offset;
    if(offset>=0)
    {
        snprintf(command, 10, "WMO%02u.%02u\n", offset/1000, offset%1000);
        fy6900_write(command, 9);
    }
    else
    {
        snprintf(command, 11, "WMO-%02u.%02u\n", -offset/1000, -offset%1000);
        fy6900_write(command, 10);
    }
}

void setCh2Offset(int32_t offset)
{
    char command[] = "WFO00.00\n";
    gDeviceState.ch2Offset = offset;
    if(offset>=0)
    {
        snprintf(command, 10, "WFO%02u.%02u\n", offset/1000, offset%1000);
        fy6900_write(command, 9);
    }
    else
    {
       snprintf(command, 11, "WFO-%02u.%02u\n", -offset/1000, -offset%1000);
       fy6900_write(command, 10);
   }
}

void initDevice(void)
{
    Serial.write((uint8_t*)"\n", 1);

    setCh1Output(0);
    setCh1Wave(EWaveType_Sine);
    setCh1Freq(1000);
    setCh1Ampl(1000);
    setCh1Offset(0);

    setCh2Output(0);
    setCh2Wave(EWaveType_Sine);
    setCh2Freq(1000);
    setCh2Ampl(1000);
    setCh2Offset(0);
}

#endif
