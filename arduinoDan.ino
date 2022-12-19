/*
  Fade

  This example shows how to fade an LED on pin 9 using the analogWrite()
  function.

  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Fade
*/

int blinker_leds = 9;         // blinkry
int front_leds = 8;         // predni
int back_leds = 7;         // zadni
int power_relay = 6;         // output true to control on relay_1 by the remote control button 1
int input_relay = 5;         // input signal from the second relay controled by the remote control button 2
int output_audio_control = 4;         // output pin connected to the audio module


int ttl_seconds = 60;  // Time to live seconds

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(blinker_leds, OUTPUT);
  pinMode(front_leds, OUTPUT);
  pinMode(back_leds, OUTPUT);
  pinMode(power_relay, OUTPUT);
  pinMode(input_relay, INPUT);
  pinMode(output_audio_control, OUTPUT);
}

void blink() {
  int run = 1;
  int brightness = 10;  // how bright the LED is
  int fadeAmount = 5;  // how many points to fade the LED by
  do {
    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    if (brightness < 10) { 
      run = 0; 
      brightness = 0; 
    }
    // wait for 30 milliseconds to see the dimming effect
    analogWrite(blinker_leds, brightness);

    delay(5);
  } while (run == 1);
}

void blink_multi(int times){
  for (int tim=0; tim<times; tim = tim + 1) {
    blink();
  }
}

void start_audio(){
  int audio_active_milliseconds = 3000;  // in ms
  digitalWrite(output_audio_control, HIGH); 
  delay(audio_active_milliseconds);
  digitalWrite(output_audio_control, LOW); 
}

void start_car(){
  int relay2_output_state = 0;
  digitalWrite(power_relay, HIGH);  // keep power on
  digitalWrite(front_leds, HIGH);  // turn on
  digitalWrite(back_leds, HIGH);  // turn on
  digitalWrite(output_audio_control, LOW);  // no audio
  analogWrite(blinker_leds, 0);  // no blinkers
  // alive_counter = counting 100 ms each - or 0.1 seconds
  for(int alive_counter=0; alive_counter<ttl_seconds*10; alive_counter=alive_counter+1) {
    delay(100);
    
    relay2_output_state = digitalRead(input_relay);
    if (relay2_output_state == HIGH) {
      blink_multi(2);  // prolonges the ttl_seconds by the time of the blink_multi execution
      start_audio();  // prolonges the ttl_seconds by the time of the start_audio execution
    }

  }
  digitalWrite(front_leds, LOW);  // turn off
  digitalWrite(back_leds, LOW);  // turn off
  digitalWrite(power_relay, LOW); // suicide
}




// the loop routine runs over and over again forever:
void loop() {
  start_car();
  delay(1000000); // just wait forever as start_car function kills the power to atm after ttl_seconds
}
