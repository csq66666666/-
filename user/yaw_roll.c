
#include "yaw_roll.h"
#include "Delay.h"
#include "up_1.h"
#include "down_1.h"
static int t=1;
static float up_init_angle;
static float down_init_angle;

static float up_angle;
static float down_angle;
static float yaw_all;
static float roll_all;
void init_angle_yaw_roll(void)
{
if(t)
{up_init_angle=bsp_as5666GetAngle_up()*360.0;
 down_init_angle=bsp_as5666GetAngle_down()*360.0;
t=0;
}
}


float roll_angle(void)
{
up_angle=bsp_as5666GetAngle_up()*360.0;
down_angle=bsp_as5666GetAngle_down()*360.0;
roll_all=(((up_angle-up_init_angle)+(down_angle-down_init_angle))/2)/3.666;
return roll_all;

}

//	


float yaw_angle(void)
{
   
up_angle=bsp_as5666GetAngle_up()*360.0;
down_angle=bsp_as5666GetAngle_down()*360.0;
yaw_all=((up_angle-up_init_angle)+(down_angle-down_init_angle)*(-1.0))/2;
return -(yaw_all/4.0f);
}
