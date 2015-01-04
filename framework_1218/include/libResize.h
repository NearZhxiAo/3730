#ifndef ti_sdo_dmai_Resize_h_
#define ti_sdo_dmai_Resize_h_

#include "system_includes.h"
#include "system_debug.h"
#include <linux/omap_resizer.h>
#include <linux/videodev.h>
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

/**
 * @brief       Handle through which to reference a Resize job.
 */

typedef struct Resize_Object
{
	int fd;
	Resize_WindowType hWindowType;
	Resize_WindowType vWindowType;
	Resize_FilterType hFilterType;
	Resize_FilterType vFilterType;
	int inSize;
	int outSize;
}Resize_Object;


typedef struct Resize_Object *Resize_Handle;

/**
 * @brief       Attributes used to create a Resize job.
 * @see         Resize_Attrs_DEFAULT.
 */
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
} Resize_Attrs;

/**
 * @brief       Default attributes for a Resize job.
 * @code
 * hWindowType      = Resize_WindowType_BLACKMAN,
 * vWindowType      = Resize_WindowType_BLACKMAN,
 * hFilterType      = Resize_FilterType_LOWPASS,
 * vFilterType      = Resize_FilterType_LOWPASS,
 * rszRate          = 0xe
 * @endcode
 */

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @brief       Creates a Resize job.
 *
 * @param[in]   attrs       #Resize_Attrs to use for creating the Resize job.
 *
 * @retval      Handle for use in subsequent operations (see #Resize_Handle).
 * @retval      NULL for failure.
 */
extern Resize_Handle Resize_create(Resize_Attrs *attrs);

/**
 * @brief       Configure a Resize job.
 *
 * @param[in]   hResize     The #Resize_Handle of the job to configure.
 * @param[in]   hSrcBuf     The source buffer to configure for. Note that only
 *                          the buffer properties are accessed, the pointers
 *                          are not used.
 * @param[in]   hDstBuf     The destination buffer to configure for. Note that
 *                          only the buffer properties are accessed, the
 *                          pointers are not used.
 *
 * @retval      Dmai_EOK for success.
 * @retval      "Negative value" for failure, see Dmai.h.
 *
 * @remarks     #Resize_create must be called before this function.
 */
extern int Resize_config(Resize_Handle hResize,
                         Buffer_Handle hSrcBuf, Buffer_Handle hDstBuf);

/**
 * @brief       Execute a Resize job.
 *
 * @param[in]   hResize     The #Resize_Handle of the job to execute.
 * @param[in]   hSrcBuf     The #Buffer_Handle of the source buffer to
 *                          copy from.
 * @param[in]   hDstBuf     The #Buffer_Handle of the destination buffer
 *                          to copy to.
 *
 * @retval      Dmai_EOK for success.
 * @retval      "Negative value" for failure, see Dmai.h.
 *
 * @remarks     #Resize_create must be called before this function.
 * @remarks     #Resize_config must be called before this function.
 */
extern int Resize_execute(Resize_Handle hResize,
                          unsigned long hSrcBuf, unsigned long hDstBuf);

/**
 * @brief       Deletes a Resize job.
 *
 * @param[in]   hResize     The #Resize_Handle of the job to delete.
 *
 * @retval      Dmai_EOK for success.
 * @retval      "Negative value" for failure, see Dmai.h.
 *
 * @remarks     #Resize_create must be called before this function.
 */
extern int Resize_delete(Resize_Handle hResize);

#if defined (__cplusplus)
}
#endif

/*@}*/

#endif /* ti_sdo_dmai_Resize_h_ */
