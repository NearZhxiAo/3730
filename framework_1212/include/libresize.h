#ifndef	_LIB_RESIZE_H_ 
#define	_LIB_RESIZE_H_

#include "system_debug.h"
#include "system_includes.h"
#include <linux/omap_resizer.h>
#include <linux/videodev.h>
#define RDRV_RESZ_SPEC__MAX_FILTER_COEFF 32

#ifdef __cplusplus
extern "C"{
#endif

#define RESIZER_DEVICE  "/dev/omap-resizer"



typedef enum {
    Resize_WindowType_HANN = 0,
    Resize_WindowType_BLACKMAN,
    Resize_WindowType_TRIANGULAR,
    Resize_WindowType_RECTANGULAR
} Resize_WindowType;

/**
 * @brief The filter types supported for coefficient calculation.
 * @remarks Only applicable on dm6446 linux.
 */
typedef enum {
    Resize_FilterType_BILINEAR = 0,
    Resize_FilterType_BICUBIC,
    Resize_FilterType_LOWPASS
} Resize_FilterType;


typedef struct Resize_Attrs {
    /**
     * @brief Horizontal window type.
     * @remarks Only applicable on dm6446 linux.
     */
    Resize_WindowType   hWindowType;

    /**
     * @brief Vertical window type.
     * @remarks Only applicable on dm6446 linux.
     */
    Resize_WindowType   vWindowType;

    /**
     * @brief Horizontal filter type.
     * @remarks Only applicable on dm6446 linux.
     */
    Resize_FilterType   hFilterType;

    /**
     * @brief Vertical filter type.
     * @remarks Only applicable on dm6446 linux.
     */
    Resize_FilterType   vFilterType;

    /** @brief The rate of the resizer H/W to use.
      * @remarks Only applicable on dm6446 Linux */
    int                 rszRate;

	int out_width;
	int out_height;
} ResizeAttr;



typedef struct __Resize_Object
{
	int resize_fd;
	Resize_WindowType   hWindowType;
	Resize_WindowType   vWindowType;
	Resize_FilterType   hFilterType;
	Resize_FilterType   vFilterType;
	int               inSize;
	int               outSize;

}Resize_Object;

typedef Resize_Object* Resize_Handle;

Resize_Handle RESIZE_INIT(ResizeAttr *attr);
int RESIZE_EXEC(Resize_Handle phandle, unsigned long physInbuf, unsigned long physOutbuf);
int RESIZE_DESTROY(Resize_Handle phandle);

#ifdef __cplusplus
}
#endif
#endif  
