// The following code serves as the integration of the chess engine, movement script, and the sensing script to create a functional chessboard

// Chess Engine Initialization Script (DO NOT TOUCH)
#define W while
#define M 0x88
#define S 128
#define I 8000

#define MYRAND_MAX 65535     /* 16bit pseudo random generator */
long  N, T;                  /* N=evaluated positions+S, T=recursion limit */
short Q, O, K, R, k=16;      /* k=moving side */
char *p, c[5], Z;            /* p=pointer to c, c=user input, computer output, Z=recursion counter */

char L,
w[]={0,2,2,7,-1,8,12,23},                             /* relative piece values    */
o[]={-16,-15,-17,0,1,16,0,1,16,15,17,0,14,18,31,33,0, /* step-vector lists */
     7,-1,11,6,8,3,6,                                 /* 1st dir. in o[] per piece*/
     6,3,5,7,4,5,3,6};                                /* initial piece setup      */
/* board is left part, center-pts table is right part, and dummy */  

char b[]={     
  22, 19, 21, 23, 20, 21, 19, 22, 28, 21, 16, 13, 12, 13, 16, 21,
  18, 18, 18, 18, 18, 18, 18, 18, 22, 15, 10,  7,  6,  7, 10, 15,
   0,  0,  0,  0,  0,  0,  0,  0, 18, 11,  6,  3,  2,  3,  6, 11,
   0,  0,  0,  0,  0,  0,  0,  0, 16,  9,  4,  1,  0,  1,  4,  9,
   0,  0,  0,  0,  0,  0,  0,  0, 16,  9,  4,  1,  0,  1,  4,  9,
   0,  0,  0,  0,  0,  0,  0,  0, 18, 11,  6,  3,  2,  3,  6, 11,
   9,  9,  9,  9,  9,  9,  9,  9, 22, 15, 10,  7,  6,  7, 10, 15,
  14, 11, 13, 15, 12, 13, 11, 14, 28, 21, 16, 13, 12, 13, 16, 21, 0
};

char bk[16*8+1];

unsigned int seed=0;
uint32_t  byteBoard[8];

char sym[17] = {".?pnkbrq?P?NKBRQ"};
int mn=1;
char lastH[5], lastM[5];
unsigned short ledv=1;

String inputString = "";
bool stringComplete = false;  // whether the string is complete

// End of Chess Engine Initialization 
// ----------------------------------------------------------------------------------------
// Inclusions
  #include <AccelStepper.h>
  #include <ezButton.h>

// Movement Initialization
  // Motor Pin Connections
    const int dirPin = 5;
    const int stepPin = 2;
    const int dirPin2 =6;
    const int stepPin2 = 3;
    
  // Magnet Pin
    const int magnet = 11;
    
  // Defining Stepper Motors with Respective Pins
    AccelStepper stepper1(AccelStepper::DRIVER, stepPin, dirPin);
    AccelStepper stepper2(AccelStepper::DRIVER, stepPin2, dirPin2);
    
  // Limit Switches
    ezButton limitswitch(10);
    ezButton limitswitch2(9);
    
// conquered count 
int pieceCount = 32;
int pieceCountNew = 0;
int moveCount = 0;
int count = 0;

// Sensing Initialization 

  // Boards to be used for comparisons
    int initialBoard[8][8] = {
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1}
    };
  
    int copyBoard[8][8] = {
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1}
    };
  
    int chessboard[8][8];

  
  // Calibration Board for Sensors 
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

void setup() {
  Serial.begin(9600);
  Serial.println("*** MicroMaxChess ***");
  lastH[0] = 0;
  serialBoard();
  //checkSetup(copyBoard);
  // Stepper Motor Settings
    stepper1.setMaxSpeed(2100);
    stepper1.setAcceleration(2400);
    stepper2.setMaxSpeed(2100);
    stepper2.setAcceleration(2400);
    float pos1 = 200;
    float pos2 = 200;
    
  // Limit Switch Settings
    pinMode(9, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);
    limitswitch.setDebounceTime(5); // set debounce time to 50 milliseconds
    limitswitch2.setDebounceTime(5);
    
  // Electromagnet Settings
    pinMode(magnet, OUTPUT); 
    
  // Sensing Pins 
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

  // Check if board is setup properly
  
  
}


