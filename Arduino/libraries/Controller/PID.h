// #ifndef PID_H
// #define PID_H

// #define PID_MEMORY_SIZE 60
// #define PID_DIFFERENTIAL_TIME_MILLIS 60000

// class PID {
//  private:
//   float integral;
//   float differential;
//   float memory_values[PID_MEMORY_SIZE];
//   uint32_t memory_times[PID_MEMORY_SIZE];
//   int8_t memory_index;
//   bool memory_wrapped;

//  public:
//   PID();
//   void apply(float value);
//   float getP() { return (memory_index >= 0) ? memory_values[memory_index] : 0; }
//   float getI() { return integral; }
//   float getD() { return differential; }
// };
// #endif
