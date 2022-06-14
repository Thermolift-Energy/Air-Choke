
//Declare pin functions on Arduino
#define STEP_PIN 2 //defines which pin the step pin is on
#define DIR_PIN 3 //defines which pin the direction pin is on
#define EN_PIN 4 //defines which pin the enable is on
#define ANALOG_IN A0 //defines which pin the anlog signal is read on 
#define CHOKE_POSITION  0 //defines the positon when the system should be choked
#define TOL 5 //defines how many steps change is required in order to move

//Declare variables for functions
int currentValue = 0; //Holds the current position of the valve
int refValue = 0; //Holds the new position the valve should be at
int stepsNeeded = 0; //Holds the number of steps needed to get to the new position

void setup() {

  pinMode(STEP_PIN, OUTPUT); //Set the step pin as an output
  pinMode(DIR_PIN, OUTPUT); //set the dir pin as output
  pinMode(EN_PIN, OUTPUT); //set the enable pin as an output

  digitalWrite(STEP_PIN, LOW); //Ensure the step pin is set low
  digitalWrite(DIR_PIN, LOW); //Ensure the dir pin has a defined polarity. (The value is irrelevant)
  digitalWrite(EN_PIN, LOW); //Make the enable pin low to enable the driver
}


void loop() {
  captureValue();
  stepsNeeded = abs(refValue - currentValue) > TOL ? (refValue - currentValue) : 0; //Check if the number of steps needed is larger than the tolerance, if it is, then return that, otherwise return 0 steps to move
  Step(abs(stepsNeeded), stepsNeeded > 0 ? 1 : 0); //Move the valve to the new position. The ternary operator decides which direction based on the sign of the steps
  delay(100);

  /*Step(1000,1);
    delay(100);*/
}

void Step(int steps, bool dir) //Function to step in a specific direction a certain number of times
{
  //digitalWrite(EN_PIN, LOW); //Enable the stepper driver
  if (dir == 1) {
    digitalWrite(DIR_PIN, HIGH); //Pull direction pin low to move "forward" and update the current position
    currentValue += steps;
  } else {
    digitalWrite(DIR_PIN, LOW); //Pull direction pin high to move "reverse" and update the current position
    currentValue -= steps;
  }
  for (int i = 0; i < steps; i++) //Loop through the number of steps
  {
    digitalWrite(STEP_PIN, HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(STEP_PIN, LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  //digitalWrite(EN_PIN, LOW); //Disable the stepper driver
}

void captureValue() {
  double sum = 0;
  double sum_count = 10; //How many samples to read
  for (int i = 0; i < sum_count; i++) {
    sum += floor(map(analogRead(ANALOG_IN), 0, 1023, 0, 50)); // Map the new position measured from the pot to a step value
    delay(10); //Delay 10ms in between reads
  }
  refValue = sum/sum_count; //Calculate the average of the incoming values
}
