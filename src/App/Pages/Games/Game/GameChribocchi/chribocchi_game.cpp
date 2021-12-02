#include "common.h"

/*  Defines  */

#define BOXES   128
#ifdef DEBUG
#define SNOWS   16
#else
#define SNOWS   64
#endif

enum {
    STATE_START = 0,
    STATE_GAME,
    STATE_OVER,
    STATE_PAUSE,
};

enum {
    ALIGN_LEFT = 0,
    ALIGN_CENTER,
    ALIGN_RIGHT,
};

#define coord(x)        ((x) * 64)
#define coordInv(x)     ((x) / 64)
#define secs(s)         ((s) * 60)
#define mid(a, b, c)    max(min((b), (c)), (a))
#define sign(n)         (((n) > 0) - ((n) < 0))

/*  Typedefs  */

typedef struct {
    int16_t x, y;
    int8_t  vx, vy;
} BOX;

typedef struct {
    int8_t x, y;
} SNOW;

/*  Local Functions  */

static void startGame(void);
static void moveCat(void);
static void initBoxes(void);
static void throwFirstBox(void);
static void moveBoxes(void);
static void boundBox(BOX *pBox, int gap);
static int  getNewBoxIndex(void);
static void initSnows(void);
static void moveSnows(void);

static void drawCat(void);
static void drawSantaClaus(void);
static void drawBoxes(void);
static void drawSnows(void);
static void drawStrings(void);
static int  drawFigure(int16_t x, int16_t y, int value, uint8_t align);

/*  Local Variables  */

PROGMEM static const uint8_t imgBox[6][8] = { // 8x8 x6
    { 0x7E, 0x99, 0x99, 0xFF, 0xFF, 0x99, 0x99, 0x7E },
    { 0x7E, 0x8D, 0xC9, 0xF9, 0x9F, 0x93, 0xB1, 0x7E },
    { 0x7C, 0x47, 0xED, 0xB9, 0x9D, 0xB7, 0xE2, 0x3E },
    { 0x3C, 0x66, 0xE7, 0x99, 0x99, 0xE7, 0x66, 0x3C },
    { 0x3E, 0xE2, 0xB7, 0x9D, 0xB9, 0xED, 0x47, 0x7C },
    { 0x7E, 0xB1, 0x93, 0x9F, 0xF9, 0xC9, 0x8D, 0x7E },
};

PROGMEM static const uint8_t imgCat[8][48] = { // 24x12 x8
    {
        0x7C, 0xFE, 0xC7, 0xFE, 0xFC, 0xC6, 0xFF, 0xFE, 0xFC, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
        0xE0, 0xC0, 0xC0, 0x80, 0xC0, 0x62, 0x32, 0x1C, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x0F, 0x0F,
        0x03, 0x03, 0x0F, 0x0F, 0x03, 0x03, 0x03, 0x03, 0x0F, 0x0F, 0x03, 0x03, 0x0F, 0x0F, 0x00, 0x00
    },{
        0x7C, 0xFE, 0xC7, 0xFE, 0xFC, 0xC6, 0xFF, 0xFE, 0xFC, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
        0xE0, 0xC0, 0xC0, 0x80, 0xC0, 0xC0, 0x62, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x0F,
        0x0F, 0x03, 0x03, 0x0F, 0x0F, 0x03, 0x03, 0x07, 0x07, 0x03, 0x03, 0x07, 0x07, 0x03, 0x00, 0x00
    },{
        0x7C, 0xFE, 0xC7, 0xFE, 0xFC, 0xC6, 0xFF, 0xFE, 0xFC, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
        0xE0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x80, 0xF8, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x03,
        0x0F, 0x0F, 0x03, 0x0F, 0x0F, 0x03, 0x0F, 0x0F, 0x03, 0x03, 0x0F, 0x0F, 0x03, 0x01, 0x01, 0x00
    },{
        0x7C, 0xFE, 0xDF, 0xFE, 0xFC, 0xDE, 0xFF, 0xFE, 0xFC, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
        0xE0, 0xC0, 0xC0, 0x80, 0xC0, 0xC0, 0x62, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07,
        0x07, 0x03, 0x03, 0x07, 0x07, 0x03, 0x03, 0x0F, 0x0F, 0x03, 0x03, 0x0F, 0x0F, 0x03, 0x00, 0x00
    },{
        0x1C, 0x32, 0x62, 0xC0, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xFC,
        0xFE, 0xFF, 0xC6, 0xFC, 0xFE, 0xC7, 0xFE, 0x7C, 0x00, 0x00, 0x0F, 0x0F, 0x03, 0x03, 0x0F, 0x0F,
        0x03, 0x03, 0x03, 0x03, 0x0F, 0x0F, 0x03, 0x03, 0x0F, 0x0F, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00
    },{
        0x3C, 0x62, 0xC0, 0xC0, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xFC,
        0xFE, 0xFF, 0xC6, 0xFC, 0xFE, 0xC7, 0xFE, 0x7C, 0x00, 0x00, 0x03, 0x07, 0x07, 0x03, 0x03, 0x07,
        0x07, 0x03, 0x03, 0x0F, 0x0F, 0x03, 0x03, 0x0F, 0x0F, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00
    },{
        0xF8, 0x80, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xFC,
        0xFE, 0xFF, 0xC6, 0xFC, 0xFE, 0xC7, 0xFE, 0x7C, 0x00, 0x01, 0x01, 0x03, 0x0F, 0x0F, 0x03, 0x03,
        0x0F, 0x0F, 0x03, 0x0F, 0x0F, 0x03, 0x0F, 0x0F, 0x03, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00
    },{
        0x3C, 0x62, 0xC0, 0xC0, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xFC,
        0xFE, 0xFF, 0xDE, 0xFC, 0xFE, 0xDF, 0xFE, 0x7C, 0x00, 0x00, 0x03, 0x0F, 0x0F, 0x03, 0x03, 0x0F,
        0x0F, 0x03, 0x03, 0x07, 0x07, 0x03, 0x03, 0x07, 0x07, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00
    }
};

