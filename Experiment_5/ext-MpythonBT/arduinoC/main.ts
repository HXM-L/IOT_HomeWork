


//% color="#AA278D" iconWidth=50 iconHeight=40
namespace MpythonBT {


    //% block="MpythonBT init name [NAME]" blockType="command"
    //% NAME.shadow="string"  NAME.defl="ESP32BT"
    export function MpythonBTInit(parameter: any, block: any) {
        let name=parameter.NAME.code;
        console.log(name);
        Generator.addInclude('MpythonBTInitinclude', '#include "BluetoothSerial.h"');
        Generator.addObject("MpythonBTInitobject", "BluetoothSerial", "SerialBT");
        Generator.addSetup("MpythonBTInitsetup", `SerialBT.begin(${name});`);

    }

    //% block="MpythonBT available" blockType="boolean"
    export function MpythonBTAvailable(parameter: any, block: any) {

        Generator.addCode(["SerialBT.available()",Generator.ORDER_UNARY_POSTFIX]);
    }

     //% block="MpythonBT read " blockType="reporter"
     export function MpythonBTRead(parameter: any, block: any) {

        Generator.addCode(["SerialBT.read()",Generator.ORDER_UNARY_POSTFIX]);
    }
     //% block="MpythonBT write [STR]" blockType="command"
     //% STR.shadow="number" STR.defl="1"
     export function MpythonBTWrite(parameter: any, block: any) {
            let str = parameter.STR.code;
        Generator.addCode(`SerialBT.write(${str});`);
    }

}
