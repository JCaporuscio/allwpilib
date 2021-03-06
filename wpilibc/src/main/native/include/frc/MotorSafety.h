/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <wpi/mutex.h>
#include <wpi/raw_ostream.h>

#include "frc/DriverStation.h"
#include "frc/ErrorBase.h"
#include "frc/Timer.h"
#include "frc/Watchdog.h"

namespace frc {

/**
 * This base class runs a watchdog timer and calls the subclass's StopMotor()
 * function if the timeout expires.
 *
 * The subclass should call Feed() whenever the motor value is updated.
 */
class MotorSafety : public ErrorBase {
 public:
  MotorSafety() = default;
  virtual ~MotorSafety() = default;

  MotorSafety(MotorSafety&& rhs);
  MotorSafety& operator=(MotorSafety&& rhs);

  /**
   * Feed the motor safety object.
   *
   * Resets the timer on this object that is used to do the timeouts.
   */
  void Feed();

  /**
   * Set the expiration time for the corresponding motor safety object.
   *
   * @param expirationTime The timeout value in seconds.
   */
  void SetExpiration(double expirationTime);

  /**
   * Retrieve the timeout value for the corresponding motor safety object.
   *
   * @return the timeout value in seconds.
   */
  double GetExpiration() const;

  /**
   * Determine if the motor is still operating or has timed out.
   *
   * @return true if the motor is still operating normally and hasn't timed out.
   */
  bool IsAlive() const;

  /**
   * Enable/disable motor safety for this device.
   *
   * Turn on and off the motor safety option for this PWM object.
   *
   * @param enabled True if motor safety is enforced for this object.
   */
  void SetSafetyEnabled(bool enabled);

  /**
   * Return the state of the motor safety enabled flag.
   *
   * Return if the motor safety is currently enabled for this device.
   *
   * @return True if motor safety is enforced for this device.
   */
  bool IsSafetyEnabled() const;

  virtual void StopMotor() = 0;
  virtual void GetDescription(wpi::raw_ostream& desc) const = 0;

 private:
  static constexpr double kDefaultSafetyExpiration = 0.1;

  Watchdog m_watchdog{kDefaultSafetyExpiration, [this] { TimeoutFunc(); }};

  // True if motor safety is enabled for this motor
  bool m_enabled = false;

  mutable wpi::mutex m_thisMutex;

  void TimeoutFunc();
};

}  // namespace frc
