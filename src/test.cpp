#include <gpiod.hpp>
#include <iostream>
#include <unistd.h>

#define CHIP "gpiochip0"
#define STEP_LINE 17
#define DIR_LINE 27

void pulse(gpiod::line_request& req, int step_offset, int delay_us) {
    req.set_value(step_offset, 1);
    usleep(delay_us);
    req.set_value(step_offset, 0);
    usleep(delay_us);
}

int main() {
    gpiod::chip chip(CHIP);

    gpiod::line_request req = chip.prepare_request()
        .set_consumer("stepper")
        .add_line_settings(
            gpiod::line_settings()
                .set_direction(gpiod::line::direction::OUTPUT),
            {STEP_LINE, DIR_LINE}
        )
        .do_request();

    // DIR = 0
    req.set_value(DIR_LINE, 0);

    std::cout << "Forward\n";
    for (int i = 0; i < 2000; i++) {
        pulse(req, STEP_LINE, 500);
    }

    sleep(1);

    req.set_value(DIR_LINE, 1);

    std::cout << "Backward\n";
    for (int i = 0; i < 2000; i++) {
        pulse(req, STEP_LINE, 500);
    }

    return 0;
}