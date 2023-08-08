#define CLK 34
int cnt = 0;
bool reset = false;
bool sounds[36]; 
byte registers; //global byte for the register sw

void setup() {
  Serial1.begin(35000);
  //PORTA
  pinMode(22, OUTPUT);  //data bus 0
  pinMode(23, OUTPUT);  //data bus 1
  pinMode(24, OUTPUT);  //data bus 2
  pinMode(25, OUTPUT);  //data bus 3
  pinMode(26, OUTPUT);  //data bus 4
  pinMode(27, OUTPUT);  //data bus 5
  pinMode(28, OUTPUT);  //data bus 6
  pinMode(29, OUTPUT);  //data bus 7
  //PORTC
  pinMode(37, OUTPUT);   //address 0
  pinMode(36, OUTPUT);   //address 1
  pinMode(35, OUTPUT);   //address 2
  pinMode(CLK, OUTPUT);  // clk
}

/*
pin 10-11-12 ->   A,B,C címző pin -> DEMUX
pin 13 -> G1 pin -> DEMUX
2-9 databus a SN74LS374 egységekhez

1. G1 L (DEMUX MINDEN kijárat H) -> így ki lehet írni a címet és az adatot, az decoder, illetve a flip-flop állapota nem változik
2. G1 H (DEMUX Az adott címen L) -> A decoderből a címzett D-FF óra lábára L érték kerül, az FF állapota nem változik
3. G1 L (DEMUX MINDEN kijárat H) -> alapállapotra vált a decoder, a felfutó él hatására az adatbusz beíródik a a címzett D-FF-be
*/

void flash() {
  //varible for output Flip-flop states
  byte outPORTS[4] = { 0xff, 0xff, 0xff, 0xff };
  //sounds-> FF
  for (int i = 0; i < 27; i++) {
    if (sounds[i]) {
      outPORTS[i / 8] |= (1 << (i % 8));
    }
  }
  //Register -> FF
  outPORTS[3] |= registers; 

  //Write Procedure
  digitalWrite(CLK, LOW);
  for (int i = 0; i < 4; i++) {
    digitalWrite(CLK, LOW);

    PORTC = i;
    PORTA = ~outPORTS[i];
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
}



void loop() {

  int data = 0;
  if (Serial1.available() > 0) {
    data = Serial1.read();
    //sw start
    switch (data) {
      case 124:
        reset = true;
        break;
      case 240:
        if (reset) {
          reset = false;
          flash();
          cnt = 0;
        }
        break;
      default:
        if (cnt < 12) {
          //calculate active sound
          sounds[11 - cnt] = bitRead(data, 1);
          sounds[23 - cnt] = bitRead(data, 2);
          sounds[35 - cnt] = bitRead(data, 4);
        } else if (cnt == 12) {
          //register
          for (int i = 3; i < 6; i++)
            registers = data;
            registers = registers >> 1;
        }
        cnt++;
        break;
    }
  }
}