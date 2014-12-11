#ifndef	_LIB_RESZ_H 
#define	_LIB_RESZ_H

#ifdef __cplusplus
extern "C"{
#endif

#define OSA_SOK   0
#define OSA_EFAIL -1


#define DRV_DATA_FORMAT_YUV420     2
#define DRV_DATA_FORMAT_YUV422     1
#define FALSE                      0
#define TRUE                       1

#define OSA_align(value, align)   ((( (value) + ( (align) - 1 ) ) / (align) ) * (align) )


#define ALG_VID_CODEC_H264         0
#define ALG_VID_ENC_MODE_STANDARD  0


typedef struct _Buffer
{
    unsigned char *virtAddr;
    unsigned char *physAddr;
}Buffer;

typedef struct _BufferPool
{
    int Num;
    Buffer *buffer;
}BufferPool;


typedef enum _SRC_MODE
{
    SRC_MOEDE_START,
    SRC_MODE_1,
    SRC_MODE_2,
    SRC_MODE_3,
    SRC_MODE_4,
    SRC_MODE_5,
    SRC_MODE_END
}SRC_MODE;







typedef struct {
          
    unsigned short outType;

    unsigned int flipH;
    unsigned int flipV;

    unsigned char  *outVirtAddr;
    unsigned char  *outPhysAddr;
    unsigned short outWidth;
    unsigned short outHeight;
    unsigned short outOffsetH;
    unsigned short outOffsetV;

} DRV_ReszOutPrm;







typedef struct {
          
    unsigned short inType;

    unsigned char  *inVirtAddr;
    unsigned char  *inPhysAddr;
    unsigned short inStartX;
    unsigned short inStartY;
    unsigned short inWidth;
    unsigned short inHeight;
    unsigned short inOffsetH; 
   unsigned short  inOffsetV; 

    DRV_ReszOutPrm *pOut;

    short   enableInvAlaw;     
    short   enableInvDpcm;

    unsigned short clkDivM;
    unsigned short clkDivN;

} DRV_ReszRunPrm;



BufferPool *RSZ_INIT(int num, int size);
succeed_type RSZ_EXIT(BufferPool *buffer_pool);

succeed_type RSZ_RSZRUN( unsigned int inWidth, unsigned int inHeight, unsigned int *outphyaddr,unsigned int outphynum );
succeed_type RSZ_DDRRUN(unsigned short Width, unsigned short Height, unsigned char*inPhysAddr, SRC_MODE src_mode);
int RSZ_GETFD();

#ifdef __cplusplus
}
#endif
#endif  
