
#include <stdio.h>
#include <functional>
#include <algorithm>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_mrt.h"

#include "mcxn-kit.h"
#include "i2c-lib.h"
#include "si4735-lib.h"

void _mcu_initialization() __attribute__(( constructor( 0x100 ) ));

void _mcu_initialization()
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    CLOCK_EnableClock( kCLOCK_Gpio0 );
    CLOCK_EnableClock( kCLOCK_Gpio1 );
    CLOCK_EnableClock( kCLOCK_Gpio2 );
    CLOCK_EnableClock( kCLOCK_Gpio3 );
    CLOCK_EnableClock( kCLOCK_Gpio4 );
}


DigitalOut g_led_P3_16( P3_16 );
DigitalOut g_led_P3_17( P3_17 );

DigitalIn g_but_P3_18( P3_18 );
DigitalIn g_but_P3_19( P3_19 );
DigitalIn g_but_P3_20( P3_20 );
DigitalIn g_but_P3_21( P3_21 );

#define R	0b00000001
#define W	0b00000000

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

extern DigitalOut g_scl, g_sda;

void expander_sloupec(int pocet_led) {
    if (pocet_led < 0) {
    	pocet_led = 0;
    }

    if (pocet_led > 8) {
    	pocet_led = 8;
    }

    uint8_t maska = 0b00000000;

    if (pocet_led == 1) {
    	maska = 0b00000001;
    }

    if (pocet_led == 2) {
    	maska = 0b00000011;
    }

    if (pocet_led == 3) {
    	maska = 0b00000111;
    }

    if (pocet_led == 4) {
    	maska = 0b00001111;
    }

    if (pocet_led == 5) {
    	maska = 0b00011111;
    }

    if (pocet_led == 6) {
    	maska = 0b00111111;
    }

    if (pocet_led == 7) {
    	maska = 0b01111111;
    }

    if (pocet_led == 8) {
    	maska = 0b11111111;
    }

    uint8_t send_data = maska;


    uint8_t adresa = 0x40 | (5 << 1);

    i2c_start();
    i2c_output(adresa | 0);
    i2c_output(send_data);
    i2c_stop();
}

void radio_hlasitost(int hlasitost) {
    if (hlasitost > 63) {
    	hlasitost = 63;
    }

    i2c_start();
    i2c_output(SI4735_ADDRESS | 0);
    i2c_output(0x12);
    i2c_output(0x00);
    i2c_output(0x40);
    i2c_output(0x00);
    i2c_output(0x00);
    i2c_output(hlasitost);
    i2c_stop();
}

void radio_frekvence(int frekvence) {
    i2c_start();
    i2c_output(SI4735_ADDRESS | 0);
    i2c_output(0x20);
    i2c_output(0x00);
    i2c_output(frekvence >> 8);
    i2c_output(frekvence & 0xFF);
    i2c_output(0x00);
    i2c_stop();

    delay_ms(100);
}

void search(int nahoru) {
    i2c_start();
    i2c_output(SI4735_ADDRESS | 0);
    i2c_output(0x21);

    if (nahoru == 1) {
        i2c_output(0x0C);
    } else {
        i2c_output(0x04);
    }
    i2c_stop();

    delay_ms(100);
}

void stav() {
    uint8_t s1, s2, rssi, snr, mult, cap;
    int frekvence;

    i2c_start();
    i2c_output(SI4735_ADDRESS | 0);
    i2c_output(0x22);
    i2c_output(0x00);

    i2c_start();
    i2c_output(SI4735_ADDRESS | 1);

    s1 = i2c_input(); i2c_ack();
    s2 = i2c_input(); i2c_ack();

    frekvence = i2c_input() << 8; i2c_ack();
    frekvence = frekvence | i2c_input(); i2c_ack();

    rssi = i2c_input(); i2c_ack();
    snr  = i2c_input(); i2c_ack();
    mult = i2c_input(); i2c_ack();
    cap  = i2c_input(); i2c_nack();
    i2c_stop();

    PRINTF("Frekvence: %d.%d MHz\r\n", frekvence / 100, frekvence % 100);
    PRINTF("Sila signalu (RSSI): %d\r\n", rssi);
    PRINTF("Kvalita signalu (SNR): %d dB\r\n", snr);
}


