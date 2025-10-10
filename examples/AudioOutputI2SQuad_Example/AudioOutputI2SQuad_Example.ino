// Demonstration of AudioOutputI2SQuad_F32 four channel I2S output object.
// Greg Raven KF5N October 2025.
// The first left and right channels are output on pin 7, which drives the
// Teensy Audio Adapter.  The second left and right channels are output on pin 32.

#include <Arduino.h>
#include <Audio.h>
#include <stdio.h>
#include <stdlib.h>
#include <OpenAudio_ArduinoLibrary.h>  // Manually installed library.
#include <TimeLib.h>                   // Part of Teensy Time library.
#include <arm_math.h>
#include <arm_const_structs.h>
#include <AudioStream_F32.h>

const float sample_rate_Hz = 48000.0;
const int audio_block_samples = 128;   // Always 128

AudioControlSGTL5000 sgtl5000_1;
AudioSettings_F32 audio_settings(sample_rate_Hz, audio_block_samples);
AudioSynthWaveformSine_F32 tone0(audio_settings), tone1, tone2, tone3;  // Tones for SSB calibration and IMD testing.
AudioOutputI2SQuad_F32 i2s_out(audio_settings);

AudioConnection_F32 connect0(tone0, 0, i2s_out, 0);
AudioConnection_F32 connect1(tone1, 0, i2s_out, 1);
AudioConnection_F32 connect2(tone2, 0, i2s_out, 2);
AudioConnection_F32 connect3(tone3, 0, i2s_out, 3);

void setup() {

  Serial.begin(115200);

  /* check for CrashReport stored from previous run */
  if (CrashReport) {
    Serial.print(CrashReport);
  }

  uint32_t* dmaErrors;
  dmaErrors = (uint32_t*)(0x400E8000 + 0x4);
  Serial.printf("DMA errors = %u\n", *dmaErrors);

  sgtl5000_1.enable();
  sgtl5000_1.setAddress(LOW);  // This sets to one of two possible I2C addresses, controlled by a jumper on the Audio Adapter.
  AudioMemory(200);
  AudioMemory_F32(200);
  sgtl5000_1.volume(0.8);  // Set headphone volume.
  sgtl5000_1.unmuteHeadphone();
  Serial.printf("Please listen for tones!\n");
  tone0.amplitude(0.1);
  tone0.frequency(1000);
  tone1.amplitude(0.1);
  tone1.frequency(1760.0);
  tone2.amplitude(0.004);
  tone2.frequency(440);
  tone3.amplitude(0.1);
  tone3.frequency(2000);
  tone0.begin();
  tone1.begin();
  tone2.begin();
  tone3.begin();  // The transmitter operates at 48ksps.
}

void loop() {

  delay(5000);
  // After 5 seconds, turn off all tones.
  tone0.end();
  tone1.end();
  tone2.end();
  tone3.end();

  delay(2000);
  // After 2 seconds, turn on tone0.
  tone0.begin();

  delay(2000);
  // After 2 seconds, turn on tone1.
  tone1.begin();

  delay(2000);
  // After 2 seconds, turn on tone2.
  tone2.begin();

  delay(2000);
  // After 2 seconds, turn on tone3.
  tone3.begin();
}
