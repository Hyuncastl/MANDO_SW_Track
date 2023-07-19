/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Driver_Can.h"
/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
AppLedType        g_led;                                    /* Global LED configuration and control structure(전역 LED구성 및 제어 구조체)*/
/***********************************************************************/
/*Define*/ 
/***********************************************************************/
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
/***********************************************************************/
/*Static Function Prototype*/ 
/***********************************************************************/
/* Macro to define Interrupt Service Routine.*/
IFX_INTERRUPT(canIsrTxHandler, 0, ISR_PRIORITY_CAN_TX);                             //CAN TX 인터럽트
/***********************************************************************/
/*Variable*/ 
/***********************************************************************/
App_MulticanBasic g_MulticanBasic; /**< \brief Demo information */                  //데모 정보 수집
volatile CanRxMsg rec;                                                              //수신된 CAN 메시지를 저장
int a[8]={0,0x1,0x2,0x3,0x4,0x5,0x6,0x7};                                           //CAN 메시지의 데이터 부분, 수신된 메시지를 이 배열에 저장하여 활용가능
/***********************************************************************/
/*Function*/ 
/***********************************************************************/
void Driver_Can_Init(void)
{
    /* create module config */
    IfxMultican_Can_Config canConfig;
    IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);

    /* Interrupt configuration*/

    canConfig.nodePointer[TX_INTERRUPT_SRC_ID].priority = ISR_PRIORITY_CAN_TX;  //CAN 통신에서 전송 인터럽트 소스 ID에 해당하는 노드의 우선순위를 설정

    /* initialize module */
    IfxMultican_Can_initModule(&g_MulticanBasic.drivers.can, &canConfig);       //CAN Module 초기화

    /* create CAN node config */
    IfxMultican_Can_NodeConfig canNodeConfig;
    IfxMultican_Can_Node_initConfig(&canNodeConfig, &g_MulticanBasic.drivers.can);

    canNodeConfig.baudrate = 500000UL;                                                                      // CAN 속도 설정 500kbps
    {
        canNodeConfig.nodeId    = IfxMultican_NodeId_0;//(IfxMultican_NodeId)((int)IfxMultican_NodeId_0);   // CAN의 Node ID 설정  0번으로 설정
        canNodeConfig.rxPin     = &IfxMultican_RXD0B_P20_7_IN;                                              // 입력핀 설정
        canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
        canNodeConfig.txPin     = &IfxMultican_TXD0_P20_8_OUT;                                              // 출력핀 설정
        canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

        IfxMultican_Can_Node_init(&g_MulticanBasic.drivers.canSrcNode, &canNodeConfig);                     // CAN node 초기화
    }

    /* Create message object config */
    IfxMultican_Can_MsgObjConfig canMsgObjConfig;                                                           // CAN message object configuration
    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_MulticanBasic.drivers.canSrcNode);

    canMsgObjConfig.msgObjId              = 0;                                                              //256개의 message object가 있음
    canMsgObjConfig.messageId             = 0x100;
    canMsgObjConfig.acceptanceMask        = 0x7FFFFFFFUL;
    canMsgObjConfig.frame                 = IfxMultican_Frame_transmit;                                     // CAN TX설정
    canMsgObjConfig.control.messageLen    = IfxMultican_DataLengthCode_8;                                   // Data 길이 8
    canMsgObjConfig.control.extendedFrame = FALSE;                                                          // Extended ID 사용 안함
    canMsgObjConfig.control.matchingId    = TRUE;

    canMsgObjConfig.txInterrupt.enabled = TRUE;
    canMsgObjConfig.txInterrupt.srcId = TX_INTERRUPT_SRC_ID;

    /* initialize message object */
    IfxMultican_Can_MsgObj_init(&g_MulticanBasic.drivers.canSrcMsgObj, &canMsgObjConfig);

    /* IO Port */
    //IfxPort_setPinMode(port, pinIndex, (IfxPort_Mode)(index | mode));
    IfxPort_setPinModeOutput(STB, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    /* Set STB Pin of CAN chip (low-level active) */
    IfxPort_setPinLow(STB);                                                                                 // STB pin Low로 설정
}

void Driver_Can_TxTest(void)
{
    const uint32 dataLow  = 0x12340000;
    const uint32 dataHigh = 0x9abc0000;

    /* Transmit Data */
    {
        IfxMultican_Message msg;                                                                                //msg를 초기화
        IfxMultican_Message_init(&msg, 0x100, dataLow, dataHigh, IfxMultican_DataLengthCode_8);                 //(초기화할 메시지 객체의 주소,ID,dataLow,dataHigh, 메시지의 데이터 길이 8바이트)
        while (IfxMultican_Can_MsgObj_sendMessage(&g_MulticanBasic.drivers.canSrcMsgObj, &msg) == IfxMultican_Status_notSentBusy)  // 메시지를 전송할 수 있는 상태인지 확인, 전송이 완료될 때까지 반복
        {}
    }
}

