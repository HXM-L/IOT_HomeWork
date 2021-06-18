/*===========================================================

VoiceRecognition.cpp��������ʶ����ļ���

�Ϻ���λ���������޹�˾ ��Ȩ����
http://www.dfrobot.com.cn

2014-8-11

=============================================================
*/
#include "PinMap.h"
#include "VoiceRecognition.h"
#include "Arduino.h"

uint8_t g_Mic;
int RSTB=9;//RSTB���Ŷ���
int CS=4;//RSTB���Ŷ���
uint8_t MIC_VOL=0x55;//ADC�����ʼֵ
uint8_t speech_endpoint=0x10;//�����˵����ʼֵ
uint8_t speech_start_time=0x08;//�����˵��⿪ʼʱ���ʼֵ
uint8_t speech_end_time=0x10;//�����˵������ʱ���ʼֵ
uint8_t voice_max_length=0xC3;//�������ʱ�䣬Ĭ��20��
uint8_t noise_time=0x02;//�����ϵ�����ʱ��
//uint8_t ASR_time�ʱ��
int readflag=0;
int readnum=0;

VoiceRecognition::VoiceRecognition(){}

int VoiceRecognition::read()//ʶ������ȡ
{
	if(readflag==1)
	{	
		readflag=0;
		return readnum;
	}
	return -1;
}
void update()//�жϷ�����
{ 
	uint8_t Asr_Count=0;
	if((readReg(0x2b) & 0x10) && readReg(0xb2)==0x21 && readReg(0xbf)==0x35)//���������ʶ���жϡ�DSP�С�ASR��������
	{
		writeReg(0x29,0) ;///////////���ж�
		writeReg(0x02,0) ;/////////////��FIFO�ж�
		Asr_Count = readReg(0xba);//���жϸ�����Ϣ
		if(Asr_Count>0 && Asr_Count<4) //////�����ʶ����
		{
			readnum=readReg(0xc5);
			readflag=1;
		}	
		writeReg(0x2b,0);//////����жϱ��
		writeReg(0x1C,0);////////ò�ƹ���˷簡~~Ϊë
	}
	readReg(0x06);  
	delay(10);
	readReg(0x06);  
	writeReg(0x89, 0x03);  
	delay(5);
	writeReg(0xcf, 0x43);  
	delay(5);
	writeReg(0xcb, 0x02);
	writeReg(0x11, PLL_11);  
	writeReg(0x1e,0x00);
	writeReg(0x19, PLL_ASR_19); 
	writeReg(0x1b, PLL_ASR_1B);	
	writeReg(0x1d, PLL_ASR_1D);
	delay(10);
	writeReg(0xcd, 0x04);
	writeReg(0x17, 0x4c); 
	delay(5);
	writeReg(0xcf, 0x4f);  
	writeReg(0xbd, 0x00);
	writeReg(0x17, 0x48);
	delay(10);
	writeReg(0x3c, 0x80);  
	writeReg(0x3e, 0x07);
	writeReg(0x38, 0xff);  
	writeReg(0x3a, 0x07);
	writeReg(0x40, 0);   
	writeReg(0x42, 8);
	writeReg(0x44, 0); 
	writeReg(0x46, 8); 
	delay(1);	
	writeReg(0x1c, 0x09);////////��˷����ñ���
	writeReg(0xbd, 0x20);/////////��������
	writeReg(0x08, 0x01);///////////�����FIFO_DATA
	delay( 1);
	writeReg(0x08, 0x00);////////////���ָ��FIFO����д��һ��00H
	delay( 1);
	writeReg(0xb2, 0xff);////////��0xB2дFF
	writeReg(0x37, 0x06);////////��ʼʶ��
	delay( 5 );
	writeReg(0x1c, g_Mic);////////ѡ����˷�
	writeReg(0x29, 0x10);////////��ͬ���ж�
	writeReg(0xbd, 0x00);/////////����Ϊ����ʶ��
}	
void cSHigh() {//CS����
  digitalWrite(CS, HIGH);
}
void cSLow() {//CS������
  digitalWrite(CS, LOW);
}
void writeReg(unsigned char address,unsigned char value)////////д�Ĵ������������Ĵ�����ַ�����ݣ�
{	
	cSLow();////����CS
	delay(10);
	transfer(0x04);////////////дָ��
	transfer(address);
	transfer(value);
	cSHigh();////����CS
 }