int main()
{

    i2c_init();

    if (si4735_init() != 0) {
        PRINTF("Error while starting radio\r\n");
        return 0;
    }
    PRINTF("Radio ready.\r\n");

    int aktualni_frekvence = 10140;
    int aktualni_hlasitost = 30;

    radio_hlasitost(aktualni_hlasitost);
    radio_frekvence(aktualni_frekvence);
    stav();

    int vol_stisknuto_predtim = 0;

    int btn_dolu_stav = 0;
    int btn_dolu_casovac = 0;

    int btn_nahoru_stav = 0;
    int btn_nahoru_casovac = 0;

    int pocet_rozsvicenych_ledek = 0;
    int smer_animace = 1;
    int casovac_animace = 0;

    while (1) {


        if (g_but_P3_18.read() == 0) {
            if (vol_stisknuto_predtim == 0) {
                vol_stisknuto_predtim = 1;

                aktualni_hlasitost = aktualni_hlasitost + 15;
                if (aktualni_hlasitost > 60) {
                    aktualni_hlasitost = 0;
                }
                radio_hlasitost(aktualni_hlasitost);
                PRINTF("Hlasitost je ted: %d\r\n", aktualni_hlasitost);
            }
        } else {
            vol_stisknuto_predtim = 0;
        }

        if (btn_dolu_stav == 0) {
            if (g_but_P3_19.read() == 0) {
                btn_dolu_stav = 1;
            }
        }
        else if (btn_dolu_stav == 1) {
            if (g_but_P3_19.read() != 0) {
                btn_dolu_stav = 2;
                btn_dolu_casovac = 0;
            }
        }

        else if (btn_dolu_stav == 2) {
            if (g_but_P3_19.read() == 0) {
                PRINTF("Vyhledavam stanici dolu...\r\n");
                search(0);
                stav();
                btn_dolu_stav = 3;
            } else {
                btn_dolu_casovac++;
                if (btn_dolu_casovac > 25) {
                    aktualni_frekvence = aktualni_frekvence - 10;
                    PRINTF("Ladim dolu na: %d.%d MHz\r\n", aktualni_frekvence / 100, aktualni_frekvence % 100);
                    radio_frekvence(aktualni_frekvence);
                    stav();
                    btn_dolu_stav = 0;
                }
            }
        }

        else if (btn_dolu_stav == 3) {
            if (g_but_P3_19.read() != 0) {
                btn_dolu_stav = 0;
            }
        }

        if (btn_nahoru_stav == 0) {
            if (g_but_P3_20.read() == 0) btn_nahoru_stav = 1;
        } else if (btn_nahoru_stav == 1) {
            if (g_but_P3_20.read() != 0) {
                btn_nahoru_stav = 2;
                btn_nahoru_casovac = 0;
            }
        } else if (btn_nahoru_stav == 2) {
            if (g_but_P3_20.read() == 0) {
                PRINTF("Vyhledavam stanici nahoru - dvojklik...\r\n");
                search(1);
                stav();
                btn_nahoru_stav = 3;
            } else {
                btn_nahoru_casovac++;
                if (btn_nahoru_casovac > 25) {
                    aktualni_frekvence = aktualni_frekvence + 10;
                    PRINTF("Ladim nahoru na: %d.%d MHz\r\n", aktualni_frekvence / 100, aktualni_frekvence % 100);
                    radio_frekvence(aktualni_frekvence);
                    stav();
                    btn_nahoru_stav = 0;
                }
            }
        } else if (btn_nahoru_stav == 3) {
            if (g_but_P3_20.read() != 0) {
            	btn_nahoru_stav = 0;
            }
        }

        casovac_animace++;
        if (casovac_animace >= 12) {
            casovac_animace = 0;

            expander_sloupec(pocet_rozsvicenych_ledek);

            pocet_rozsvicenych_ledek = pocet_rozsvicenych_ledek + smer_animace;

            if (pocet_rozsvicenych_ledek >= 8) {
                smer_animace = -1;
            }
            if (pocet_rozsvicenych_ledek <= 0) {
                smer_animace = 1;
            }
        }

        delay_ms(10);
    }

    return 0;
}