void CAN_send(CanRxMsg *message)
{


    IfxMultican_Message msg;

    const unsigned dataLow = message->Data[0]|(message->Data[1]<<8)|(message->Data[2]<<16)|(message->Data[3]<<24);
    const unsigned dataHigh = message->Data[4]|(message->Data[5]<<8)|(message->Data[6]<<16)|(message->Data[7]<<24);


    IfxMultican_Message_init(&msg,message->ID,dataLow,dataHigh,message->DLC);
                            //(초기화할 메시지 객체의 주소,ID,dataLow,dataHigh,message의 DLC 멤버)
    while (IfxMultican_Can_MsgObj_sendMessage(&g_MulticanBasic.drivers.canSrcMsgObj, &msg) == IfxMultican_Status_notSentBusy) // 메시지를 전송할 수 있는 상태인지 확인, 전송이 완료될 때까지 반복
    {

    }
}

void CAN_TEST(void)
{
    CanRxMsg MES;                                               //CAN 수신 메시지를 나타내는 구조체
    int i=0;
    MES.ID=0x890;                                               // MES의 ID 멤버에 값 0x890
    MES.IDE=0;                                                  // MES의 IDE 멤버에 값 0
    MES.DLC=8;                                                  // MES의 DLC 멤버에 값 8
    for(i=0; i<8; i++)
    {
        MES.Data[i]=a[i];                                        // MES.Data 배열에 a 배열의 요소들을 복사
    }
    CAN_send(&MES);                                              // MES 변수의 주소를 전달, CAN 메시지 전송
}

/* Interrupt Service Routine (ISR) called once the TX interrupt has been generated.
 * Turns on the LED1 to indicate successful CAN message transmission.
 */
void canIsrTxHandler(void)
{
    /* Just to indicate that the CAN message has been transmitted by turning on LED1 */
    blinkLED1();

    //IfxPort_setPinLow(g_led.led1.port, g_led.led1.pinIndex);
}


void initLed(void)
{
    /* ======================================================================
     * Configuration of the pins connected to the LEDs:
     * ======================================================================
     *  - define the GPIO port
     *  - define the GPIO pin that is the connected to the LED
     *  - define the general GPIO pin usage (no alternate function used)
     *  - define the pad driver strength
     * ======================================================================
     */
    g_led.led1.port      = &MODULE_P00;
    g_led.led1.pinIndex  = PIN5;
    g_led.led1.mode      = IfxPort_OutputIdx_general;                    //출력 모드로 설정
    g_led.led1.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;       //패드 드라이버의 속도 등급을 나타냄

    g_led.led2.port      = &MODULE_P00;
    g_led.led2.pinIndex  = PIN6;
    g_led.led2.mode      = IfxPort_OutputIdx_general;                    //출력 모드로 설정
    g_led.led2.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;       //패드 드라이버의 속도 등급을 나타냄

    /* Initialize the pins connected to LEDs to level "HIGH"; will keep the LEDs turned off as default state */

    IfxPort_setPinHigh(g_led.led1.port, g_led.led1.pinIndex);           //IfxPort_setPinState(port, pinIndex, IfxPort_State_high)
    IfxPort_setPinHigh(g_led.led2.port, g_led.led2.pinIndex);

    /* Set the pin input/output mode for both pins connected to the LEDs */

    IfxPort_setPinModeOutput(g_led.led1.port, g_led.led1.pinIndex, IfxPort_OutputMode_pushPull, g_led.led1.mode);  //IfxPort_setPinMode(port, pinIndex, (IfxPort_Mode)(index | mode))
    IfxPort_setPinModeOutput(g_led.led2.port, g_led.led2.pinIndex, IfxPort_OutputMode_pushPull, g_led.led2.mode);  //pin을 output으로 설정.

    /* Set the pad driver mode for both pins connected to the LEDs */

    IfxPort_setPinPadDriver(g_led.led1.port, g_led.led1.pinIndex, g_led.led1.padDriver); //IfxPort_setPinPadDriver(Ifx_P *port, uint8 pinIndex, IfxPort_PadDriver padDriver)
    IfxPort_setPinPadDriver(g_led.led2.port, g_led.led2.pinIndex, g_led.led2.padDriver);  //Pad Driver pin 설정
}

void blinkLED1(void)                                                                //timer interrupt
{
    //IfxPort_togglePin(LED1);                                                      /* Toggle the state of the LED1 */
    IfxPort_setPinHigh(LED1);                                                       //LED1 pin high 설정
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME_1ms));    /* Wait 1 milliseconds */
    IfxPort_setPinLow(LED1);                                                        //LED1 pin low 설정
}
void blinkLED2(void)                                                                //LED blink 함수
{
    IfxPort_togglePin(LED2);                                                         /* Toggle the state of the LED2 */
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME_10ms));    /* Wait 10 milliseconds */
}
