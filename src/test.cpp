#include <gpiod.hpp>
#include <iostream>
#include <unistd.h>

#define CHIP "/dev/gpiochip0"
#define STEP 24
#define DIR 25

void pulse(gpiod::line_request& req) {
    req.set_value(STEP, gpiod::line::value::ACTIVE);
    usleep(500);
    req.set_value(STEP, gpiod::line::value::INACTIVE);
    usleep(500);
}

int main() {
    gpiod::chip chip(CHIP);

    gpiod::line_settings settings;
    settings.set_direction(gpiod::line::direction::OUTPUT);

    gpiod::line_request req =
        chip.prepare_request()
            .set_consumer("stepper")
            .add_line_settings(STEP, settings)
            .add_line_settings(DIR, settings)
            .do_request();

    // direction forward
    req.set_value(DIR, gpiod::line::value::INACTIVE);

    std::cout << "Forward\n";
    for (int i = 0; i < 2000; i++) {
        pulse(req);
    }

    sleep(1);

    req.set_value(DIR, gpiod::line::value::ACTIVE);

    std::cout << "Backward\n";
    for (int i = 0; i < 2000; i++) {
        pulse(req);
    }

    return 0;
}