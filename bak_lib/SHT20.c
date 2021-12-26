/***************************************************************
*   File name    :  SHT20.c  
*   Description  :  SHT20 Humidity and Temperature Sensors 
*     M C U      :  STC89C52RC
*   Compiler     :  Keil uVision V4.00a (C51)
*   Created by   :  dingshidong
*   Copyright    :  Copyright(c) https://wclh.taobao.com/   
*   Created data :  2015.05.08  
*   Vision       :  V1.0
*****************************************************************/
#include <STC89.H> //stc89cxx head file
#include "SHT20.H" //sht20 head file 
/*================================================================
��  Name  ��Pin Description
��Function��ģ��IICʹ�õ����Ŷ��壬ע��Ҫ�������裬SDA��SCL������10K������
��  Notes ��
================================================================*/
/*************Pin Description*************
   P2.6------SCK     (SHT20)
   P2.7------DATA    (SHT20)
*****************************************/
sbit SDA=P2^7;                 // define data pin
sbit SCL=P2^6;                 // define clock pin

/*================================================================
��  Name  ��
��Function��SHT20��I2C��ַ
��  Notes ��
================================================================*/
#define SHT20ADDR 0x80
/*================================================================
��  Name  ��
��Function��Ӧ��λ��ƽ����
��  Notes ��
================================================================*/
#define ACK            0            //Ӧ���ź�
#define NACK        1             //��Ӧ���ź�

/*================================================================
��  Name  ��void Delay(uint t)
��Function��delay Nms
��  Notes ��
================================================================*/
void Delay(uint t)
{
        uchar i;
        while(t--)
        {
                for(i=0;i<200;i++);
        }                                           
}
/*------------------------------------------------------------*/
/**********************Simulation I2C Function*****************/
/*------------------------------------------------------------*/

/*================================================================
��  Name  ��void I2CDelay (uchar t)
��Function��ģ��IIC�õĶ���ʱ us
��  Notes ��
================================================================*/
void I2CDelay (uchar t)          
{
        while(t--);
}

/*================================================================
��  Name  ��void I2CInit(void)
��Function��I2C��ʼ��������״̬
��  Notes ��
================================================================*/
void I2CInit(void)
{
        SDA = 1;
        I2CDelay(1); 
        SCL = 1;
        I2CDelay(1);
}

/*================================================================
��  Name  ��void I2CStart(void)
��Function��I2C��ʼ�ź�
��  Notes ��SCL��SDAͬΪ�ߣ�SDA����ɵ�֮��SCL����ɵ�          
================================================================*/
void I2CStart(void)
{        
          SDA = 1;                            //������ʼ�����������ź�--ZLG
          SCL = 1;
          I2CDelay(5);                    //��ʼ��������ʱ�����4.7us,��ʱ--ZLG
          SDA = 0;                            //������ʼ�ź�-ZLG
        I2CDelay(5);                    //��ʼ��������ʱ�����4��s--ZLG
          SCL = 0;                            //ǯסI2C���ߣ�׼�����ͻ��������--ZLG
        I2CDelay(5);         
}

/*================================================================
��  Name  ��void I2CStop(void)
��Function��I2Cֹͣ�ź�
��  Notes ��SCL��SDAͬΪ�ͣ�SCL����ɸ�֮��SDA����ɸ�           
================================================================*/
void I2CStop(void)
{        
          SDA = 0;                        //���ͽ��������������ź�--ZLG
          SCL = 0;
        I2CDelay(5);
          SCL = 1;                        //���ͽ���������ʱ���ź�--ZLG
          I2CDelay(5);                //������������ʱ�����4��s--ZLG
          SDA = 1;                        //����I2C���߽����ź�--ZLG
          I2CDelay(5); 
}

/*================================================================
��  Name  ��uchar I2C_Write_Byte(uchar WRByte)
��Function��I2Cдһ���ֽ����ݣ�����ACK����NACK
��  Notes ���Ӹߵ��ͣ����η���
================================================================*/
uchar I2C_Write_Byte(uchar Write_Byte)         //Sendbyte
{
        uchar i;      
        for(i=0;i<8;i++)              //Ҫ���͵����ݳ���Ϊ8λ--ZLG
        {         
                if(Write_Byte&0x80)              //�жϷ���λ--ZLG
                {
                        SDA = 1;        
                }
                else
                {
                        SDA = 0;
                }
                I2CDelay(1);                
                SCL=1;                                  //���SDA�ȶ�������SCL���������أ��ӻ���⵽��������ݲ���      
                I2CDelay(5);                        //��֤ʱ�Ӹߵ�ƽ���ڴ���4��s--ZLG
                SCL=0;
                I2CDelay(1);
                Write_Byte <<= 1;
        } 
        I2CDelay(4);
        SDA = 1;                                  //8λ��������ͷ������ߣ�׼������Ӧ��λ-ZLG
    SCL = 1;                      //MCU��֪SHT2X���ݷ�����ϣ��ȴ��ӻ���Ӧ���ź�        
        I2CDelay(5);                   
        /*�������ж�I2C���߽���Ӧ��Ӧ���ź���ACK����NACK*/
        if(SDA==1)                                   //SDAΪ�ߣ��յ�NACK
                return NACK;        
        else                                           //SDAΪ�ͣ��յ�ACK                
                return ACK;        
        SCL=0;
        I2CDelay(15);   
}

