///////////////////////////////////////////////////////////////////////////////////////////
#define LEFT 0
#define RIGHT 1

#define FORWARD true
#define BACKWARD false

#define LAST 2
#define CURRENT 1
#define NEXT 3

#define MY 1
#define REAL 0
#define LINEAR  0
#define ANGULAR 1

#define FORLENTH 0
#define FORSPEED 1
 
#define MOTOR_COUNT 2
#define MOTOR_REDUCTION 1/17
#define MOTOR_MAX_SPEED 255 
#define MOTOR_MIN_SPEED 0
///////////////////////////////////////////////////////////////////////////////////////////
#define ENCODER_MODE RISING
#define ENCODER_TIKS 170
///////////////////////////////////////////////////////////////////////////////////////////
#define ROBOT_WIDTH_M 20
#define WHEEL_DEAMETR_M 20
const float WHEEL_LENTH = WHEEL_DEAMETR_M * PI;

double robot_speed[2][2] = 
{   
    {0/*REAL*/, 0/*MY*/},
    {0.0 /*LINEAR*/, 0.0 /*ANGULAR*/}
};
double robot_route = 0.0;



///////////////////////////////////////////////////////////////////////////////////////////
#define left_motor_forward_pin 5
#define left_motor_backward_pin 6
#define left_encoder_pin 2


#define right_motor_forward_pin 11
#define right_motor_backward_pin 10
#define right_encoder_pin 3
///////////////////////////////////////////////////////////////////////////////////////////

int motor_speed[MOTOR_COUNT] = {0, 0};
bool motor_dir[MOTOR_COUNT] = {0, 0};
double motor_real_speed[MOTOR_COUNT] = {0.0, 0.0};

 unsigned long encoder_tik_time[MOTOR_COUNT][3] = 
{
    {0 /*LEFT*/, 0 /* currnet*/, 0 /*last*/},
    {0 /*RIGHT*/, 0 /* currnet*/, 0 /*last*/}
};
unsigned int encoder_tiks[MOTOR_COUNT][2] = 
{
    {0, 0},
    {0, 0}
};
///////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
    pinMode(left_motor_forward_pin, OUTPUT);
    pinMode(left_motor_backward_pin, OUTPUT);
    pinMode(right_motor_forward_pin, OUTPUT);
    pinMode(right_motor_backward_pin, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(left_encoder_pin), left_encoder, ENCODER_MODE);
    attachInterrupt(digitalPinToInterrupt(right_encoder_pin), right_encoder, ENCODER_MODE);
    attachInterrupt(digitalPinToInterrupt(left_encoder_pin), calculate_real_robot, CHANGE);

    encoder_tik_time[LEFT][LAST] = millis();
    encoder_tik_time[RIGHT][LAST] = millis();
}

void left_encoder()
{
    encoder_tiks[LEFT][FORSPEED]++;
    encoder_tiks[LEFT][FORLENTH]++;
    encoder_tik_time[LEFT][CURRENT] = millis() - encoder_tik_time[LEFT][LAST];  

    if (encoder_tik_time[LEFT][CURRENT] >= 100)
    {
        motor_real_speed[LEFT] = encoder_tiks[LEFT][FORSPEED] * 60000 / encoder_tik_time[LEFT][CURRENT]; 
        encoder_tiks[LEFT][FORSPEED] = 0;
        encoder_tik_time[LEFT][LAST] = millis();
    }
}

void right_encoder()
{
    encoder_tiks[RIGHT][FORSPEED]++;
    encoder_tiks[RIGHT][FORLENTH]++;
    encoder_tik_time[RIGHT][CURRENT] = millis() - encoder_tik_time[RIGHT][LAST];
    
    if (encoder_tik_time[RIGHT][CURRENT] >= 100)
    {
        motor_real_speed[RIGHT] = encoder_tiks[RIGHT][FORSPEED] * 60000 / encoder_tik_time[RIGHT][CURRENT]; 
        encoder_tiks[RIGHT][FORSPEED] = 0;
        encoder_tik_time[RIGHT][LAST] = millis();
    }
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

void calculate_real_robot()
{
    robot_speed[REAL][LINEAR] = ((motor_real_speed[LEFT] + motor_real_speed[RIGHT]) / 2) * MOTOR_REDUCTION;
    robot_speed[REAL][ANGULAR] = ((motor_real_speed[RIGHT] - motor_real_speed[LEFT]) / ROBOT_WIDTH_M) * MOTOR_REDUCTION;
    robot_route = WHEEL_DEAMETR_M * PI * (((encoder_tiks[LEFT][FORLENTH] + encoder_tiks[RIGHT][FORLENTH]) / 2) / ENCODER_TIKS);
}

void loop()
{


}


