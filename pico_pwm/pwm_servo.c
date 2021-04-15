#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/time.h"
#include "hardware/irq.h"

#define SERVO_PIN (1)

void on_pwm_wrap() 
{
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(pwm_gpio_to_slice_num(SERVO_PIN));
    // To turn servo 0 deg.
    pwm_set_gpio_level(SERVO_PIN, 0);
    sleep_ms(2000);
    // To turn servo 45 deg.
    pwm_set_gpio_level(SERVO_PIN, 0.5);
    sleep_ms(2000);
    // To turn servo 90 deg.
    pwm_set_gpio_level(SERVO_PIN, 1);
    sleep_ms(2000);
    // To turn servo 180 deg.
    pwm_set_gpio_level(SERVO_PIN, 1.5);
    sleep_ms(2000);
}

int main() 
{    
    // Set SERVO_PIN as PWM function and get what slice no. it belongs to.
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // To get default PWM configs.
    pwm_config config = pwm_get_default_config();
    // Divide the sys_clk by provided value. sys_clk = 125,
    // therefore to get 50hz  = 125/2.5 .. 8.4div <--> 2.5period
    pwm_config_set_clkdiv(&config, 8.4f);
    // Set the wrap to 20 just to get 20 points analogous to 20ms for 50hz.
    pwm_set_wrap(slice_num, 20);
    // Load the configs and run th PWM channel.
    pwm_init(slice_num, &config, true);

    while (1)
        tight_loop_contents();
}