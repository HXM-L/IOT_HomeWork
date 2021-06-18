
//% color="#AA278D" iconWidth=50 iconHeight=40
namespace DFRobot_DFR0177_VoiceRecognitionShield {
    //% block="Voice Init" blockType="command"
    export function voiceInit(){
        Generator.addInclude("voiceInclude","#include <VoiceRecognition.h>");
        Generator.addObject("voiceObject","VoiceRecognition","DFR0177_Voice;");
        Generator.addSetup("voiceInit","DFR0177_Voice.init();");
        
    }

    //% block="Voice add Command [CMD][NUM]" blockType="command"
    //% CMD.shadow="String" CMD.defl="ni hao"
    //% NUM.shadow="range" NUM.params.min=0    NUM.params.max=49  NUM.defl="0"
    export function voiceAddCommand(parameter:any){
        let cmd=parameter.CMD.code;
        let num=parameter.NUM.code;
        Generator.addSetup(`voiceAddCommand${cmd}${num}`,`DFR0177_Voice.addCommand(${cmd},${num});`);
        
    }
    //% block="VoicevoiceMaxLength[E]" blockType="command"
    //% E.shadow="range" E.defl="20" E.params.min="0" E.params.max="20"
    export function  VoicevoiceMaxLength(parameter:any,block:any){
        let e=parameter.E.code;
        Generator.addCode(`DFR0177_Voice.speechEndpoint(${e}); `);
        
    }
        //% block="---"
        export function noteSep() {

        }
    //% block="Voice Start" blockType="command"
    export function voiceStart(){
        Generator.addSetup("voiceStart","DFR0177_Voice.start();");
        
    }
    //% block="Voice read" blockType="reporter"
    export function voiceRead(){
        Generator.addCode("DFR0177_Voice.read()");
        
    }

    

    /*
    //% block="VoicemicVol[A]" blockType="command"
    //% A.shadow="range" A.defl="85" A.params.min="10" A.params.max="100"
    export function VoicemicVol(parameter:any,block:any){
        let a=parameter.A.code;
        Generator.addCode(`DFR0177_Voice.micVol(${a}); `);
        
    }
   //% block="VoicespeechEndpoint[B]" blockType="command"
    //% B.shadow="range" B.defl="15" B.params.min="10" B.params.max="50"
    export function VoicespeechEndpoint(parameter:any,block:any){
        let b=parameter.B.code;
        Generator.addCode(`DFR0177_Voice.speechEndpoint(${b}); `);
        
    }

    //% block=" VoicespeechStartTime[C]" blockType="command"
    //% C.shadow="range" C.defl="80" C.params.min="0" C.params.max="500"
    export function  VoicespeechStartTime(parameter:any,block:any){
        let c=parameter.C.code;
        Generator.addCode(`DFR0177_Voice.speechEndpoint(${c}); `);
        
    }
     //% block="  VoicespeechEndTime[D]" blockType="command"
    //% D.shadow="range" D.defl="150" D.params.min="0" D.params.max="2000"
    export function  VoicespeechEndTime(parameter:any,block:any){
        let d=parameter.D.code;
        Generator.addCode(`DFR0177_Voice.speechEndpoint(${d}); `);
        
    }


    //% block="VoicenoiseTime[H]" blockType="command"
    //% H.shadow="range" H.defl="40" H.params.min="0" H.params.max="5100"
    export function  VoicenoiseTime(parameter:any,block:any){
        let h=parameter.H.code;
        Generator.addCode(`DFR0177_Voice.noiseTime(${h});`);
        
    }
    
   //"DFRobot_DFR0177_VoiceRecognitionShield.VoicemicVol|block": "麦克风放大增益[A]",
    //"DFRobot_DFR0177_VoiceRecognitionShield.VoicespeechEndpoint|block": "语音/背噪对比度[B]",
    //"DFRobot_DFR0177_VoiceRecognitionShield.VoicespeechStartTime|block": "背噪/语音辨别时间[C]ms ",
    //"DFRobot_DFR0177_VoiceRecognitionShield.VoicespeechEndTime|block": "语音-背噪辨别时间[D]ms ",
   
    //"DFRobot_DFR0177_VoiceRecognitionShield.VoicenoiseTime|block": "上点噪点忽略时间[H]ms "*/
}
