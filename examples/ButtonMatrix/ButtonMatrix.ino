/*
	PinMatrix is a wrapper around grid matrices.
*/

//Required library include. Must be BEFORE includeing PinMatrix.h
#include <SketchBinder.h>

#include <PinMatrix.h>

//My row and column pins.
const byte rowCount = 2;
const byte columnCount = 3;

//Pin arrays must not be pointers and must be const
const byte rowPins[] = { 2, 3 };
const byte columnPins[] = { 4, 5, 6 };

//Debouncing time
const int debounce = 20;

//Clean way:
auto buttonMatrix = createMatrix(rowPins, columnPins, debounce);

//Verbose way:
//PinMatrix<rowCount, columnCount> buttonMatrix = PinMatrix<rowCount, columnCount>(rowPins, columnPins, debounce);

void setup() {
	Serial.begin(115200);
	buttonMatrix.updateHandler += buttonUpdated;
}

void loop() {
	//Must be called if SketchBinder.h is not included correctly.
	//buttonMatrix.read();
}

void buttonUpdated(byte row, byte column, bool state) {
	Serial.print("button on row ");
	Serial.print(row);
	Serial.print(", column ");
	Serial.print(column);
	Serial.print(" was ");

	if (state) {
		Serial.println("pushed.");
	} else {
		Serial.println("released.");
	}
}