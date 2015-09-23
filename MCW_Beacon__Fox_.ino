/*
 * Modulated CW Beacon for fox hunting
 * Created for the MSOE ARC (W9HHX)
 * 
 * @author Adam Meek (adam.w.meek@gmail.com) KC9ZQJ
 */

// --- Settings below this line --- //

// Message to broadcast
char *   message = "W9HHX/B";   

// Length of the pause between broadcasts (sec)
#define  pauseLen  5             

// Speed which Morse code will be keyed (Words / min)
#define  keySpeed  15             

// --- Code below this line --- //
// --- Do not change unless you know what you are doing! --- //

char i;
char *morseTable[53] = {

  ".-",         // a
  "-..",        // b
  "-.-.",       // c
  "-..",        // d
  ".",          // e
  "..-.",       // f
  "--.",        // g
  "....",       // h
  "..",
  ".---",
  "-.-",
  ".-..",
  "--",         // m
  "-.",
  "---",
  ".--.",
  "--.-",
  ".-.",
  "...",
  "-",
  "..-",
  "...-",
  ".--",        // w
  "-..-",       // x
  "-.--",       // y
  "--..",       // z
  "-----",      // 0
  ".----",      // 1
  "..---",      // 2
  "...--",      // 3
  "....-",      // 4
  ".....",      // 5
  "-....",      // 6
  "--...",      // 7
  "---..",      // 8
  "----.",      // 9 
  "-.-.--",     // !
  ".-..-.",     // "
  "...-..-",    // $
  ".-...",      // &
  ".----.",     // '
  "-.--.",      // (
  "-.--.-",     // )
  ".-.-.",      // + 
  "--..--",     // ,
  "-...-",      // - 
  ".-.-.-",     // .
  "-..-.",      // /
  "---...",     // : 
  "-.-.-.",     // ;
  "-...-",      // =
  "..--..",     // ?
  ".--.-."      // @
};

char nextChar;
int unitInMs;

#define ledPin  13
#define pwmPin  7

void keyChar(char toKey);

void setup() {
  
  i = 0;

  /*
   * Calculating the delay times:
   * The definition of a "word" (eg, words / min) is "PARIS"
   *  - This is 50 units of time total (inc. space between after word):
   *    .--.  .-   .-.   ..   ... 
   *    11  3  5 3  7  3  3 3  5  7  = 50
   *  
   * 1 min    1 word    60 sec   1000 ms 
   * ------  --------  -------  --------   = (1/wpm) * 60,000 / 50 ms/unit
   * 1 word   50 units  1 min    1 sec
   * 
   * = 60,000 / (50 * wpm) 
   * 
   * = 6,000 / (5 * wpm)
   */

   
   unitInMs = 6000 / (5 * keySpeed);

   pinMode(ledPin, OUTPUT);

   delay(1000 * pauseLen);

}

void loop() {

  nextChar = message[i];

  if(nextChar != '\0'){

    keyChar(nextChar);
    i++;
    
  }
  else{

    delay(1000 * pauseLen);
    i = 0;
  }


}

// -- Implementation -- //

void keyChar(char toKey){

  // First, lookup char in morseTable

  if(toKey >= 65 && toKey <= 90){       // Upper case
    toKey = toKey - 65;
  }
  else if(toKey >= 97 && toKey <= 122){ // Lower case 
    toKey = toKey - 97;
  }
  else if(toKey >= 48 && toKey <= 57){  // Numbers 
    toKey = toKey - 22;
  }
  else if(toKey == 33){                 // !
    toKey = 36;
  }
  else if(toKey == 34){                 // "
    toKey = 37;
  }
  else if(toKey == 35){                 // $
    toKey = 38;
  }
  else if(toKey >= 38 && toKey <= 41){ // & to )
    toKey++;
  }
  else if(toKey >= 43 && toKey <= 47){ // + to /
    //do nothing, these parts line up 
  }
  else if(toKey == 33){                 // :
    toKey = 48;
  }
  else if(toKey == 34){                 // ;
    toKey = 49;
  }
  else if(toKey == 35){                 // =
    toKey = 50;
  }
  else if(toKey == 63){                 // ?
    toKey = 51;
  }
  else if(toKey == 64){                 // @
    toKey = 52;
  }
  else{

    // treat it as a space
    delay(7 * unitInMs);
    return;
  }

  char j = 0;
  char next;

  // Now, parse through entry in morse table and key dits and dahs
  do{
    next = morseTable[toKey][j];
    j++;

    /*
     * International Morse Code Timing Rules:
     * 1. length of dit         1 unit
     * 2. length of dah         3 units
     * 3. pause between keys    1 unit
     * 4. pause between letters 3 units
     * 5. pause between words   7 units
     *  
     */
    if(next == '.'){
      // dit
      digitalWrite(ledPin, HIGH);
      tone(pwmPin, 800, unitInMs);
      delay(unitInMs);
      digitalWrite(ledPin, LOW);
    }
    else if(next == '-'){
      // dah
      digitalWrite(ledPin, HIGH);
      tone(pwmPin, 800, 3*unitInMs - 30);
      delay(3 * unitInMs); 
      digitalWrite(ledPin, LOW);
    }

    delay(unitInMs);
    
  }while(next != '\0');

  delay(3 * unitInMs);
  
}






