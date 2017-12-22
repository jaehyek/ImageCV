#include "jpgexifinfo.h"

const char *estr[] = {
        "(no error)", "not JPEG file", "no EXif header", "JPEG header corrupted"
};
const char *oristr[] = {
        "normal", "inverse", "rot 180", "rot 180 + inverse", "rot 90 + inverse", "rot 90",
        "rot 270 + inverse", "rot 270"
};

int getRotationFromJpg(const char *filename)
{
    FILE *fp;
    int orientation = 1; // default
    int ecode = 0;
/*
  if (argc < 2) {
    printf("usage: orientation <JPEG file>\n");
    return 0;
  }
  */
    fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("failed to open file\n");
        return 0;
    }

    ecode = getOrientation(fp, &orientation);
    if (ecode)
    {
        printf("%s\n", estr[ecode]);
    }
    else
    {
        if (orientation >= 1 && orientation <= 8)
        {
            return orientation;
            ////printf("Orientation: %s(%d)\n", oristr[orientation - 1], orientation);
        }
        else
        {
            printf("illegal orientation(%d)\n", orientation);
        }
    }
    fclose(fp);////
    return -1;
}

int getOrientation(FILE *fp, int *pori)
{
    unsigned char marker[8];

    // check JPEG header
    fread(marker, sizeof(char), 2, fp);
    if (marker[0] != 0xff || marker[1] != 0xd8)
    {
        return 1;
    }

    while (1)
    {
        if (fread(marker, sizeof(char), 2, fp) != 2)
        {
            return 1;
        }
        if (marker[0] == 0xff && marker[1] == 0xe1)
        {
            // APP1 header
            int len;
            if (fread(marker, sizeof(char), 2, fp) != 2)
            {
                return 1;
            }
            len = (marker[0] << 8) + marker[1] - 2 - 6;
            if (fread(marker, sizeof(char), 6, fp) != 6)
            {
                return 1;
            }
            if (memcmp(marker, "Exif\0\0", 6) != 0)
            {
                // not exif header
                fseek(fp, len, SEEK_CUR);
            }
            else
            {
                // exif header; go ahead
                Exif exif;
                memset(&exif, 0, sizeof(exif));
                exif.offset = ftell(fp);
                return analyzeExif(&exif, fp, pori);
            }
        }
        else if (marker[0] == 0xff && (marker[1] & 0xf0) == 0xe0)
        {
            // other header
            int oft;
            if (fread(marker, sizeof(char), 2, fp) != 2)
            {
                return 1;
            }
            oft = (marker[0] << 8) + marker[1] - 2;
            fseek(fp, oft, SEEK_CUR);
        }
        else
        {
            // No Exif header
            return 2;
        }
    }
}

int analyzeExif(Exif *exif, FILE *fp, int *pori)
{
    unsigned char marker[4];
    short ntag = 0;
    short i;

    // byte order byte
    if (fread(&exif->tiff.order, sizeof(short), 1, fp) != 1)
    {
        return 1;
    }
    // TIFF version; 0x2a
    if (fread(&exif->tiff.version, sizeof(char), 2, fp) != 2)
    {
        return 3;
    }
    if (toShort(exif, &exif->tiff.version[0]) != 0x2a)
    {
        return 3;
    }
    // offset to IFD0
    if (fread(marker, sizeof(char), 4, fp) != 4)
    {
        return 3;
    }
    exif->tiff.offsetIFD0 = toLong(exif, marker);
    fseek(fp, exif->offset + exif->tiff.offsetIFD0, SEEK_SET);

    // IFD0 tag quantity
    if (fread(marker, sizeof(char), 2, fp) != 2)
    {
        return 3;
    }
    ntag = toShort(exif, marker);
    for (i = 0; i < ntag; ++i)
    {
        short tag;
        if (fread(marker, sizeof(char), 2, fp) != 2)
        {
            return 3;
        }
        tag = toShort(exif, marker);
        if (tag != TAG_ORIENTATION)
        {
            fseek(fp, 10, SEEK_CUR);
        }
        else
        {
            // orientation tag を発見
            // TYPE は short
            if (fread(marker, sizeof(char), 2, fp) != 2)
            {
                return 3;
            }
            if (toShort(exif, marker) != TYPE_SHORT)
            {
                return 3;
            }

            // パラメータのカウント数は 1
            if (fread(marker, sizeof(char), 4, fp) != 4)
            {
                return 3;
            }
            if (toLong(exif, marker) != 1)
            {
                return 1;
            }
            // short value は詰めて格納される
            if (fread(marker, sizeof(char), 2, fp) != 2)
            {
                return 3;
            }
            *pori = toShort(exif, marker);
            return 0;
        }
    }

    // デフォルトの orientation
    *pori = 1;
    return 0;
}

int isBigEndian(Exif *exif)
{
    if (exif->tiff.order == 0x4d4d)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

short toShort(Exif *exif, unsigned char *b)
{
    if (!isBigEndian(exif))
    {
        return (short) (b[0] + (b[1] << 8));
    }
    else
    {
        return (short) ((b[0] << 8) + b[1]);
    }
}

long toLong(Exif *exif, unsigned char *b)
{
    if (!isBigEndian(exif))
    {
        return (long) (b[0] + (b[1] << 8) + (b[2] << 16) + (b[3] << 24));
    }
    else
    {
        return (long) ((b[0] << 24) + (b[1] << 16) + (b[2] << 8) + b[3]);
    }
}