PROGMEM static const uint8_t imgSantaClaus[256] = { // 64x32
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xC0, 0x00,
    0xD0, 0x80, 0x70, 0x00, 0xC0, 0x90, 0x40, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC8, 0xF8, 0xFC, 0x3C, 0x1C, 0x1F, 0x07, 0x0D,
    0x1D, 0x73, 0xA9, 0x6B, 0x66, 0x92, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0x00, 0x40, 0xC0, 0x80, 0x60, 0xFE, 0x7D,
    0xFB, 0x75, 0xAB, 0x16, 0x18, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
    0xC0, 0xE0, 0xE0, 0xC0, 0xF8, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0,
    0xF0, 0xF0, 0xD8, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x08, 0x00, 0x04, 0x00, 0x02, 0x00, 0xE1, 0xF8, 0xFF, 0x00, 0x08, 0x58, 0xAD, 0x17, 0xEE, 0xF5,
    0xF6, 0xF5, 0xF7, 0xF8, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0x1F, 0x07, 0x03,
    0x00, 0x03, 0x0F, 0x0D, 0x0B, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03,
    0x03, 0x03, 0x03, 0x07, 0x0E, 0x1E, 0x34, 0x6C, 0xD8, 0xB0, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x78, 0x84, 0x83, 0x80, 0x83, 0x87, 0x8F, 0xEF, 0x9F, 0x8E, 0xCE, 0xBF, 0x8F, 0x8F,
    0x8F, 0x8F, 0x8F, 0x8F, 0xBF, 0xCF, 0x8F, 0x9F, 0xE7, 0x87, 0x87, 0x83, 0x91, 0x90, 0xE0, 0x00
};

