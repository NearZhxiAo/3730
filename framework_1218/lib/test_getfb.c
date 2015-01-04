#include <stdio.h>

static int readSysFs(char *fileName, char *val, int length)
{
    FILE *fp;
    int ret;
    int len;
    char *tok;

    fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("Failed to open %s for reading\n", fileName);
        return -1;
    }

    memset(val, '\0', length);

    ret = fread(val, 1, length, fp);

    if (ret < 1) {
        printf("Failed to read sysfs variable from %s\n", fileName);
        return -1;
    }

    tok = strtok(val, "\n");
    len = tok ? strlen(tok) : strlen(val);
    val[len] = '\0';

    fclose(fp);

    return 0;
}
static int getFbResolution(char *str, unsigned int *w, unsigned int *h)
{
    int ret;
    char fileName[100], val[100], *ptr;

    sprintf(fileName, "/sys/devices/platform/omapdss/%s/timings", str);

    ret = readSysFs(fileName, val, 100);
    if (ret < 0) {
        printf("Failed to read from %s\n", fileName);
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

int main()
{
	int width, height;

	getFbResolution("display2", &width, &height);

	printf("width is %d, height is %d...\r\n", width, height);
	return 0;
}
