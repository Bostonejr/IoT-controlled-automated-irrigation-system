#include "thingProperties.h"
#include "RTC.h"


  // Defining pins for the solenoid valves and pump
  int valve_1 = 3;
  int valve_2 = 5;
  int pump = 6;

  // Constants for mapping based on raw wet and dry soil moisture sensor values
  const int dry_value_1 = 610;
  const int wet_value_1 = 326;

  const int dry_value_2 = 790;
  const int wet_value_2 = 385;

  const int dry_value_3 = 187;
  const int wet_value_3 = 85;

  const int dry_value_4 = 777;
  const int wet_value_4 = 444;

 

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  
  // Setting the pin modes for the valve pins and pump pin
  pinMode(valve_1, OUTPUT);
  pinMode(valve_2, OUTPUT);
  pinMode(pump, OUTPUT);

  // Making sure valves are closed at the beginning of the code and pump is off
  // Low now means relay is on and high now means relay is off
  digitalWrite(valve_1, LOW);
  digitalWrite(valve_2, LOW);
  digitalWrite(pump, HIGH);

  // Setting up our initial time
  RTC.begin();
  // RTCTime initialTime(02, Month::AUGUST, 2025, 12, 45, 00, DayOfWeek::TUESDAY, SaveLight::SAVING_TIME_INACTIVE);
  // RTC.setTime(initialTime);

  
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  // declaring initial states for the valves in the cloud variables
  valve_state1 = 1;
  valve_state2 = 1;
  pump_state = 0;
}

