#ifndef __ANO_DT__
#define __ANO_DT__
#include "Arduino.h"

#define BYTE0(dwTemp) (*((char *)(&dwTemp)))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

class data_send
{
private:
    // uint32_t  uart_baud = 115200;
    // uint32_t  uart_baud = 460800;

    uint8_t ano_data_buff[50];
    uint8_t vscope_data_buff[10];
    uint8_t dscope_data_buff[42];
    uint8_t vofa_data_buff[42];

    void send_data(uint8_t *dataToSend, uint8_t length)
    {
        // uart_putbuff(USART_8, dataToSend, length);
        // Serial.print((char*)dataToSend);
        Serial.write(dataToSend, length);
    }

    // uint16_t crc_check(uint8_t *Buf, uint8_t CRC_CNT)
    // {
    //     uint16_t CRC_Temp;
    //     uint8_t i, j;
    //     CRC_Temp = 0xffff;

    //     for (i = 0; i < CRC_CNT; i++)
    //     {
    //         CRC_Temp ^= Buf[i];
    //         for (j = 0; j < 8; j++)
    //         {
    //             if (CRC_Temp & 0x01)
    //                 CRC_Temp = (CRC_Temp >> 1) ^ 0xa001;
    //             else
    //                 CRC_Temp = CRC_Temp >> 1;
    //         }
    //     }
    //     return (CRC_Temp);
    // }

    // void float_to_byte(float *target, unsigned char *buf, unsigned char beg)
    // {
    //     unsigned char *point;
    //     point = (unsigned char *)target; //得到float的地址
    //     buf[beg] = point[0];
    //     buf[beg + 1] = point[1];
    //     buf[beg + 2] = point[2];
    //     buf[beg + 3] = point[3];
    // }

    // void dscope_fill(float Data, unsigned char Channel)
    // {
    //     if ((Channel > 10) || (Channel == 0))
    //         return; //通道个数大于10或等于0，直接跳出，不执行函数
    //     else
    //     {
    //         switch (Channel)
    //         {
    //         case 1:
    //             float_to_byte(&Data, dscope_data_buff, 1);
    //             break;
    //         case 2:
    //             float_to_byte(&Data, dscope_data_buff, 5);
    //             break;
    //         case 3:
    //             float_to_byte(&Data, dscope_data_buff, 9);
    //             break;
    //         case 4:
    //             float_to_byte(&Data, dscope_data_buff, 13);
    //             break;
    //         case 5:
    //             float_to_byte(&Data, dscope_data_buff, 17);
    //             break;
    //         case 6:
    //             float_to_byte(&Data, dscope_data_buff, 21);
    //             break;
    //         case 7:
    //             float_to_byte(&Data, dscope_data_buff, 25);
    //             break;
    //         case 8:
    //             float_to_byte(&Data, dscope_data_buff, 29);
    //             break;
    //         case 9:
    //             float_to_byte(&Data, dscope_data_buff, 33);
    //             break;
    //         case 10:
    //             float_to_byte(&Data, dscope_data_buff, 37);
    //             break;
    //         }
    //     }
    // }

    // uint8_t dscope_gen(unsigned char Channel_Number)
    // {
    //     if ((Channel_Number > 10) || (Channel_Number == 0))
    //     {
    //         return 0;
    //     }
    //     else
    //     {
    //         dscope_data_buff[0] = '$'; //帧头

    //         switch (Channel_Number)
    //         {
    //         case 1:
    //             dscope_data_buff[5] = 5;
    //             return 6;
    //             break;
    //         case 2:
    //             dscope_data_buff[9] = 9;
    //             return 10;
    //             break;
    //         case 3:
    //             dscope_data_buff[13] = 13;
    //             return 14;
    //             break;
    //         case 4:
    //             dscope_data_buff[17] = 17;
    //             return 18;
    //             break;
    //         case 5:
    //             dscope_data_buff[21] = 21;
    //             return 22;
    //             break;
    //         case 6:
    //             dscope_data_buff[25] = 25;
    //             return 26;
    //             break;
    //         case 7:
    //             dscope_data_buff[29] = 29;
    //             return 30;
    //             break;
    //         case 8:
    //             dscope_data_buff[33] = 33;
    //             return 34;
    //             break;
    //         case 9:
    //             dscope_data_buff[37] = 37;
    //             return 38;
    //             break;
    //         case 10:
    //             dscope_data_buff[41] = 41;
    //             return 42;
    //             break;
    //         }
    //     }
    //     return 0;
    // }

public:
    data_send() {}

    void init(uint32_t uart_baud = 115200)
    {
    }

    void send_vofa_4(float data1, float data2, float data3, float data4)
    {
        float data[4];
        data[0] = data1;
        data[1] = data2;
        data[2] = data3;
        data[3] = data4;
        send_data((uint8_t *)data, sizeof(float) * 4);

        char tail[4] = {0x00, 0x00, 0x80, 0x7f};
        send_data((uint8_t *)tail, 4);
    }

    void send_vofa_8(float data1, float data2, float data3, float data4,
                     float data5, float data6, float data7, float data8)
    {
        float data[8];
        data[0] = data1;
        data[1] = data2;
        data[2] = data3;
        data[3] = data4;
        data[4] = data5;
        data[5] = data6;
        data[6] = data7;
        data[7] = data8;
        send_data((uint8_t *)data, sizeof(float) * 8);

        char tail[4] = {0x00, 0x00, 0x80, 0x7f};
        send_data((uint8_t *)tail, 4);
    }

