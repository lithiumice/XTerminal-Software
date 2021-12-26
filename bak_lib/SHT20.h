/***************************************************************
*   File name    :  SHT20.h  
*   Description  :  SHT20 Humidity and Temperature Sensors 
*     M C U      :  STC89C52RC
*   Compiler     :  Keil uVision V4.00a (C51)
*   Created by   :  dingshidong
*   Copyright    :  Copyright(c) https://wclh.taobao.com/   
*   Created data :  2015.05.08  
*   Vision       :  V1.0
*****************************************************************/

/*****************Function Declaration*************************/
/*----define to easier to use-----*/
        #define uchar unsigned char         
        #define uint  unsigned int
        #define ulong unsigned long

/*================================================================
��  Name  ��void Delay(uint t)
��Function��delay
��  Notes ��
================================================================*/
void Delay(uint t);

/*================================================================
��  Name  ��void I2CDelay (uchar t)
��Function��ģ��IIC�õĶ���ʱ
��  Notes ��
================================================================*/
void I2CDelay (uchar t);

/*================================================================
��  Name  ��void I2CInit(void)
��Function��I2C��ʼ��������״̬
��  Notes ��
================================================================*/
void I2CInit(void);

/*================================================================
��  Name  ��void I2CStart(void)
��Function��I2C��ʼ�ź�
��  Notes ��SCL��SDAͬΪ�ߣ�SDA����ɵ�֮��SCL����ɵ�
================================================================*/
void I2CStart(void);

/*================================================================
���� �ơ�void I2CStop(void)
���� �ܡ�I2Cֹͣ�ź�
���� ע��SCL��SDAͬΪ�ͣ�SCL����ɸ�֮��SDA����ɸ�
================================================================*/
void I2CStop(void);

/*================================================================
��  Name  ��uchar I2C_Write_Byte(uchar WRByte)
��Function��I2Cдһ���ֽ����ݣ�����ACK����NACK
��  Notes ���Ӹߵ��ͣ����η���
================================================================*/
uchar I2C_Write_Byte(uchar Write_Byte);

/*================================================================
��  Name  ��uchar I2C_Read_Byte(uchar AckValue)
��Function��I2C��һ���ֽ����ݣ���ڲ������ڿ���Ӧ��״̬��ACK����NACK
��  Notes ���Ӹߵ��ͣ����ν���
================================================================*/
uchar I2C_Read_Byte(uchar AckValue);


/*================================================================
��  Name  ��void SoftReset(void)
��Function��SHT20�����λ���������е���
��  Notes ���Ӹߵ��ͣ����ν���
================================================================*/
void SoftReset(void);

/*================================================================
��  Name  ��void SET_Resolution(void)
��Function��д�Ĵ��������÷ֱ���
��  Notes ��
================================================================*/
void SET_Resolution(void);

/*================================================================
��  Name  ��float ReadSht20(char whatdo)
��Function����ȡ��������
��  Notes ��
================================================================*/
float ReadSht20(char whatdo);