void loop() {
  ArduinoCloud.update();

  // getting current time to RTC
  RTCTime currentTime;
  RTC.getTime(currentTime);

  // storing the hour
  int hour = currentTime.getHour();
  // int hour = 18;
  Serial.print(currentTime.getHour());
  // Serial.print(":");
  // Serial.print(currentTime.getMinutes());
  // Serial.print(":");
  // Serial.print(currentTime.getSeconds());
  Serial.println();
  delay(1000);

  // Reading the varying raw sensor data
  int cap_Sensor_1_raw = analogRead(A0);
  int cap_Sensor_2_raw = analogRead(A1);
  int cap_Sensor_3_raw = analogRead(A3);
  int cap_Sensor_4_raw = analogRead(A2);

  // Getting  sensor values in percentages
  // Getting  sensor values in percentages
 int cap_Sensor_1 = map(cap_Sensor_1_raw, wet_value_1, dry_value_1, 0, 100);
 int cap_Sensor_2 = map(cap_Sensor_2_raw, wet_value_2, dry_value_2, 0, 100);
 int cap_Sensor_3 = map(cap_Sensor_3_raw, wet_value_3, dry_value_3, 0, 100);
 int cap_Sensor_4 = map(cap_Sensor_4_raw, wet_value_4, dry_value_4, 0, 100);

  // Taking the averages of each bed's moisture level and linking them to the cloud variables
  moisture_level_2 = cap_Sensor_2;
  moisture_level_1 = cap_Sensor_4;

  Serial.print("raw_sensor_1:");
  Serial.print(cap_Sensor_1_raw);
  Serial.println();
  Serial.print("raw_sensor_2:");
  Serial.print(cap_Sensor_2_raw); 
  Serial.println();
  Serial.print("raw_sensor_4:");
  Serial.print(cap_Sensor_4_raw);
  Serial.println();
  Serial.print("moisture_level_2: ");
  Serial.print(moisture_level_2);
  Serial.println();
  Serial.print("moisture_level_1: ");
  Serial.print(moisture_level_1);
  
  delay(1000);

  

  
  // Checking conditions using the real-time clock
  // Checking for second bed (valve 2)
  // super dry is 87%
  // super wet is 0-6%
  if (hour == 7 || hour == 18){
    // water plants based on moisture level 2
    // moisture level 2
    if(moisture_level_2 >= 60){
      digitalWrite(valve_2, HIGH);
      // update valve2's state to off/open
      valve_state2 = 0;
      digitalWrite(pump, LOW);
      pump_state = 1;
      // 7 mins
      delay(420000);
      digitalWrite(pump, HIGH);
      digitalWrite(valve_2, LOW);
      
      
    }
    else if(moisture_level_2> 40 && moisture_level_2< 60){
      digitalWrite(valve_2, HIGH);
      // update valve2's state to off/open
      valve_state2 = 0;
      digitalWrite(pump, LOW);
      pump_state = 1;
      // 5 mins
      delay(300000);
      digitalWrite(pump, HIGH);
      digitalWrite(valve_2, LOW);
      
      
    }
    else if(moisture_level_2> 10 && moisture_level_2< 40){
      digitalWrite(valve_2, HIGH);
      // update valve2's state to off/open
      valve_state2 = 0;
      digitalWrite(pump, LOW);
      pump_state = 1;
      // 3 mins
      delay(180000);
      digitalWrite(pump, HIGH);
      digitalWrite(valve_2, LOW);
      
      
    }
    else if(moisture_level_2 > 5 && moisture_level_2 < 10){
      digitalWrite(valve_2, HIGH);
      // update valve2's state to off/open
      valve_state2 = 0;
      digitalWrite(pump, LOW);
      pump_state = 1;
      // 2mins
      delay(120000);  
      digitalWrite(pump, HIGH);
      digitalWrite(valve_2, LOW);
      
      
    }
  }
  
  else{
    // Don't water plants in bed 2
    // Turn pump off
    digitalWrite(pump, HIGH);
    pump_state = 0;
    // Turn valves on/close valves
    digitalWrite(valve_2, LOW);
    // update valve2's state to on/closed
    valve_state2 = 1;
    Serial.print("I was ran(Didn't water bed 2)!!!!");
    Serial.println();
    
  }
  

  // Checking for first bed (valve 1)
  if (hour == 7 || hour == 18){
    // water plants based on moisture level 1
    // moisture level 1
    if(moisture_level_1 >= 60){
      digitalWrite(valve_1, HIGH);
      // update valve1's state to off/open
      valve_state1 = 0;
      digitalWrite(pump, LOW);
      pump_state = 1;
      // 7 mins
      delay(420000);
      digitalWrite(pump, HIGH);
      digitalWrite(valve_1, LOW);
      
      
    }
    else if(moisture_level_1> 40 && moisture_level_1< 60){
      digitalWrite(valve_1, HIGH);
      // update valve1's state to off/open
      valve_state1 = 0;
      digitalWrite(pump, LOW);
      pump_state = 1;
      // 5 mins
      delay(300000);
      digitalWrite(pump, HIGH);
      digitalWrite(valve_1, LOW);
      
      
    }
    else if(moisture_level_1> 10 && moisture_level_1< 40){
      digitalWrite(valve_1, HIGH);
      // update valve1's state to off/open
      valve_state1 = 0;
      digitalWrite(pump, LOW);
      pump_state = 1;
      // 3 mins
      delay(180000);
      digitalWrite(pump, HIGH);
      digitalWrite(valve_1, LOW);
      
      
    }
    else if(moisture_level_1 > 5 && moisture_level_1 < 10){
      digitalWrite(valve_1, HIGH);
      // update valve1's state to off/open
      valve_state1 = 0;
      digitalWrite(pump, LOW);
      pump_state = 1;
      // 2mins
      delay(120000);  
      digitalWrite(pump, HIGH);
      digitalWrite(valve_1, LOW);
      
      
    }
  }
  
  else{
    // Don't water plants in bed 1
    // Turn pump off
    digitalWrite(pump, HIGH);
    pump_state = 0;
    // Turn valves on/close valve 1
    digitalWrite(valve_1, LOW);
    // update valve1's state to on/closed
    valve_state1 = 1;
    Serial.print("I was ran(Didn't water bed 1!!!!");
    Serial.println();
    
  }
  
}





/*
  Since ValveState1 is READ_WRITE variable, onValveState1Change() is
  executed every time a new value is received from IoT Cloud.
*/
void onValveState1Change()  {
  // Add your code here to act upon ValveState1 change
  // if(valve_state1 == 1){
  //   digitalWrite(valve_1, HIGH);
  // }else{
  //   digitalWrite(valve_1, LOW);
  // }
}
// /*
//   Since ValveState2 is READ_WRITE variable, onValveState2Change() is
//   executed every time a new value is received from IoT Cloud.
// */
void onValveState2Change()  {
  // Add your code here to act upon ValveState2 change
  //  if(valve_state2 == 1){
  //   digitalWrite(valve_2, HIGh);
  // }else{
  //   digitalWrite(valve_2, LOW);
  // }
}
/*
  Since PumpState is READ_WRITE variable, onPumpStateChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onPumpStateChange()  {
  // Add your code here to act upon PumpState change
  // if(pump_state == 1){ //that is 1 equals to off position on the dashboard switch
  //   digitalWrite(pump, HIGH);
  // }else{
  //   digitalWrite(pump, LOW);
  // }
 

}
