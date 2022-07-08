#include <time.h>
#include <stdlib.h>
#include <board.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include "periph_conf.h"

#define R1 (GPIO_PIN(PORT_A, 10))
#define R2 (GPIO_PIN(PORT_D, 12))
#define R3 (GPIO_PIN(PORT_A, 9))
#define R4 (GPIO_PIN(PORT_C, 6))
#define R5 (GPIO_PIN(PORT_B, 15))
#define R6 (GPIO_PIN(PORT_C, 9))
#define R7 (GPIO_PIN(PORT_D, 9))
#define R8 (GPIO_PIN(PORT_D, 15))
#define C1 (GPIO_PIN(PORT_D, 14))
#define C2 (GPIO_PIN(PORT_D, 11))
#define C3 (GPIO_PIN(PORT_D, 13))
#define C4 (GPIO_PIN(PORT_A, 8))
#define C5 (GPIO_PIN(PORT_C, 7))
#define C6 (GPIO_PIN(PORT_C, 8))
#define C7 (GPIO_PIN(PORT_D, 10))
#define C8 (GPIO_PIN(PORT_D, 8))
#define COM (GPIO_PIN(PORT_A, 12))
#define UP (GPIO_PIN(PORT_C, 10))
#define DWN (GPIO_PIN(PORT_C, 12))
#define LFT (GPIO_PIN(PORT_D, 1))
#define RHT (GPIO_PIN(PORT_D, 0))
#define MID (GPIO_PIN(PORT_C, 11))
#define SET (GPIO_PIN(PORT_A, 15))
#define RST (GPIO_PIN(PORT_A, 11))
#define UNUSED(x) (void)(x)

const int DEBOUNCE_TIME = 200000;
char count = 1;
uint32_t debounce_timer = 0, last_turn = 0;
int r = 0, d = 2, l = 1, X[64] = {0}, Y[64] = {0}, matrix[8][8] = {0};
bool death = false;

void SelectRow(int Row) {
    if (Row == 0) gpio_write(R1, 0); else gpio_write(R1, 1);
    if (Row == 1) gpio_write(R2, 0); else gpio_write(R2, 1);
    if (Row == 2) gpio_write(R3, 0); else gpio_write(R3, 1);
    if (Row == 3) gpio_write(R4, 0); else gpio_write(R4, 1);
    if (Row == 4) gpio_write(R5, 0); else gpio_write(R5, 1);
    if (Row == 5) gpio_write(R6, 0); else gpio_write(R6, 1);
    if (Row == 6) gpio_write(R7, 0); else gpio_write(R7, 1);
    if (Row == 7) gpio_write(R8, 0); else gpio_write(R8, 1);
}

void Set_LED_in_Active_Row(int Column, int state) {
    if (Column == 0) gpio_write(C1, state);
    if (Column == 1) gpio_write(C2, state);
    if (Column == 2) gpio_write(C3, state);
    if (Column == 3) gpio_write(C4, state);
    if (Column == 4) gpio_write(C5, state);
    if (Column == 5) gpio_write(C6, state);
    if (Column == 6) gpio_write(C7, state);
    if (Column == 7) gpio_write(C8, state);
}

static void upCallback(void *arg) {
    UNUSED(arg);
    if ((xtimer_usec_from_ticks(xtimer_now()) - debounce_timer) > DEBOUNCE_TIME && count == 2) {
        if (d != 3)
            d = 1;
        debounce_timer = xtimer_usec_from_ticks(xtimer_now());
    }
    count = count % 2 + 1;
}

static void downCallback(void *arg) {
    UNUSED(arg);
    if ((xtimer_usec_from_ticks(xtimer_now()) - debounce_timer) > DEBOUNCE_TIME && count == 2) {
        if (d != 1)
            d = 3;
        debounce_timer = xtimer_usec_from_ticks(xtimer_now());
    }
    count = count % 2 + 1;
}

static void leftCallback(void *arg) {
    UNUSED(arg);
    if ((xtimer_usec_from_ticks(xtimer_now()) - debounce_timer) > DEBOUNCE_TIME && count == 2) {
        if (d != 2)
            d = 0;
        debounce_timer = xtimer_usec_from_ticks(xtimer_now());
    }
    count = count % 2 + 1;
}

static void rightCallback(void *arg) {
    UNUSED(arg);
    if ((xtimer_usec_from_ticks(xtimer_now()) - debounce_timer) > DEBOUNCE_TIME && count == 2) {
		if (d != 0)
            d = 2;
        debounce_timer = xtimer_usec_from_ticks(xtimer_now());
    }
    count = count % 2 + 1;
}

static void middleCallback(void *arg) {
    UNUSED(arg);
    if ((xtimer_usec_from_ticks(xtimer_now()) - debounce_timer) > DEBOUNCE_TIME && count == 2) {
        r = 1;
        debounce_timer = xtimer_usec_from_ticks(xtimer_now());
    }
    count = count % 2 + 1;
}

static void setCallback(void *arg) {
    UNUSED(arg);
    if ((xtimer_usec_from_ticks(xtimer_now()) - debounce_timer) > DEBOUNCE_TIME && count == 2) {
        debounce_timer = xtimer_usec_from_ticks(xtimer_now());
    }
    count = count % 2 + 1;
}

