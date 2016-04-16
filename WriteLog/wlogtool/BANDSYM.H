#ifndef BANDSYM_H
#define BANDSYM_H

#define CW_MASK (1 << 3)
#define USB_MASK (1 << 2)
#define LSB_MASK  2
#define AM_MASK   (1 << 5)
#define PHONE_MASK (LSB_MASK | USB_MASK)
#define FSK_MASK (1 << 6)
#define FM_MASK  (1 << 4)
#define ALL_MODES_MASK (CW_MASK|USB_MASK|LSB_MASK|AM_MASK|FSK_MASK|FM_MASK)
#define BAND_DISABLED   0X4000  /* Set this bit to turn OFF this band*/

struct band_stru
{
    long    low;
    long    high;
	 int     mask;
	 const char *Title;
};

typedef struct band_stru FAR * BandPtr_t;
typedef const struct band_stru FAR * ConstBandPtr_t;

#endif
