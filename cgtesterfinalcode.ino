#include <HX711_ADC.h>
#include <math.h>

// Define HX711 instances for four load cells
HX711_ADC loadCell1(2, 3);     // F1 - Front Center (now origin)
HX711_ADC loadCell2(4, 5);     // F2 - Right Center
HX711_ADC loadCell3(11, 10);   // F3 - Rear Center
HX711_ADC loadCell4(9, 8);     // F4 - Left Center

// Calibration factors (adjust after calibration)
float calibrationFactor1 = 126.46;
float calibrationFactor2 = 128.65;
float calibrationFactor3 = 133.16;
float calibrationFactor4 = 123.10;

// Platform coordinates relative to F1 (0, 0)
const float x1 =   0.0,   y1 =   20.0;
const float x2 =  20.0,   y2 = 0.0;
const float x3 =   20.0,   y3 = 40.0;
const float x4 = 40.0,   y4 = 20.0;

// Utility to format floats for clean alignment
String formatFloat(float val, int width = 8, int decimals = 2) {
  char buf[16];
  dtostrf(val, width, decimals, buf);
  return String(buf);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing 4 Load Cells...");

  loadCell1.begin();
  loadCell2.begin();
  loadCell3.begin();
  loadCell4.begin();

  loadCell1.start(2000);
  loadCell1.setCalFactor(calibrationFactor1);
  loadCell1.tare();

  loadCell2.start(2000);
  loadCell2.setCalFactor(calibrationFactor2);
  loadCell2.tare();

  loadCell3.start(2000);
  loadCell3.setCalFactor(calibrationFactor3);
  loadCell3.tare();

  loadCell4.start(2000);
  loadCell4.setCalFactor(calibrationFactor4);
  loadCell4.tare();

  Serial.println("Tare complete ✅ Ready to calculate CG (relative to F1).");
}

void loop() {
  // Update all load cells
  loadCell1.update();
  loadCell2.update();
  loadCell3.update();
  loadCell4.update();

  // Read force values
  float F1 = loadCell1.getData();
  float F2 = loadCell2.getData();
  float F3 = loadCell3.getData();
  float F4 = loadCell4.getData();

  // Total vertical force (Z-axis) and weight
  float Z_force = F1 + F2 + F3 + F4;
  float weight = Z_force;  // For clarity (same value)

  if (Z_force == 0) {
    Serial.println("Total force is zero ❗");
    delay(500);
    return;
  }

  // Center of gravity (X, Y)
  float Xcg = (F1 * x1 + F2 * x2 + F3 * x3 + F4 * x4) / Z_force;
  float Ycg = (F1 * y1 + F2 * y2 + F3 * y3 + F4 * y4) / Z_force;

  // Output
  Serial.println("---------------------------------------------------------------------------------------------");
  Serial.println("   F1       F2       F3       F4      Xcg      Ycg     Z-axis   Weight");
  Serial.println("---------------------------------------------------------------------------------------------");

  Serial.print(formatFloat(F1));     Serial.print("  ");
  Serial.print(formatFloat(F2));     Serial.print("  ");
  Serial.print(formatFloat(F3));     Serial.print("  ");
  Serial.print(formatFloat(F4));     Serial.print("  ");
  Serial.print(formatFloat(Xcg));    Serial.print("  ");
  Serial.print(formatFloat(Ycg));    Serial.print("  ");
  Serial.print(formatFloat(Z_force));Serial.print("  ");
  Serial.print(formatFloat(weight)); Serial.println();

  Serial.println("---------------------------------------------------------------------------------------------\n");

  delay(500);
}