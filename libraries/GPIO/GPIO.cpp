#include "GPIO.h"

using namespace GPIO;

//
// GENERIC PIN
//

constexpr gpio::gpio(const gpio_num_t pin,
                     const gpio_config_t& config,
                     const bool invertedLogic)
                     : _pin{pin},
                       _config{config},
                       _invertedLogic{invertedLogic}
{
  this->init();
}


[[nodiscard]] esp_err_t gpio::init(void)
{
  esp_err_t status{ESP_OK};

  status |= gpio_config(&(this->_config));

  return status;
}


//
// OUTPUT
// 

output::output(const gpio_num_t pin,
               const bool invertedLogic,
               const pullUpDownConfig pUpDownConf)
               : gpio {
                   pin,
                   gpio_config_t{
                     .pin_bit_mask  = static_cast<uint64_t>(1) << pin,
                     .mode          = GPIO_MODE_OUTPUT,
                     .pull_up_en    = pUpDownConf == pullUpDownConfig::PULLUP ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,  
                     .pull_down_en  = pUpDownConf == pullUpDownConfig::PULLDOWN ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE,
                     .intr_type     = GPIO_INTR_DISABLE
                   },
                   invertedLogic
                 }
{
  this->init();
}

[[nodiscard]] esp_err_t output::init(void)
{
  esp_err_t status{gpio::init()};

  if (status == ESP_OK)
    status |= this->set(false);

  return status;
}

esp_err_t output::set(const bool state)
{
  this->_state = state;
  return gpio_set_level(this->_pin, this->_invertedLogic ? !state : state);
}

esp_err_t output::toggle(void)
{
  return this->set(!this->_state);
}

bool output::state(void)
{
  return this->_state;
}

//
// INPUT
//

input::input(const gpio_num_t pin,
               const bool invertedLogic,
               const pullUpDownConfig pUpDownConf,
               const gpio_int_type_t interruptType)
               : gpio {
                   pin,
                   gpio_config_t{
                     .pin_bit_mask  = static_cast<uint64_t>(1) << pin,
                     .mode          = GPIO_MODE_INPUT,
                     .pull_up_en    = pUpDownConf == pullUpDownConfig::PULLUP ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,  
                     .pull_down_en  = pUpDownConf == pullUpDownConfig::PULLDOWN ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE,
                     .intr_type     = interruptType
                   },
                   invertedLogic
                 }
{
  this->init();
}

[[nodiscard]] esp_err_t input::init(void)
{
  esp_err_t status{gpio::init()};

  this->read();

  return status;
}

esp_err_t input::set(const bool state)
{
  return ESP_ERR_NOT_ALLOWED;
}

void input::read(void)
{
  this->_state = bool(gpio_get_level(this->_pin));
}

bool input::state(void)
{ 
  this->read();
  return this->_invertedLogic ? !(this->_state) : this->_state;
}