static void resetCallback(void *arg) {
    UNUSED(arg);
    if ((xtimer_usec_from_ticks(xtimer_now()) - debounce_timer) > DEBOUNCE_TIME && count == 2) {
        debounce_timer = xtimer_usec_from_ticks(xtimer_now());
    }
    count = count % 2 + 1;
}

void init_gpio(void) {
    xtimer_init();
    gpio_init(R1, GPIO_OUT);
    gpio_init(R2, GPIO_OUT);
    gpio_init(R3, GPIO_OUT);
    gpio_init(R4, GPIO_OUT);
    gpio_init(R5, GPIO_OUT);
    gpio_init(R6, GPIO_OUT);
    gpio_init(R7, GPIO_OUT);
    gpio_init(R8, GPIO_OUT);
    gpio_init(C1, GPIO_OUT);
    gpio_init(C2, GPIO_OUT);
    gpio_init(C3, GPIO_OUT);
    gpio_init(C4, GPIO_OUT);
    gpio_init(C5, GPIO_OUT);
    gpio_init(C6, GPIO_OUT);
    gpio_init(C7, GPIO_OUT);
    gpio_init(C8, GPIO_OUT);
    gpio_init(COM, GPIO_OUT);
    int cnt = 0;
    gpio_write(COM, 0);
    gpio_irq_enable(SET);
    gpio_irq_enable(RST);
    gpio_irq_enable(UP);
    gpio_irq_enable(DWN);
    gpio_irq_enable(LFT);
    gpio_irq_enable(RHT);
    gpio_irq_enable(MID);
    gpio_init_int(SET, GPIO_IN_PU, (3 << 0), setCallback, (void *) cnt);
    gpio_init_int(RST, GPIO_IN_PU, (3 << 0), resetCallback, (void *) cnt);
    gpio_init_int(UP, GPIO_IN_PU, (3 << 0), upCallback, (void *) cnt);
    gpio_init_int(DWN, GPIO_IN_PU, (3 << 0), downCallback, (void *) cnt);
    gpio_init_int(LFT, GPIO_IN_PU, (3 << 0), leftCallback, (void *) cnt);
    gpio_init_int(RHT, GPIO_IN_PU, (3 << 0), rightCallback, (void *) cnt);
    gpio_init_int(MID, GPIO_IN_PU, (3 << 0), middleCallback, (void *) cnt);
}

bool dead(bool death) {
	if (death) {
		while (1) {
			for (int x = 0; x < 8; x++) {
					SelectRow(x);
					for (int y = 0; y < 8; y++) {
						Set_LED_in_Active_Row(y, x == y || 7 - y == x);
					}
					xtimer_msleep(2);
				}
			if (r == 1) {
				death = false;
				break;
			}
		}
		for (int x = 0; x < 8; x++) {
			SelectRow(x);
			for (int y = 0; y < 8; y++) {
				Set_LED_in_Active_Row(y, 0);
			};
		}
		for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                matrix[x][y] = 0;
            }
        }
		l = 1;
		X[0] = 0;
		Y[0] = 0;
		d = 2;
	}
	return death;
}

int main(void) {
    // x - down
    // y - right
    init_gpio();

    bool flag;
    int i, x, y, rx = 0, ry = 0;
    X[0] = 0;
    Y[0] = 0;
    for (i = 1; i < l; i++) {
        X[i] = X[0] - i;
        Y[i] = Y[0];
        matrix[X[i]][Y[i]] = 1;
    }
    rx = 4;
    ry = 4;
    xtimer_sleep(2);
    while (1) {
        if (xtimer_usec_from_ticks(xtimer_now()) - last_turn > 1000000) {
            last_turn = xtimer_usec_from_ticks(xtimer_now());
            matrix[X[l - 1]][Y[l - 1]] = 0;
            for (i = l - 1; i > 0; i--) {
                X[i] = X[i - 1];
                Y[i] = Y[i - 1];
            }
            if (d == 0) //left
                Y[0] = Y[0] - 1;
            else if (d == 1) //up
                X[0] = X[0] - 1;
            else if (d == 2) //right
                Y[0] = Y[0] + 1;
            else if (d == 3) //down
                X[0] = X[0] + 1;
            matrix[X[0]][Y[0]] = 1;

            if (Y[0] < 0 || Y[0] > 7 || X[0] < 0 || X[0] > 7) {
                death = true;
            }

            for (i = 4; i < l; i++) {
                if (X[0] == X[i] && Y[0] == Y[i]) {
                    death = true;
                }
            }

            if (Y[0] == ry && X[0] == rx) {
                l++;
                if(l == 64){
					death = true;
                    break;
                }
                while (1) {
                    rx = rand() % 8;
                    ry = rand() % 8;
                    flag = true;
                    for (i = 1; i <= l; i++) {
                        if (ry == Y[i] && rx == X[i]) {
                            flag = false;
                            break;
                        }
                    }
                    if (flag) break;
                }
            }
            matrix[rx][ry] = 1;
        }
        for (x = 0; x < 8; x++) {
            SelectRow(x);
            for (y = 0; y < 8; y++) {
                Set_LED_in_Active_Row(y, matrix[x][y]);
            }
            xtimer_msleep(2);
        }
        dead(death);
		if (dead(death) == false) {
			death = false;
			r = 0;
		}
    }
    return 0;
}