PROGMEM static const uint8_t imgFigures[10][20] = { // 10x14 x10
    {
        0xC0, 0xF8, 0xFC, 0x1E, 0x06, 0x02, 0xFE, 0xFE, 0xF8, 0x00,
        0x07, 0x1F, 0x1F, 0x18, 0x18, 0x1E, 0x0F, 0x07, 0x00, 0x00
    },{
        0x00, 0x00, 0x10, 0x18, 0xE8, 0xFC, 0xFE, 0x1E, 0x02, 0x00,
        0x10, 0x10, 0x18, 0x1E, 0x1F, 0x1F, 0x11, 0x10, 0x00, 0x00
    },{
        0x00, 0x00, 0x08, 0x0C, 0x86, 0xC2, 0xFE, 0x7E, 0x1C, 0x00,
        0x10, 0x1C, 0x1E, 0x1B, 0x19, 0x18, 0x1C, 0x0C, 0x00, 0x00
    },{
        0x00, 0x00, 0x04, 0x46, 0x62, 0xFE, 0xDE, 0x8E, 0x00, 0x00,
        0x18, 0x18, 0x10, 0x18, 0x1C, 0x0F, 0x07, 0x03, 0x00, 0x00
    },{
        0x00, 0x80, 0xC0, 0xE0, 0x30, 0xFC, 0xFE, 0xFE, 0x0E, 0x00,
        0x02, 0x03, 0x03, 0x02, 0x1E, 0x1F, 0x1F, 0x03, 0x02, 0x00
    },{
        0x00, 0x00, 0x00, 0x38, 0x3E, 0xE6, 0xE6, 0xC6, 0x07, 0x03,
        0x18, 0x18, 0x10, 0x18, 0x1C, 0x0F, 0x07, 0x03, 0x00, 0x00
    },{
        0xC0, 0xE0, 0xF8, 0x78, 0x2C, 0xE6, 0xE6, 0xC2, 0x02, 0x00,
        0x07, 0x1F, 0x1F, 0x18, 0x18, 0x1F, 0x0F, 0x07, 0x00, 0x00
    },{
        0x00, 0x08, 0x0E, 0x8E, 0xC6, 0x76, 0x3E, 0x0E, 0x06, 0x02,
        0x00, 0x1C, 0x1E, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
    },{
        0x00, 0x00, 0x9C, 0xBC, 0xFE, 0xE2, 0xF2, 0x1E, 0x0C, 0x00,
        0x0E, 0x1F, 0x1F, 0x10, 0x18, 0x1F, 0x0F, 0x07, 0x00, 0x00
    },{
        0x00, 0x78, 0xFC, 0xFE, 0x86, 0x82, 0xFE, 0xFE, 0xF8, 0x00,
        0x10, 0x10, 0x18, 0x18, 0x0E, 0x07, 0x07, 0x01, 0x00, 0x00
    }
};

PROGMEM static const uint8_t imgDash[8] = { // 8x4
    0x0F, 0x0F, 0x03, 0x00, 0x0F, 0x0F, 0x03, 0x00
};

