/* --COPYRIGHT--,BSD
 * Copyright (c) 2010, Texas Instruments Incorporated
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

#include <xdc/std.h>

#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/VideoStd.h>
#include <ti/sdo/dmai/Display.h>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>

#include "priv/_SysFs.h"

#define MODULE_NAME     "Dmai"

/******************************************************************************
 * writeSysFs
 ******************************************************************************/
static int writeSysFs(char *fileName, char *val)
{
    FILE *fp;
    char *valString;

    valString = malloc(strlen(val) + 1);

    if (valString == NULL) {
        SYS_ERROR("Failed to allocate memory for temporary string\n");
        return -1;
    }

    fp = fopen(fileName, "w");

    if (fp == NULL) {
        SYS_ERROR("Failed to open %s for writing\n", fileName);
        free(valString);
        return -1;
    }

    if (fwrite(val, strlen(val) + 1, 1, fp) != 1) {
        SYS_ERROR("Failed to write sysfs variable %s to %s\n",
                  fileName, val);
        fclose(fp);
        free(valString);
        return -1;
    }

    fclose(fp);

    free(valString);

    return 0;
}

/******************************************************************************
 * readSysFs
 ******************************************************************************/
static int readSysFs(char *fileName, char *val, int length)
{
    FILE *fp;
    int ret;
    int len;
    char *tok;

    fp = fopen(fileName, "r");

    if (fp == NULL) {
        SYS_ERROR("Failed to open %s for reading\n", fileName);
        return -1;
    }

    memset(val, '\0', length);

    ret = fread(val, 1, length, fp);

    if (ret < 1) {
        SYS_ERROR("Failed to read sysfs variable from %s\n", fileName);
        return -1;
    }

    tok = strtok(val, "\n");
    len = tok ? strlen(tok) : strlen(val);
    val[len] = '\0';

    fclose(fp);

    return 0;
}

/******************************************************************************
 * getFbResolution
 *****************************************************************************/
static int getFbResolution(char *str, unsigned int *w, unsigned int *h)
{
    int ret;
    char fileName[100], val[100], *ptr;

    sprintf(fileName, "/sys/devices/platform/omapdss/%s/timings", str);

    ret = readSysFs(fileName, val, 100);
    if (ret < 0) {
        SYS_ERROR("Failed to read from %s\n", fileName);
        return -1;
    }

    ptr = val;
    while (*ptr != ',')
        ptr++;
    *w = strtoul(ptr+1, &ptr, 0);

    while (*ptr != ',')
        ptr++;
    *h = strtoul(ptr+1, &ptr, 0);

    return 0;
}

/******************************************************************************
 * configure_fb
 *****************************************************************************/
static int configure_fb (unsigned int width, unsigned int height) 
{
   	int fd;
    struct fb_var_screeninfo varInfo;

    fd = open("/dev/fb0", O_RDWR);

    if (fd == -1) {
	    SYS_ERROR("Failed to open /dev/fb0");
	    return -1;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &varInfo) == -1) {
        SYS_ERROR("Failed FBIOGET_VSCREENINFO on /dev/fb0");
        close(fd);
	    return -1;
    }

    if (varInfo.rotate == 1 || varInfo.rotate == 3) {
        varInfo.xres = height;
        varInfo.yres = width;
    } else {
        varInfo.xres = width;
        varInfo.yres = height;
    }

    varInfo.xres_virtual    = width;
    varInfo.yres_virtual    = height ;

    SYS_ERROR("Configuring /dev/fb0, width=%d, height=%d\n", varInfo.xres,
        varInfo.yres);

    if (ioctl(fd, FBIOPUT_VSCREENINFO, &varInfo) < 0) {
        SYS_ERROR("Failed FBIOPUT_VSCREENINFO on /dev/fb0");
        return -1;
    }

    close(fd);

    return 0;        
}

/*******************************************************************
 * config_dss_sysfs
 ******************************************************************/
static inline int config_dss_sysfs(const char *file, char *val)
{
    char f_name[100];

    sprintf(f_name, "/sys/devices/platform/omapdss/%s", file);
    return writeSysFs(f_name, val);
}

/********************************************************************
 * _SysFs_change
 ********************************************************************/
