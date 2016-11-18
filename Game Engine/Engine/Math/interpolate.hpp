//
//  interpolate.hpp
//  game engine
//
//  Created by Indi Kernick on 4/08/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef interpolate_hpp
#define interpolate_hpp

#include <math.h>
#include "constants.hpp"
#include "pow.hpp"

namespace Math {
  template<typename T>
  inline T lerp(double t, T from, T to) {
    return from + (to - from) * t;
  }
  
  ///Uses the -PId2 to 0 range of the sin function
  inline double sinIn(double t) {
    return 1 + sin(PId2 * t - PId2);
  }
  ///Uses the 0 to PId2 range of the sin function
  inline double sinOut(double t) {
    return sin(PId2 * t);
  }
  ///Uses the -PId2 to PId2 range of the sin function
  inline double sinInOut(double t) {
    return (1 + sin(PI * t - PId2)) / 2;
  }
  
  ///Uses the -2 to 0 range of the erf function
  inline double errorIn(double t) {
    return 1 + erf(2 * t - 2);
  }
  ///Uses the 0 to 2 range of the erf function
  inline double errorOut(double t) {
    return erf(2 * t);
  }
  ///Uses the -2 to 2 range of the erf function
  inline double errorInOut(double t) {
    return (1 + erf(4 * t - 2)) / 2;
  }
  
  inline double quadIn(double t) {
    return Pow<2>::calc(t);
  }
  inline double quadOut(double t) {
    return t * (2 - t);
  }
  inline double quadInOut(double t) {
    return t < 0.5 ? 2 * Pow<2>::calc(t)
                   : -1 + (4 - 2 * t) * t;
  }
  
  inline double cubicIn(double t) {
    return Pow<3>::calc(t);
  }
  inline double cubicOut(double t) {
    return Pow<3>::calc(t - 1) + 1;
  }
  inline double cubicInOut(double t) {
    return t < 0.5 ? 4 * Pow<3>::calc(t)
                   : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
  }
  
  inline double quartIn(double t) {
    return Pow<4>::calc(t);
  }
  inline double quartOut(double t) {
    return 1 - Pow<4>::calc(t - 1);
  }
  inline double quartInOut(double t) {
    return t < 0.5 ? 8 * Pow<4>::calc(t)
                   : 1 - 8 * Pow<4>::calc(t - 1);
  }
  
  inline double quintIn(double t) {
    return Pow<5>::calc(t);
  }
  inline double quintOut(double t) {
    return 1 + Pow<5>::calc(t - 1);
  }
  inline double quintInOut(double t) {
    return t < 0.5 ? 16 * Pow<5>::calc(t)
                   : 1 + 16 * Pow<5>::calc(t - 1);
  }
};

#endif
