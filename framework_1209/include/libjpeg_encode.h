
#ifndef ti_sdo_dmai_ce_Ienc1_h_
#define ti_sdo_dmai_ce_Ienc1_h_

#if defined (__cplusplus)
extern "C" {
#endif


void Jpeg_init(char *inbuffer, char *outbuffer);
void Jpeg_encode(int *length);
void Jpeg_destroy();

#if defined (__cplusplus)
}
#endif

/*@}*/

#endif /* ti_sdo_dmai_ce_Ienc1_h_ */
