#include "HAL/HAL.h"
#include "App/Utils/ButtonEvent/ButtonEvent.h"
#include "App/Accounts/Account_Master.h"

#if 1

static ButtonEvent EncoderPush(2000);
static ButtonEvent EncoderA(2000);
static ButtonEvent EncoderB(2000);

static bool EncoderEnable = true;
static volatile int16_t EncoderDiff = 0;
static bool EncoderDiffDisable = false;
uint8_t enc_long_push_flag = 0;
Account *actEncoder;

void HAL::Encoder_SetEnable(bool en)
{
    EncoderEnable = en;
}

bool HAL::Encoder_GetIsPush()
{
    if (!EncoderEnable)
    {
        return false;
    }

    return (digitalRead(CONFIG_ENCODER_PUSH_PIN) == LOW);
}

static bool Encoder_GetIsA()
{

    return (digitalRead(CONFIG_ENCODER_A_PIN) == LOW);
}

static bool Encoder_GetIsB()
{

    return (digitalRead(CONFIG_ENCODER_B_PIN) == LOW);
}

static void Encoder_PushHandler(ButtonEvent *btn, int event)
{
    if (event == ButtonEvent::EVENT_PRESSED)
    {
        HAL::enc_btn_first_push_flag = 1;
        Serial.println("Encoder btn EVENT_PRESSED");

        HAL::Buzz_Tone(500, 20);
        EncoderDiffDisable = true;
    }
    else if (event == ButtonEvent::EVENT_RELEASED)
    {
        HAL::Buzz_Tone(700, 20);
        EncoderDiffDisable = false;
    }
    else if (event == ButtonEvent::EVENT_LONG_PRESSED)
    {
        Serial.println("Encoder btn EVENT_LONG_PRESSED");

        enc_long_push_flag = 1;
        HAL::Audio_PlayMusic("Shutdown");
        // HAL::Power_Shutdown();
    }
}

static void Encoder_AHandler(ButtonEvent *btn, int event)
{
    if (event == ButtonEvent::EVENT_PRESSED)
    {
        EncoderDiff++;
        Serial.println("Encoder_AHandler");

    }
    else if (event == ButtonEvent::EVENT_RELEASED)
    {
    }
    else if (event == ButtonEvent::EVENT_LONG_PRESSED)
    {
    }
}

static void Encoder_BHandler(ButtonEvent *btn, int event)
{
    if (event == ButtonEvent::EVENT_PRESSED)
    {
        EncoderDiff--;
        Serial.println("Encoder_BHandler");

    }
    else if (event == ButtonEvent::EVENT_RELEASED)
    {
    }
    else if (event == ButtonEvent::EVENT_LONG_PRESSED)
    {
    }
}

void HAL::Encoder_Init()
{
    pinMode(CONFIG_ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(CONFIG_ENCODER_B_PIN, INPUT_PULLUP);
    pinMode(CONFIG_ENCODER_PUSH_PIN, INPUT_PULLUP);

    EncoderPush.EventAttach(Encoder_PushHandler);
    EncoderA.EventAttach(Encoder_AHandler);
    EncoderB.EventAttach(Encoder_BHandler);

    actEncoder = new Account("Encoder", AccountSystem::Broker(), sizeof(int16_t), nullptr);
}

void HAL::Encoder_Update()
{
    EncoderPush.EventMonitor(Encoder_GetIsPush());
    EncoderA.EventMonitor(Encoder_GetIsA());
    EncoderB.EventMonitor(Encoder_GetIsB());
}

int16_t HAL::Encoder_GetDiff()
{
    int16_t diff = (EncoderDiff);

    if (diff != 0)
    {
        EncoderDiff = 0;
        HAL::Buzz_Tone(300, 5);
        HAL::enc_btn_first_push_flag = 1;
        actEncoder->Commit((const void *)&diff, sizeof(int16_t));
        actEncoder->Publish();
    }

    return diff;
}

#else
static ButtonEvent EncoderPush(2000);
// static ButtonEvent EncoderPush(5000);

static bool EncoderEnable = true;
static volatile int16_t EncoderDiff = 0;
static bool EncoderDiffDisable = false;
uint8_t enc_long_push_flag = 0;
Account *actEncoder;

static void Encoder_IrqHandler()
{
    if (!EncoderEnable)
    {
        return;
    }

    static volatile int count, countLast;
    static volatile uint8_t a0, b0;
    static volatile uint8_t ab0;
    uint8_t a = digitalRead(CONFIG_ENCODER_A_PIN);
    uint8_t b = digitalRead(CONFIG_ENCODER_B_PIN);
    if (a != a0)
    {
        a0 = a;
        if (b != b0)
        {
            b0 = b;
            count += ((a == b) ? 1 : -1);
            if ((a == b) != ab0)
            {
                count += ((a == b) ? 1 : -1);
            }
            ab0 = (a == b);
        }
    }

    if (count != countLast)
    {
        EncoderDiff += (count - countLast) > 0 ? 1 : -1;
        countLast = count;
    }
}

static void Encoder_PushHandler(ButtonEvent *btn, int event)
{
    if (event == ButtonEvent::EVENT_PRESSED)
    {
        HAL::enc_btn_first_push_flag = 1;
        Serial.println("Encoder btn EVENT_PRESSED");

        HAL::Buzz_Tone(500, 20);
        EncoderDiffDisable = true;
    }
    else if (event == ButtonEvent::EVENT_RELEASED)
    {
        HAL::Buzz_Tone(700, 20);
        EncoderDiffDisable = false;
    }
    else if (event == ButtonEvent::EVENT_LONG_PRESSED)
    {
        Serial.println("Encoder btn EVENT_LONG_PRESSED");

        enc_long_push_flag = 1;
        HAL::Audio_PlayMusic("Shutdown");
        // HAL::Power_Shutdown();
    }
}

static void Encoder_RotateHandler(int16_t diff)
{
    HAL::Buzz_Tone(300, 5);
    HAL::enc_btn_first_push_flag = 1;
    actEncoder->Commit((const void *)&diff, sizeof(int16_t));
    actEncoder->Publish();
}

void HAL::Encoder_Init()
{
    pinMode(CONFIG_ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(CONFIG_ENCODER_B_PIN, INPUT_PULLUP);
    pinMode(CONFIG_ENCODER_PUSH_PIN, INPUT_PULLUP);
    attachInterrupt(CONFIG_ENCODER_A_PIN, Encoder_IrqHandler, CHANGE);

    EncoderPush.EventAttach(Encoder_PushHandler);
    actEncoder = new Account("Encoder", AccountSystem::Broker(), sizeof(int16_t), nullptr);
}

void HAL::Encoder_Update()
{
    EncoderPush.EventMonitor(Encoder_GetIsPush());
}

int16_t HAL::Encoder_GetDiff()
{
#ifdef BOARD_WROOM32
    int16_t diff = (EncoderDiff / 2);
#else
    int16_t diff = -(EncoderDiff / 2);
#endif

    if (diff != 0)
    {
        EncoderDiff = 0;
        Encoder_RotateHandler(diff);
    }

    return diff;
}

bool HAL::Encoder_GetIsPush()
{
    if (!EncoderEnable)
    {
        return false;
    }

    return (digitalRead(CONFIG_ENCODER_PUSH_PIN) == LOW);
}

void HAL::Encoder_SetEnable(bool en)
{
    EncoderEnable = en;
}
#endif