PROGMEM static const uint8_t imgReady[168] = { // 56x19 "Ready?"
    0xC0, 0xF0, 0x38, 0x18, 0xFC, 0xF4, 0xF4, 0x94, 0xCC, 0xFC, 0x7C, 0x38, 0x00, 0x00, 0x00, 0x80,
    0xC0, 0x40, 0xC0, 0xC0, 0x00, 0x00, 0x80, 0x80, 0xC0, 0x40, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x80,
    0x80, 0xC0, 0x40, 0xE0, 0xFC, 0xFE, 0x1F, 0x03, 0x83, 0xC3, 0xC0, 0x80, 0x00, 0xC0, 0xC0, 0xC0,
    0x00, 0x00, 0x0C, 0x8C, 0xC4, 0xFC, 0x78, 0x00, 0x03, 0x27, 0x34, 0x3E, 0x3F, 0x3F, 0x21, 0x3F,
    0x7F, 0xFE, 0xE0, 0x80, 0x00, 0xBC, 0xBF, 0x3F, 0x36, 0x33, 0x1B, 0x09, 0x3C, 0x3F, 0x3F, 0x31,
    0x38, 0x3F, 0x3F, 0x1F, 0x08, 0x3C, 0x3E, 0x3F, 0x31, 0x18, 0x3E, 0x3F, 0x1F, 0x1D, 0x0C, 0x01,
    0x01, 0x01, 0x8F, 0xFF, 0xFC, 0x3F, 0x0F, 0x03, 0x30, 0x37, 0x37, 0x03, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

PROGMEM static const uint8_t imgFever[104] = { // 52x14 "Fever!!"
    0xE0, 0xF0, 0x18, 0x0C, 0x84, 0xFC, 0xFC, 0xBC, 0xC4, 0xC4, 0x1C, 0x1C, 0x04, 0x80, 0xC0, 0x40,
    0xC0, 0xC0, 0x80, 0xC0, 0x40, 0x40, 0xC0, 0xC0, 0x40, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x80, 0xC0,
    0x40, 0xC0, 0xC0, 0x00, 0x80, 0x80, 0xC0, 0x00, 0xC0, 0xC0, 0x60, 0x60, 0x00, 0xFC, 0xFC, 0x7C,
    0x00, 0xFC, 0xFC, 0x7C, 0x03, 0x23, 0x22, 0x38, 0x3F, 0x3F, 0x27, 0x00, 0x01, 0x01, 0x00, 0x3C,
    0x3F, 0x3F, 0x36, 0x33, 0x1B, 0x09, 0x01, 0x01, 0x1E, 0x3F, 0x3F, 0x31, 0x18, 0x0E, 0x07, 0x01,
    0x3C, 0x3F, 0x3F, 0x36, 0x33, 0x1B, 0x09, 0x00, 0x30, 0x3F, 0x3F, 0x0F, 0x03, 0x00, 0x00, 0x00,
    0x30, 0x37, 0x37, 0x00, 0x30, 0x37, 0x37, 0x00
};

PROGMEM static const uint8_t imgOver[512] = { // 128x32 "Have a Boring Holiday"
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x3C, 0x06, 0xC2, 0xFE, 0xFE, 0x7E, 0x42, 0x40, 0xE0, 0xFC,
    0xFE, 0x0F, 0x03, 0x81, 0xC3, 0x63, 0x30, 0x90, 0xF0, 0xF0, 0x30, 0x60, 0x70, 0x90, 0xD0, 0xF0,
    0x70, 0x00, 0x10, 0xF0, 0xF0, 0x00, 0x00, 0xC0, 0xE0, 0xB0, 0xD0, 0xF0, 0x70, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x90, 0xF0, 0xF0, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xD8, 0xD8, 0xD0, 0x18, 0x1E, 0x0F, 0x07, 0x01, 0x00, 0x10, 0x1F, 0x1F, 0x1F,
    0x10, 0x00, 0x1E, 0x1F, 0x1F, 0x18, 0x1C, 0x1F, 0x1F, 0x0F, 0x04, 0x00, 0x00, 0x07, 0x1F, 0x1F,
    0x0C, 0x06, 0x83, 0x81, 0x80, 0x80, 0x9F, 0x9F, 0x1F, 0x19, 0x0C, 0x84, 0xC0, 0xC0, 0x40, 0xC0,
    0xC0, 0x00, 0x00, 0x1E, 0x1F, 0x1F, 0x18, 0x1C, 0x1F, 0xDF, 0xCF, 0xE4, 0x60, 0x60, 0x00, 0xC0,
    0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x60, 0x60, 0x60, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x78, 0xFC, 0xC6, 0x83, 0x01, 0xE1, 0xFE, 0xFE, 0x1E, 0x19, 0xFF, 0xFF, 0xE7, 0x00,
    0xC0, 0xF0, 0xF8, 0x0C, 0x04, 0xFC, 0xFC, 0xF8, 0x00, 0x08, 0xE8, 0xFC, 0xFC, 0x3C, 0x0C, 0x06,
    0x16, 0xF8, 0xFC, 0xFC, 0x1C, 0x00, 0x08, 0xC8, 0xFC, 0xFC, 0x3C, 0x18, 0xC8, 0xFC, 0xFC, 0x3C,
    0x80, 0xE0, 0xF0, 0x38, 0x0C, 0xC4, 0xFC, 0xFC, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0E, 0x0F, 0x81, 0xF0, 0xFF, 0x7F, 0x1F, 0x10, 0xD0, 0xF8, 0xFF, 0x3F, 0x03, 0xC0, 0xF0, 0xF8,
    0x0C, 0x04, 0xFC, 0xFC, 0xF8, 0x00, 0x00, 0xF8, 0xFF, 0x7F, 0x8F, 0x80, 0x10, 0xF8, 0xFC, 0xFC,
    0x1C, 0x00, 0x80, 0xE0, 0xF0, 0x38, 0x0C, 0xC4, 0xFE, 0xFF, 0xBF, 0x81, 0x80, 0xE0, 0xF0, 0x18,
    0x0C, 0xE4, 0xFC, 0xFC, 0x0C, 0x10, 0x18, 0x1C, 0xFC, 0xF8, 0xC0, 0xFC, 0xFC, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x07, 0x07, 0x07, 0x05, 0x04, 0x07, 0x03, 0x03, 0x01, 0x00,
    0x03, 0x07, 0x07, 0x04, 0x06, 0x03, 0x01, 0x00, 0x00, 0x06, 0x07, 0x07, 0x01, 0x00, 0x00, 0x00,
    0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x06, 0x07, 0x07, 0x01, 0x00, 0x07, 0x07, 0x07, 0x62, 0xF2,
    0xF7, 0x9F, 0x8F, 0xC4, 0xE6, 0x7F, 0x7F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06,
    0x04, 0x06, 0x07, 0x03, 0x01, 0x00, 0x00, 0x04, 0x07, 0x07, 0x07, 0x04, 0x00, 0x03, 0x07, 0x07,
    0x04, 0x06, 0x03, 0x01, 0x00, 0x00, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x07, 0x07, 0x07, 0x03,
    0x01, 0x00, 0x07, 0x07, 0x07, 0x06, 0x03, 0x07, 0x07, 0x03, 0x03, 0x01, 0x07, 0x07, 0x07, 0x06,
    0x07, 0x07, 0x07, 0xC3, 0xC1, 0xC0, 0xC0, 0x60, 0x78, 0x3F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00
};

PROGMEM static const byte soundStart[] = {
    0x90, 72, 0, 100, 0x80, 0, 25,
    0x90, 74, 0, 100, 0x80, 0, 25,
    0x90, 76, 0, 100, 0x80, 0, 25,
    0x90, 77, 0, 100, 0x80, 0, 25,
    0x90, 79, 0, 200, 0x80, 0xF0
};

PROGMEM static const byte soundMeow[] = {
    0x90, 117, 0, 30,
    0x90, 116, 0, 30,
    0x90, 115, 0, 30,
    0x90, 113, 0, 40, 0x80, 0xF0
};

PROGMEM static const byte soundOver[] = {
    0x90, 55, 0, 120, 0x80, 0, 10,
    0x90, 54, 0, 140, 0x80, 0, 20,
    0x90, 53, 0, 160, 0x80, 0, 30,
    0x90, 52, 0, 180, 0x80, 0, 40,
    0x90, 51, 0, 200, 0x80, 0, 50,
    0x90, 50, 0, 220, 0x80, 0, 60,
    0x90, 49, 0, 240, 0x80, 0, 70,
    0x90, 48, 0, (byte)260, 0x80, 0xF0
};

static uint8_t  state;
static bool     toDraw;
static uint32_t gameFrames;
static int      counter;
static int      timer;
static int8_t   ignoreCnt;
static uint     score;
static bool     isHiscore;
static uint8_t  catX;
static uint8_t  catAnim;
static uint8_t  meowY;
static uint8_t  boxCnt, boxIdx;
static BOX      boxAry[BOXES];
static SNOW     snowAry[SNOWS];

/*---------------------------------------------------------------------------*/
/*                              Main Functions                               */
/*---------------------------------------------------------------------------*/

void initGame(void)
{
    initBoxes();
    initSnows();
    catX = 160;
    catAnim = 0;
    meowY = 0;
    startGame();
}

bool updateGame(void)
{
#ifdef DEBUG
    if (dbgRecvChar == 'z') {
        initBoxes();
        startGame();
        dprintln("Reset");
}
#endif

    /*  In case of pausing  */
    if (state == STATE_PAUSE) {
        if (arduboy_game1.buttonDown(A_BUTTON | B_BUTTON)) {
            state = STATE_GAME;
            ignoreCnt = 30; // a half second
            toDraw = true;
            dprintln("Resume");
        }
        return false;
    }

    /*  Usual case  */
    moveCat();
    moveBoxes();
    if (SNOWS - boxCnt > 0) moveSnows();
    counter--;
    if (state == STATE_START) {
        if (counter == secs(1)) {
            throwFirstBox();
        } else if (counter == 0) {
            state = STATE_GAME;
        }
    } else if (state == STATE_GAME) {
        if (timer > 0) timer--;
        gameFrames++;
        if (boxCnt == 0) {
            state = STATE_OVER;
            isHiscore = (setLastScore(score, gameFrames) == 0);
            counter = secs(8);
            arduboy_game1.playScore2(soundOver, 1);
            dprint("Game Over: score=");
            dprintln(score);
        } else if (ignoreCnt == 0 && arduboy_game1.buttonDown(A_BUTTON | B_BUTTON)) {
            state = STATE_PAUSE;
            dprintln("Pause");
        }
    } else if (state == STATE_OVER) {
        if (ignoreCnt == 0 && arduboy_game1.buttonDown(A_BUTTON | B_BUTTON)) {
            startGame();
        }
    }
    if (ignoreCnt > 0) ignoreCnt--;
    toDraw = true;

    return (state == STATE_OVER && counter == 0);
}

void drawGame(void)
{
    if (toDraw) {
        if (boxCnt >= BOXES / 3 && state == STATE_GAME && counter % 2 == 0) {
            return; // Frame skip
        }
        arduboy_game1.clear();
        arduboy_game1.drawFastVLine2(0, 0, 64, WHITE);
        arduboy_game1.drawFastVLine2(127, 0, 64, WHITE);
        drawSnows();
        if (state != STATE_PAUSE) drawBoxes();
        drawCat();
        if (state == STATE_START) drawSantaClaus();
        drawStrings();
        toDraw = false;
    }
}

/*---------------------------------------------------------------------------*/
/*                             Control Functions                             */
/*---------------------------------------------------------------------------*/

static void startGame()
{
    state = STATE_START;
    gameFrames = 0;
    counter = secs(2);
    timer = 5 * secs(60); // 2 minutes
    score = 0;
    arduboy_game1.playScore2(soundStart, 0);
    dprintln("Start Game");
}

static void moveCat(void)
{
    int vx = 0;
    if (state == STATE_START) {
        vx = sign(64 - catX);
        catX += vx;
    } else {
        if (arduboy_game1.buttonPressed(LEFT_BUTTON)) vx--;
        if (arduboy_game1.buttonPressed(RIGHT_BUTTON)) vx++;
        catX = mid(0, catX + vx, 128);
    }

    if (vx != 0) {
        catAnim = ((catAnim & 0xF) + 1) & 0xF;
        if (vx > 0) catAnim |= 0x10;
    } else {
        catAnim &= 0x10;
    }

    if (meowY > 0) {
        meowY--;
    } else if (0 && arduboy_game1.buttonDown(DOWN_BUTTON)) {
        meowY = 8;
        arduboy_game1.playScore2(soundMeow, 2);
        dprintln("meow");
    }
}

static void initBoxes(void)
{
    for (int i = 0; i < BOXES; i++) {
        boxAry[i].x = -1;
    }
    boxCnt = 0;
    boxIdx = 0;
}

static void throwFirstBox(void)
{
    BOX *pBox = &boxAry[0];
    pBox->x = coord(64);
    pBox->y = coord(32);
    pBox->vx = 0;
    pBox->vy = -64;
    boxCnt = 1;
}

static void moveBoxes(void)
{
    boolean isBound = false;
    for (int i = 0; i < BOXES; i++) {
        BOX *pBox = &boxAry[i];
        if (pBox->x < 0) continue;
        uint16_t lastY = pBox->y;
        pBox->x += pBox->vx;
        pBox->y += pBox->vy;
        pBox->vy++;

        if (pBox->x < coord(4)) {
            pBox->x = coord(4);
            pBox->vx = -pBox->vx * 3 / 4;
        } else if (pBox->x > coord(124)) {
            pBox->x = coord(124);
            pBox->vx = -pBox->vx * 3 / 4;
        }

        int gap = pBox->x - coord(catX);
        if (timer > 0 && pBox->y >= coord(60) && lastY < coord(60) && abs(gap) <= coord(10)) {
            boundBox(pBox, coordInv(gap));
            isBound = true;
            score++;
        } else if (pBox->y > coord(70)) {
            pBox->x = -1;
            boxCnt--;
        }
    }

    if (isBound) {
        arduboy_game1.tunes.tone((timer > secs(30)) ? 1440 : 1920, 15);
        dprint("boxCnt=");
        dprintln(boxCnt);
    }
}

static void boundBox(BOX *pBox, int gap)
{
    int16_t x = pBox->x;
    int8_t  vx = pBox->vx;
    int8_t  vy = -pBox->vy / 2;

    int mul = 2;
    if (timer > secs(30)) {
        mul = 2;
        gap -= 4;
    } else {
        mul = 3;
        gap -= 8;
    }
    for (int i = 0; i < mul; i++, gap += 8) {
        if (i > 0 && boxCnt < BOXES) {
            pBox = &boxAry[getNewBoxIndex()];
            pBox->x = x;
            boxCnt++;
        }
        pBox->y = coord(60);
        pBox->vx = vx + gap;
        pBox->vy = vy - (40 - abs(gap));
    }
}

static int getNewBoxIndex(void)
{
    do {
        boxIdx = (boxIdx + 1) % BOXES;
    } while (boxAry[boxIdx].x >= 0);
    return boxIdx;
}

static void initSnows(void)
{
    for (int i = 0; i < SNOWS; i++) {
        SNOW *pSnow = &snowAry[i];
        pSnow->x = random(128);
        pSnow->y = random(64);
    }
}

static void moveSnows(void)
{
    for (int i = 0; i < SNOWS; i++) {
        SNOW *pSnow = &snowAry[i];
        pSnow->x += random(-1, 2);
        pSnow->y += random(2);
        if (pSnow->y >= 64) {
            pSnow->x = random(128);
            pSnow->y = 0;
        }
    }
}

/*---------------------------------------------------------------------------*/
/*                              Draw Functions                               */
/*---------------------------------------------------------------------------*/

static void drawCat(void)
{
    if (meowY > 0) {
        arduboy_game1.printEx(catX - 12, 46 + meowY / 2, F("MEOW"));
    }
    arduboy_game1.fillRect2(catX - 12 + (catAnim & 0x10), 55, 8, 3, BLACK);
    arduboy_game1.drawBitmap(catX - 12, 52, imgCat[catAnim / 4], 24, 12, WHITE);
}

static void drawSantaClaus(void)
{
    arduboy_game1.drawBitmap(counter * 2 - 112, 16, imgSantaClaus, 64, 32, WHITE);
}

static void drawBoxes(void)
{
    for (int i = 0; i < BOXES; i++) {
        BOX *pBox = &boxAry[i];
        if (pBox->x < 0) continue;
        int16_t x = coordInv(pBox->x) - 4;
        int16_t y = coordInv(pBox->y) - 4;
        int8_t anim = x % 6;
        arduboy_game1.drawBitmap(x, y, imgBox[anim], 8, 8, WHITE);
    }
}

static void drawSnows(void)
{
    for (int i = 0, c = SNOWS - boxCnt; i < c; i++) {
        SNOW *pSnow = &snowAry[i];
        arduboy_game1.drawPixel(pSnow->x, pSnow->y, WHITE);
    }
}

static void drawStrings(void)
{
    if (state == STATE_START) {
        arduboy_game1.drawBitmap(36, 0, imgReady, 56, 19, WHITE);
    } else {
        /*  Score  */
        drawFigure(2, 0, score, ALIGN_LEFT);

        /*  Timer  */
        if (timer >= secs(60)) {
            drawFigure(92, 0, timer / 3600, ALIGN_LEFT);
            arduboy_game1.drawBitmap(102, 0, imgDash, 4, 4, WHITE);
            drawFigure(116, 0, timer % 3600 / 60, ALIGN_RIGHT);
        } else {
            drawFigure(98, 0, timer / 60, ALIGN_RIGHT);
            arduboy_game1.drawBitmap(108, 0, imgDash, 8, 4, WHITE);
            drawFigure(116, 0, timer % 60 / 6, ALIGN_LEFT);
        }
        if (state == STATE_GAME) {
            if (timer <= secs(30) && timer > secs(27)) {
                arduboy_game1.drawBitmap(38, 16, imgFever, 52, 14, (timer % 8 < 4) ? WHITE : BLACK);
            }
        } else if (state == STATE_OVER) {
            int y = 12;
            if (counter > 458) {
                y -= (counter - 458) * 2;
            } else if (counter > secs(7)) {
                y += counter % 2;
            }
            arduboy_game1.drawBitmap(0, y, imgOver, 128, 32, WHITE);
            if (isHiscore && counter % 8 < 4) {
                arduboy_game1.printEx(31, 46, F("NEW RECORD!"));
            }
        } else if (state == STATE_PAUSE) {
            arduboy_game1.printEx(49, 46, F("PAUSE"));
        }
    }
}

static int  drawFigure(int16_t x, int16_t y, int value, uint8_t align)
{
    int k = (value > 9) ? drawFigure(x - align * 5, y, value / 10, align) : 0;
    arduboy_game1.drawBitmap(x + k, y, imgFigures[value % 10], 10, 14, WHITE);
    return k + 10 - align * 5;
}
