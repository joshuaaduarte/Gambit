int rowInput[8]= {A8, A9, A10, A11, A12, A13, A14, A15}; 
int chessboard[8][8];
int calibration[8][8] = {
  {419, 427, 420, 424, 422, 418, 421, 423},
  {408, 436, 437, 425, 427, 412, 423, 421},
  {394, 433, 420, 432, 422, 407, 412, 421},
  {409, 427, 435, 415, 420, 416, 421, 429},
  {414, 416, 428, 432, 434, 402, 410, 408},
  {406, 434, 433, 438, 452, 405, 410, 437},
  {421, 401, 421, 418, 430, 422, 401, 416},
  {420, 418, 430, 434, 415, 408, 436, 413}
  };

//int calibration[8][8] = {
//  {419, 427, 420, 424, 422, 418, 421, 423},
//  {408, 436, 437, 425, 427, 412, 423, 421},
//  {394, 433, 420, 432, 422, 407, 412, 421},
//  {409, 427, 435, 415, 420, 416, 421, 429},
//  {406, 434, 433, 438, 452, 405, 410, 437},
//  {414, 416, 428, 432, 434, 402, 410, 408},
//  {421, 401, 421, 418, 430, 422, 401, 416},
//  {420, 418, 430, 434, 415, 408, 436, 413}
//};


void setup() {

  Serial.begin(9600);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);

  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);
  
}

void loop() {

    for (int j = 0; j < 8;j++){
     for(int i = 22; i < 30; i++){
       digitalWrite(i, HIGH);
       chessboard[22-i][j] = analogRead(rowInput[j]) - calibration[j][i-22];
       if (chessboard[22-i][j] < -5){
        chessboard[22-i][j] = 1;
        Serial.print(chessboard[22-i][j]);
       } else{
        chessboard[22-i][j] = 0;
        Serial.print(chessboard[22-i][j]);
       }
       Serial.print(" ");
       digitalWrite(i, LOW);
     }
       Serial.println();     
    }
    Serial.println();     
   delay(1000);

  
  
}



  
