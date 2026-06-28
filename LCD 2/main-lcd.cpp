#include <stdio.h>
#include <stdlib.h>
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

extern uint8_t g_font8x8[256][8];


void pixel_vetsi(int x, int y, uint16_t barvicka, int velikost) {
    for (int iy = 0; iy < velikost; iy++) {
        for (int ix = 0; ix < velikost; ix++) {
            if (x + ix < LCD_WIDTH && y + iy < LCD_HEIGHT) {
                lcd_put_pixel(x + ix, y + iy, barvicka);
            }
        }
    }
}

void pismeno(int pozice_x, int pozice_y, char pismeno, uint16_t barva_textu, int zvetseni) {
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

void text(int x, int y, const char* text, uint16_t barva, int zvetseni) {
    int pos_x = x;
    while (*text != '\0') {
        pismeno(pos_x, y, *text, barva, zvetseni);
        pos_x += 8 * zvetseni;
        text++;
    }
}

void obdelnik(int x, int y, int sirka, int vyska, uint16_t barva) {
    for (int i = 0; i < sirka; i++) {
        for (int j = 0; j < vyska; j++) {
            if (x + i >= 0 && x + i < LCD_WIDTH && y + j >= 0 && y + j < LCD_HEIGHT) {
                lcd_put_pixel(x + i, y + j, barva);
            }
        }
    }
}

int hrac_x;
int hrac_y;
int hrac_sirka = 80;
int hrac_vyska = 15;
int stary_hrac_x;

int objekt_x;
int objekt_y;
int objekt_velikost = 20;
int stary_objekt_x;
int stary_objekt_y;
int rychlost_objektu = 5;

int skore = 0;
int skore_prekreslit = 1;

void draw_player() {
    obdelnik(hrac_x, hrac_y, hrac_sirka, hrac_vyska, 0x07E0);
}

void erase_player() {
    obdelnik(stary_hrac_x, hrac_y, hrac_sirka, hrac_vyska, 0x0000);
}

void draw_object() {
    obdelnik(objekt_x, objekt_y, objekt_velikost, objekt_velikost, 0xF800);
}

void erase_object() {
    obdelnik(stary_objekt_x, stary_objekt_y, objekt_velikost, objekt_velikost, 0x0000);
}

void draw_score() {
    if (skore_prekreslit == 1) {
        obdelnik(10, 10, 150, 24, 0x0000);

        char text_skore[20];
        sprintf(text_skore, "Skore: %d", skore);

        text(10, 10, text_skore, 0xFFFF, 2);

        skore_prekreslit = 0;
    }
}

void update_player(int dotyk_x) {
    stary_hrac_x = hrac_x;

    hrac_x = dotyk_x - (hrac_sirka / 2);

    if (hrac_x < 0) {
    	hrac_x = 0;
    }
    if (hrac_x > LCD_WIDTH - hrac_sirka) {
    	hrac_x = LCD_WIDTH - hrac_sirka;
    }

    if (stary_hrac_x != hrac_x) {
        erase_player();
        draw_player();
    }
}

void update_object() {
    stary_objekt_x = objekt_x;
    stary_objekt_y = objekt_y;

    objekt_y += rychlost_objektu;

    erase_object();
    draw_object();
}

void check_collision() {
    if (objekt_y + objekt_velikost >= hrac_y && objekt_y <= hrac_y + hrac_vyska && objekt_x + objekt_velikost >= hrac_x && objekt_x <= hrac_x + hrac_sirka) {

        skore++;
        skore_prekreslit = 1;

        erase_object();

        objekt_y = 0;
        objekt_x = rand() % (LCD_WIDTH - objekt_velikost);
        stary_objekt_x = objekt_x;
    }

    else if (objekt_y >= LCD_HEIGHT) {
        erase_object();

        objekt_y = 0;
        objekt_x = rand() % (LCD_WIDTH - objekt_velikost);
        stary_objekt_x = objekt_x;
    }
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

    hrac_y = LCD_HEIGHT - 30;
    hrac_x = (LCD_WIDTH / 2) - (hrac_sirka / 2);
    stary_hrac_x = hrac_x;

    objekt_y = 0;
    objekt_x = LCD_WIDTH / 2;
    stary_objekt_y = objekt_y;
    stary_objekt_x = objekt_x;

    draw_player();
    draw_object();
    draw_score();

    while (1) {
        cts_points_t dotyky;
        int mame_dotyk = cts_get_ts_points(&dotyky);

        if (mame_dotyk > 0 && dotyky.m_num_points > 0) {
            uint16_t dotyk_x = dotyky.m_points[0].x;
            update_player(dotyk_x);
        }

        update_object();
        check_collision();
        draw_score();

        for (int i = 0; i < 400000; i++) {}
    }

    return 0;
}
