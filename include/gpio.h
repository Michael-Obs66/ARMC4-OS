#ifndef GPIO_H
#define GPIO_H

void gpio_set_direction(int pin, int output);
void gpio_write(int pin, int value);
int gpio_read(int pin);

#endif
