#include "HAL.h"
#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;
TwoWire maxIIC;

void HAL::sensors_init()
{
    maxIIC.setPins(15,2);//scl:io2 sda:io15

    if (!particleSensor.begin(maxIIC, I2C_SPEED_FAST))
    {
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
    }

    particleSensor.setup(0); // Configure sensor. Turn off LEDs
    // particleSensor.setup(); //Configure sensor. Use 25mA for LED drive
}

void HAL::sensors_max30102_data()
{
      Serial.print(" R[");
      Serial.print(particleSensor.getRed());
      Serial.print("] IR[");
      Serial.print(particleSensor.getIR());
      Serial.print("] G[");
      Serial.print(particleSensor.getGreen());
      Serial.print("]");
      Serial.println();
    // uint32_t tmp = particleSensor.getRed();
}
