// #include "PID.h"

// PID::PID() {
//   memory_index = -1;
// }

// void PID::apply(float value) {
//   uint32_t current_time = millis();

//   // first value special case
//   if (memory_index == -1) {
//     memory_index++;
//     memory_values[memory_index] = value;
//     memory_times[memory_index] = current_time;
//     return;
//   }

//   // update integral
//   float last_value = memory_values[memory_index];
//   uint32_t last_time = memory_times[memory_index];
//   integral += value * (float)(current_time - last_time) / 1000;

//   // update differential
  

//   // store value

//   return lpf;
// }
