
enum SIZE {
    //% block="29*29"
    1,
    //% block="58*58"
    2
}

enum LINE {
    //% block="1"
    1,
    //% block="2"
    2,
    //% block="3"
    3,
    //% block="4"
    4
}

enum BTN {
    //% block="A"
    A,
    //% block="B"
    B,
    //% block="A+B"
    AB
}


//% color="#9900CC" iconWidth=50 iconHeight=40
namespace oled12864 {
     //% block="username：[SS] password: [SN]  ipaddress:[sp1],[sp2],[sp3],[sp4] 数据库名称:[sdb]" blockType="command"
    //% SS.shadow="string"   SS.defl="root"
    //% SN.shadow="string"   SN.defl="root" 
    //% sp1.shadow="number"   sp1.defl="192"
    //% sp2.shadow="number"   sp2.defl="168"
    //% sp3.shadow="number"   sp3.defl="226"
    //% sp4.shadow="number"   sp4.defl="207"
    //% sdb.shadow="string"   sdb.defl="experience_5",
    export function InitMySQL(parameter: any, block: any) {  
         /* 方形积木,数据库连接参数初始化 */  
        let username=parameter.SS.code;
        let password=parameter.SN.code;
        let ip1=parameter.sp1.code;
        let ip2=parameter.sp2.code;
        let ip3=parameter.sp3.code;
        let ip4=parameter.sp4.code;
        Generator.addInclude('Mylibraray', '#include <MySQL_Connection.h>');
        // Generator.addInclude('Mylibraray', '#include <Ethernet.h>');
        Generator.addObject(`MACAddress`, `byte`, `mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };`);
        Generator.addObject(`IPAddress`, `IPAddress`, `server_addr(${ip1},${ip2},${ip3},${ip4});`);
        Generator.addObject(`char1`, `char`, `user[] = ${username};`);
        Generator.addObject(`char2`, `char`, `password[] = ${password};`);
        Generator.addObject(`EthernetClient`, `espClient`, `client;`);
        Generator.addObject(`MySQL_Connection`, `MySQL_Connection`, `conn((Client *)&client);`);
        Generator.addSetup("MySQLsetup", `Serial.begin(115200);`);
        Generator.addSetup("MySQLsetup1", `while (!Serial);`);
        Generator.addSetup("MySQLsetup1", `Serial.println("Connecting...");`);
    }


    //% block="connect MYSQL" blockType="command"
    export function ConnectMySQL(parameter: any, block: any) {

    }

     //% block="connect database success?" blockType="boolean"
    export function ConnectFlag(parameter: any, block: any) {
        /* 菱形积木返回数据库连接是否成功 */
        Generator.addCode([`conn.connect(server_addr, 3306, user, password)`,Generator.ORDER_UNARY_POSTFIX]);
    }

     //% block="table name:[st]" blockType="command"
    //% st.shadow="string"   SS.defl="userinfo"
    export function SelectMySQL(parameter: any, block: any) {
        

    }
   
}
