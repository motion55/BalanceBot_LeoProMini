
//SDA	2
//SCL	3
const int ENA = 10;	//PB6 = OC1B (PWM)
const int IN1 = A3;	//PF4	
const int IN2 = A2;	//PF5
const int IN3 = A1;	//PF6
const int IN4 = A0;	//PF7
const int ENB = 9;	//PB5 = OC1A (PWM)
//
const int	ENCB_L = 15;	//PB1 - PCINT1
const int	ENCA_L = 16;	//PB2	- PCINT2
const int	ENCB_R = 14;	//PB3 - PCINT3	
const int	ENCA_R = 8; 	//PB4 - PCINT4

int speed, direction;

void setup() {
  // put your setup code here, to run once:
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
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, HIGH);
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, HIGH);
	}
	else
	{
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, LOW);
		digitalWrite(IN3, HIGH);
		digitalWrite(IN4, LOW);
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
	delay(20);
}
