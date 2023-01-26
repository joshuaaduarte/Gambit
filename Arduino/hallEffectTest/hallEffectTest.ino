


int chessboard[4][4];

void setup() {

  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

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

  int flag1 = 0;
  int flag2 = 0; 
  while (flag1 == 0 && flag2 == 0){
    int rowOutput[8] = {2, 3, 4, 5, 6, 7, 8, 9};
    int rowInput[8]= {A8, A9, A10, A11, A12, A13, A14, A15}; 
    for (int j = 0; j < 8;j++){
     for(int i = 2; i < 10; i++){
       digitalWrite(i, HIGH);
       chessboard[2-i][j] = analogRead(rowInput[j]);
       Serial.print(chessboard[2-i][j]);
       Serial.print(" ");
       //Serial.print(digitalRead(rowDetection[i]));
       digitalWrite(i, LOW);
     }
        Serial.println();     
    }
    Serial.println();
    
  delay(2000);
  }
  
  
}
  

  