// Functions

  // Functions pertaining to scanning
    // Scan and Print array 
    void scanPrint(){
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
      int rowInput[8]= {A8, A9, A10, A11, A12, A13, A14, A15}; 
      for (int j = 0; j < 8;j++){
       for(int i = 22; i < 30; i++){
         digitalWrite(i, HIGH);
         chessboard[22-i][j] = analogRead(rowInput[j]) - calibration[j][i-22];
         if (chessboard[22-i][j] < -5){
          chessboard[22-i][j] = 1;
          Serial.print(chessboard[22-i][j]);
         } 
         else{
          chessboard[22-i][j] = 0;
          Serial.print(chessboard[22-i][j]);
         }
         Serial.print(" ");
         digitalWrite(i, LOW);
       }
          Serial.println();     
      }
      Serial.println();     
    }

    // Check Setup to see if scan matches what 'board' is 
      void checkSetup(int board[8][8]){
          int flag1 = 0;
          int flag2 = 0;
          int flag3 = 0;
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
      
          
          int rowInput[8]= {A8, A9, A10, A11, A12, A13, A14, A15}; 
          while(flag3==0 ){
            flag1 = 0;
            flag2 = 0;
            for (int j = 0; j < 8;j++){
             for(int i = 22; i < 30; i++){
               digitalWrite(i, HIGH);
               chessboard[22-i][j] = analogRead(rowInput[j]) - calibration[j][i-22];
               if (chessboard[22-i][j] < -5){
                chessboard[22-i][j] = 1;
                if (chessboard[22-i][j] != board[j][i-22]){
                  flag2 = 1;
                }
               } 
               else{
                chessboard[22-i][j] = 0;
                if (chessboard[22-i][j] != board[j][i-22]){
                  flag1 = 1;
                }
               }
               digitalWrite(i, LOW);
             }
            }
            if (flag1 == 0 && flag2 == 0){
              flag3 = 1;
              Serial.println("Correct Setup. Your Move.");
            }
            else{
              Serial.println("Incorrect Setup");
              delay(5000);
            }
          }
      }

      
    // Compares scan with 'compareBoard' and outputs 4 character array 'c' that indicated where it was picked up from and where it was placed using a2a4 format.
      char compareFunction(int compareBoard[8][8], char c[5]){
          int flag1 = 0;
          int flag2 = 0;
          char rows[8] = {'1','2','3','4','5','6','7','8'};
          char columns[8] = {'a','b','c','d','e','f','g','h'};
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
          int rowInput[8]= {A8, A9, A10, A11, A12, A13, A14, A15}; 
          while( flag1==0 || flag2==0 ){
          for (int j = 0; j < 8;j++){
           for(int i = 22; i < 30; i++){
             digitalWrite(i, HIGH);
             chessboard[22-i][j] = analogRead(rowInput[j]) - calibration[j][i-22];
             if (chessboard[22-i][j] < -5){
              chessboard[22-i][j] = 1;
              // Where piece was placed
              if (chessboard[22-i][j] != compareBoard[j][i-22]){
                c[2] = columns[i-22];
                c[3] = rows[7-j];
                flag2 = 1;
              }
             } 
             else{
              chessboard[22-i][j] = 0;
              // Where piece was picked up
              if (chessboard[22-i][j] != compareBoard[j][i-22]){
                c[0] = columns[i-22];
                c[1] = rows[7-j];
                flag1 = 1;
              }
             }
             //Serial.print(" ");
             digitalWrite(i, LOW);
           }
           //Serial.println();     
          }
          }
          c[4] = 0; 
          return c;
      }

    // Chess Move Function that will output four character move and can determine if a piece is conquered or not
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
                  c[4] = '1';
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
      


  // Functions pertaining to movement 
    // Turn On Electromagnet
      void magnetOn(){
        digitalWrite(magnet, HIGH); 
      }
    // Turn Off Electromagnet
      void magnetOff(){
        digitalWrite(magnet, LOW);
      }
    // Conquer Movement

    // Move A to B Vertical

    // Move A to B Horizontal

    // Move A to B Diagonal

    // Move A to B Knight

    // Move A to B Castle 

    // Homing 
    void home(){
      limitswitch.loop();
      while (digitalRead(10) == HIGH){
        stepper1.move(200);
        stepper2.move(200);
        stepper1.run();
        stepper2.run();
      }
      
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      
      limitswitch2.loop();
      while (digitalRead(9) == HIGH){
        stepper1.move(200);
        stepper2.move(-200);
        stepper1.run();
        stepper2.run();
      }
    }