    // void send_dscope_4(float data1, float data2, float data3, float data4)
    // {
    //     int Send_Count;
    //     dscope_fill(data1, 1);
    //     dscope_fill(data2, 2);
    //     dscope_fill(data3, 3);
    //     dscope_fill(data4, 4);
    //     Send_Count = dscope_gen(4);
    //     for (int i = 0; i < Send_Count; i++)
    //     {
    //         uart_putchar(USART_3, dscope_data_buff[i]);
    //     }
    // }

    // void send_vscope_4(int16_t data1, int16_t data2, int16_t data3, int16_t data4)
    // {
    //     uint8_t *dat = vscope_data_buff;
    //     uint16_t CRC16 = 0;

    //     dat[0] = (uint8_t)((uint16_t)data1 & 0xff);
    //     dat[1] = (uint8_t)((uint16_t)data1 >> 8);

    //     dat[2] = (uint8_t)((uint16_t)data2 & 0xff);
    //     dat[3] = (uint8_t)((uint16_t)data2 >> 8);

    //     dat[4] = (uint8_t)((uint16_t)data3 & 0xff);
    //     dat[5] = (uint8_t)((uint16_t)data3 >> 8);

    //     dat[6] = (uint8_t)((uint16_t)data4 & 0xff);
    //     dat[7] = (uint8_t)((uint16_t)data4 >> 8);

    //     CRC16 = crc_check(dat, 8);
    //     dat[8] = (uint8_t)(CRC16 & 0xff);
    //     dat[9] = (uint8_t)(CRC16 >> 8);

    //     send_data(dat, 10);
    // }

    void send_ano_pid(uint8_t group, float p1_p, float p1_i, float p1_d, float p2_p,
                      float p2_i, float p2_d, float p3_p, float p3_i, float p3_d)
    {
        uint8_t _cnt = 0;
        short _temp;

        ano_data_buff[_cnt++] = 0xAA;
        ano_data_buff[_cnt++] = 0xAA;
        ano_data_buff[_cnt++] = 0x10 + group - 1;
        ano_data_buff[_cnt++] = 0;

        _temp = (short)(p1_p * 1000);
        ano_data_buff[_cnt++] = BYTE1(_temp);
        ano_data_buff[_cnt++] = BYTE0(_temp);
        _temp = (short)(p1_i * 1000);
        ano_data_buff[_cnt++] = BYTE1(_temp);
        ano_data_buff[_cnt++] = BYTE0(_temp);
        _temp = (short)(p1_d * 1000);
        ano_data_buff[_cnt++] = BYTE1(_temp);
        ano_data_buff[_cnt++] = BYTE0(_temp);
        _temp = (short)(p2_p * 1000);
        ano_data_buff[_cnt++] = BYTE1(_temp);
        ano_data_buff[_cnt++] = BYTE0(_temp);
        _temp = (short)(p2_i * 1000);
        ano_data_buff[_cnt++] = BYTE1(_temp);
        ano_data_buff[_cnt++] = BYTE0(_temp);
        _temp = (short)(p2_d * 1000);
        ano_data_buff[_cnt++] = BYTE1(_temp);
        ano_data_buff[_cnt++] = BYTE0(_temp);
        _temp = (short)(p3_p * 1000);
        ano_data_buff[_cnt++] = BYTE1(_temp);
        ano_data_buff[_cnt++] = BYTE0(_temp);
        _temp = (short)(p3_i * 1000);
        ano_data_buff[_cnt++] = BYTE1(_temp);
        ano_data_buff[_cnt++] = BYTE0(_temp);
        _temp = (short)(p3_d * 1000);
        ano_data_buff[_cnt++] = BYTE1(_temp);
        ano_data_buff[_cnt++] = BYTE0(_temp);

        ano_data_buff[3] = _cnt - 4;

        uint8_t sum = 0;
        for (uint8_t i = 0; i < _cnt; i++)
            sum += ano_data_buff[i];

        ano_data_buff[_cnt++] = sum;

        send_data(ano_data_buff, _cnt);
    }

    void send_ano_8(short data1, short data2, short data3, short data4,
                    short data5, short data6, short data7, short data8)
    {
        uint8_t _cnt = 0;
        ano_data_buff[_cnt++] = 0xAA; //匿名协议帧头  0xAAAA
        ano_data_buff[_cnt++] = 0xAA;
        ano_data_buff[_cnt++] = 0xF1; //使用用户协议帧0xF1
        ano_data_buff[_cnt++] = 16;   //8个short 长度 16个字节

        ano_data_buff[_cnt++] = BYTE1(data1);
        ano_data_buff[_cnt++] = BYTE0(data1);

        ano_data_buff[_cnt++] = BYTE1(data2);
        ano_data_buff[_cnt++] = BYTE0(data2);

        ano_data_buff[_cnt++] = BYTE1(data3);
        ano_data_buff[_cnt++] = BYTE0(data3);

        ano_data_buff[_cnt++] = BYTE1(data4);
        ano_data_buff[_cnt++] = BYTE0(data4);

        ano_data_buff[_cnt++] = BYTE1(data5);
        ano_data_buff[_cnt++] = BYTE0(data5);

        ano_data_buff[_cnt++] = BYTE1(data6);
        ano_data_buff[_cnt++] = BYTE0(data6);

        ano_data_buff[_cnt++] = BYTE1(data7);
        ano_data_buff[_cnt++] = BYTE0(data7);

        ano_data_buff[_cnt++] = BYTE1(data8);
        ano_data_buff[_cnt++] = BYTE0(data8);

        uint8_t sum = 0;
        for (uint8_t i = 0; i < _cnt; i++)
            sum += ano_data_buff[i];
        ano_data_buff[_cnt++] = sum;

        send_data(ano_data_buff, _cnt);
    }
};

#endif