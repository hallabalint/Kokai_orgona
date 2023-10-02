bool registers[4][8];
int cnt = 0;
bool reset = false;
bool sounds[36];

void setup() {
  Serial1.begin(39000);
  Serial.begin(38400);
  pinMode(2, OUTPUT);   //data bus 0
  pinMode(3, OUTPUT);   //data bus 1
  pinMode(4, OUTPUT);   //data bus 2
  pinMode(5, OUTPUT);   //data bus 3
  pinMode(6, OUTPUT);   //data bus 4
  pinMode(7, OUTPUT);   //data bus 5
  pinMode(8, OUTPUT);   //data bus 6
  pinMode(9, OUTPUT);   //data bus 7
  pinMode(10, OUTPUT);  //address 0
  pinMode(11, OUTPUT);  //address 1
  pinMode(12, OUTPUT);  //address 2
  pinMode(13, OUTPUT);  // clk
}

// arduino -> data bus port map
int pinselect(int inp) {
  switch (inp) {
    case 0:
      return 2;
    case 1:
      return 4;
    case 2:
      return 3;
    case 3:
      return 8;
    case 4:
      return 9;
    case 5:
      return 7;
    case 6:
      return 5;
    case 7:
      return 6;
    default:
      return 0;
  }
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
  int db = 0;
  if (sounds[24])
    Serial.println("I");
  for (int i = 0; i < 27; i++) {
    registers[i / 8][i % 8] = sounds[i];
  }
  //Serial.println(registers[3][3]);
  digitalWrite(13, LOW);
  for (int k = 0; k < 4; k++) {
    digitalWrite(13, LOW);
    /*
    */
    digitalWrite(12, bitRead(k, 2));
    digitalWrite(11, bitRead(k, 1));
    digitalWrite(10, bitRead(k, 0));
    digitalWrite(pinselect(0), !registers[k][0]);
    digitalWrite(pinselect(1), !registers[k][1]);
    digitalWrite(pinselect(2), !registers[k][2]);
    digitalWrite(pinselect(3), !registers[k][3]);
    digitalWrite(pinselect(4), !registers[k][4]);
    digitalWrite(pinselect(5), !registers[k][5]);
    digitalWrite(pinselect(6), !registers[k][6]);
    digitalWrite(pinselect(7), !registers[k][7]);

    //delay(1);
    //delay(5);
    digitalWrite(13, HIGH);
    //delay(1);
    /*if (4) {
      Serial.print(bitRead(k,2));
      Serial.print(bitRead(k,1));
      Serial.print(bitRead(k,0));
      Serial.print(!registers[k][0]);
      Serial.print(!registers[k][1]);
      Serial.print(!registers[k][2]),
      Serial.print(!registers[k][3]);
      Serial.print(!registers[k][4]);
      Serial.print(!registers[k][5]);
      Serial.print(!registers[k][6]);
      Serial.println(!registers[k][7]);
    }*/
    //talán delay()
    digitalWrite(13, LOW);
    // delay(1);
  }
}



void loop() {

  int data = 0;
  if (Serial1.available() > 0) {
    data = Serial1.read();
    Serial.println(data);
    //sw start
    switch (data) {
      case 248: //35000 124
        reset = true;
        break;
      case 240: // 35000 240
        if (reset) {
          reset = false;
          flash();
          cnt = 0;
        }
        break;
      default:

        if (cnt < 12) {
          /*
          registers[(12-cnt)/8][(12-cnt)%8] = bitRead(data, 1);
          registers[(24-cnt)/8][(24-cnt)%8] = bitRead(data, 2);
          registers[(36-cnt)/8][(36-cnt)%8] = bitRead(data, 3);
          */
          //Serial.println(data);
          sounds[11 - cnt] = bitRead(data, 1);
          sounds[23 - cnt] = bitRead(data, 2);
          sounds[35 - cnt] = bitRead(data, 4);
        } else if (cnt == 12) {
          for (int i = 3; i < 6; i++)
            registers[3][i] = bitRead(data, i + 1);
        }
        cnt++;
        break;
    }
  }
}