// Functions
// ------------------------------------------------------------------


void loop() {
  int r;
  int conquer = 0;
  Serial.println(pieceCount);
  Serial.println(pieceCountNew);

  // Scan board and see if it matches the initial board setup. 

  if (moveCount != 0){

    if (pieceCount > pieceCountNew){
      conquer = 1;
      count += 1;
      pieceCount = pieceCountNew;
    }
      movement(lastM, count, conquer);
  }
  checkSetup(copyBoard);
  

  
  // Take move from human
  
//  while (stringComplete == false) {
//    getserialchar();
//  }
//  Serial.print(mn);
//  Serial.print(". ");
//  Serial.print(inputString.substring(0,4));
//  c[0] = inputString.charAt(0);
//  c[1] = inputString.charAt(1);
//  c[2] = inputString.charAt(2);
//  c[3] = inputString.charAt(3);
//  c[4] = 0;
  // clear the string:
  chessMove(copyBoard, c);
  if (c[4] == '1'){
    pieceCount = pieceCount - 1;
    c[4] = '0';
  }
  inputString = "";
  stringComplete = false;
  Serial.print(" Think ");                       /* Turn for ARDUINO */

  K = *c - 16 * c[1] + 799, L = c[2] - 16 * c[3] + 799; /* parse entered move */
  N = 0;
  T = 0x3F;                                 /* T=Computer Play strength */
  bkp();                                    /* Save the board just in case */
  r = D(-I, I, Q, O, 1, 3);                 /* Check & do the human movement */
  if ( !(r > -I + 1) ) {
    Serial.println("Lose ");
    gameOver();
  }
  if (k == 0x10) {                          /* The flag turn must change to 0x08 */
    Serial.println("No valid move");
    return;
  }

  strcpy(lastH, c);                         /* Valid human movement */

  mn++;                                     /* Next move */

  K = I;
  N = 0;
  T = 0x3F;                                 /* T=Computer Play strength */
  r = D(-I, I, Q, O, 1, 3);                 /* Think & do*/
  if ( !(r > -I + 1) ) {
    Serial.println("Lose*");
    gameOver();
  }

  strcpy(lastM, c);                         /* Valid ARDUINO movement */
  r = D(-I, I, Q, O, 1, 3);
  if ( !(r > -I + 1) ) {
    Serial.println(lastM);
    gameOver();
  }
Serial.println(lastM);
   serialBoard();

moveCount += 1;

}


/* User interface routines */

