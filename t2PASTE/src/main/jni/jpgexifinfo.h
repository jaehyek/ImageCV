#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct _Tiff_
{
    unsigned short order;
    unsigned char version[2];
    long offsetIFD0;
};

typedef struct _Exif_
{
    long offset;
    struct _Tiff_ tiff;
} Exif;

#define TAG_ORIENTATION (0x0112)
#define TYPE_SHORT (3)

int getRotationFromJpg(const char *filename);

int isBigEndian(Exif *exif);

short toShort(Exif *exif, unsigned char *);

long toLong(Exif *exif, unsigned char *);

int getOrientation(FILE *fp, int *pori);

int analyzeExif(Exif *exif, FILE *fp, int *pori);
