// Copyright 2012 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// DSP utility routines.

#ifndef STMLIB_UTILS_DSP_DSP_H_
#define STMLIB_UTILS_DSP_DSP_H_

#include "../../stmlib/stmlib.h"

#include <cmath>
#include <math.h>

#define TEST

namespace stmlib {

#define MAKE_INTEGRAL_FRACTIONAL(x) \
  int32_t x ## _integral = static_cast<int32_t>(x); \
  float x ## _fractional = x - static_cast<float>(x ## _integral);

inline float Interpolate(const float* table, float index, float size) {
  index *= size;
  MAKE_INTEGRAL_FRACTIONAL(index)
  float a = table[index_integral];
  float b = table[index_integral + 1];
  return a + (b - a) * index_fractional;
}


inline float InterpolateHermite(const float* table, float index, float size) {
  index *= size;
  MAKE_INTEGRAL_FRACTIONAL(index)
  const float xm1 = table[index_integral - 1];
  const float x0 = table[index_integral + 0];
  const float x1 = table[index_integral + 1];
  const float x2 = table[index_integral + 2];
  const float c = (x1 - xm1) * 0.5f;
  const float v = x0 - x1;
  const float w = c + v;
  const float a = w + v + (x2 - x0) * 0.5f;
  const float b_neg = w + a;
  const float f = index_fractional;
  return (((a * f) - b_neg) * f + c) * f + x0;
}

inline float InterpolateWrap(const float* table, float index, float size) {
  index -= static_cast<float>(static_cast<int32_t>(index));
  index *= size;
  MAKE_INTEGRAL_FRACTIONAL(index)
  float a = table[index_integral];
  float b = table[index_integral + 1];
  return a + (b - a) * index_fractional;
}

#define ONE_POLE(out, in, coefficient) out += (coefficient) * ((in) - out);
#define SLOPE(out, in, positive, negative) { \
  float error = (in) - out; \
  out += (error > 0 ? positive : negative) * error; \
}
#define SLEW(out, in, delta) { \
  float error = (in) - out; \
  float d = (delta); \
  if (error > d) { \
    error = d; \
  } else if (error < -d) { \
    error = -d; \
  } \
  out += error; \
}

inline float Crossfade(float a, float b, float fade) {
  return a + (b - a) * fade;
}

inline float SoftLimit(float x) {
  return x * (27.0f + x * x) / (27.0f + 9.0f * x * x);
}

inline float SoftClip(float x) {
  if (x < -3.0f) {
    return -1.0f;
  } else if (x > 3.0f) {
    return 1.0f;
  } else {
    return SoftLimit(x);
  }
}

#ifdef TEST
  inline int32_t Clip16(int32_t x) {
    if (x < -32768) {
      return -32768;
    } else if (x > 32767) {
      return 32767;
    } else {
      return x;
    }
  }
  inline uint16_t ClipU16(int32_t x) {
    if (x < 0) {
      return 0;
    } else if (x > 65535) {
      return 65535;
    } else {
      return x;
    }
  }
#else
  inline int32_t Clip16(int32_t x) {
    int32_t result;
    __asm ("ssat %0, %1, %2" : "=r" (result) :  "I" (16), "r" (x) );
    return result;
  }
  inline uint32_t ClipU16(int32_t x) {
    uint32_t result;
    __asm ("usat %0, %1, %2" : "=r" (result) :  "I" (16), "r" (x) );
    return result;
  }
#endif
  
#ifdef TEST
  inline float Sqrt(float x) {
    return sqrtf(x);
  }
#else
  inline float Sqrt(float x) {
    float result;
    __asm ("vsqrt.f32 %0, %1" : "=w" (result) : "w" (x) );
    return result;
  }
#endif

inline int16_t SoftConvert(float x) {
  return Clip16(static_cast<int32_t>(SoftLimit(x * 0.5f) * 32768.0f));
}

}  // namespace stmlib

#endif  // STMLIB_UTILS_DSP_DSP_H_

// Copyright 2012 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// A set of basic operands, especially useful for fixed-point arithmetic, with
// fast ASM implementations.

#ifndef STMLIB_UTILS_DSP_H_
#define STMLIB_UTILS_DSP_H_

#include "../stmlib.h"

namespace stmlib {
    
    inline int16_t Interpolate824(const int16_t* table, uint32_t phase)
    __attribute__((always_inline));
    
    inline uint16_t Interpolate824(const uint16_t* table, uint32_t phase)
    __attribute__((always_inline));
    
    inline int16_t Interpolate824(const uint8_t* table, uint32_t phase)
    __attribute__((always_inline));
    
    inline uint16_t Interpolate88(const uint16_t* table, uint16_t index)
    __attribute__((always_inline));
    
    inline int16_t Interpolate88(const int16_t* table, uint16_t index)
    __attribute__((always_inline));
    
    inline int16_t Interpolate1022(const int16_t* table, uint32_t phase)
    __attribute__((always_inline));
    
