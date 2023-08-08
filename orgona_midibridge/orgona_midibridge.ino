#define MIDI_BAUD 74880
#define debug
int data[8];
int cnt = 0;
bool reset = false;

void print(int data) {
  for (byte mask = 0x80; mask; mask >>=1) {
    if (mask & data)
      Serial.print(1);
    else
      Serial.print(0);
  }
}

void setup() {
  Serial.begin(MIDI_BAUD);
  Serial1.begin(32440);
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

void writeOut(int data, int reg) {
  if (reg > 7)
    return;
  digitalWrite(13, LOW);
  digitalWrite(10, bitRead(reg,0));
  digitalWrite(11, bitRead(reg,1));
  digitalWrite(12, bitRead(reg,2));
  for (int i = 0; i < 8; i++) {
    digitalWrite(pinselect(i)+2, bitRead(data, i) == 1 ? LOW : HIGH);
  }
  digitalWrite(13, HIGH);
}

void loop() {
  int data = 0;
  

  if (Serial1.available() > 0) {
    data = Serial1.read();
    //sw start
#ifndef debug
    switch (data) {
      case 248:
        reset = true;
        break;
      case 240:
        if (reset) {
          Serial.println("");
          reset = false;
          int cnt = 0;
          break;
        }
      default:
        if (reset) {
        writeOut(188, cnt-1);
         Serial.print(188, BIN);
          reset = false;
        }
        writeOut(data, cnt);
        print(data);
        cnt++;
       break;
    }
  //sw end
  #endif
  #ifdef debug
  Serial.println(data, DEC);
  #endif
  }

}