void getserialchar() {
  while (Serial.available() > 0) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
unsigned short myrand(void) {
  unsigned short r = (unsigned short)(seed % MYRAND_MAX);
  return r = ((r << 11) + (r << 7) + r) >> 1;
}
/* recursive minimax search */
/* (q,l)=window, e=current eval. score, */
/* E=e.p. sqr.z=prev.dest, n=depth; return score */
short D(short q, short l, short e, unsigned char E, unsigned char z, unsigned char n) {
  short m, v, i, P, V, s;
  unsigned char t, p, u, x, y, X, Y, H, B, j, d, h, F, G, C;
  signed char r;
  if (++Z > 30) {                                   /* stack underrun check */
    --Z; return e;
  }
  q--;                                          /* adj. window: delay bonus */
  k ^= 24;                                      /* change sides             */
  d = Y = 0;                                    /* start iter. from scratch */
  X = myrand() & ~M;                            /* start at random field    */
  W(d++ < n || d < 3 ||                         /* iterative deepening loop */
    z & K == I && (N < T & d < 98 ||            /* root: deepen upto time   */
                   (K = X, L = Y & ~M, d = 3)))                /* time's up: go do best    */
  { x = B = X;                                   /* start scan at prev. best */
    h = Y & S;                                   /* request try noncastl. 1st*/
    P = d < 3 ? I : D(-l, 1 - l, -e, S, 0, d - 3); /* Search null move         */
    m = -P < l | R > 35 ? d > 2 ? -I : e : -P;   /* Prune or stand-pat       */
    ++N;                                         /* node count (for timing)  */
    do {
      u = b[x];                                   /* scan board looking for   */
      if (u & k) {                                /*  own piece (inefficient!)*/
        r = p = u & 7;                             /* p = piece type (set r>0) */
        j = o[p + 16];                             /* first step vector f.piece*/
        W(r = p > 2 & r < 0 ? -r : -o[++j])        /* loop over directions o[] */
        { A:                                        /* resume normal after best */
          y = x; F = G = S;                         /* (x,y)=move, (F,G)=castl.R*/
          do {                                      /* y traverses ray, or:     */
            H = y = h ? Y ^ h : y + r;               /* sneak in prev. best move */
            if (y & M)break;                         /* board edge hit           */
            m = E - S & b[E] && y - E < 2 & E - y < 2 ? I : m; /* bad castling             */
            if (p < 3 & y == E)H ^= 16;              /* shift capt.sqr. H if e.p.*/
            t = b[H]; if (t & k | p < 3 & !(y - x & 7) - !t)break; /* capt. own, bad pawn mode */
            i = 37 * w[t & 7] + (t & 192);           /* value of capt. piece t   */
            m = i < 0 ? I : m;                       /* K capture                */
            if (m >= l & d > 1)goto C;               /* abort on fail high       */
            v = d - 1 ? e : i - p;                   /* MVV/LVA scoring          */
            if (d - !t > 1)                          /* remaining depth          */
            { v = p < 6 ? b[x + 8] - b[y + 8] : 0;    /* center positional pts.   */
              b[G] = b[H] = b[x] = 0; b[y] = u | 32;  /* do move, set non-virgin  */
              if (!(G & M))b[F] = k + 6, v += 50;     /* castling: put R & score  */
              v -= p - 4 | R > 29 ? 0 : 20;           /* penalize mid-game K move */
              if (p < 3)                              /* pawns:                   */
              { v -= 9 * ((x - 2 & M || b[x - 2] - u) + /* structure, undefended    */
                          (x + 2 & M || b[x + 2] - u) - 1  /*        squares plus bias */
                          + (b[x ^ 16] == k + 36))          /* kling to non-virgin King */
                     - (R >> 2);                       /* end-game Pawn-push bonus */
                V = y + r + 1 & S ? 647 - p : 2 * (u & y + 16 & 32); /* promotion or 6/7th bonus */
                b[y] += V; i += V;                     /* change piece, add score  */
              }
              v += e + i; V = m > q ? m : q;          /* new eval and alpha       */
              C = d - 1 - (d > 5 & p > 2 & !t & !h);
              C = R > 29 | d < 3 | P - I ? C : d;     /* extend 1 ply if in check */
              do
                s = C > 2 | v > V ? -D(-l, -V, -v,     /* recursive eval. of reply */
                                       F, 0, C) : v;    /* or fail low if futile    */
              W(s > q&++C < d); v = s;
              if (z && K - I && v + I && x == K & y == L) /* move pending & in root:  */
              { Q = -e - i; O = F;                     /*   exit if legal & found  */
                R += i >> 7; --Z; return l;            /* captured non-P material  */
              }
              b[G] = k + 6; b[F] = b[y] = 0; b[x] = u; b[H] = t; /* undo move,G can be dummy */
            }
            if (v > m)                               /* new best, update max,best*/
              m = v, X = x, Y = y | S & F;            /* mark double move with S  */
            if (h) {
              h = 0;  /* redo after doing old best*/
              goto A;
            }
            if (x + r - y | u & 32 |                 /* not 1st step,moved before*/
                p > 2 & (p - 4 | j - 7 ||             /* no P & no lateral K move,*/
                         b[G = x + 3 ^ r >> 1 & 7] - k - 6     /* no virgin R in corner G, */
                         || b[G ^ 1] | b[G ^ 2])               /* no 2 empty sq. next to R */
               )t += p < 5;                           /* fake capt. for nonsliding*/
            else F = y;                              /* enable e.p.              */
          } W(!t);                                  /* if not capt. continue ray*/
        }
      }
    } W((x = x + 9 & ~M) - B);                 /* next sqr. of board, wrap */
C: if (m > I - M | m < M - I)d = 98;           /* mate holds to any depth  */
    m = m + I | P == I ? m : 0;                  /* best loses K: (stale)mate*/
    if (z && d > 2)
    { *c = 'a' + (X & 7); c[1] = '8' - (X >> 4); c[2] = 'a' + (Y & 7); c[3] = '8' - (Y >> 4 & 7); c[4] = 0;
      char buff[150];
    }
  }                                             /*    encoded in X S,8 bits */
  k ^= 24;                                      /* change sides back        */
  --Z; return m += m < e;                       /* delayed-loss bonus       */
}
void gameOver() {
  for (;;);
}
void bkp() {
  for (int i = 0; i < 16 * 8 + 1; i++) {
    bk[i] = b[i];
  }
}
void serialBoard(){
  pieceCountNew = 0;
  Serial.println("  +-----------------+");
  for(int i=0; i<8; i++){
        Serial.print(' ');
        Serial.print(8-i);
        Serial.print("| ");
        for(int j=0; j<8; j++){
            char c = sym[b[16*i+j]&15];
            Serial.print(c);
            Serial.print(' ');
            if (c == '.'){
              copyBoard[i][j]=0;
            }
            else {
              copyBoard[i][j]=1;
              pieceCountNew += 1;
            } 
        }
        Serial.println('|');
  }
  Serial.println("  +-----------------+");
  Serial.println("   a b c d e f g h");
}

//---------------Movement-------------------
int conquerFunction(int board[8][8], int conquer){
          int flag1 = 0;
          int flag2 = 0;
          int flag3 = 0;
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
      
          
          int rowInput[8]= {A8, A9, A10, A11, A12, A13, A14, A15}; 
            flag1 = 0;
            flag2 = 0;
            for (int j = 0; j < 8;j++){
             for(int i = 22; i < 30; i++){
               digitalWrite(i, HIGH);
               chessboard[22-i][j] = analogRead(rowInput[j]) - calibration[j][i-22];
               if (chessboard[22-i][j] < -5){
                chessboard[22-i][j] = 1;
                if (chessboard[22-i][j] != board[j][i-22]){
                  conquer = 1;
                  Serial.println("Computer Conquer");
                }
               } 
               else{
                chessboard[22-i][j] = 0;

               }
               digitalWrite(i, LOW);
             }
            }
            return conquer;
}

void movement(char lastM[5], int count, int conquer){
    char rows[8] = {'1','2','3','4','5','6','7','8'};
    char columns[8] = {'a','b','c','d','e','f','g','h'};
    
    int rowStep[8] = {2075,1805,1515,1220, 945, 650, 365, 70};
    int columnStep[8] = {430,715,1020,1300,1590, 1885,2175,2450};
    int countStepConquerX[2] = {140,250};
    int rowStepConquer[16] = {2075,2075,1805,1805,1515,1515,1220,1220, 945, 945,650, 650,365,365,70,70}; 
    int coordinateOneX = 0;
    int coordinateOneY = 0;
    int coordinateTwoX = 0;
    int coordinateTwoY = 0;

    int rotationOneX = 0;
    int rotationOneY = 0;
    int rotationTwoX = 0;
    int rotationTwoY = 0;

    int flag1 = 1;
    int flag2 = 1;

    const int magnet = 11;
    pinMode(magnet, OUTPUT); 
    // Break down given character array into coordinates
    for (int i=0; i<8; i++){
      if(lastM[0] == columns[i]){
        coordinateOneX = i;
        }
      if(lastM[2] == columns[i]){
        coordinateTwoX = i;
        }
    }
    for (int i=0; i<8; i++){
      if(lastM[1] == rows[i]){
        coordinateOneY = i;
        }
      if(lastM[3] == rows[i]){
        coordinateTwoY = i;
        }
    }
    
    int flagCastle = 0;
        
    rotationOneX = columnStep[coordinateOneX];
    rotationOneY = rowStep[coordinateOneY];

    rotationTwoX = columnStep[coordinateTwoX];
    rotationTwoY = rowStep[coordinateTwoY];

    // Determine what type of movement it is
    // Conquer ------------------------------------------------------------------------------------------------
    if (conquer==1){
      //move to conquered piece square
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationTwoX);
        stepper2.moveTo(rotationTwoX);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      //Move to initial Y coordinate
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationTwoY);
        stepper2.moveTo(-rotationTwoY);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      // Turn on Magnet
      digitalWrite(magnet, HIGH);
      // move over by half a square towards user
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-142);
        stepper2.moveTo(-142);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      // move to edge of board
      int toEdge = rotationTwoX - 250;
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(toEdge);
        stepper2.moveTo(-toEdge);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
     // Travel to designated y slot for placement
      int toPlaceholder = rotationTwoY + 142 - rowStepConquer[16-count];
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(toPlaceholder);
        stepper2.moveTo(toPlaceholder);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1; 
     // Places in proper column
     if (count%2 == 0){
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(countStepConquerX[0]);
        stepper2.moveTo(-countStepConquerX[0]);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;            
     }
     else{
    while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(countStepConquerX[1]);
        stepper2.moveTo(-countStepConquerX[1]);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
     }
     // Turns off magnet and homes the motor
     digitalWrite(magnet, LOW);
      limitswitch.loop();
      while (digitalRead(10) == HIGH){
          stepper1.move(200);
          stepper2.move(200);
          stepper1.run();
          stepper2.run();
      }
      
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
                        
      limitswitch2.loop();
      while (digitalRead(9) == HIGH){
      stepper1.move(200);
      stepper2.move(-200);
      stepper1.run();
      stepper2.run();
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);      
      
    }

    
    // Castle ------------------------------------------------------------------------------------------------
    if ((((coordinateOneX == 4) && (coordinateOneY == 7)) && ((coordinateTwoX == 2) && (coordinateTwoY == 7))) || (((coordinateOneX == 4) && (coordinateOneY == 7)) && ((coordinateTwoX == 6) && (coordinateTwoY == 7)))){
      flagCastle = 1;
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationOneX);
        stepper2.moveTo(rotationOneX);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      //Move to initial Y coordinate
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationOneY);
        stepper2.moveTo(-rotationOneY);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      // Turn on Magnet
      digitalWrite(magnet, HIGH);      

    // move king forwards


    // Move king over to two
    if (coordinateTwoX == 2){
    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(590);
      stepper2.moveTo(-590);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1; 
        while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(60);
      stepper2.moveTo(60);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1;
    
    digitalWrite(magnet, LOW);
    
    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(590);
      stepper2.moveTo(-590);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1; 

    digitalWrite(magnet, HIGH);

    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(-180);
      stepper2.moveTo(-180);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1;

    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(-910);
      stepper2.moveTo(910);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1; 

    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(170);
      stepper2.moveTo(170);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1;
        
           
    }

    else{
    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(-590);
      stepper2.moveTo(590);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1; 
        while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(60);
      stepper2.moveTo(60);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1;
    
    digitalWrite(magnet, LOW);   

while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(-250);
      stepper2.moveTo(250);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1; 

    digitalWrite(magnet, HIGH);

    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(-180);
      stepper2.moveTo(-180);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1;

    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(565);
      stepper2.moveTo(-565);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1; 

    while (flag1 == 1 && flag2 == 1){
      stepper1.moveTo(170);
      stepper2.moveTo(170);
      stepper1.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
        flag1 = 0;
        flag2 = 0;
      }
    }
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    flag1 = 1;
    flag2 = 1;

       
    }
    
    // Turn off Magnet
    digitalWrite(magnet, LOW);
    //
     
    }    
    // Horizontal ------------------------------------------------------------------------------------------------
    if ((coordinateOneY == coordinateTwoY) && (flagCastle == 0)){
      // Move to coordinate One
      // Move to initial X coordinate
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationOneX);
        stepper2.moveTo(rotationOneX);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      //Move to initial Y coordinate
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationOneY);
        stepper2.moveTo(-rotationOneY);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      // Turn on Magnet
      digitalWrite(magnet, HIGH);
      rotationTwoX = abs(columnStep[coordinateTwoX]-columnStep[coordinateOneX]);
      rotationTwoY = abs(rowStep[coordinateTwoY]-rowStep[coordinateOneY]);
      
      if (coordinateTwoX > coordinateOneX){
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationTwoX);
        stepper2.moveTo(rotationTwoX);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;        
      }
      else{
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(rotationTwoX);
        stepper2.moveTo(-rotationTwoX);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;         
      }
    }

    // vertical ------------------------------------------------------------------------------------------------
    if (coordinateOneX == coordinateTwoX){
      // Move to coordinate One
      // Move to initial X coordinate
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationOneX);
        stepper2.moveTo(rotationOneX);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      //Move to initial Y coordinate
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationOneY);
        stepper2.moveTo(-rotationOneY);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      // Turn on Magnet
      digitalWrite(magnet, HIGH);
      rotationTwoX = abs(columnStep[coordinateTwoX]-columnStep[coordinateOneX]);
      rotationTwoY = abs(rowStep[coordinateTwoY]-rowStep[coordinateOneY]);
      
      if (coordinateTwoY > coordinateOneY){
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(rotationTwoY+55);
        stepper2.moveTo(rotationTwoY+55);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;        
      }
      else{
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationTwoY);
        stepper2.moveTo(-rotationTwoY);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;         
      }
    }



    // knight ------------------------------------------------------------------------------------------------
    int rowChange = abs(coordinateOneY - coordinateTwoY);
    int colChange = abs(coordinateOneX - coordinateTwoX);

    // height movement
    int heightRotation = 293*(coordinateOneY - coordinateTwoY);
    int sideRotationRight = (280*(coordinateTwoX - coordinateOneX))-140;
    int sideRotationLeft = (280*(coordinateOneX - coordinateTwoX))-140;

    if ((rowChange == 2 && colChange == 1) || (rowChange == 1 && colChange == 2)){
      // Move to coordinate One
      // Move to initial X coordinate
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationOneX);
        stepper2.moveTo(rotationOneX);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      //Move to initial Y coordinate
      while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-rotationOneY);
        stepper2.moveTo(-rotationOneY);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
      }
      stepper1.setCurrentPosition(0);
      stepper2.setCurrentPosition(0);
      flag1 = 1;
      flag2 = 1;
      // Turn on Magnet
      digitalWrite(magnet, HIGH);
            
      // if knight is moving to right
      if (coordinateOneX < coordinateTwoX){
        //move knight over to line
        while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(-142);
        stepper2.moveTo(142);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
        }
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        flag1 = 1;
        flag2 = 1;
        while (flag1 == 1 && flag2 == 1){
          stepper1.moveTo(-heightRotation);
          stepper2.moveTo(-heightRotation);
          stepper1.run();
          stepper2.run();
          if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
            flag1 = 0;
            flag2 = 0;
          }
        }
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        flag1 = 1;
        flag2 = 1;

        
        while (flag1 == 1 && flag2 == 1){
          stepper1.moveTo(-sideRotationRight);
          stepper2.moveTo(sideRotationRight);
          stepper1.run();
          stepper2.run();
          if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
            flag1 = 0;
            flag2 = 0;
          }
        }
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        flag1 = 1;
        flag2 = 1;
        
     }

      else{
        //move knight over to line
        while (flag1 == 1 && flag2 == 1){
        stepper1.moveTo(142);
        stepper2.moveTo(-142);
        stepper1.run();
        stepper2.run();
        if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
          flag1 = 0;
          flag2 = 0;
        }
        }
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        flag1 = 1;
        flag2 = 1;
        while (flag1 == 1 && flag2 == 1){
          stepper1.moveTo(-heightRotation);
          stepper2.moveTo(-heightRotation);
          stepper1.run();
          stepper2.run();
          if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
            flag1 = 0;
            flag2 = 0;
          }
        }
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        flag1 = 1;
        flag2 = 1;

        
        while (flag1 == 1 && flag2 == 1){
          stepper1.moveTo(sideRotationLeft);
          stepper2.moveTo(-sideRotationLeft);
          stepper1.run();
          stepper2.run();
          if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
            flag1 = 0;
            flag2 = 0;
          }
        }
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        flag1 = 1;
        flag2 = 1;
        
      }
               
      }
