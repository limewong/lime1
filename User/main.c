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
  GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
  GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO,
  GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
  GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//��ʼ��
}

void UART_config(void) {
  // >>> �ǵ���� NVIC.c, UART.c, UART_Isr.c <<<
  COMx_InitDefine		COMx_InitStructure;					//�ṹ����
  COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
  COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
  COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
  COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
  COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
  UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4

  NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
  UART1_SW(UART1_SW_P30_P31);		// ����ѡ��, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}


void sys_init() {
  EAXSFR(); // ��չ�Ĵ���ʹ��
  GPIO_config();
  UART_config();
  EA = 1; // �ж��ܿ���
	
	// ��ʼ��Ӳ������
  Light_init();
	Battery_init();
	Buzzer_init();
	Ultrasonic_init();
	Motors_init();
}

void start_main() _task_ 0 {
  // ��ʼ������
  sys_init();

  // ������������
  os_create_task(1);

  // ���ٻ����Լ�
  os_delete_task(0);
}
void Key_on_keyup() {
  
	Light_off(ALL); // ȫ��Ϩ��
	
//	Buzzer_close();
//	Motors_stop();
}


int counter = 0;
void Key_on_keydown() {
//	float voltage;
//	char rst;
//	float distance;
  // ȫ������
	Light_on(ALL);  
	// ��ȡVCC��ѹ -----------------------------------
//	voltage = Battery_get_voltage();
//	printf("voltage: %.3f V\n", voltage);
	
	// ������			-----------------------------------
	
	// �������
//	rst = Ultrasonic_get_distance(&distance);
//	if(rst == SUCCESS){ // 0
//		printf("distance: %.3f\n", distance);
//	}else { // -1, 1, 2
//		printf("���ʧ��: %.3f Rst:%d\n", distance, (int)rst);
//	}

	// 0ǰ����1���ˣ�2��ƽ�� 3��ƽ�ƣ�4���ͷ��5�ҵ�ͷ
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