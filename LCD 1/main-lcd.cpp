
#include <stdio.h>
#include <math.h>
#include <functional>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_mrt.h"
#include "fsl_lpspi.h"

#include "mcxn-kit.h"
#include "lcd_lib.h"
#include "cts_lib.h"

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


extern uint8_t g_font8x8[256][8];

void pixel_vetsi(int x, int y, uint16_t barvicka, int velikost) {
    for (int iy = 0; iy < velikost; iy++) {
        for (int ix = 0; ix < velikost; ix++) {
            lcd_put_pixel(x + ix, y + iy, barvicka);
        }
    }
}

void napsat_pismeno(int pozice_x, int pozice_y, char pismeno, uint16_t barva_textu, int zvetseni) {
    uint8_t *znak = g_font8x8[(int)pismeno];

    for (int radek = 0; radek < 8; radek++) {
        uint8_t bajt = znak[radek];

        if (bajt & 1) {
        	pixel_vetsi(pozice_x + 0 * zvetseni, pozice_y + radek * zvetseni, barva_textu, zvetseni);
        }
        if (bajt & 2) {
        	pixel_vetsi(pozice_x + 1 * zvetseni, pozice_y + radek * zvetseni, barva_textu, zvetseni);
        }
        if (bajt & 4) {
        	pixel_vetsi(pozice_x + 2 * zvetseni, pozice_y + radek * zvetseni, barva_textu, zvetseni);
        }
        if (bajt & 8) {
        	pixel_vetsi(pozice_x + 3 * zvetseni, pozice_y + radek * zvetseni, barva_textu, zvetseni);
        }
        if (bajt & 16) {
        	pixel_vetsi(pozice_x + 4 * zvetseni, pozice_y + radek * zvetseni, barva_textu, zvetseni);
        }
        if (bajt & 32) {
        	pixel_vetsi(pozice_x + 5 * zvetseni, pozice_y + radek * zvetseni, barva_textu, zvetseni);
        }
        if (bajt & 64) {
        	pixel_vetsi(pozice_x + 6 * zvetseni, pozice_y + radek * zvetseni, barva_textu, zvetseni);
        }
        if (bajt & 128) {
        	pixel_vetsi(pozice_x + 7 * zvetseni, pozice_y + radek * zvetseni, barva_textu, zvetseni);
        }
    }
}

void cara(int x0, int y0, int x1, int y1, uint16_t barva) {
    int dx = x1 - x0;
    if (dx < 0) dx = -dx;
    int dy = y1 - y0;
    if (dy < 0) dy = -dy;

    int sx = -1;
    if (x0 < x1) sx = 1;

    int sy = -1;
    if (y0 < y1) sy = 1;

    int odchylka = dx - dy;

    while (1) {
        lcd_put_pixel(x0, y0, barva);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int odchylka2 = odchylka * 2;
        if (odchylka2 > -dy) {
            odchylka = odchylka - dy;
            x0 = x0 + sx;
        }
        if (odchylka2 < dx) {
            odchylka = odchylka + dx;
            y0 = y0 + sy;
        }
    }
}

void elipsa(int stred_x, int stred_y, int poloosa_x, int poloosa_y, uint16_t barva) {
    for (float stupne = 0; stupne <= 360; stupne += 0.5) {
        float radiany = stupne * 3.14159265 / 180.0;

        int x = stred_x + (poloosa_x * cos(radiany));
        int y = stred_y + (poloosa_y * sin(radiany));

        lcd_put_pixel(x, y, barva);
    }
}

void smazat_prostredek() {
    for (int y = 40; y < LCD_HEIGHT - 40; y++) {
        for (int x = 0; x < LCD_WIDTH; x++) {
            lcd_put_pixel(x, y, 0x0000);
        }
    }
}

void menu(uint16_t zvolena_barva) {
    uint16_t barva_r = 0xF800;
    uint16_t barva_g = 0x07E0;
    uint16_t barva_b = 0x001F;
    uint16_t barva_w = 0xFFFF;

    int mezera = LCD_WIDTH / 4;
    int zvetseni = 3;


    napsat_pismeno(mezera * 0 + 30, 5, 'R', barva_r, zvetseni);
    napsat_pismeno(mezera * 1 + 30, 5, 'G', barva_g, zvetseni);
    napsat_pismeno(mezera * 2 + 30, 5, 'B', barva_b, zvetseni);
    napsat_pismeno(mezera * 3 + 30, 5, 'W', barva_w, zvetseni);

    napsat_pismeno(LCD_WIDTH / 3, LCD_HEIGHT - 35, 'T', barva_w, zvetseni);
    napsat_pismeno(LCD_WIDTH / 3 * 2, LCD_HEIGHT - 35, 'O', barva_w, zvetseni);
}


int main()
{
    PRINTF( "LCD demo started!\r\n" );

    lcd_init();
    if ( cts_init() < 0 )
    {
        PRINTF( "LCD not initialized!\r\n" );
    }

    for (int y = 0; y < LCD_HEIGHT; y++) {
        for (int x = 0; x < LCD_WIDTH; x++) {
            lcd_put_pixel(x, y, 0x0000);
        }
    }

    uint16_t active_barva = 0xFFFF;
    menu(active_barva);

    int zmacknuto = 0;

    while (1) {
        cts_points_t dotyky;
        int dotyk = cts_get_ts_points(&dotyky);

        if (dotyk > 0 && dotyky.m_num_points > 0) {
            if (zmacknuto == 0) {
                zmacknuto = 1;

                uint16_t dotyk_x = dotyky.m_points[0].x;
                uint16_t dotyk_y = dotyky.m_points[0].y;

                if (dotyk_y < 40) {
                    int mezera = LCD_WIDTH / 4;
                    if (dotyk_x < mezera * 1) {
                        active_barva = 0xF800;
                    } else if (dotyk_x < mezera * 2) {
                        active_barva = 0x07E0;
                    } else if (dotyk_x < mezera * 3) {
                        active_barva = 0x001F;
                    } else if (dotyk_x < LCD_WIDTH) {
                        active_barva = 0xFFFF;
                    }
                    menu(active_barva);
                }
                else if (dotyk_y > LCD_HEIGHT - 40) {
                    if (dotyk_x < LCD_WIDTH / 2) {
                        smazat_prostredek();
                        int stred_xx = LCD_WIDTH / 2;
                        int stred_yy = LCD_HEIGHT / 2;

                        int bod1_x = stred_xx;
                        int bod1_y = stred_yy - 28;

                        int bod2_x = stred_xx - 25;
                        int bod2_y = stred_yy + 14;

                        int bod3_x = stred_xx + 25;
                        int bod3_y = stred_yy + 14;

                        cara(bod1_x, bod1_y, bod2_x, bod2_y, active_barva);
                        cara(bod2_x, bod2_y, bod3_x, bod3_y, active_barva);
                        cara(bod3_x, bod3_y, bod1_x, bod1_y, active_barva);
                    } else if (dotyk_x >= LCD_WIDTH / 2) {
                        smazat_prostredek();
                        elipsa(LCD_WIDTH / 2, LCD_HEIGHT / 2, 40, 25, active_barva);
                    }
                }
            }
        } else {
            zmacknuto = 0;
        }
    }

    return 0;
}
