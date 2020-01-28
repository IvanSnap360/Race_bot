//#include <ros.h>
///////////////////////////////////////////////////////////////////////////////////////////
#define LEFT 0
#define RIGHT 1
#define FORWARD true
#define BACKWARD false
#define LAST 2
#define CURRENT 1
#define NEXT 3

#define MOTOR_COUNT 2
#define MOTOR_MAX_SPEED 255 
#define MOTOR_MIN_SPEED 0
///////////////////////////////////////////////////////////////////////////////////////////
#define ENCODER_MODE RISING
///////////////////////////////////////////////////////////////////////////////////////////
#define ROBOT_WIDTH_M 20
#define WHEEL_DEAMETR_M 20
const float WHEEL_LENTH = WHEEL_DEAMETR_M * PI;

float angular = 0.0;
float linear = 0.0;


///////////////////////////////////////////////////////////////////////////////////////////
#define left_motor_forward_pin 5
#define left_motor_backward_pin 6
#define left_encoder_pin 0


#define right_motor_forward_pin 11
#define right_motor_backward_pin 10
#define right_encoder_pin 1
///////////////////////////////////////////////////////////////////////////////////////////

int motor_speed[MOTOR_COUNT] = {0, 0};
bool motor_dir[MOTOR_COUNT] = {0, 0};
double motor_real_speed[MOTOR_COUNT] = {0.0, 0.0};

unsigned int encoder_tik_time[MOTOR_COUNT][3] = 
{
    {0, 0, 0},
    {0, 0, 0}
};


///////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
    pinMode(left_motor_forward_pin, OUTPUT);
    pinMode(left_motor_backward_pin, OUTPUT);
    pinMode(right_motor_forward_pin, OUTPUT);
    pinMode(right_motor_backward_pin, OUTPUT);

    attachInterrupt(left_encoder_pi, left_encoder, ENCODER_MODE);
    attachInterrupt(right_encoder_pin, right_encoder, ENCODER_MODE);

}

void loop()
{

}

void left_encoder()
{
    encoder_tik_time[LEFT][CURRENT] = millis() - encoder_tik_time[LEFT][LAST];
}

void right_encoder()
{
    encoder_tik_time[RIGHT][CURRENT] = millis() - encoder_tik_time[RIGHT][LAST];
}

void motor_controller(bool Ldir_ = FORWARD, int Lspeed_ = 0, int Rspeed_ = 0, bool Rdir_ = FORWARD)
{
    if (Lspeed_ > MOTOR_MAX_SPEED)
        Lspeed_ = MOTOR_MAX_SPEED;
    else if (Lspeed_ < MOTOR_MIN_SPEED)
        Lspeed_ = MOTOR_MIN_SPEED;

    if (Rspeed_ > MOTOR_MAX_SPEED)
        Rspeed_ = MOTOR_MAX_SPEED;
    else if (Rspeed_ < MOTOR_MIN_SPEED)
        Rspeed_ = MOTOR_MIN_SPEED;


    if (Ldir_ == FORWARD)
    {
        analogWrite(left_motor_forward_pin, Lspeed_);
        analogWrite(left_motor_backward_pin, Lspeed_ * 0);
    }
    else if (Ldir_ == BACKWARD)
    {
        analogWrite(left_motor_forward_pin, Lspeed_ * 0);
        analogWrite(left_motor_backward_pin, Lspeed_);
    }

    if (Rdir_ == FORWARD)
    {
        analogWrite(right_motor_forward_pin, Rspeed_);
        analogWrite(right_motor_backward_pin, Rspeed_ * 0);
    }
    else if (Rdir_ == BACKWARD)
    {
        analogWrite(right_motor_forward_pin, Rspeed_ * 0);
        analogWrite(right_motor_backward_pin, Rspeed_);
    }

}
