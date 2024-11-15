#pragma once

// #ifdef __cplusplus
// extern "C" {
// #endif

#include "driver/gpio.h"

namespace GPIO 
{

enum class pullUpDownConfig {
  DISABLED,
  PULLUP,
  PULLDOWN
};

//
// GENERIC PIN
//
class gpio {
protected:
  const gpio_num_t _pin;
  const gpio_config_t _config;

  const bool _invertedLogic = false;


public:
  constexpr gpio(const gpio_num_t pin,
       const gpio_config_t& config,
       const bool invertedLogic);

  [[nodiscard]] esp_err_t init(void);

  virtual esp_err_t set(const bool state) =0;

  virtual bool state(void) =0;

};
//
// OUTPUT
// 

class output : public gpio {
protected:
  bool _state = false;

public:
output(const gpio_num_t pin,
       const bool invertedLogic = false,
       const pullUpDownConfig pUpDownConf = pullUpDownConfig::DISABLED);

  [[nodiscard]] esp_err_t init(void);

  esp_err_t set(const bool state);
  esp_err_t toggle();

  bool state(void);

};

//
// INPUT
//

class input : public gpio {
protected:
  bool _state;

  void read(void);

public:
  input(const gpio_num_t pin,
        const bool invertedLogic = false,
        const pullUpDownConfig pUpDownConf = pullUpDownConfig::DISABLED,
        const gpio_int_type_t interruptType = GPIO_INTR_DISABLE);

  [[nodiscard]] esp_err_t init(void);

  esp_err_t set(const bool state);

  bool state(void);

};

}; // namespace GPIO

// #ifdef __cplusplus
// }
// #endif