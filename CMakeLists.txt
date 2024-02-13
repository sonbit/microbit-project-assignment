cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(microbit-project)

target_sources(app PRIVATE src/main.c src/drivers/button.c src/drivers/led.c src/drivers/speaker.c)