//    int rowChange = abs(coordinateOneY - coordinateTwoY);
//    int colChange = abs(coordinateOneX - coordinateTwoX);
      if (rowChange == colChange){
        // Go to initial x and y coordinate
        while (flag1 == 1 && flag2 == 1){
          stepper1.moveTo(-rotationOneX);
          stepper2.moveTo(rotationOneX);
          stepper1.run();
          stepper2.run();
          if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
            flag1 = 0;
            flag2 = 0;
          }
        }
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        flag1 = 1;
        flag2 = 1;
        //Move to initial Y coordinate
        while (flag1 == 1 && flag2 == 1){
          stepper1.moveTo(-rotationOneY);
          stepper2.moveTo(-rotationOneY);
          stepper1.run();
          stepper2.run();
          if (stepper1.distanceToGo() == 0 && stepper2.distanceToGo() == 0){
            flag1 = 0;
            flag2 = 0;
          }
        }
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        flag1 = 1;
        flag2 = 1;
        // Turn on Magnet
        digitalWrite(magnet, HIGH);
        // determine slope
        int slope = (coordinateTwoY - coordinateOneY)/(coordinateTwoX - coordinateOneX);
        int slopeMove = (coordinateTwoY - coordinateOneY)*605;

        if (slope == 1){
          while (flag1 == 1 && flag2 == 1){
            stepper2.moveTo(slopeMove);
            stepper2.run();
            if (stepper2.distanceToGo() == 0){
              flag1 = 0;
              flag2 = 0;
            }
        }
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        flag1 = 1;
        flag2 = 1;                  
        }
        else{
          while (flag1 == 1 && flag2 == 1){
            stepper1.moveTo(slopeMove);
            stepper1.run();
            if (stepper1.distanceToGo() == 0){
              flag1 = 0;
              flag2 = 0;
            }          
        }
        stepper1.setCurrentPosition(0);
        stepper2.setCurrentPosition(0);
        flag1 = 1;
        flag2 = 1;           
        }
      }

    

    digitalWrite(magnet, LOW); 
    delay(1000);
    limitswitch.loop();
while (digitalRead(10) == HIGH){
    stepper1.move(200);
    stepper2.move(200);
    stepper1.run();
    stepper2.run();
}

stepper1.setCurrentPosition(0);
stepper2.setCurrentPosition(0);
                  
limitswitch2.loop();
while (digitalRead(9) == HIGH){
stepper1.move(200);
stepper2.move(-200);
stepper1.run();
stepper2.run();
}
stepper1.setCurrentPosition(0);
stepper2.setCurrentPosition(0);

}