unsigned char readReg(unsigned char address)///���Ĵ������������Ĵ�����ַ��
{ 
	unsigned char result;
	cSLow();////����CS
	delay(10);
	transfer(0x05);///////////��ָ��
	transfer(address);
	result=transfer(0x00);
	cSHigh();///����CS
	return(result);
 }
byte transfer(byte _data) /////////////////SPI���ݽ���
{
	SPDR = _data;
	while (!(SPSR & _BV(SPIF)));
	return SPDR;
}

void VoiceRecognition::init(uint8_t mic)////////ģ�����ã�����Ϊ��˷�ѡ��MIC/MONO����˿ӡ����,��SETUP�е���
{
	if(mic==MIC)
	{
		g_Mic=MIC;
	}
	else if(mic==MONO)
	{
		g_Mic=MONO;
	}
	pinMode(RSTB,OUTPUT);
	pinMode(CS, OUTPUT);
	cSHigh();

	pinMode(SPI_MISO_PIN, INPUT);
	pinMode(SPI_MOSI_PIN, OUTPUT);
	pinMode(SPI_SCK_PIN, OUTPUT);

	#ifndef SOFTWARE_SPI
	  // SS must be in output mode even it is not chip select
	  pinMode(LD_CHIP_SELECT_PIN, OUTPUT);
	  digitalWrite(LD_CHIP_SELECT_PIN, HIGH); // disable any SPI device using hardware SS ����ss
	  // Enable SPI, Master, clock rate f_osc/128
	  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);/////��ʼ��SPI�Ĵ���
	  // clear double speed
	  SPSR &= ~(1 << SPI2X);//2����
	#endif  // SOFTWARE_SPI	

	SPCR = (SPCR & ~SPI_MODE_MASK) | 0x08;//����SCK��̬��ƽ��ȡ��ʱ�䣬0x08ΪSCK��̬Ϊ�ߵ�ƽ���½�����Ч
	reset();//LD3320��λ����
	
	#if defined(__AVR_ATmega32U4__)
		attachInterrupt(1,update,LOW);//���ж�	
	#else
		attachInterrupt(0,update,LOW);//���ж�	
	#endif
	
	ASR_init();///����ʶ���ʼ������	
}

void VoiceRecognition::reset()//LD3320��λ����
{
  digitalWrite(RSTB,HIGH);
  delay(1);
  digitalWrite(RSTB,LOW);
  delay(1);
  digitalWrite(RSTB,HIGH);
  delay(1);
  cSLow();
  delay(1);
  cSHigh();
  delay(1);  
  writeReg(0xb9, 0x00);
}

