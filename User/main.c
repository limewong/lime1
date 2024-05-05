#include "config.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"

#include "Light.h"
#include "Key.h"
#include "Battery.h"
#include "Buzzer.h"
#include "Ultrasonic.h"
#include "Motors.h"


void GPIO_config(void) {
  GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
  GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO,
  GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
  GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
}

void UART_config(void) {
  // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
  COMx_InitDefine		COMx_InitStructure;					//结构定义
  COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
  COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
  COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
  COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
  COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
  UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

  NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
  UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}


void sys_init() {
  EAXSFR(); // 扩展寄存器使能
  GPIO_config();
  UART_config();
  EA = 1; // 中断总开关
	
	// 初始化硬件驱动
  Light_init();
	Battery_init();
	Buzzer_init();
	Ultrasonic_init();
	Motors_init();
}

void start_main() _task_ 0 {
  // 初始化外设
  sys_init();

  // 创建其他任务
  os_create_task(1);

  // 销毁回收自己
  os_delete_task(0);
}
void Key_on_keyup() {
  
	Light_off(ALL); // 全部熄灭
	
//	Buzzer_close();
//	Motors_stop();
}


int counter = 0;
void Key_on_keydown() {
//	float voltage;
//	char rst;
//	float distance;
  // 全部点亮
	Light_on(ALL);  
	// 读取VCC电压 -----------------------------------
//	voltage = Battery_get_voltage();
//	printf("voltage: %.3f V\n", voltage);
	
	// 蜂鸣器			-----------------------------------
	
	// 触发测距
//	rst = Ultrasonic_get_distance(&distance);
//	if(rst == SUCCESS){ // 0
//		printf("distance: %.3f\n", distance);
//	}else { // -1, 1, 2
//		printf("测距失败: %.3f Rst:%d\n", distance, (int)rst);
//	}

	// 0前进，1后退，2左平移 3右平移，4左掉头，5右掉头
	switch(counter){
		case 0:Motors_forward(30);break;
		case 1:Motors_backward(30);break;
		case 2:Motors_left(40, 0);break;
		case 3:Motors_right(40, 0);break;
		case 4:Motors_around(40, 0);break;
		case 5:Motors_around(40, 1);break;
		default: Motors_stop();break;
	}
	counter++;
	counter %= 6; // 0,1,2,3,4,5
	
	os_wait2(K_TMO, 200);
	Motors_stop();
	
}

void task1() _task_ 1 {
  Key_init();
  while(1) {
    Key_scan();
    os_wait2(K_TMO, 2);// 10ms = 5ms * 2
  }
}