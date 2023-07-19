#ifndef DRIVER_CAN
#define DRIVER_CAN

/***********************************************************************/
/*Include*/ 
/***********************************************************************/
#include "Ifx_Types.h"              //마이크로컨트롤러의 데이터 타입 정의를 포함
#include "IfxCpu.h"                 //인터럽트 관련 함수
#include "IfxMultican_Can.h"        //CAN 관련 함수
#include "IfxPort.h"                //Port 관련 함수
#include "Bsp.h"                    //BSP에 필요한 함수


/***********************************************************************/
/*Define*/ 
/***********************************************************************/

#define STB         &MODULE_P20,6                               //STB 모듈 PIN 설정

#define TX_INTERRUPT_SRC_ID         IfxMultican_SrcId_0         /* Transmit interrupt service request ID (전송 인터럽트 요청 ID 제공) */
#define ISR_PRIORITY_CAN_TX         2                           /* Define the CAN TX interrupt priority (CAN TX 인터럽트 우선순위 정의) */
#define PIN5                        5                           /* LED1 used in TX ISR is connected to this pin (LED1 해당 핀에 연결) */
#define PIN6                        6                           /* LED2 used in RX ISR is connected to this pin (LED2 해당 핀에 연결) */


#define WAIT_TIME_1ms               1                           //WAIT_TIME_1ms 정의
#define WAIT_TIME_5ms               5                           //WAIT_TIME_5ms 정의
#define WAIT_TIME_10ms              10                          //WAIT_TIME_10ms 정의
#define WAIT_TIME_20ms              20                          //WAIT_TIME_20ms 정의
#define WAIT_TIME_50ms              50                          //WAIT_TIME_50ms 정의
#define WAIT_TIME_100ms             100                         //WAIT_TIME_100ms 정의

#define LED1         &MODULE_P00,5
#define LED2         &MODULE_P00,6
/***********************************************************************/
/*Typedef*/ 
/***********************************************************************/
typedef struct
{
    struct
    {
        IfxMultican_Can        can;                            /**< \brief CAN driver handle (CAN 모듈과 연결된 기능을 제어하고 데이터를 송수신하는 역할을 수행) */
        IfxMultican_Can_Node   canSrcNode;                    /**< \brief CAN Source Node (CAN source node는 CAN 통신에서 데이터를 송신하는 노드를 나타냄)*/
        IfxMultican_Can_MsgObj canSrcMsgObj;                 /**< \brief CAN Source Message object(CAN 통신에서 데이터 송신에 사용되는 메시지 객체를 나타냄) */
    }drivers;
} App_MulticanBasic;                                        //App_MulticanBasic 구조체를 정의
// 구조체의 맴버 = can, canSrcNode, canSrcMsgObj

typedef struct
{
    unsigned long ID;                                        //CAN 메시지의 식별자
    unsigned char IDE;                                       //CAN 메시지의 확장 데이터 비트
    unsigned char DLC;                                       //CAN 메시지의 데이터 길이
    unsigned char Data[8];                                   //CAN 메시지의 데이터
} CanRxMsg;

typedef struct
{
    IfxPort_Pin_Config              led1;                  /* LED1 configuration structure (LED1 구성 구조)  */
    IfxPort_Pin_Config              led2;                  /* LED2 configuration structure (LED2 구성 구조)  */
} AppLedType;                                               //AppLedType 구조체를 정의


/***********************************************************************/
/*External Variable*/ 
/***********************************************************************/
IFX_EXTERN App_MulticanBasic g_MulticanBasic;


/***********************************************************************/
/*Global Function Prototype*/ 
/***********************************************************************/

//extern void 함수 - extern으로 다른 소스 파일의 전역 변수 사용 가능.
extern void Driver_Can_Init(void);                          // CAN 초기화 함수 선언
extern void Driver_Can_TxTest(void);                        // CAN 송신 테스트 함수 선언
extern void CAN_send(CanRxMsg *message);                    // CAN 메시지 송신 함수 선언
extern void CAN_TEST(void);                                 // CAN 테스트 함수 선언
void initLed(void);                                         // LED 초기화 함수 선언
void blinkLED1(void);                                       // LED1 함수 선언
void blinkLED2(void);                                       // LED2 함수 선언
#endif /* DRIVER_STM */
