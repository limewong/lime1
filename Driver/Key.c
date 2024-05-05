#include "Key.h"

static void GPIO_config(u8 gpio, u8 pin) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = pin;		//ָ��Ҫ��ʼ����IO,
	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_InitStructure.Mode = GPIO_PullUp;	
	GPIO_Inilize(gpio, &GPIO_InitStructure);//��ʼ��
}


void Key_init(){
	GPIO_config(GPIO_KEY);
	
	KEY = 1;
}

u8 last_key_state = 1;
void Key_scan(){
	
	// ״̬�����仯��ʱ��
	if(KEY != last_key_state){
		// ��¼���µ�״̬
		last_key_state = KEY;
		// ���ݰ�ť���µĵ�ƽ���ж�̧���ǰ���
		if(KEY){ // 1 �������أ���->�ߣ�̧��
			Key_on_keyup();			
		}else { // 0 ���½��أ���->�ͣ�����
			Key_on_keydown();
		}
	}
}
