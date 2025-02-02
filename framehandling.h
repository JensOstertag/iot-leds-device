long fh_LastTime;
float fh_DeltaTime;

void setupFrameHandling() {
  fh_LastTime = millis();
}

float deltaTime() {
  return fh_DeltaTime;
}

void frameHandling() {
  long durationOfLastLoop = millis() - fh_LastTime;
  if(durationOfLastLoop < 0) {
    durationOfLastLoop = 0;
  }
  fh_DeltaTime = ((float) durationOfLastLoop) / ((float) 1000);
  fh_LastTime = millis();

  Serial.print("DeltaTime: ");
  Serial.print(deltaTime());
  Serial.print(" --- FPS: ");
  Serial.println(1 / deltaTime());
}
