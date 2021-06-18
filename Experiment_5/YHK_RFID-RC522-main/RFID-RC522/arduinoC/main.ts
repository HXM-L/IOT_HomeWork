enum LINE {
    //% block="1"
    1,
    //% block="2"
    2,
    //% block="3"
    3,
    //% block="4"
    4,
    //% block="5"
    5,
    //% block="6"
    6,
    //% block="7"
    7,
    //% block="8"
    8,
    //% block="9"
    9,
    //% block="10"
    10,
    //% block="11"
    11,
    //% block="12"
    12,
    //% block="13"
    13
}

//% color="#33CCFF" iconWidth=50 iconHeight=40
namespace MFRC522 {
    //% block="RFID-RC522模块初始化，SDA引脚[SS] RST引脚[RST]" blockType="command"
    //% SS.shadow="dropdown" SS.options="LINE" SS.defl="LINE.10"
    //% RST.shadow="dropdown" RST.options="LINE" RST.defl="LINE.9"
    export function MFRC522Init(parameter: any, block: any) {
        let ss = parameter.SS.code;
        let rst = parameter.RST.code;
        Generator.addInclude("MFRC522_1","#include <SPI.h>");
        Generator.addInclude("MFRC522_2","#include <MFRC522.h>");
        Generator.addObject("MFRC522_3","MFRC522",`rfid(${ss},${rst});`);
        Generator.addObject("MFRC522_4","MFRC522::MIFARE_Key",`key;`);
        Generator.addObject("MFRC522_5","byte",`nuidPICC[4];`);
        //Generator.addSetup("MFRC522_6",`Serial.begin(9600); `);
        Generator.addSetup("MFRC522_6",`SPI.begin(); `);
        Generator.addSetup("MFRC522_7",`rfid.PCD_Init(); `);
        Generator.addSetup("MFRC522_8",`for (byte i = 0; i < 6; i++) {
            key.keyByte[i] = 0xFF;
        }`);
    }

    //% block="检测新卡号" blockType="command"
    export function MFRC522xk(parameter: any, block: any) {
        Generator.addCode(`if(!rfid.PICC_IsNewCardPresent())
        return;`);
        Generator.addCode(`if(!rfid.PICC_ReadCardSerial())
        return;`);
        Generator.addCode(`for (byte i = 0; i < 4; i++) {
            nuidPICC[i] = rfid.uid.uidByte[i];
        }`); 
        Generator.addCode(`for (byte i = 0; i < 4; i++) {
            Serial.print(nuidPICC[i] < 0x10 ? "0" : "");
            Serial.println(nuidPICC[i], DEC);
          }`);
        Generator.addCode(`rfid.PICC_HaltA();`);
        Generator.addCode(`rfid.PCD_StopCrypto1();`);
        Generator.addCode(`Serial.println();`);
    }
    
}