void VoiceRecognition::ASR_init()////////////��ʼ������ʶ��ģʽ��
{
	  //����״̬��� 
	  readReg(0x06);  
//	  writeReg(0x17, 0x35); 
	  delay(10);
	  readReg(0x06);  
	  writeReg(0x89, 0x03);  
	  delay(5);
	  writeReg(0xcf, 0x43);  
	  delay(5);
	  writeReg(0xcb, 0x02);
	  writeReg(0x11, PLL_11);  
	  writeReg(0x1e,0x00);
	  writeReg(0x19, PLL_ASR_19); 
	  writeReg(0x1b, PLL_ASR_1B);	
	  writeReg(0x1d, PLL_ASR_1D);
	  delay(10);
	  writeReg(0xcd, 0x04);
	  writeReg(0x17, 0x4c); 
	  delay(5);
//	  writeReg(0xb9, 0x00);
	  writeReg(0xcf, 0x4f);  
	  writeReg(0xbd, 0x00);
	  writeReg(0x17, 0x48);
	  delay(10);
	  writeReg(0x3c, 0x80);  
	  writeReg(0x3e, 0x07);
	  writeReg(0x38, 0xff);  
	  writeReg(0x3a, 0x07);
	  writeReg(0x40, 0);   
	  writeReg(0x42, 8);
	  writeReg(0x44, 0); 
	  writeReg(0x46, 8); 
	  delay(1);
}
void VoiceRecognition::addCommand(char *pass,int num)
{

	int i;
		writeReg(0xc1, num);//�ַ����
		writeReg(0xc3, 0 );//����ʱ����00
		writeReg(0x08, 0x04);//�����
  
		delay(1);
		writeReg(0x08, 0x00);//
		delay(1);
	for(i=0;i<=80;i++)
	{
		if (pass[i] == 0)
			break;
		writeReg(0x5, pass[i]);///д��FIFO_EXT
	}
	writeReg(0xb9, i);//д�뵱ǰ�����ַ�������
	writeReg(0xb2, 0xff);//////////B2ȫдff 
	writeReg(0x37, 0x04);//�������
}
unsigned char VoiceRecognition::start()//////��ʼʶ��
{
	writeReg(0x35, MIC_VOL);////adc���棻��Ӱ��ʶ��Χ������
	
	writeReg(0xb3, speech_endpoint);//�����˵������

	writeReg(0xb4, speech_start_time);//�����˵���ʼʱ��
	
	writeReg(0xb5, speech_end_time);//��������ʱ��

	writeReg(0xb6, voice_max_length);//��������ʱ��
	
	writeReg(0xb7, noise_time);//����ʱ��
	
	writeReg(0x1c, 0x09);////////��˷����ñ���

	writeReg(0xbd, 0x20);/////////��������
	writeReg(0x08, 0x01);///////////�����FIFO_DATA
	delay( 1);
	writeReg(0x08, 0x00);////////////���ָ��FIFO����д��һ��00H
	delay( 1);
	if(check_b2() == 0)////////��ȡ0xB2�Ĵ����������DSPû����״̬��RETURN 0
	{
		return 0;
	}
	writeReg(0xb2, 0xff);////////��0xB2дFF

	writeReg(0x37, 0x06);////////��ʼʶ��
	delay( 5 );
	writeReg(0x1c, g_Mic);////////ѡ����˷�
	writeReg(0x29, 0x10);////////��ͬ���ж�
	writeReg(0xbd, 0x00);/////////����Ϊ����ʶ��

	return 1;////����1
}

int check_b2()////////�������оƬ�����Ƿ�����������DSP�Ƿ�æ�������û�����������/�з���1
{ 
  for (int j=0; j<10; j++)
	{
	  if (readReg(0xb2) == 0x21)
		{
			return 1;
		}
	  delay(10);		
	}
  return 0;
}

void VoiceRecognition::micVol(uint8_t vol)//����ADC���棬������0x00~0xFF,����10-60����
{
	MIC_VOL=vol;
	writeReg(0x35, MIC_VOL);////adc���棻��Ӱ��ʶ��Χ������
}
void VoiceRecognition::speechEndpoint(uint8_t speech_endpoint_)//���������˵��⣬������0x00~0xFF,����10-40����
{
	speech_endpoint=speech_endpoint_;
	writeReg(0xb3, speech_endpoint);//�����˵������
}

void VoiceRecognition::speechStartTime(uint8_t speech_start_time_)//���������˵���ʼʱ�䣬������0x00~0x30,��λ10MS����
{
	speech_start_time=speech_start_time_;
	writeReg(0xb4, speech_start_time);//�����˵���ʼʱ��
}
void VoiceRecognition::speechEndTime(uint8_t speech_end_time_)//���������˵����ʱ�䣨���ּ��ʱ�䣩��������0x00~0xC3,��λ10MS����
{
	speech_end_time=speech_end_time_;
	writeReg(0xb5, speech_end_time);//��������ʱ��
}
void VoiceRecognition::voiceMaxLength(uint8_t voice_max_length_)//�������ʱ�䣬������0x00~0xC3,��λ100MS����
{
	voice_max_length=voice_max_length_;
	writeReg(0xb6, voice_max_length);//����
}
void VoiceRecognition::noiseTime(uint8_t noise_time_)//�ϵ������Թ���������0x00~0xff,��λ20MS����
{
	noise_time=noise_time_;
	writeReg(0xb7, noise_time);//����ʱ��
}