    inline int16_t Interpolate115(const int16_t* table, uint32_t phase)
    __attribute__((always_inline));
    
    inline int16_t Crossfade(
                             const int16_t* table_a,
                             const int16_t* table_b,
                             uint32_t phase,
                             uint16_t balance)
    __attribute__((always_inline));
    
    inline int16_t Crossfade(
                             const uint8_t* table_a,
                             const uint8_t* table_b,
                             uint32_t phase,
                             uint16_t balance)
    __attribute__((always_inline));
    
    inline int16_t Crossfade1022(
                                 const uint8_t* table_a,
                                 const uint8_t* table_b,
                                 uint32_t phase,
                                 uint16_t balance)
    __attribute__((always_inline));
    
    inline int16_t Crossfade115(
                                const uint8_t* table_a,
                                const uint8_t* table_b,
                                uint16_t phase,
                                uint16_t balance)
    __attribute__((always_inline));
    
    inline int16_t Mix(int16_t a, int16_t b, uint16_t balance) {
        return (a * (65535 - balance) + b * balance) >> 16;
    }
    
    inline uint16_t Mix(uint16_t a, uint16_t b, uint16_t balance) {
        return (a * (65535 - balance) + b * balance) >> 16;
    }
    
    inline int16_t Interpolate824(const int16_t* table, uint32_t phase) {
        int32_t a = table[phase >> 24];
        int32_t b = table[(phase >> 24) + 1];
        return a + ((b - a) * static_cast<int32_t>((phase >> 8) & 0xffff) >> 16);
    }
    
    inline uint16_t Interpolate824(const uint16_t* table, uint32_t phase) {
        uint32_t a = table[phase >> 24];
        uint32_t b = table[(phase >> 24) + 1];
        return a + ((b - a) * static_cast<uint32_t>((phase >> 8) & 0xffff) >> 16);
    }
    
    inline int16_t Interpolate824(const uint8_t* table, uint32_t phase) {
        int32_t a = table[phase >> 24];
        int32_t b = table[(phase >> 24) + 1];
        return (a << 8) + \
        ((b - a) * static_cast<int32_t>(phase & 0xffffff) >> 16) - 32768;
    }
    
    inline uint16_t Interpolate88(const uint16_t* table, uint16_t index) {
        int32_t a = table[index >> 8];
        int32_t b = table[(index >> 8) + 1];
        return a + ((b - a) * static_cast<int32_t>(index & 0xff) >> 8);
    }
    
    inline int16_t Interpolate88(const int16_t* table, uint16_t index) {
        int32_t a = table[index >> 8];
        int32_t b = table[(index >> 8) + 1];
        return a + ((b - a) * static_cast<int32_t>(index & 0xff) >> 8);
    }
    
    inline int16_t Interpolate1022(const int16_t* table, uint32_t phase) {
        int32_t a = table[phase >> 22];
        int32_t b = table[(phase >> 22) + 1];
        return a + ((b - a) * static_cast<int32_t>((phase >> 6) & 0xffff) >> 16);
    }
    
    inline int16_t Interpolate115(const int16_t* table, uint16_t phase) {
        int32_t a = table[phase >> 5];
        int32_t b = table[(phase >> 5) + 1];
        return a + ((b - a) * static_cast<int32_t>(phase & 0x1f) >> 5);
    }
    
    inline int16_t Crossfade(
                             const int16_t* table_a,
                             const int16_t* table_b,
                             uint32_t phase,
                             uint16_t balance) {
        int32_t a = Interpolate824(table_a, phase);
        int32_t b = Interpolate824(table_b, phase);
        return a + ((b - a) * static_cast<int32_t>(balance) >> 16);
    }
    
    inline int16_t Crossfade(
                             const uint8_t* table_a,
                             const uint8_t* table_b,
                             uint32_t phase,
                             uint16_t balance) {
        int32_t a = Interpolate824(table_a, phase);
        int32_t b = Interpolate824(table_b, phase);
        return a + ((b - a) * static_cast<int32_t>(balance) >> 16);
    }
    
    inline int16_t Crossfade1022(
                                 const int16_t* table_a,
                                 const int16_t* table_b,
                                 uint32_t phase,
                                 uint16_t balance) {
        int32_t a = Interpolate1022(table_a, phase);
        int32_t b = Interpolate1022(table_b, phase);
        return a + ((b - a) * static_cast<int32_t>(balance) >> 16);
    }
    
    inline int16_t Crossfade115(
                                const int16_t* table_a,
                                const int16_t* table_b,
                                uint16_t phase,
                                uint16_t balance) {
        int32_t a = Interpolate115(table_a, phase);
        int32_t b = Interpolate115(table_b, phase);
        return a + ((b - a) * static_cast<int32_t>(balance) >> 16);
    }
    
}  // namespace stmlib

#endif  // STMLIB_UTILS_DSP_H_
