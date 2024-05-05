#include "Light.h"


void Light_init(){
	LIGHT_GPIO_INIT();
	
	// Ĭ��ȫ��Ϩ��
	Light_off(ALL);
}


// ����ָ���ĵ�
void Light_on(Light light){
	switch(light){
		case ALL:			
			LED_L = 1;
			LED_R	= 1;
			break;
		case LEFT:			
			LED_L = 1;
			break;
		case RIGHT:			
			LED_R	= 1;
			break;
		default:
			break;
	}
}

// Ϩ��ָ���ĵ�
void Light_off(Light light){
	switch(light){
		case ALL:			
			LED_L = 0;
			LED_R	= 0;
			break;
		case LEFT:			
			LED_L = 0;
			break;
		case RIGHT:			
			LED_R	= 0;
			break;
		default:
			break;
	}

}