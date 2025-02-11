#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22
#define PWM_FREQ 50  // 50Hz
#define CLOCK_DIV 64 // Divisor de clock para gerar 50Hz
#define PWM_WRAP 39062 // Valor de wrap para 50Hz (125MHz / 64 / 50)

void set_servo_position(uint slice_num, uint channel, uint pulse_width_us) {
    // Converte o tempo de pulso em microssegundos para o valor do registrador PWM
    uint pulse_width = (pulse_width_us * PWM_WRAP) / 20000;
    pwm_set_chan_level(slice_num, channel, pulse_width);
    pwm_set_enabled(slice_num, true);
}

int main() {
    stdio_init_all();

    // Configuração do PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);

    // Configura o período do PWM para 20ms (50Hz)
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, CLOCK_DIV);  // Divisor de clock = 64
    pwm_config_set_wrap(&config, PWM_WRAP);     // Valor de wrap para 50Hz
    pwm_init(slice_num, &config, true);

    // Posição 180 graus (2.400µs)
    set_servo_position(slice_num, channel, 2400);
    sleep_ms(5000);

    // Posição 90 graus (1.470µs)
    set_servo_position(slice_num, channel, 1470);
    sleep_ms(5000);

    // Posição 0 graus (500µs)
    set_servo_position(slice_num, channel, 500);
    sleep_ms(5000);

    // Movimentação periódica entre 0 e 180 graus
    while (true) {
        // Movimento de 0 a 180 graus
        for (int pulse_width = 500; pulse_width <= 2400; pulse_width += 5) {
            set_servo_position(slice_num, channel, pulse_width);
            sleep_ms(10);
        }
        // Movimento de 180 a 0 graus
        for (int pulse_width = 2400; pulse_width >= 500; pulse_width -= 5) {
            set_servo_position(slice_num, channel, pulse_width);
            sleep_ms(10);
        }
    }

    return 0;
}