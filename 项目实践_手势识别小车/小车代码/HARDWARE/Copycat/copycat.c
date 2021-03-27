#include "copycat.h"

static struct Copycat_Pid pid;							//��¼PID����

void Copycat(void){										//��ƨ��ģ��
	extern u8 ex_mode;
	int speed = 0;
	Copycat_PID_Reset();								//PID��������
	while(ex_mode == MODE_OFF){
		Ultrasonic_Work();
		if(Capture_Register_Empyt()){					//����һ�θߵ�ƽ
			speed = Copycat_PID();						//����PID����
			Capture_Register_Reset();					//��������
		}
		if(speed >= 0){									//�����ƾ���ҪС����Ҫǰ��
			if(speed > Copycat_Speed1)
				speed = Copycat_Speed1;
			else if(speed < Copycat_Speed2)
				speed = 0;
			Car_Turn_Speed_pwm(BACK, speed, 0);
		}
		else{											//�����ƾ���Ҫ����Ҫ����
			if(speed < -Copycat_Speed1)
				speed = -Copycat_Speed1;
			else if(speed > -Copycat_Speed2)
				speed = 0;
			Car_Turn_Speed_pwm(STRAIGHT, -speed, 0);
		}
		delay_ms(10);
		//printf("%d\r\n", speed);
		//printf("%d\t%d\t%d\t%d\r\n", speed, pid.last_error, pid.prev_error, pid.sum_error);
	}
	Car_Turn_Speed_pwm(STOP, 0, 0);						//����ʱͣ��
	return;
}

int Copycat_PID(void){									//PID�����㷨
	int now = Capture_Register_Display();				//��ȡ��ǰ����
	int error;
	int d_error;
	if(now > 200)
		now = 200;
	error = Distance - now;								//ƫ��
	d_error = pid.last_error - pid.prev_error;			//΢��
	pid.sum_error += error;								//����
	pid.prev_error = pid.last_error;
	pid.last_error = error;
	now = (Copycat_PID_P * error + Copycat_PID_I * pid.sum_error + Copycat_PID_D * d_error) / Copycat_PID_K;
	return now;
}

void Copycat_PID_Reset(void){							//PID��������
	pid.last_error = pid.prev_error = pid.sum_error = 0;
	return;
}
