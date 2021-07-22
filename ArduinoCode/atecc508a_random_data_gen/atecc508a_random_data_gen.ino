/*
  Using the SparkFun Cryptographic Co-processor Breakout ATECC508a (Qwiic)
  By: Pete Lewis
  SparkFun Electronics
  Date: August 5th, 2019
  License: This code is public domain but you can buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Please buy a board from SparkFun!
  https://www.sparkfun.com/products/15573

  ////////////////////////////// 
  /////////////// ABOUT
  ////////////////////////////// 
  
  This example shows how to use the Random Number Generator on the Sparkfun Cryptographic Co-processor.
  It will print random numbers once a second on the serial terminal at 115200.
  
  Note, this chip is a bit more capable than the built in Arduino function random().

  random(min, max);

  It has a basic random(min, max). This will return a random number of type long.
  Note, it is slightly different than the arduino version, and it DOES require min AND max.
  
  It also has the following 4 other functions that can be useful in other aplications.

  getRandomByte();
  getRandomInt();
  getRandomLong();

  Each of these functions return a random number of the type written into the name.

  And lastly...
  updateRandom32Bytes(); // will create 32 bytes of random data and store it in atecc.random32Bytes[]

  After calling "updateRandom32Bytes()", then your random 32 bytes of data are available at a public varaible
  within the library instance. If your instance was named "atecc" as in the SparkFun examples,
  this array would be "atecc.random32Bytes[]". 
  This can be useful as an time-to-live-token (aka NONCE) to send to another device.
  
  //////////////////////////////
  /////////////// CONFIGURATION
  //////////////////////////////  
  
  Note, this requires that your device be configured with SparkFun Standard Configuration settings.
  If you haven't already, configure both devices using Example1_Configuration.
  Install artemis in boards manager: http://boardsmanager/All#Sparkfun_artemis
  Plug in your controller board (e.g. Artemis Redboard, Nano, ATP) into your computer with USB cable.
  Connect your Cryptographic Co-processor to your controller board via a qwiic cable.
  Select TOOLS>>BOARD>>"SparkFun Redboard Artemis"
  Select TOOLS>>PORT>> "COM 3" (note, Alice and Bob will each have a unique COM PORT)
  Click upload, and follow prompts on serial monitor at 115200.

*/

#include <SparkFun_ATECCX08a_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_ATECCX08a
#include <Wire.h>

/* 
*  In the paper we have mentioned to use baud rate as 115200 but
*  it is fine use any baud rate that is supported on both sides
*   e.g. hardware(ESP32-DevKitC V4) and software (Realterm)
*  We only need to make sure that we are using the same baud
*  rate at both sides.
*/
#define SYS_BAUD_RATE 115200

/*
 * ESP32-DevKitC V4 does not have an extra OnBoard LED, so we connect
 * a LED to GPIO 2. We will use this LED to indicate that long 
 * running task is completed.
*/
#define TASK_LED  2

/*
* flag indicating that LED is OFF
* it is used to turn on LED one time
*/
bool FLAG_LED_OFF = true;

/* 
* On ESP32-DevKitC V4, default I2C pins are
*    GPIO 21 -> SDA
*    GPIO 22 -> SCL
*    
* So, connect ATECC508a accordingly
*/

ATECCX08A atecc;

void setup() {
  // configure i2c port 
  Wire.begin();

  // configure serial port
  Serial.begin(SYS_BAUD_RATE);

  // configure onboard led
  pinMode(TASK_LED,OUTPUT);

  // set led state to OFF
  digitalWrite(TASK_LED,LOW);
  
  if (atecc.begin() == true)
  {
    Serial.println("Successful wakeUp(). I2C connections are good.");
  }
  else
  {
    Serial.println("Device not found. Check wiring.");
    while (1); // stall out forever
  }

  // lock device to make RNG usable
  lock();

  printInfo(); // see function below for library calls and data handling

  // check for configuration
  if (!(atecc.configLockStatus && atecc.dataOTPLockStatus && atecc.slot0LockStatus))
  {
    Serial.print("Device not configured. Please enable locking code and run again");
    while (1); // stall out forever.
  }
}

// byte counter
int i = 0;

void loop()
{
  /* 
   * Start collecting data 
   * We are collecting 5000 bytes extra as at the begining, ESP32-DevKit V1 
   *   sends boot log and debug logs from out code which are to be discarded.
   */
  
  while(i <= ( (1000 * 1000000) + 5000)) {
    // read 32 bytes at a time
    atecc.updateRandom32Bytes();

    // send random data to PC over serial port
    for (int j = 0; j < 32 ; j++)
    {
      if ((atecc.random32Bytes[j] >> 4) == 0) Serial.print("0"); // print preceeding high nibble if it's zero
      Serial.print(atecc.random32Bytes[j], HEX);
    }
    i += 32;
  }

  // turn on ON_BOARD LED indicating job is complete
  if (FLAG_LED_OFF) {
    digitalWrite(TASK_LED,HIGH);
    FLAG_LED_OFF = false;
  }
  
  delay(1000);
}

void printInfo()
{
  // Read all 128 bytes of Configuration Zone
  // These will be stored in an array within the instance named: atecc.configZone[128]
  atecc.readConfigZone(false); // Debug argument false (OFF)

  // Print useful information from configuration zone data
  Serial.println();

  Serial.print("Serial Number: \t");
  for (int i = 0 ; i < 9 ; i++)
  {
    if ((atecc.serialNumber[i] >> 4) == 0) Serial.print("0"); // print preceeding high nibble if it's zero
    Serial.print(atecc.serialNumber[i], HEX);
  }
  Serial.println();

  Serial.print("Rev Number: \t");
  for (int i = 0 ; i < 4 ; i++)
  {
    if ((atecc.revisionNumber[i] >> 4) == 0) Serial.print("0"); // print preceeding high nibble if it's zero
    Serial.print(atecc.revisionNumber[i], HEX);
  }
  Serial.println();

  Serial.print("Config Zone: \t");
  if (atecc.configLockStatus) Serial.println("Locked");
  else Serial.println("NOT Locked");

  Serial.print("Data/OTP Zone: \t");
  if (atecc.dataOTPLockStatus) Serial.println("Locked");
  else Serial.println("NOT Locked");

  Serial.print("Data Slot 0: \t");
  if (atecc.slot0LockStatus) Serial.println("Locked");
  else Serial.println("NOT Locked");

  Serial.println();
}

void lock() {
  int retval;
  
  Serial.print("Locking Config Zone: \t");
  retval = atecc.lockConfig();
  if (retval == 0)
    Serial.println("Success");
  else
    Serial.println("Failed");
    
  Serial.print("Locking Data and OTP Zone: \t");
  retval = atecc.lockDataAndOTP();
  if (retval == 0)
    Serial.println("Success");
  else
    Serial.println("Failed");

  Serial.print("Locking DataSlot0: \t");
  retval = atecc.lockDataSlot0();
  if (retval == 0)
    Serial.println("Success");
  else
    Serial.println("Failed");
}
