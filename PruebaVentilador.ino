const int alt = 7;
const int cont = 8;
int speed = 200;

void setup(){
  pinMode(alt,OUTPUT);
  pinMode(cont,OUTPUT);
}

void loop(){
  analogWrite(cont,LOW);
  analogWrite(alt,speed);
  delay(2000);
  analogWrite(cont,LOW);
  analogWrite(alt,LOW);
  delay(2000);
}
