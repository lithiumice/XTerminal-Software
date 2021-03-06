#include "common.h"

/*  Defines  */

enum {
    STATE_MENU = 0,
    STATE_RECORD,
    STATE_CREDIT,
};

enum {
    MENU_START = 0,
    MENU_SOUND,
    MENU_RECORD,
    MENU_CREDIT,
};

enum {
    RECORD_NOT_READ = 0,
    RECORD_INITIAL,
    RECORD_STORED,
};

#define EEPROM_ADDR_BASE    832
#define EEPROM_SIGNATURE    0x034E424FUL // "OBN\x03"

/*  Typedefs  */

typedef struct
{
    const uint8_t *pImg;
    uint8_t width;
} TITLE_LETTER;

/*  Local Functions  */

static bool handleButtons(void);
static void setSound(bool enabled);
static void playSound1(void);
static void playSound2(void);
static void readRecord(void);

static void drawTitleMenu(void);
static void drawTitleRecord(void);
static void drawTitleCredit(void);

/*  Local Variables  */

PROGMEM static const uint8_t imgTitle1[192] = { // 96x16
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xC0, 0x40,
    0xA0, 0x60, 0xA0, 0x50, 0xB0, 0x50, 0xB0, 0x50, 0xA0, 0x60, 0xA0, 0x40, 0xC0, 0x00, 0x80, 0x80,
    0x80, 0xE0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFC, 0xF0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0,
    0xF8, 0xFE, 0xFE, 0xFE, 0xFC, 0xF0, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xC0, 0xC0, 0x80,
    0x00, 0x02, 0x86, 0xFC, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xC0, 0xB0, 0x58, 0xAC, 0x56, 0xAA, 0x55, 0xAB, 0x55, 0xAA, 0x55, 0xAA, 0x55,
    0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0x0A, 0xE5, 0xFA, 0xFD, 0xFE, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x7F, 0x07, 0x01, 0x80, 0x80, 0x42, 0x42, 0x42, 0x41, 0x21, 0x21, 0x21, 0x11, 0x11, 0x10,
    0x08, 0x08, 0x08, 0x88, 0x84, 0x84, 0x44, 0x42, 0x42, 0x22, 0x22, 0x21, 0x11, 0x10, 0x00, 0x00
};