/*================================================================
��  Name  ��uchar I2C_Read_Byte(uchar AckValue)
��Function��I2C��һ���ֽ����ݣ���ڲ������ڿ���Ӧ��״̬��ACK����NACK
��  Notes ���Ӹߵ��ͣ����ν���
================================================================*/
uchar I2C_Read_Byte(uchar AckValue)//receivebyte
{
        uchar i,RDByte=0;
        SCL=0;                                                  //��ʱ����Ϊ�ͣ�׼����������λ--ZLG
        SDA = 1;                                  //�ͷ�����,��������Ϊ���뷽ʽ--ZLG        
        for (i=0;i<8;i++) 
        {        
                SCL = 1;                          //SCL�ߵ�ƽ�ڼ䣬�ɼ�SDA�źţ�����Ϊ��Ч���� //��ʱ����Ϊ��ʹ��������������Ч--ZLG 
                I2CDelay(2);
                RDByte <<= 1;                  //��λ
                if(SDA==1)                           //������ȡ����
                {
                        RDByte |= 0x01;
                }
                else
                {
                        RDByte &= 0xfe;
                }
                I2CDelay(1);
                SCL = 0;                             //�½��أ��ӻ�������һλֵ
                I2CDelay(6);
        }   
        /*������I2C���߷���Ӧ���ź�ACK����NACK*/
        SDA = AckValue;                      //Ӧ��״̬        
        I2CDelay(3);
        SCL = 1;                         
          I2CDelay(5);                  //ʱ�ӵ͵�ƽ���ڴ���4��s--ZLG
          SCL = 0;                                  //��ʱ���ߣ�ǯסI2C�����Ա��������--ZLG               
          I2CDelay(15);
        //SDA = 1;        
        return RDByte;
}

/*================================================================
��  Name  ��void SoftReset(void)
��Function��SHT20�����λ���������е���
��  Notes ���Ӹߵ��ͣ����ν���
================================================================*/
void SoftReset(void)                    
{
        I2CInit();                       //I2C initialize
        I2CStart();                      //start I2C
        I2C_Write_Byte(SHT20ADDR&0xfe);  //I2C address + write
        I2C_Write_Byte(0xfe);                         //soft reset
        I2CStop();                       //stop I2C
}

/*================================================================
��  Name  ��void SET_Resolution(void)
��Function��д�Ĵ��������÷ֱ���
��  Notes ��
================================================================*/
void SET_Resolution(void)                    
{
  I2CStart();                                                                         //Start I2C 
  if(I2C_Write_Byte(SHT20ADDR&0xfe)==ACK)       //I2C address + write + ACK
  {
   if(I2C_Write_Byte(0xe6)==ACK)                            //д�û��Ĵ���
    {  
     if(I2C_Write_Byte(0x83)==ACK);                            //���÷ֱ���   11bit RH% ����ʱ�䣺12ms(typ.) & 11bit T�� ����ʱ�䣺9ms(typ.) 
    }
  }
   I2CStop();                                   //Stop I2C         
}
 
/*================================================================
��  Name  ��float ReadSht20(char whatdo)
��Function��������ģʽ����ȡ��������
��  Notes ��
================================================================*/
float ReadSht20(char whatdo)
{
        float temp;
        uchar MSB,LSB;
        float Humidity,Temperature;

        SET_Resolution();
        I2CStart();        
        if(I2C_Write_Byte(SHT20ADDR&0xfe)==ACK)                  //I2C address + write + ACK
        {        
            if(I2C_Write_Byte(whatdo)==ACK)                      //Command
                {
                 do
                  {
                        Delay(6);
                        I2CStart();        
                        }while(I2C_Write_Byte(SHT20ADDR|0x01)==NACK);     //I2C address + read        + NACK 

                        MSB = I2C_Read_Byte(ACK);                                              //Data(MSB)
                        LSB = I2C_Read_Byte(ACK);                                              //Data(LSB)
                        I2C_Read_Byte(NACK);                                                          //Checksum  + NACK 
                        I2CStop();                                                                              //Stop I2C
                        LSB &= 0xfc;                                          //Data (LSB) �ĺ���λ�ڽ����������ǰǰ���á�0�� 
                        temp = MSB*256 + LSB;                                                          //ʮ������ת��ʮ����

                        if (whatdo==((char)0xf5))                                          //No Hold Master Mode��read humidity
                        {        
                          /*-- calculate relative humidity [%RH] --*/ 
                                Humidity =(temp*125)/65536-6;                 //��ʽ: RH%= -6 + 125 * SRH/2^16
                                return Humidity;                              //����ֵ��humidity
                        }                                                                                                  
                        else                                                                                      //No Hold Master Mode��read temperature
                        {        
                           /*-- calculate temperature [��C] --*/
                                Temperature = (temp*175.72)/65536-46.85;      //��ʽ:T= -46.85 + 175.72 * ST/2^16
                                return Temperature;                                      //����ֵ��temperature
                        }
                 }
          }
        return 0;
}
