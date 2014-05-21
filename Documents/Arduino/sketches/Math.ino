/**
* Set constants
*/
// Set number of math expressions to display
const unsigned int NUMBER_OF_EXPRESSIONS = 10;

// Set random noise pin (must be analog and unused)
const unsigned int PIN_RANDOM = 0;

/**
* Declare variables
*/
// Set the operators
char operators[3] = {'+', '-', '*'};

// Set variables
unsigned int variableA = 0;
unsigned int variableB = 0;

// Set answer
signed int expressionAnswer = 0;

// Set min and max values for the variables
unsigned int variableMax = 0;
unsigned int variableMin = 1;

// Whether or not the program is done
boolean done = false;

/**
* Setup program
*/
void setup() {
  // Start serial output
  Serial.begin(9600);
}

/**
* Run program
*/
void loop() {
  // Run program
  if(!done) {
    // Print expressions
    for(int i = 0; i < NUMBER_OF_EXPRESSIONS; i++) {
      // Create new random seed
      randomSeed(analogRead(PIN_RANDOM));

      // Get random operator
      char expressionOperator = operators[random(0, 3)];

      // Set max value for variables
      variableMax = (expressionOperator == '+' || expressionOperator == '-') ? 100 : 10;

      // Get random variables
      variableA = random(variableMin, variableMax);
      variableB = random(variableMin, variableMax);

      // Calculate answer
      switch(expressionOperator) {
        case '+':
          expressionAnswer = variableA + variableB;
          break;
        case '-':
          expressionAnswer = variableA - variableB;
          break;
        case '*':
          expressionAnswer = variableA * variableB;
          break;
      }

      // Print expression number
      Serial.print(i + 1);
      Serial.print(": ");

      // Print variable A
      Serial.print(variableA);

      // Print operator
      Serial.print(" ");
      Serial.print(expressionOperator);
      Serial.print(" ");

      // Print variable B
      Serial.print(variableB);

      // Print answer
      Serial.print(" = ");
      Serial.println(expressionAnswer);

      delay(1000);
    }

    // Set boolean to true
    done = true;
  }
}