PROGMEM static const uint8_t imgTitle2[224] = { // 112x16
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,
    0x40, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0x2E, 0xEE, 0xEE, 0xEE, 0xDE, 0xDE,
    0xDC, 0xDD, 0xBC, 0xB9, 0xBA, 0x79, 0x72, 0xE5, 0xCA, 0x95, 0xAA, 0x55, 0xAA, 0x55, 0x2A, 0x10,
    0x8F, 0xBF, 0xFF, 0xD7, 0x57, 0x7F, 0xFF, 0xE0, 0xE0, 0x3F, 0x3F, 0x8F, 0x8F, 0xBF, 0x3F, 0x3F,
    0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xDF, 0x57, 0x57, 0x77, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xDF, 0x07, 0x03, 0x01, 0x00, 0x20, 0x20, 0x10, 0x10, 0x10, 0x08,
    0x08, 0x08, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0xC3, 0xC5, 0x89, 0x12, 0x32, 0x64, 0x04,
    0x09, 0x0E, 0x08, 0x00, 0x00, 0xE0, 0xFC, 0x7F, 0x0F, 0x3F, 0xF8, 0xFF, 0xFB, 0xF3, 0xFF, 0x7F,
    0xB3, 0xDB, 0xDD, 0xFC, 0xFE, 0x7F, 0xBF, 0xCE, 0xF5, 0xFB, 0xFD, 0xFE, 0xFE, 0x7F, 0x3F, 0x1F,
    0x0F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x33, 0x7B, 0x7F, 0x7F, 0x7F, 0x3F, 0x1F, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

PROGMEM static const uint8_t imgTitle3[224] = { // 56x32
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x0A, 0x0A,
    0x8A, 0x8A, 0xCA, 0xCA, 0xB3, 0x91, 0x91, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xEE, 0xEF, 0x53, 0x49,
    0xCD, 0xDE, 0xDF, 0xDF, 0xFF, 0x9F, 0xCE, 0xE6, 0xF2, 0xFA, 0xFE, 0xFE, 0x01, 0xFE, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0xC0, 0x40, 0xA0, 0x60, 0xB0, 0x58, 0xA8, 0x54, 0xAA, 0xD6, 0xE2, 0xFA,
    0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7E, 0x36, 0xB9, 0x3F, 0xA0, 0x3F, 0xBF, 0x3F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x3D, 0xBB, 0x37, 0xB7, 0x57, 0x97, 0x47, 0xA7, 0x53, 0xA9, 0x00,
    0x00, 0x80, 0xC0, 0xC0, 0x48, 0x58, 0xD0, 0xF0, 0xE0, 0xEC, 0xEE, 0xEE, 0xDC, 0x3D, 0xFA, 0xF5,
    0xEA, 0x15, 0xAA, 0x53, 0x6B, 0x37, 0x27, 0x17, 0x1B, 0x0B, 0x0B, 0x0D, 0x06, 0x02, 0x02, 0x03,
    0x02, 0x05, 0x0E, 0x75, 0xAA, 0x55, 0xBA, 0x7D, 0x7E, 0x3D, 0x8A, 0x55, 0xAA, 0x55, 0xAA, 0x55,
    0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x06, 0x03, 0x03, 0x39,
    0x0F, 0x03, 0x01, 0x03, 0x0F, 0x0C, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFD, 0xAA, 0x55,
    0xBE, 0x7F, 0xBE, 0x1D, 0x82, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x00
};

PROGMEM static const char menuText[] = "START GAME\0SOUND \0RECORD\0CREDIT";
PROGMEM static const char creditText[] = "- " APP_TITLE " -\0\0" APP_RELEASED \
        "\0PROGREMMED BY OBONO\0\0THIS PROGRAM IS\0" "RELEASED UNDER\0" "THE MIT LICENSE.";

static uint8_t  state;
static bool     toDraw;
static bool     sound;
static int8_t   menuPos;
static uint16_t lastScore = 0;
static uint8_t  recordState = RECORD_NOT_READ;
static uint16_t hiScore[10];
static uint16_t playCount;
static uint32_t playFrames;

/*---------------------------------------------------------------------------*/
/*                              Main Functions                               */
/*---------------------------------------------------------------------------*/

void initTitle(void)
{
    state = STATE_MENU;
    menuPos = MENU_START;
    toDraw = true;
    setSound(arduboy_game1.audio.enabled());
    if (recordState == RECORD_NOT_READ) {
        readRecord();
    }
}

bool updateTitle(void)
{
    bool ret = false;
    if (state == STATE_MENU) {
        ret = handleButtons();
    } else {
        if (arduboy_game1.buttonDown(A_BUTTON | B_BUTTON)) {
            state = STATE_MENU;
            playSound2();
            toDraw = true;
        }
    }

#ifdef DEBUG
    if (dbgRecvChar == 'r') {
        arduboy.eepSeek(EEPROM_ADDR_BASE);
        for (int i = 0; i < 8; i++) {
            arduboy.eepWrite32(0);
        }
        recordState = RECORD_INITIAL;
        dprintln("Clean EEPROM");
    }
#endif

    randomSeed(rand() ^ micros()); // Shuffle random
    return ret;
}

void drawTitle(void)
{
    if (toDraw) {
        arduboy_game1.clear();
        switch (state) {
        case STATE_MENU:
            drawTitleMenu();
            break;
        case STATE_RECORD:
            drawTitleRecord();
            break;
        case STATE_CREDIT:
            drawTitleCredit();
            break;
        }
        toDraw = false;
    }
}

uint8_t setLastScore(int score, uint32_t frames)
{
    lastScore = score;

    /*  Updarte best 10  */
    int r = 10;
    uint16_t h;
    while (r > 0 && (h = hiScore[r - 1]) < score) {
        if (--r < 9) hiScore[r + 1] = h;
    }
    if (r < 10) hiScore[r] = score;
    playCount++;
    playFrames += frames;

    /*  Store record to EEPROM  */
    if (recordState == RECORD_INITIAL) {
        arduboy_game1.eepSeek(EEPROM_ADDR_BASE);
        arduboy_game1.eepWrite32(EEPROM_SIGNATURE);
    }
    arduboy_game1.eepSeek(EEPROM_ADDR_BASE + 4 + r * 2);
    for (int i = r; i < 10; i++) {
        arduboy_game1.eepWrite16(hiScore[i]);
    }
    arduboy_game1.eepWrite16(playCount);
    arduboy_game1.eepWrite32(playFrames);

    uint16_t checkSum = (EEPROM_SIGNATURE & 0xFFFF) + (EEPROM_SIGNATURE >> 16) * 2;
    for (int i = 0; i < 10; i++) {
        checkSum += hiScore[i] * (i + 3);
    }
    checkSum += playCount * 13;
    checkSum += (playFrames & 0xFFFF) * 14 + (playFrames >> 16) * 15;
    arduboy_game1.eepWrite16(checkSum);

    recordState = RECORD_STORED;
    return r;
}

/*---------------------------------------------------------------------------*/
/*                             Control Functions                             */
/*---------------------------------------------------------------------------*/

static bool handleButtons()
{
    bool ret = false;
    if (arduboy_game1.buttonDown(UP_BUTTON)) {
        if (menuPos-- == MENU_START) menuPos = MENU_CREDIT;
        playSound1();
        toDraw = true;
    }
    if (arduboy_game1.buttonDown(DOWN_BUTTON)) {
        if (menuPos++ == MENU_CREDIT) menuPos = MENU_START;
        playSound1();
        toDraw = true;
    }
    if (arduboy_game1.buttonDown(A_BUTTON | B_BUTTON)) {
        switch (menuPos) {
        case MENU_START:
            ret = true;
            arduboy_game1.audio.saveOnOff();
            break;
        case MENU_SOUND:
            setSound(!sound);
            playSound2();
            break;
        case MENU_RECORD:
            state = STATE_RECORD;
            playSound2();
            break;
        case MENU_CREDIT:
            state = STATE_CREDIT;
            playSound2();
            break;
        }
        toDraw = true;
    }
    return ret;
}

static void setSound(bool enabled)
{
    if (enabled) {
        arduboy_game1.audio.on();
    } else {
        arduboy_game1.audio.off();
    }
    sound = enabled;
}

static void playSound1(void)
{
    arduboy_game1.tunes.tone(440, 10);
}

static void playSound2(void)
{
    arduboy_game1.tunes.tone(587, 20);
}

static void readRecord(void)
{
    bool isRegular = false;
    arduboy_game1.eepSeek(EEPROM_ADDR_BASE);
    if (arduboy_game1.eepRead32() == EEPROM_SIGNATURE) {
        uint16_t checkSum = (EEPROM_SIGNATURE & 0xFFFF) + (EEPROM_SIGNATURE >> 16) * 2;
        for (int i = 0; i < 13; i++) {
            checkSum += arduboy_game1.eepRead16() * (i + 3);
        }
        isRegular = (arduboy_game1.eepRead16() == checkSum);
    }

    if (isRegular) {
        /*  Read record from EEPROM  */
        arduboy_game1.eepSeek(EEPROM_ADDR_BASE + 4);
        for (int i = 0; i < 10; i++) {
            hiScore[i] = arduboy_game1.eepRead16();
        }
        playCount = arduboy_game1.eepRead16();
        playFrames = arduboy_game1.eepRead32();
        recordState = RECORD_STORED;
    } else {
        /*  Initialize record  */
        for (int i = 0; i < 10; i++) {
            hiScore[i] = 0;
        }
        playCount = 0;
        playFrames = 0;
        recordState = RECORD_INITIAL;
    }
}

/*---------------------------------------------------------------------------*/
/*                              Draw Functions                               */
/*---------------------------------------------------------------------------*/

static void drawTitleMenu(void)
{
    /*  Title image  */
    arduboy_game1.drawBitmap(32, 0, imgTitle1, 96, 16, WHITE);
    arduboy_game1.drawBitmap(8, 16, imgTitle2, 112, 16, WHITE);
    arduboy_game1.drawBitmap(0, 32, imgTitle3, 56, 32, WHITE);

    /*  Last score  */
    if (lastScore > 0) {
        arduboy_game1.setCursor(0, 0);
        arduboy_game1.print(lastScore);
    }

    /*  Menu items  */
    char buf[12];
    const char *p = menuText;
    for (int i = 0; i < 4; i++) {
        strcpy_P(buf, p);
        p += arduboy_game1.printEx(68 - (i == menuPos) * 4, i * 6 + 40, buf) + 1;
        if (i == MENU_SOUND) {
            arduboy_game1.print((sound) ? F("ON") : F("OFF"));
        }
    }
    arduboy_game1.fillRect2(56, menuPos * 6 + 40, 5, 5, WHITE);
}

static void drawTitleRecord(void)
{
    arduboy_game1.printEx(22, 4, F("BEST 10 SCORES"));
    arduboy_game1.drawFastHLine2(0, 12, 128, WHITE);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            int r = i * 5 + j;
            arduboy_game1.printEx(i * 60 + 4 - (r == 9) * 6, j * 6 + 14, F("["));
            arduboy_game1.print(r + 1);
            arduboy_game1.print(F("] "));
            arduboy_game1.print(hiScore[r]);
        }
    }
    arduboy_game1.drawFastHLine2(0, 44, 128, WHITE);
    arduboy_game1.printEx(16, 48, F("PLAY COUNT "));
    arduboy_game1.print(playCount);
    arduboy_game1.printEx(16, 54, F("PLAY TIME  "));
    arduboy_game1.print(playFrames / 3600); // minutes
    char buf[6];
    sprintf(buf, "'%02d\"", playFrames / 60 % 60); // seconds
    arduboy_game1.print(buf);
}

static void drawTitleCredit(void)
{
    char buf[20];
    const char *p = creditText;
    for (int i = 0; i < 8; i++) {
        strcpy_P(buf, p);
        uint8_t len = strlen(buf);//, sizeof(buf));
        p += arduboy_game1.printEx(64 - len * 3, i * 6 + 8, buf) + 1;
    }
}
