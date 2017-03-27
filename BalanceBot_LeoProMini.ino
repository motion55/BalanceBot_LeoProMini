
//SDA	2
//SCL	3
#define ENA_RUN  1
#define ENB_RUN  1

const int ENA = 10;	//PB6 = OC1B (PWM)
const int IN1 = A3;	//PF4	
const int IN2 = A2;	//PF5
const int IN3 = A1;	//PF6
const int IN4 = A0;	//PF7
const int ENB = 9;	//PB5 = OC1A (PWM)
//
const int ENCA_R = 8;   //PB4 - PCINT4
const int ENCB_R = 14;  //PB3 - PCINT3  
const int ENCA_L = 16;	//PB2	- PCINT2
const int ENCB_L = 15;  //PB1 - PCINT1
const unsigned char PCINT_mask = (1<<1)+(1<<2)+(1<<3)+(1<<4);
volatile unsigned char PINB_val;
volatile unsigned long ENCODER_R;
volatile unsigned long ENCODER_L;

int speed, direction;

void setup() {
  // put your setup code here, to run once:
  pinMode(ENCA_R, INPUT_PULLUP);
  pinMode(ENCB_R, INPUT_PULLUP);
  pinMode(ENCA_L, INPUT_PULLUP);
  pinMode(ENCB_L, INPUT_PULLUP);
  
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  speed = 0;
  direction = 1;
  SerialUSB.begin(115200);

  ENCODER_R = 0;
  ENCODER_L = 0;
  PINB_val = PORTB;
  PCMSK0 = PCINT_mask;
  PCIFR = 1;
  PCICR = 1;
}

void setSpeed(int Speed)
{
	if (Speed == 0)
	{
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, LOW);
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, LOW);
	}
	else
	if (Speed < 0)
	{
		Speed = -Speed;
#if ENA_RUN  
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, HIGH);
#else    
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
#endif   
#if ENB_RUN  
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, HIGH);
#else    
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
#endif   
	}
	else
	{
#if ENA_RUN  
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, LOW);
#else    
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
#endif   
#if ENB_RUN  
		digitalWrite(IN3, HIGH);
		digitalWrite(IN4, LOW);
#else    
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
#endif   
	}
	if (Speed > 255)
		Speed = 255;
	analogWrite(ENA, Speed);
	analogWrite(ENB, Speed);
}

void loop() {
  // put your main code here, to run repeatedly:
	if (speed >= 255)
	{
		speed = 255;
		direction = -1;
	}
	else
	if (speed <= -255)
	{
		speed = -255;
		direction = 1;
	}
	speed += direction;
	setSpeed(speed);
#if 1
  //SerialUSB.println(ENCODER_L, HEX);
  SerialUSB.println(ENCODER_L, HEX);
#else  
  SerialUSB.println((PINB_val&0x1E)|0x80, BIN);
#endif  
  delay(20);
}

ISR(PCINT0_vect)
{
  unsigned char old_val = PINB_val;
  unsigned char new_val = PINB;
  if (new_val & old_val & 0x6)
  {
    char delta = 0;
    if (old_val & 4)
    {
      if (new_val & 2) delta = 1;
      else delta = -1;
    }
    else
    {
      if (new_val & 2) delta = -1;
      else delta = 1;
    }
    ENCODER_L += delta;      
  }
  
  if (new_val & old_val & 0x18)
  {
    char delta = 0;
    if (old_val & 0x10)
    {
      if (new_val & 8) delta = -1;
      else delta = 1;
    }
    else
    {
      if (new_val & 8) delta = 1;
      else delta = -1;
    }
    ENCODER_R += delta;      
  }
  
  PINB_val = new_val;
}