int _SysFs_change(Display_Output *displayOutput, char* displayDevice,
                  VideoStd_Type *videoType, int *rotation)
{
    char str[6];
    unsigned int width, height;

#if 0
    if (*displayOutput == Display_Output_SYSTEM) {
        readSysFs("/sys/devices/platform/omapdss/manager0/display", str, 5);
        if (strcmp(str, "lcd") == 0) {
            *displayOutput = Display_Output_LCD;
        }
        else if (strcmp(str, "dvi") == 0) {
            *displayOutput = Display_Output_DVI;
        }
        else {
            SYS_ERROR("unknown display output\n");
        }
        return 0;
    }
#endif
   // if (*displayOutput == Display_Output_LCD) {
        SYS_ERROR("Setting display to LCD\n");

        /* Disable overlays and displays and break the link */
        config_dss_sysfs("overlay0/enabled", "0");
        config_dss_sysfs("overlay1/enabled", "0");
        config_dss_sysfs("overlay2/enabled", "0");
        config_dss_sysfs("overlay0/manager", "\n");
        config_dss_sysfs("overlay1/manager", "\n");
        config_dss_sysfs("overlay2/manager", "\n");
        config_dss_sysfs("display0/enabled", "0");
        config_dss_sysfs("display1/enabled", "0");
        config_dss_sysfs("display2/enabled", "0");

        getFbResolution("display0", &width, &height);
        configure_fb(width, height);

        /* Set the links and enable overlays: Currentl only GFX is enabled */
        config_dss_sysfs("overlay0/manager", "lcd");
        config_dss_sysfs("overlay1/manager", "lcd");
        config_dss_sysfs("overlay2/manager", "lcd");
        config_dss_sysfs("manager0/display", "lcd");
        config_dss_sysfs("display0/enabled", "1");
        config_dss_sysfs("overlay0/enabled", "1");

        configure_fb(width, height);
   // }

#if 0
    if (*displayOutput == Display_Output_DVI) {
	    SYS_ERROR("Setting display to DVI\n");

        /* Disable overlays and displays and break the link */
        config_dss_sysfs("overlay0/enabled", "0");
        config_dss_sysfs("overlay1/enabled", "0");
        config_dss_sysfs("overlay2/enabled", "0");
        config_dss_sysfs("overlay0/manager", "\n");
        config_dss_sysfs("overlay1/manager", "\n");
        config_dss_sysfs("overlay2/manager", "\n");
        config_dss_sysfs("display0/enabled", "0");
        config_dss_sysfs("display1/enabled", "0");
        config_dss_sysfs("display2/enabled", "0");

        if (*videoType == VideoStd_480P) {
            config_dss_sysfs("display2/timings", "27903,720/96/24/96,480/32/10/3");
        }

        if (*videoType == VideoStd_720P_60) {
            config_dss_sysfs("display2/timings", "72434,1280/190/120/32,720/13/3/5");
        }

        getFbResolution("display2", &width, &height);
        configure_fb(width, height);

        config_dss_sysfs("manager0/display", "dvi");
        config_dss_sysfs("overlay0/manager", "lcd");
        config_dss_sysfs("overlay1/manager", "lcd");
        config_dss_sysfs("overlay2/manager", "lcd");
        config_dss_sysfs("overlay0/enabled", "1");
        config_dss_sysfs("display2/enabled", "1");

        configure_fb(width, height);
	}

    if (*displayOutput == Display_Output_SVIDEO || *displayOutput == Display_Output_COMPOSITE) {
		SYS_ERROR("Setting display to COMPOSITE/SVIDEO\n");

        /* Disable overlays and displays and break the link */
        config_dss_sysfs("overlay0/enabled", "0");
        config_dss_sysfs("overlay1/enabled", "0");
        config_dss_sysfs("overlay2/enabled", "0");
        config_dss_sysfs("overlay0/manager", "\n");
        config_dss_sysfs("overlay1/manager", "\n");
        config_dss_sysfs("overlay2/manager", "\n");
        config_dss_sysfs("display0/enabled", "0");
        config_dss_sysfs("display1/enabled", "0");
        config_dss_sysfs("display2/enabled", "0");

        getFbResolution("display1", &width, &height);
        configure_fb(width, height);

        config_dss_sysfs("overlay0/manager", "tv");
        config_dss_sysfs("overlay1/manager", "tv");
        config_dss_sysfs("overlay2/manager", "tv");
        config_dss_sysfs("overlay0/enabled", "1");
        config_dss_sysfs("display1/enabled", "1");

        configure_fb(width, height);
    }

#endif
    return 0;
}

