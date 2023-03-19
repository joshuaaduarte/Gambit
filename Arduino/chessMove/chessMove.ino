// This function returns the 4 character movement of where a piece was picked up and placed

int compareBoard[8][8] = { 
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 0, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0},
  {1, 1, 1, 1, 1, 0, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
  };

char c[5];

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

  chessMove(compareBoard, c);
  Serial.print("Picked Up:");
  Serial.print(c[0]);
  Serial.println(c[1]);
  Serial.print("Placed:");
  Serial.print(c[2]);
  Serial.println(c[3]);
  delay(5000);

}


// Compare function with what is scanned
char chessMove(int compareBoard[8][8], char c[5]){
    int flag1 = 0;
    int flag2 = 0;
    int flag3 = 0;
    int flagBreak = 0;
    int flagF = 0; 
    int pieceCheckOne = 0;
    int piece2CheckOne = 0;
    int pieceCheckTwo = 0;
    int piece2CheckTwo = 0;
    char rows[8] = {'1','2','3','4','5','6','7','8'};
    char columns[8] = {'a','b','c','d','e','f','g','h'};
    int chessboard[8][8];
    int copyBoard[8][8];
    int copyBoard2[8][8];
    c[4] = 0;
    
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
    int rowInput[8]= {A8, A9, A10, A11, A12, A13, A14, A15};  
    // While loop that will not stop until a move is made 
    while( flagF==0 ){

      // While loop that will run until it recognizes a move has initiated (a piece is picked up)
      while( flag1 == 0){
        // Scan board to see if board is altered 
        for (int j = 0; j < 8;j++){
         for(int i = 22; i < 30; i++){
           digitalWrite(i, HIGH);
           chessboard[22-i][j] = analogRead(rowInput[j]) - calibration[j][i-22];
           if (chessboard[22-i][j] < -5){
            chessboard[22-i][j] = 1;
            copyBoard[j][i-22] = 1;
            // Piece was added
            if (chessboard[22-i][j] != compareBoard[j][i-22]){
              Serial.println("Piece has been added. Please Remove");
            }
           } 
           else{
            chessboard[22-i][j] = 0;
            copyBoard[j][i-22] = 0;
            // Where piece was picked up
            if (chessboard[22-i][j] != compareBoard[j][i-22]){
              c[0] = columns[i-22];
              c[1] = rows[7-j];
              pieceCheckOne = i-22;
              piece2CheckOne = j;
              flag1 = 1;
            }
           }
           digitalWrite(i, LOW);
         }
        }  
      }
    // While loop that determines if a piece is placed once picked up or a potential conquered move is happening
    while (flag2 == 0){
      // Scan board to see if it is altered from the copy board that was just made from previous scan
      for (int j = 0; j < 8;j++){
       for(int i = 22; i < 30; i++){
         digitalWrite(i, HIGH);
         chessboard[22-i][j] = analogRead(rowInput[j]) - calibration[j][i-22];
         if (chessboard[22-i][j] < -5){
          chessboard[22-i][j] = 1;
          copyBoard2[j][i-22] = 1;

          // Actions if piece is placed
          if (chessboard[22-i][j] != copyBoard[j][i-22]){
            if (pieceCheckOne == 22-i && piece2CheckOne == j){
              Serial.println("piece placed back down");
              c[2] = columns[i-22];
              c[3] = rows[7-j];
              flag2 = 1;
              flagBreak = 1;
            }
            else{
              c[2] = columns[i-22];
              c[3] = rows[7-j];
              flag2 = 1;
              flagBreak = 1;
            }
          }
         } 
         else{
          chessboard[22-i][j] = 0;
          copyBoard2[j][i-22] = 0;
          // Check if second piece is picked up
          if (chessboard[22-i][j] != copyBoard[j][i-22]){
            flag2 = 1;
            pieceCheckTwo = 22-i;
            piece2CheckTwo = j;
          }
         }
         digitalWrite(i, LOW);
       }
      }
    }

    // Break if second move was determine
    if (flagBreak == 1){
      break;
    }
      
    // while loop to tell if piece was conquered
    while (flag3 == 0){
      // scan to see if piece was placed in conquered spot or a different square which would result in an error
      for (int j = 0; j < 8;j++){
       for(int i = 22; i < 30; i++){
         digitalWrite(i, HIGH);
         chessboard[22-i][j] = analogRead(rowInput[j]) - calibration[j][i-22];
         if (chessboard[22-i][j] < -5){
          chessboard[22-i][j] = 1;
          // Where piece was placed
          if (chessboard[22-i][j] != copyBoard2[j][i-22]){
            if (pieceCheckTwo == 22-i && piece2CheckTwo == j){
              Serial.println("piece conquered");
              c[2] = columns[i-22];
              c[3] = rows[7-j];
              flag3 = 1;
              flagF = 1;
            }
            else{
              Serial.println("Piece placed in non conquered square. Error");
              delay(2500);
            }
          }
         } 
         else{
          chessboard[22-i][j] = 0;
          // Where piece was picked up
          if (chessboard[22-i][j] != copyBoard2[j][i-22]){
            Serial.println("Third Piece Picked Up. Error");
            delay(2500);
          }
         }
         digitalWrite(i, LOW);
       }
       
      }
    }
    

    }
    return c;
}



  
