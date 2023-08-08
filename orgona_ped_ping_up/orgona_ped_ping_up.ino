int cnt = 0;
//declare input pins for read
bool state = true;
//setup
void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); //data bus 0
  pinMode(3, OUTPUT); //data bus 1
  pinMode(4, OUTPUT); //data bus 2
  pinMode(5, OUTPUT); //data bus 3
  pinMode(6, OUTPUT); //data bus 4
  pinMode(7, OUTPUT); //data bus 5
  pinMode(8, OUTPUT); //data bus 6
  pinMode(9, OUTPUT); //data bus 7
  pinMode(10, OUTPUT);//address 0
  pinMode(11, OUTPUT);//address 1
  pinMode(12, OUTPUT);//address 2
  pinMode(13, OUTPUT);// clk

}

void ping_up() {
  // put your main code here, to run repeatedly:
   for (int i = 0; i<8; i++) {
    digitalWrite(13, LOW);
    digitalWrite(10, bitRead(i,0));
    digitalWrite(11, bitRead(i,1));
    digitalWrite(12, bitRead(i,2));
    for (int j = 0; j<8; j++) {
      digitalWrite(pinselect(j)+2, LOW);
      digitalWrite(13, LOW);
      digitalWrite(13, HIGH);
      Serial.print(i);
      Serial.print(" ");
      Serial.println(j);
      delay(1000);
      digitalWrite(pinselect(j)+2, 1);
      digitalWrite(13, LOW);
      delay(5);
      digitalWrite(13, HIGH);
    }
  }
}


int pinselect(int inp) {
  switch (inp) {
    case 0:
      return 0;
    case 1:
      return 2;
    case 2:
      return 1;
    case 3:
      return 6;
    case 4:
      return 7;
    case 5:
      return 5;
    case 6:
      return 3;
    case 7:
      return 4;
    default:
      return 0;
  }
}

void loop() {
ping_up();
}