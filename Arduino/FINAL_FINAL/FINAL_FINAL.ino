// Initialization for chess engine code

#define W while
#define M 0x88
#define S 128
#define I 8000

#define MYRAND_MAX 65535     /* 16bit pseudo random generator */
long  N, T;                  /* N=evaluated positions+S, T=recursion limit */
short Q, O, K, R, k=16;      /* k=moving side */
char *p, c[5], Z;            /* p=pointer to c, c=user input, computer output, Z=recursion counter */

int copyBoard[8][8] = { {1, 1, 1, 1, 1, 1, 1, 1},
{1, 1, 1, 1, 1, 1, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{1, 1, 1, 1, 1, 1, 1, 1},
{1, 1, 1, 1, 1, 1, 1, 1}};
int chessboard[8][8];
int start[4];
int copyBoard2[8][8];
int backup[8][8] = { {1, 1, 1, 1, 1, 1, 1, 1},
{1, 1, 1, 1, 1, 1, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{1, 1, 1, 1, 1, 1, 1, 1},
{1, 1, 1, 1, 1, 1, 1, 1}};

int flag = 1;

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

// Initialization for LED screen
# include <LiquidCrystal.h>
// Vss = ground, VDD = power, V0 = resistor, Rw = ground, A = power, K = ground
const int rs = 39, en = 37, d4 = 41, d5 =33, d6 = 35, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Initialization for Movement code
# include <Stepper.h>

// global variables
// needed for specific motor, not always this value
int stepsPerRev = 200;
int motSpeed = 140;
int dt = 500;

// Initialize variable for conquering: if 0 then not conquered, if 1 then conquered
int conquer = 0;
// counter for how many pieces were previously conquered to know which spot to move piece to
int counter = 0;

// Initialize limit switch
int pin1 = 22;
int pin2 = 24;
#include <ezButton.h>
ezButton limitSwitch1(pin1); // create ezButton object that attach to pin1
ezButton limitSwitch2(pin2); // create ezButton object that attach to pin2

// how to increment, sequence is diff for diff motors
// stepper 1 is for left motor
// stepper 2 is for right motor
// Stepper format: myStepper#(stepsPerREv, IN1pin, IN3pin, IN2pin, IN4pin) 
Stepper myStepper2(stepsPerRev, 13, 12, 11, 10);
Stepper myStepper1(stepsPerRev, 7, 6, 5, 4);

float sqSize = 2.5; // inches
float sqMid = 0.5*sqSize; // distance from edge to center of square

// set up programs for motor movement 
// See drive drawing for location of motors --> counterclockwise is considered positive
// Move bar up
void Right(int numbersteps) {
  for (int s = 0; s < numbersteps; s++){
    myStepper1.step(1);
    myStepper2.step(-1);
  }
  }
// Move bar down
void Left(int numbersteps) {
  for (int s = 0; s < numbersteps; s++){
  myStepper1.step(-1);
  myStepper2.step(1);
  }
}

// Move slider left
void Up(float numbersteps) {
  for (int s = 0; s < numbersteps; s++){
  myStepper1.step(1);
  myStepper2.step(1);
  }
}                                                                                                                                                                                                                                                             

// Move slider right
void Down(float numbersteps) {
  for (int s = 0; s < numbersteps; s++){
  myStepper1.step(-1);
  myStepper2.step(-1);
  }
}

// #############################################################################################################################################################
// SETUP FUNCTIONS

void setup() {
  // Setup for chess engine code
  Serial.begin(9600);

  // Setup for movement code
  myStepper1.setSpeed(motSpeed);
  myStepper2.setSpeed(motSpeed);

  // Setup for limit switches
  limitSwitch1.setDebounceTime(50);
  limitSwitch2.setDebounceTime(50);
  
  // Setup for LCD (number of columns, number of rows)
  lcd.begin(16, 2);
  lcd.clear();

  // Set up Electromagnet code
  int elecmag = 100;
  int time = 500;
  pinMode(2, OUTPUT); // sets pin 2 as output
  digitalWrite(2, LOW); // turn off electromagnet

  // Setup for reed switch matrix
  for(int iOut = 42; iOut < 53; iOut++)
    (iOut <= 49) ? (pinMode(iOut, OUTPUT)) : (pinMode(iOut, INPUT));
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

  // Home gantry
  limitSwitch1.loop();
  while (digitalRead(22) == HIGH){
    Up(20);
  }
  limitSwitch2.loop();
  while (digitalRead(24) == HIGH){
    Left(20);
  }
  Right(500);
  Down(500);
  
  // this portion sets up the initial board that is seen by the user 
  lastH[0] = 0;
  // Scans the reed switch matrix to see if it is set up properly 
  // define arrays for input and output pins
  // will be eight of each
  int inputs[8] = {A4, A3, A2, A1, A0, 52, 51, 50};
  int rowDetection[8] = {49, 48, 47, 46, 45, 44, 43, 42};
  bool properSetup = false; 
  int flagSetup = 1;

  while ((flagSetup == 1)){ 
    for (int i=0; i<8; i++){
      // loops are different to invert/flip matrix or can change wires
      for (int j=8; j>0; j--){
        // read all values for a row for one input column
        digitalWrite(rowDetection[j-1], HIGH);
        chessboard[i][j-1] = digitalRead(inputs[i]);
        Serial.print(chessboard[i][j-1]);
        digitalWrite(rowDetection[j-1], LOW);
        } 
      Serial.println();
      }
    Serial.println();
    //checks if this scan matches what is seen in the initial setup 
    flagSetup = 0;
    for (int iRowCheck = 0; iRowCheck < 8; iRowCheck++){
      if (flagSetup == 1){
        break;
        }
      for (int jColCheck = 0; jColCheck < 8; jColCheck++){
        if(copyBoard[iRowCheck][jColCheck] == chessboard[iRowCheck][jColCheck]){
          flagSetup = 0;
            }            
         else{
             flagSetup = 1;
             lcd.clear();
             lcd.print("Incorrect Setup");
             lcd.setCursor(0,1);
             lcd.print("Please fix board");
             delay(100);
             break;
         }
          }
          }
       }
    lcd.clear();
    lcd.print("Proper Setup");
    lcd.setCursor(0, 1);
    lcd.print("Complete");
    delay(2000);

    // Set up copyBoard2
    for (int i=0; i<8; i++){
    // loops are different to invert/flip matrix or can change wires
     for (int j=0; j<8; j++){
        copyBoard2[i][j]= copyBoard[i][j];
   }
    }
    
    // Set up backup board
    for (int i=0; i<8; i++){
    // loops are different to invert/flip matrix or can change wires
     for (int j=0; j<8; j++){
        backup[i][j]= copyBoard[i][j];
   }
    }
  Serial.print("setup complete");

}

// ############################################################################################################################################
// MAIN LOOP

void loop() {
  /* User interface routines */ 
  // scanning of the board (looks to see what move you make on the actual board)
  // checks the board to make sure everything is lined up to what it should be
  // once it says good to go it scans to check what your move is

  int r;
  int jboard = 0;
  // possible options for chess engine to give and to put into chess engine
  String engLet[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
  String engNum[8] = {"8", "7", "6", "5", "4", "3", "2", "1"};
  String engNum2[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};

  int prev_y = 0;
  int prev_x = 0;
  int temp_x = 0;
  int temp_y = 0;
  int next_y = 0;
  int next_x = 0;
  flag = 1;
  lcd.clear();
  lcd.print("Player move");
  // Take move from human by reading the serial monitor input box by calling the getserialchar function
  while (flag == 1) {
    // Scan for piece picked up
    getserialchar(chessboard, copyBoard2);
    Serial.println("Action1");
    // Find why it doesn't match
    for (int iRowCheck = 0; iRowCheck < 8; iRowCheck++){
    // Note: i is rows which corresponds to numbers on board and j is columns which corresponds to letters on board
      for (int jColCheck = 0; jColCheck < 8; jColCheck++){
        // fix method for parsing through function
        if (jColCheck == 0){
          jboard = 8;
        }
        if (jColCheck > 0){
          jboard = (jColCheck + 8) - (2 * jColCheck);
        }
      // Compare locations on board
      if((chessboard[iRowCheck][jboard - 1] == 1) && (copyBoard2[iRowCheck][jColCheck] == 0)){
         lcd.clear();
         lcd.print("Piece added");
         lcd.setCursor(0,1);
         lcd.print("Fix board");
      }         
      if((chessboard[iRowCheck][jboard - 1] == 0) && (copyBoard2[iRowCheck][jColCheck] == 1)){
        // Piece picked up - continue to next loop
        prev_y = jColCheck;
        prev_x = iRowCheck;
        flag = 0;
         }
          }
          }
  }
    // Update Copyboard with first action
    for (int i=0; i<8; i++){
    for (int j=0; j<8 ; j++){
      if ((prev_x == i) && (prev_y == j)){
        // j vertical, i horizontal
        Serial.println();
        Serial.print("i = ");
        Serial.print(i);
        Serial.print("    j = ");
        Serial.print(j);
        Serial.println();
        copyBoard2[i][j] = 0;
      }
    }
  }
  Serial.println();
  Serial.print("prev x ");
  Serial.print(prev_x);
  Serial.print("              prev y");
  Serial.print(prev_y);
  Serial.println();
  Serial.println("--NEW COPY BOARD ACTION 1--");
  for (int i=0; i<8; i++){
    Serial.print(engNum2[i]+" ");
    for (int j=0; j<8; j++){
       Serial.print(copyBoard2[i][j]);
     }
     Serial.println();
   }
    Serial.println("  ABCDEFGH");
    Serial.println();
  flag = 1;
  // Look for second action
  while (flag == 1){
    Serial.println("Action2");
    getserialchar(chessboard, copyBoard2);
  
    // Find why it doesn't match
    for (int iRowCheck = 0; iRowCheck < 8; iRowCheck++){
    // Note: i is rows which corresponds to numbers on board and j is columns which corresponds to letters on board
      for (int jColCheck = 0; jColCheck < 8; jColCheck++){
        // fix method for parsing through function
        if (jColCheck == 0){
          jboard = 8;
        }
        if (jColCheck > 0){
          jboard = (jColCheck + 8) - (2 * jColCheck);
        }
        
      // Compare locations on board
      if((chessboard[iRowCheck][jboard - 1] == 1) && (copyBoard2[iRowCheck][jColCheck] == 0)){
         // piece placed - end move or see if it was put in the same spot
         if ((iRowCheck == prev_x) && (jColCheck == prev_y)){
          lcd.clear();
          lcd.print("Make valid move");
         }
         else{
          flag = 0;
          next_x = iRowCheck;
          next_y = jColCheck;
        }     
      }    
      if((chessboard[iRowCheck][jboard - 1] == 0) && (copyBoard2[iRowCheck][jColCheck] == 1)){
        // possible conquer
        temp_y = jColCheck;
        temp_x = iRowCheck;
        flag = 2;
         }
          }
          }
  }
  // Update Copyboard with conquered action
    if (flag == 2){
     for (int i=0; i<8; i++){
      for (int j=0; j<8 ; j++){
        if ((temp_x == i) && (temp_y == j)){
          // j vertical, i horizontal
          copyBoard2[i][j] = 0;
      }
    }
  }
  }
  Serial.println();
  Serial.print("temp x ");
  Serial.print(temp_x);
  Serial.print("              temp y");
  Serial.print(temp_y);
  Serial.println();
  while (flag == 2){
    Serial.print("ACTION3");
    // scan board for a third time
    getserialchar(chessboard, copyBoard2);

    // Find why it doesn't match
    for (int iRowCheck = 0; iRowCheck < 8; iRowCheck++){
    // Note: i is rows which corresponds to numbers on board and j is columns which corresponds to letters on board
    for (int jColCheck = 0; jColCheck < 8; jColCheck++){
      // fix method for parsing through function
      if (jColCheck == 0){
          jboard = 8;
        }
      if (jColCheck > 0){
          jboard = (jColCheck + 8) - (2 * jColCheck);
        }
      // Compare locations on board
      if((chessboard[iRowCheck][jboard - 1] == 1) && (copyBoard2[iRowCheck][jColCheck] == 0)){
         // piece conquered
         if ((iRowCheck == temp_x) && (jColCheck == temp_y)){
          // proper conquer
          flag = 0;
          next_x = iRowCheck;
          next_y = jColCheck;
          conquer = 1;
         }
         else{
          lcd.clear();
          lcd.print("Invalid move");
          lcd.print("Please fix board");
        }     
      }    
      if((chessboard[iRowCheck][jboard - 1] == 0) && (copyBoard2[iRowCheck][jColCheck] == 1)){
        // Another piece picked up - error
        lcd.clear();
        lcd.print("Make valid move");
         }
          }
    }
  }
    

  // Update Copyboard with final action
  if (flag == 0){
     for (int i=0; i<8; i++){
      for (int j=0; j<8 ; j++){
        if ((next_x == i) && (next_y == j)){
          // j vertical, i horizontal
          copyBoard2[i][j] = 1;
      }
    }
  }
  }

  Serial.println();
  Serial.print("next x ");
  Serial.print(next_x);
  Serial.print("              next y");
  Serial.print(next_y);
  Serial.println();
  
  // Copyboard 2 is new user board
  inputString = "";
  String prev = "";
  String next = "";
          
  lcd.clear();
  lcd.print("Wait");
  lcd.setCursor(0, 1);
  lcd.print("Computer move");

  // Convert indexes from while loop into string to be put into chess engine
  prev += engLet[prev_y];
  prev += engNum[prev_x];
  next += engLet[next_y];
  next += engNum[next_x];
  
  inputString += prev;
  inputString += next; 

  Serial.print("  input String = ");
  Serial.print(inputString);
  
  // array c represents the move that will be entered into the chess engine (user input)
  c[0] = inputString.charAt(0);
  c[1] = inputString.charAt(1);
  c[2] = inputString.charAt(2);
  c[3] = inputString.charAt(3);
  c[4] = 0;
  
  // clear the string:
  inputString = "";
  prev = "";
  next = "";
  start[4];
  stringComplete = false;
  K = *c - 16 * c[1] + 799, L = c[2] - 16 * c[3] + 799; /* parse entered move */
  N = 0;
  T = 0x3F;                                 /* T=Computer Play strength */
  bkp();                                    /* Save the board just in case */
  r = D(-I, I, Q, O, 1, 3);                 /* Check & do the human movement */
  if ( !(r > -I + 1) ) {
    lcd.clear();
    lcd.print("Checkmate");
    lcd.setCursor(0, 1);
    lcd.print("Game over");
    Serial.print("Game over");
    delay(5000);
    
    gameOver();
  }
  if (k == 0x10) {     
    lcd.clear(); /* The flag turn must change to 0x08 */
    lcd.print("Invalid move");
    lcd.setCursor(0,1);
    lcd.print("Reset");
    Serial.println("ERROR");
    reset(chessboard, backup, copyBoard2);
    return;
  }

  strcpy(lastH, c);                         /* Valid human movement */

  mn++;                                     /* Next move */

  K = I;
  N = 0;
  T = 0x3F;                                 /* T=Computer Play strength */
  r = D(-I, I, Q, O, 1, 3);                 /* Think & do*/
  if ( !(r > -I + 1) ) {
    lcd.clear();
    lcd.print("Checkmate");
    lcd.setCursor(0, 1);
    lcd.print("Gave over");
    Serial.print("Game over");
    delay(5000);
  }

  // lastM is what the computer says the next move should be
  strcpy(lastM, c);                         /* Valid ARDUINO movement */
  r = D(-I, I, Q, O, 1, 3);                 
  if ( !(r > -I + 1) ) {
    gameOver();
    Serial.print("Game over");
  }
  // Strings from above
  // String engLet[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
  // int engNum[8] = {"8", "7", "6", "5", "4", "3", "2", "1"};
  // convert computer move to numbers
  int comp_prev_x = 0;
  int comp_prev_y = 0;
  int comp_next_x = 0;
  int comp_next_y = 0;
  Serial.println();
  Serial.print("LAST M");
  Serial.print(lastM);
  Serial.println();
  // convert previous and next moves
  for (int i=0; i<8; i++){
    if(String(lastM[0])==engLet[i]){
      comp_prev_x = i;}
    if(String(lastM[2])==engLet[i]){
      comp_next_x = i;}
  }
    
  for (int i=0; i<8; i++){
    if(String(lastM[1])==engNum[i]){
      comp_prev_y = i;}
    if(String(lastM[3])==engNum[i]){
      comp_next_y = i;}
  }

  // Update Copy Board 2 and check if conquer
  for (int i=0; i<8; i++){
    for (int j=0; j<8 ; j++){
      if ((comp_prev_x == j) && (comp_prev_y == i)){
        // j vertical, i horizontal
        copyBoard2[i][j] = 0;
      }
      if ((comp_next_x == j) && (comp_next_y == i)){
        Serial.println("PASS");
        if (copyBoard2[i][j] == 1){
          conquer = 1;
          Serial.println();
          Serial.print("i and j");
          Serial.println();
          Serial.print(i);
          Serial.println();
          Serial.print(j);
          Serial.println();
        }
        else{
        copyBoard2[i][j] = 1;
      }
    }
  }
  }
  
  // Note: lastM gives output of prevnext for the piece the engine will move
  Serial.println("  lastM = ");
  Serial.print(lastM);

  // Print new copy board
  Serial.println();
  Serial.println("--NEW COPY BOARD BEFORE MOVEMENT--");
  for (int i=0; i<8; i++){
    Serial.print(engNum[i]+" ");
     for (int j=0; j<8; j++){
       Serial.print(copyBoard2[i][j]);
     }
     Serial.println();
   }
  Serial.println("  ABCDEFGH");
  Serial.println("COUNTER IN MAIN LOOP");
  Serial.println();
  Serial.print(counter);
  Serial.println();
  // Create new movement array based on if conquered
  if (conquer == 1){
    Serial.println("CONQUERED");
    conquered_movement(lastM);
  }
  // Call movement function using chess engine output
  movement(lastM);
  conquer = 0;
  
  int flagSetup = 1;
  int rowDetection[8] = {49, 48, 47, 46, 45, 44, 43, 42};
  int inputs[8] = {A4, A3, A2, A1, A0, 52, 51, 50};
  
  // Create loop to check to make sure the new board is the same as the copy board
  while ((flagSetup == 1)){ 
    // Print new copy board
    Serial.println();
    Serial.println("--NEW COPY BOARD IN CHECK --");
    for (int i=0; i<8; i++){
      Serial.print(engNum[i]+" ");
      for (int j=0; j<8; j++){
        Serial.print(copyBoard2[i][j]);
     }
     Serial.println();
   }
  Serial.println("  ABCDEFGH");
    for (int i=0; i<8; i++){
      // loops are different to invert/flip matrix or can change wires
      for (int j=8; j>0; j--){
        // read all values for a row for one input column
        digitalWrite(rowDetection[j-1], HIGH);
        chessboard[i][j-1] = digitalRead(inputs[i]);
        digitalWrite(rowDetection[j-1], LOW);
        Serial.print(chessboard[i][j-1]);
        } 
      Serial.println();
      }
    Serial.println();
   for (int iRowCheck = 0; iRowCheck < 8; iRowCheck++){
      for (int jColCheck = 0; jColCheck < 8; jColCheck++){
        if (jColCheck == 0){
          jboard = 8;
        }
        if (jColCheck > 0){
          jboard = (jColCheck + 8) - (2 * jColCheck);
        }
        if(copyBoard2[iRowCheck][jColCheck] == chessboard[iRowCheck][jboard - 1]){
          flagSetup = 0;
            }            
        else{
             flagSetup = 2;
             lcd.clear();
             lcd.print("Move Incomplete");
             lcd.setCursor(0,1);
             lcd.print("Fix Board");
             Serial.println("FIX BOARD");
             break;
         }
          }
        if (flagSetup == 2){
          flagSetup = 1;
          break;
        }
          }
       }
  // Update backup board
  for (int i=0; i<8; i++){
    for (int j=0; j<8 ; j++){
        backup[i][j] = copyBoard2[i][j];
      }
    }
  
  lcd.clear();
  lcd.print("Player move");
     
}


//######################################################################################################

// This function will be used for movement
// string input is the previous and next move (ex. chess engine output a6b6)
void movement(const char* lastM){
  String engLet2[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
  String engNum2[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
  float sqSize = 2.5; // inches
  float sqMid = 0.5*sqSize; // distance from edge to center of square
  float steps_per_inch = 257 / 2.25;
  Serial.println();
  Serial.print("START MOVEMENT PORTION");
  Serial.println();
  // Set initial values
  int comp_prev_x = 0;
  int comp_prev_y = 0;
  int comp_next_x = 0;
  int comp_next_y = 0;
  
  // convert previous and next moves
  for (int i=0; i<8; i++){
    if(String(lastM[0])==engLet2[i]){
      comp_prev_x = i;}
    if(String(lastM[2])==engLet2[i]){
      comp_next_x = i;}
  }
    
  for (int i=0; i<8; i++){
    if(String(lastM[1])==engNum2[i]){
      comp_prev_y = 7-i;}
    if(String(lastM[3])==engNum2[i]){
      comp_next_y = 7-i;}
  }
  
  Serial.println();
  Serial.print(comp_prev_x);
  Serial.print(comp_prev_y);
  Serial.print(comp_next_x);
  Serial.print(comp_next_y);
  Serial.println();
   // travel lines
  float coordSideLR[8];
  float coordSideUD[8];

  // these coordinates for moving pieces
  for (int i=0; i<8; i++){
    coordSideLR[i] = (sqSize*i) + 4.6;
    }
  for (int i=1; i<8; i++){
    coordSideUD[i] = (sqSize*i) - sqMid + 0.8;
  }
  coordSideUD[0] = coordSideUD[1];
    
  // x is columns (so letters)
  // y is rows (so numbers)

  // always move to top left corner of piece, then in unless it is the top row
  // top row moves bottom left corner then in
  float inx = 1.23;
  float iny = 1.78;
  // Set new array with steps for gantry motors to move - assume starts and ends at (0,0)
  // format is [xCur, yCur, xMid, yMid, x Mid, yMid, xDes, yDes, xMid, yMid, xMid, yMid, xFin, yFin]
  float gantryMovements[14];
  if (comp_prev_y == 0){
    gantryMovements[2] = inx;
    gantryMovements[3] = -iny;
    gantryMovements[4] = -inx;
    gantryMovements[5] = inx;
  }
  else{
    gantryMovements[2] = inx;
    gantryMovements[3] = iny;
    gantryMovements[4] = -inx;
    gantryMovements[5] = -iny;
  }
  if (comp_next_y == 0){
    gantryMovements[8] = inx;
    gantryMovements[9] = -iny;
    gantryMovements[10] = -inx;
    gantryMovements[11] = iny;    
  }
  else{
    gantryMovements[8] = inx;
    gantryMovements[9] = iny;
    gantryMovements[10] = -inx;
    gantryMovements[11] = -iny;
  }

  int compare[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  
  // Current coordinate location
  for (int i=0; i<8; i++){
    if (comp_prev_x == compare[i]){
      gantryMovements[0] = coordSideLR[i];
    }
    
    if (comp_prev_y == compare[i]){
      gantryMovements[1] = coordSideUD[i];
    }
  }
  Serial.println();
  for (int i=0; i<14; i++){
    Serial.print(coordSideLR[i]);
    Serial.println();
  }
  // Vector between current coordinate location and desired coordinate location
  for (int i=0; i<8; i++){
    if (comp_next_x == compare[i]){
      gantryMovements[6] = coordSideLR[i]-gantryMovements[0];
      gantryMovements[12] = -coordSideLR[i] + 1;
    }
    if (comp_next_y == compare[i]){
      gantryMovements[7] = coordSideUD[i]-gantryMovements[1];
      gantryMovements[13] = -coordSideUD[i] + 1;
    }
  }

  // Convert gantryMovements to number of steps
  // format is [xCur, yCur, xMid, yMid, x Mid, yMid, xDes, yDes, xMid, yMid, xMid, yMid, xFin, yFin]
  
  float stepMovements[14];
  float numSteps;
  for (int i=0; i<14; i++){
    numSteps = 0;
    if (gantryMovements[i] != 0){
      Serial.print(gantryMovements[i]);
      Serial.print("                  ");
      numSteps = gantryMovements[i] * steps_per_inch;
      Serial.print(numSteps);
      Serial.print("                  ");
      stepMovements[i] = int(numSteps);
    }
    else {
      stepMovements[i] = 0;
      Serial.print("ZERO                  ");
      Serial.print(numSteps);
    }
    Serial.println();
  }
  Serial.println();
  Serial.print("STEP MOVEMENTS");
  for (int i=0; i<14; i++){
    Serial.print(stepMovements[i]);
  }
  Serial.println();

  // Home gantry
  limitSwitch1.loop();
  while (digitalRead(22) == HIGH){
    Up(20);
  }
  limitSwitch2.loop();
  while (digitalRead(24) == HIGH){
    Left(20);
  }
    
  // Move Stepper Motor to current location
  for (int i=0; i<4; i++){
    if ( (i == 0) or (i%2) == 0){
      if (stepMovements[i] >= 0){
        Right(stepMovements[i]);
      }
      else{
        Left(abs(stepMovements[i]));
    }
    }
    else {
      if (stepMovements[i] >= 0){
        Down(stepMovements[i]);
      }
      else {
        Up(abs(stepMovements[i]));
      }
    }
  }
  Serial.println("Current location");
  
  // Activate electromagnet
  digitalWrite(2, HIGH);

  
  // Move Stepper Motor to desired location
 for (int i=4; i<10; i++){
    if ( (i == 0) or (i%2) == 0){
      if (stepMovements[i] >= 0){
        Right(stepMovements[i]);
      }
      else{
        Left(abs(stepMovements[i]));
      }
    }
    else {
      if (stepMovements[i] >= 0){
        Down(stepMovements[i]);
      }
      else {
        Up(abs(stepMovements[i]));
      }
    }
    }

  // Deactivate electromagnet to place 
  digitalWrite(2, LOW);
  
  Serial.println("Desired location");
  delay(300);

  // Move Stepper Motor to origin
  for (int i=10; i<14; i++){
    if ( (i == 0) or (i%2) == 0){
      if (stepMovements[i] >= 0){
        Right(stepMovements[i]);
      }
      else{
        Left(abs(stepMovements[i]));
    }
    }
    else {
      if (stepMovements[i] >= 0){
        Down(stepMovements[i]);
      }
      else {
        Up(abs(stepMovements[i]));
      }
    }
    }
  Serial.println("origin");
  
  // Home gantry
  limitSwitch1.loop();
  while (digitalRead(22) == HIGH){
    Up(20);
  }
  limitSwitch2.loop();
  while (digitalRead(24) == HIGH){
    Left(20);
  }
  Right(500);
  Down(500);
  
}

// #######################################################################################################################################################
// Function for additional movement when a piece is conquered
// String is chess engine's output (ex. a6b6)
void conquered_movement(String lastM){
  Serial.println();
  Serial.print("Conquered Movement -------------------");
  Serial.println();
  Serial.println("COUNTER AT START");
  Serial.println();
  Serial.print(counter);
  Serial.println();
  float dist_from_board = 1;
  float dist_between_pieces = 1;
  float side_to_piece = (dist_between_pieces / 2);
  float sqSize = 2.5;
  float sqMid = sqSize / 2;
  // travel lines for going left to right
  float coordSide_LR[10];
  // travel lines for going up and down 
  float coordSide_UD[8];
  // coords to pickup pieces left to right
  float coordMid_LR[10];
  // coord to pickup pieces up and down
  float coordMid_UD[8];
  float steps_per_inch = 257 / 2.25;

  // these coordinates for moving pieces on the board up and down
  for (int i=1; i<8; i++){
    coordSide_UD[i] = sqSize*(i) - sqMid + 0.8;
    }
  coordSide_UD[0] = coordSide_UD[1];
  // these coordinates for getting to pieces left to right
  coordMid_LR[0] = side_to_piece;
  coordMid_LR[1] = dist_between_pieces + side_to_piece;
  for (int i=2; i<10; i++){
    coordMid_LR[i] = (sqSize*(i-2)) + sqMid;
  }
   // these coordinates for moving pieces on the board left and right
   coordSide_LR[0] = 0;
   coordSide_LR[1] = dist_between_pieces + 1;
   for (int i=2; i<10; i++){
    coordSide_LR[i] = (sqSize*(i-2)) + 4.6;
    }

    
  // possible options for chess engine to give
  String engLet2[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
  String engNum2[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
  int comp_next_x = 0;
  int comp_next_y = 0;

  // convert next moves (to know which piece to move off board
  for (int i=0; i<8; i++){
    if(String(lastM[2])==engLet2[i]){
      comp_next_x = i;}
  }
    
  for (int i=0; i<8; i++){
    if(String(lastM[3])==engNum2[i]){
      comp_next_y = 7-i;}
    
  }
  Serial.println("COMP MOVE INDEXES");
  Serial.println();
  Serial.print(comp_next_x);
  Serial.print(comp_next_y);

  // Find desired location off board based on how many pieces conquered so far
  // Start placing pieces on far column(away from playing board), then in close column (by playing board)
  
  // x is columns (so letters)
  // y is rows (so numbers)

  // always move to top left corner of piece, then in except top row
  // top row goes from bottom left corner then in
  float inx = 1.23;
  float iny = 1.78;
  // Set new array with steps for gantry motors to move - assume starts and ends at (0,0)
  // format is [xCur, yCur, xMid, yMid, x Mid, yMid, xDes, yDes, xMid, yMid, xMid, yMid, xFin, yFin]
  float gantryMovements[14];
  if (comp_next_y == 0){
    gantryMovements[2] = inx;
    gantryMovements[3] = -iny;
    gantryMovements[4] = -inx;
    gantryMovements[5] = iny;
  }
  else{
    gantryMovements[2] = inx;
    gantryMovements[3] = iny;
    gantryMovements[4] = -inx;
    gantryMovements[5] = -iny;
  }
  if ((counter == 0) || (counter == 8)){
    gantryMovements[8] = inx;
    gantryMovements[9] = -iny;
    gantryMovements[10] = -inx;
    gantryMovements[11] = iny;    
  }
  else{
    gantryMovements[8] = inx;
    gantryMovements[9] = iny;
    gantryMovements[10] = -inx;
    gantryMovements[11] = -iny;
  }

  int compare[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  
  // Current coordinate location
  for (int i=0; i<8; i++){
    if (comp_next_x == compare[i]){
      gantryMovements[0] = coordSide_LR[i+2];
    }
    
    if (comp_next_y == compare[i]){
      gantryMovements[1] = coordSide_UD[i];
    }
  }
  // temporary variable to identify which column the piece will be placed in
  float temp_x = 0;
  float temp_y = 0;
  // Location for conquered piece to be placed (temporary variables)
  if (counter < 8){
    temp_x = coordSide_LR[0];
    }
  else{
    temp_x = coordSide_LR[1];
    }

  if (counter < 8){
    temp_y = coordSide_UD[counter];
    }
  else{
    counter = counter - 8;
    temp_y = coordSide_UD[counter];
    }

   
  // Vector between current coordinate location and desired coordinate location
  for (int i=0; i<8; i++){
    if (comp_next_x == compare[i]){
      gantryMovements[6] = temp_x - gantryMovements[0];
      gantryMovements[12] = - temp_x;
    }
    if (comp_next_y == compare[i]){
      gantryMovements[7] = temp_y -gantryMovements[1];
      gantryMovements[13] = -temp_y;
    }
  }
  // Convert gantryMovements to number of steps
  // format is [xCur, yCur, xMid, yMid, x Mid, yMid, xDes, yDes, xMid, yMid, xMid, yMid, xFin, yFin]
  
  float stepMovements[14];
  float numSteps;
  for (int i=0; i<14; i++){
    numSteps = 0;
    if (gantryMovements[i] != 0){
      Serial.print(gantryMovements[i]);
      numSteps = gantryMovements[i] * steps_per_inch;
      Serial.println(numSteps);
      stepMovements[i] = int(numSteps);
    }
    else {
      stepMovements[i] = 0;
      Serial.println("ZERO");
      Serial.println(numSteps);
    }
  }

  for (int i=0; i<14; i++){
    Serial.print(stepMovements[i]);
  }
  Serial.println();
  // Home gantry
  limitSwitch1.loop();
  while (digitalRead(22) == HIGH){
    Up(20);
  }
  limitSwitch2.loop();
  while (digitalRead(24) == HIGH){
    Left(20);
  }

  // Move Stepper Motor to current location
  for (int i=0; i<4; i++){
    if ( (i == 0) or (i%2) == 0){
      if (stepMovements[i] >= 0){
        Right(stepMovements[i]);
      }
      else{
        Left(abs(stepMovements[i]));
    }
    }
    else {
      if (stepMovements[i] >= 0){
        Down(stepMovements[i]);
      }
      else {
        Up(abs(stepMovements[i]));
      }
    }
  }
  Serial.println("Current location");
  
  // Activate electromagnet
  digitalWrite(2, HIGH);
  
  // Move Stepper Motor to desired location
  for (int i=4; i<10; i++){
    if ( (i == 0) or (i%2) == 0){
      if (stepMovements[i] >= 0){
        Right(stepMovements[i]);
      }
      else{
        Left(abs(stepMovements[i]));
      }
    }
    else {
      if (stepMovements[i] >= 0){
        Down(stepMovements[i]);
      }
      else {
        Up(abs(stepMovements[i]));
      }
    }
    }

  // Deactivate electromagnet
  digitalWrite(2, LOW);
    
  Serial.println("Desired location");
  delay(300);

  // Move Stepper Motor to origin
  for (int i=10; i<14; i++){
    if ( (i == 0) or (i%2) == 0){
      if (stepMovements[i] >= 0){
        Right(stepMovements[i]);
      }
      else{
        Left(abs(stepMovements[i]));
    }
    }
    else {
      if (stepMovements[i] >= 0){
        Down(stepMovements[i]);
      }
      else {
        Up(abs(stepMovements[i]));
      }
    }
    }
  Serial.println("origin");
  // Update Copy Board 2
  for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            char c = sym[b[16*i+j]&15];
            if (c == '.'){
              copyBoard2[i][j]=0;
            }
            else {
              copyBoard2[i][j]=1;
            }
        }
  }

  counter = counter + 1;
  Serial.println("COUNTER AT END");
  Serial.println();
  Serial.print(counter);
  Serial.println();
  // Home gantry
  limitSwitch1.loop();
  while (digitalRead(22) == HIGH){
    Up(20);
    Serial.println("Test");
  }
  limitSwitch2.loop();
  while (digitalRead(24) == HIGH){
    Left(20);
  }
  Right(500);
  Down(500);
}

//#############################################################################################################################
// Using this format for getserialchar allows the matrices to be changed within the function
// This will be necessary for seeing what move changed
void getserialchar(int chessboard[8][8], int copyBoard2[8][8]) {
  String engNum2[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
  int rowDetection[8] = {49, 48, 47, 46, 45, 44, 43, 42};
  int flagSetup = 1;
  int inputs[8] = {A4, A3, A2, A1, A0, 52, 51, 50};
  int jboard = 0;
  Serial.println();
  Serial.print(" GET SERIAL CHAR FUNCTION");
  Serial.println();
  Serial.print("CHESSBOARD");
  Serial.println();
  while ((flagSetup == 1)){ 
    for (int i=0; i<8; i++){
      // loops are different to invert/flip matrix or can change wires
      for (int j=8; j>0; j--){
        // read all values for a row for one input column
        digitalWrite(rowDetection[j-1], HIGH);
        chessboard[i][j-1] = digitalRead(inputs[i]);
        digitalWrite(rowDetection[j-1], LOW);
        Serial.print(chessboard[i][j-1]);
        } 
      Serial.println();
      }
    Serial.println();
    //checks if this scan matches what is seen in the initial setup 
    // Print new copy board
    Serial.println();
    Serial.println("--NEW COPY BOARD IN GET SERIAL CHAR--");
    for (int i=0; i<8; i++){
    Serial.print(engNum2[i]+" ");
     for (int j=0; j<8; j++){
       Serial.print(copyBoard2[i][j]);
     }
     Serial.println();
   }
    Serial.println("  ABCDEFGH");
    Serial.println();
    Serial.print("Checking values");
    Serial.println();
    for (int iRowCheck = 0; iRowCheck < 8; iRowCheck++){
      if (flagSetup == 0){
        break;
        }
      for (int jColCheck = 0; jColCheck < 8; jColCheck++){
        if (jColCheck == 0){
          jboard = 8;
        }
        if (jColCheck > 0){
          jboard = (jColCheck + 8) - (2 * jColCheck);
        }
        if(copyBoard2[iRowCheck][jColCheck] == chessboard[iRowCheck][jboard - 1]){
          flagSetup = 1;
            }            
         else{
             flagSetup = 0;
             Serial.print("BREAK");
             Serial.println();
             Serial.print("i Row Check = ");
             Serial.print(iRowCheck);
             Serial.println();
             Serial.print("j Row Check = ");
             Serial.print(jColCheck);
             Serial.println(); 
             Serial.print("jboard = ");
             Serial.print(jboard);
             Serial.println();
             Serial.print("chessboard value = ");
             Serial.print(chessboard[iRowCheck][jboard-1]);
             Serial.println();
             Serial.print("copy board value = ");
             Serial.print(copyBoard2[iRowCheck][jColCheck]);
             Serial.println();
             break;
         }
          }
          }
       Serial.println();
       }
       Serial.println();
  }

// #####################################################################################################################
// In case of error, check to see when board is reset to last move (backup)
void reset(int chessboard[8][8], int backup[8][8], int copyBoard2[8][8]){
    String engLet[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    String engNum[8] = {"8", "7", "6", "5", "4", "3", "2", "1"};
    String engNum2[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
    int rowDetection[8] = {49, 48, 47, 46, 45, 44, 43, 42};
    int inputs[8] = {A4, A3, A2, A1, A0, 52, 51, 50};
    int jboard = 0;
    // Create loop to check to make sure the new board is the same as the backup board
    int flagSetup = 1;
    while ((flagSetup == 1)){ 
      // Print new copy board
      Serial.println();
      Serial.println("-- BACKUP BOARD --");
      for (int i=0; i<8; i++){
        Serial.print(engNum[i]+" ");
        for (int j=0; j<8; j++){
          Serial.print(backup[i][j]);
     }
      Serial.println();
    }
    Serial.println("  ABCDEFGH");
    for (int i=0; i<8; i++){
      // loops are different to invert/flip matrix or can change wires
      for (int j=8; j>0; j--){
        // read all values for a row for one input column
        digitalWrite(rowDetection[j-1], HIGH);
        chessboard[i][j-1] = digitalRead(inputs[i]);
        Serial.print(chessboard[i][j-1]);
        digitalWrite(rowDetection[j-1], LOW);
        } 
      Serial.println();
      }
    Serial.println();
   for (int iRowCheck = 0; iRowCheck < 8; iRowCheck++){
      for (int jColCheck = 0; jColCheck < 8; jColCheck++){
        if (jColCheck == 0){
          jboard = 8;
        }
        if (jColCheck > 0){
          jboard = (jColCheck + 8) - (2 * jColCheck);
        }
        if(backup[iRowCheck][jColCheck] == chessboard[iRowCheck][jboard - 1]){
          flagSetup = 0;
            }            
        else{
             flagSetup = 2;
             Serial.println("NEED TO RESET");
             break;
         }
          }
        if (flagSetup == 2){
          flagSetup = 1;
          break;
        }
          }
       }
    // Reset copyboard 2 to equal backup board
    for (int i=0; i<8; i++){
      for (int j=0; j<8 ; j++){
        copyBoard2[i][j] = backup[i][j];
      }
    }
}


// #####################################################################################################################
// CHESS ENGINE LOGIC - DO NOT TOUCH 

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
