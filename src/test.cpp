#include <gpiod.h>
#include <unistd.h>
#include <iostream>

#define CHIP "gpiochip0"
#define STEP_PIN 17
#define DIR_PIN 27

void pulse(gpiod_line* step, int delay_us) {
    gpiod_line_set_value(step, 1);
    usleep(delay_us);
    gpiod_line_set_value(step, 0);
    usleep(delay_us);
}

int main() {
    gpiod_chip* chip = gpiod_chip_open_by_name(CHIP);

    gpiod_line* step = gpiod_chip_get_line(chip, STEP_PIN);
    gpiod_line* dir  = gpiod_chip_get_line(chip, DIR_PIN);

    gpiod_line_request_output(step, "stepper", 0);
    gpiod_line_request_output(dir, "stepper", 0);

    gpiod_line_set_value(dir, 0);

    std::cout << "Forward\n";
    for (int i = 0; i < 2000; i++) {
        pulse(step, 500);
    }

    sleep(1);

    gpiod_line_set_value(dir, 1);

    std::cout << "Backward\n";
    for (int i = 0; i < 2000; i++) {
        pulse(step, 500);
    }

    gpiod_chip_close(chip);
    return 0;
}