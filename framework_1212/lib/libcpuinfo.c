/* --COPYRIGHT--,BSD
 * Copyright (c) 2009, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libcpuinfo.h>

#define MODULE_NAME     "Cpu"
#define TRUE             0x01
#define FALSE            0x00
#define Dmai_EIO         -3
#define Dmai_EFAIL       -1
#define Dmai_err0        printf
#define Dmai_EOK         0

static char *deviceName[Cpu_Device_COUNT] = {
    "DM355",
    "DM365",
    "DM6446",
    "DM6467",
    "DM6437",
    "OMAP3530"
};

const Cpu_Attrs Cpu_Attrs_DEFAULT = {
    0
};

/******************************************************************************
 * getType
 ******************************************************************************/
static int libgetdevice(Cpu_Device *device)
{
    int  deviceTypeFound = FALSE;
    char varBuf[20];
    char valBuf[100];
    FILE *fptr;

    fptr = fopen("/proc/cpuinfo", "r");

    if (fptr == NULL) {
        Dmai_err0("/proc/cpuinfo not found. Is /proc filesystem mounted?\n");
        return Dmai_EIO;
    }

    while (fscanf(fptr, "%8s : %[^\n]", varBuf, valBuf) != EOF) {
        if (strcmp(varBuf, "Hardware") == 0) {
            deviceTypeFound = TRUE;
            break;
        }
    }

    if (fclose(fptr) != 0) {
        return Dmai_EIO;
    }

    if (!deviceTypeFound) {
        return Dmai_EFAIL;
    }

    if (strcmp(valBuf, "DaVinci EVM") == 0) {
        *device = Cpu_Device_DM6446;
    }
    else if (strcmp(valBuf, "DaVinci DM355 EVM") == 0) {
        *device = Cpu_Device_DM355;
    }
    else if (strcmp(valBuf, "DaVinci DM6467 EVM") == 0) {
        *device = Cpu_Device_DM6467;
    }
    else if (strcmp(valBuf, "DM357 EVM") == 0) { 
        *device = Cpu_Device_DM6446;
    }
    else if (strcmp(valBuf, "OMAP3EVM Board") == 0) {
        *device = Cpu_Device_OMAP3530;
    }
    else if (strcmp(valBuf, "DaVinci DM365 EVM") == 0) {
        *device = Cpu_Device_DM365;
    }
    else {
        Dmai_err0("Unknown Cpu Type!\n");
        return Dmai_EFAIL;
    }

    return Dmai_EOK;
}

/******************************************************************************
 * libcpu_get_device
 ******************************************************************************/
static int libcpu_get_device(Cpu_Handle hCpu, Cpu_Device *device)
{
    if (hCpu) {
        *device = hCpu->device;
    }
    else {
        if (libgetdevice(device) < 0) {
            Dmai_err0("Failed to get device type\n");
            return Dmai_EFAIL;
        }
    }

    return Dmai_EOK;
}

/******************************************************************************
 * libcpu_get_device_name
 ******************************************************************************/
static char *libcpu_get_device_name(Cpu_Device device)
{
    return deviceName[device];
}

/******************************************************************************
 *libcpu_get_load
 ******************************************************************************/
static int libcpu_get_load(Cpu_Handle hCpu, int *cpuLoad)
{
    int                  cpuLoadFound = FALSE;
    unsigned long        user, nice, sys, idle, total, proc;
    unsigned long        uTime, sTime, cuTime, csTime;
    unsigned long        deltaTotal, deltaIdle, deltaProc;
    char                 textBuf[4];
    FILE                *fptr;

    /* Read the overall system information */
    fptr = fopen("/proc/stat", "r");

    if (fptr == NULL) {
        Dmai_err0("/proc/stat not found. Is the /proc filesystem mounted?\n");
        return Dmai_EIO;
    }

    /* Scan the file line by line */
    while (fscanf(fptr, "%4s %lu %lu %lu %lu %*[^\n]", textBuf,
                  &user, &nice, &sys, &idle) != EOF) {
        if (strcmp(textBuf, "cpu") == 0) {
            cpuLoadFound = TRUE;
            break;
        }
    }

    if (fclose(fptr) != 0) {
        return Dmai_EIO;
    }

    if (!cpuLoadFound) {
        return Dmai_EFAIL;
    }

    /* Read the current process information */
    fptr = fopen("/proc/self/stat", "r");

    if (fptr == NULL) {
        Dmai_err0("/proc/self/stat not found. Is /proc filesystem mounted?\n");
        return Dmai_EIO;
    }

    if (fscanf(fptr, "%*d %*s %*s %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu "
                     "%lu %lu %lu", &uTime, &sTime, &cuTime, &csTime) != 4) {
        Dmai_err0("Failed to get process load information.\n");
        fclose(fptr);
        return Dmai_EIO;
    }

    if (fclose(fptr) != 0) {
        return Dmai_EFAIL;
    }

    total = user + nice + sys + idle;
    proc = uTime + sTime + cuTime + csTime;

    /* Check if this is the first time, if so init the prev values */
    if (hCpu->prevIdle == 0 && hCpu->prevTotal == 0 && hCpu->prevProc == 0) {
        hCpu->prevIdle = idle;
        hCpu->prevTotal = total;
        hCpu->prevProc = proc;
        return Dmai_EOK;
    }

    deltaIdle = idle - hCpu->prevIdle;
    deltaTotal = total - hCpu->prevTotal;
    deltaProc = proc - hCpu->prevProc;

    hCpu->prevIdle = idle;
    hCpu->prevTotal = total;
    hCpu->prevProc = proc;

    if(deltaTotal) {
        *cpuLoad = deltaProc * 100 / deltaTotal;
    } else {
        *cpuLoad = 0;
    }

    return Dmai_EOK;
}

/******************************************************************************
 * libcpu_create
 ******************************************************************************/
static Cpu_Handle libcpu_create(Cpu_Attrs *attrs)
{
    Cpu_Handle hCpu;

    hCpu = calloc(1, sizeof(Cpu_Object));

    if (hCpu == NULL) {
        Dmai_err0("Failed to allocate space for Cpu Object\n");
        return NULL;
    }

    /* Determine type of device */
    if (libgetdevice(&hCpu->device) < 0) {
        Dmai_err0("Failed to get device type\n");
        free(hCpu);
        return NULL;
    }

    /* One initial run to initialize state */
    if (libcpu_get_load(hCpu, NULL) < 0) {
        Dmai_err0("Failed to initialize CPU object\n");
        free(hCpu);
        return NULL;
    }

    return hCpu;
}

/******************************************************************************
 * libcpu_delete
 ******************************************************************************/
static int libcpu_delete(Cpu_Handle hCpu)
{
    if (hCpu) {
        free(hCpu);
    }

    return Dmai_EOK;
}


Cpu_Handle CPU_CREATE(Cpu_Attrs *attrs)
{
	return libcpu_create(attrs);
}
int CPU_GET_LOAD(Cpu_Handle hCpu, int *cpuLoad)
{
	return libcpu_get_load(hCpu,cpuLoad);
}
int CPU_DELETE(Cpu_Handle hCpu)
{
	return libcpu_delete(hCpu);
}
int CPU_GET_DEVICE(Cpu_Handle hCpu, Cpu_Device *device)
{
	return libcpu_get_device(hCpu, device);
}
char *CPU_GET_DEVICE_NAME(Cpu_Device device)
{
	return libcpu_get_device_name(device);
}
/*****************************************Coding End***********************************/

