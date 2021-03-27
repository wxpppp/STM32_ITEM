#include "copycat.h"

static struct Copycat_Pid pid;							//记录PID控制

void Copycat(void){										//跟屁虫模块
	extern u8 ex_mode;
	int speed = 0;
	Copycat_PID_Reset();								//PID控制清零
	while(ex_mode == MODE_OFF){
		Ultrasonic_Work();
		if(Capture_Register_Empyt()){					//捕获到一次高电平
			speed = Copycat_PID();						//调用PID调整
			Capture_Register_Reset();					//捕获重置
		}
		if(speed >= 0){									//比限制距离要小，需要前进
			if(speed > Copycat_Speed1)
				speed = Copycat_Speed1;
			else if(speed < Copycat_Speed2)
				speed = 0;
			Car_Turn_Speed_pwm(BACK, speed, 0);
		}
		else{											//比限制距离要大，需要后退
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
	Car_Turn_Speed_pwm(STOP, 0, 0);						//返回时停车
	return;
}

int Copycat_PID(void){									//PID控制算法
	int now = Capture_Register_Display();				//获取当前距离
	int error;
	int d_error;
	if(now > 200)
		now = 200;
	error = Distance - now;								//偏差
	d_error = pid.last_error - pid.prev_error;			//微分
	pid.sum_error += error;								//积分
	pid.prev_error = pid.last_error;
	pid.last_error = error;
	now = (Copycat_PID_P * error + Copycat_PID_I * pid.sum_error + Copycat_PID_D * d_error) / Copycat_PID_K;
	return now;
}

void Copycat_PID_Reset(void){							//PID控制清零
	pid.last_error = pid.prev_error = pid.sum_error = 0;
	return;
}
