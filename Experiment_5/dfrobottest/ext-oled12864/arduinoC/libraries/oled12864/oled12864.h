#ifndef _OLED12864_H
#define _OLED12864_H

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "Wire.h"
#include "qrcode.h"
#include "DFRobot_SSD1306.h"


#define SSD1306_I2CBEGIN()        Wire.setClock(400000);


class OLED_12864 : public DFRobot_SSD1306
{
public:

    OLED_12864();

    ~OLED_12864();

    void qrcode(uint32_t x, uint32_t y, const String& str, uint8_t scale=2);

    void setLineWidth(uint32_t w = 1);

    uint32_t getLineWidth();

protected:

private:
    void rect(int32_t x, int32_t y, int32_t width, int32_t height, bool fill = false);
    /**
      * @brief Set screen cache refresh range
      *
      * @param address: 8-bit I2C slave address [ addr | 0 ]
      *
      * @param reg: The address of the register in the device to write
      *
      * @param columnStart: Column start position
      *
      * @param columnEnd: End of column
      *
      * @param rowStart: Line start position
      *
      * @param rowEnd: End of line
      */
    void setRefreshRange(int address, uint8_t reg, int columnStart, int columnEnd, int rowStart, int rowEnd)
	{
		#if defined(ESP_PLATFORM)
		// send a bunch of data in one xmission
		writeByte(address, reg, 0xB0 + rowStart);//set page address
		writeByte(address, reg, (2+columnStart) & 0xf);//set lower column address
		writeByte(address, reg, 0x10 | ((2+columnStart) >> 4));//set higher column address
		#elif defined(NRF5)
		// send a bunch of data in one xmission
		writeByte(address, reg, SSD1306_COLUMNADDR);
		writeByte(address, reg, columnStart);
		writeByte(address, reg, columnEnd);
		writeByte(address, reg, SSD1306_PAGEADDR);
		writeByte(address, reg, rowStart);
		writeByte(address, reg, rowEnd);
		#else
		#warning "This library only supports mpython and microbit"
		#endif
	}

    /**
      * @brief Write data to the register address in batches
      *
      * @param address: 8-bit I2C slave address [ addr | 0 ]
      *
      * @param reg: The address of the register in the device to write
      *
      * @param data: A pointer to a byte buffer containing the data to write
      *
      * @param length: The number of bytes to read
      *
      * @return Return the number of data
      */
    int writeBuffer(int address, uint8_t reg, unsigned char *data, int length)
	{
		#if defined(ESP_PLATFORM)
		int count=0;
		SSD1306_I2CBEGIN();
		Wire.beginTransmission(address);
		Wire.write(reg);
		while(count<length) {
			Wire.write(*data);
			data++;
			count++;
		}
		Wire.endTransmission();
		return count;
		#elif defined(NRF5)
		int count=0;
		SSD1306_I2CBEGIN();
		Wire.beginTransmission(address);
		Wire.write(reg);
		while(count<length) {
			Wire.write(*data);
			data++;
			count++;
		}
		Wire.endTransmission();
		return count;
		#else
		#warning "This library only supports mpython and microbit"
		#endif
	}

    /**
      * @brief Write a data to the register address
      *
      * @param address: 8-bit I2C slave address [ addr | 0 ]
      *
      * @param reg: The address of the register in the device to write.
      *
      * @param Data: to be written
      *
      * @return Return the number of data
      */
    int writeByte(uint8_t address, uint8_t reg, uint8_t data)
	{
		#if defined(ESP_PLATFORM)
		SSD1306_I2CBEGIN();
		Wire.beginTransmission(address);
		Wire.write(reg);
		Wire.write(data);
		Wire.endTransmission();
		return 1;
		#elif defined(NRF5)
		SSD1306_I2CBEGIN();
		Wire.beginTransmission(address);
		Wire.write(reg);
		Wire.write(data);
		Wire.endTransmission();
		return 1;
		#else
		#warning "This library only supports mpython and microbit"
		#endif
	}
};

#endif // _OLED12864_H
