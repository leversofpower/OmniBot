#include "Compass.h"
#include <Wire.h>
#include "HMC5883L.h"

Compass::Compass(){}

void Compass::update(){
	int error = 0;
	HMC5883L compass;

	//Serial.println("Starting the I2C interface.");
	Wire.begin(); // Start the I2C interface.
	//Serial.println("Constructing new HMC5883L");
	//Serial.println("Setting scale to +/- 1.3 Ga");
	error = compass.SetScale(1.3); // Set the scale of the compass.
	
	//The following lines introduce dealy
	//if(error != 0){ // If there is an error, print it out.
	//	Serial.println(compass.GetErrorText(error));
	//}

	//Serial.println("Setting measurement mode to continous.");
	error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
	
	//The following lines introduce dealy
	//if(error != 0){ // If there is an error, print it out.
	//	Serial.println(compass.GetErrorText(error));
	//	Serial.println("ERROR");
	//}

	////Main Loop
	//// Retrive the raw values from the compass (not scaled).
	MagnetometerRaw raw = compass.ReadRawAxis();
	//// Retrived the scaled values from the compass (scaled to the configured scale).
	MagnetometerScaled scaled = compass.ReadScaledAxis();

	//// Values are accessed like so:
	int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

	//// Calculate heading when the magnetometer is level, then correct for signs of axis.
	float heading = atan2(scaled.YAxis, scaled.XAxis);

	//// Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
	//// Find yours here: http://www.magnetic-declination.com/
	//// Mine is: 2' 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
	//// If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
	////float declinationAngle = 0.0457;
	////heading += declinationAngle;

	//// Correct for when signs are reversed.
	if(heading < 0)
		heading += 2*PI;

	// Check for wrap due to addition of declination.
	if(heading > 2*PI)
		heading -= 2*PI;

	//// Convert radians to degrees for readability.
	////float headingDegrees = heading * 180/M_PI; 
	headingDegrees = heading * 180/M_PI;
}