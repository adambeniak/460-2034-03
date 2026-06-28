
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

DigitalIn g_but_P3_18( P3_18 );
DigitalIn g_but_P3_19( P3_19 );
DigitalIn g_but_P3_20( P3_20 );
DigitalIn g_but_P3_21( P3_21 );


#define PWM_Period 25

class PWMLED
{
protected:
    pin_name_t m_led_pin;
    DigitalOut m_dig_out;
    Ticker m_ticker;
    uint32_t m_cur_time;
    uint32_t m_on_time;

public:
    PWMLED( pin_name_t t_led_pin ) :
        m_led_pin( t_led_pin ),
        m_dig_out( t_led_pin ),
        m_cur_time( 0 ),
        m_on_time( 0 )
    {
        m_ticker.attach( &PWMLED::callback_pwm, this, 1 );
    }

    void set_jas( uint32_t t_bright_level )
    {
        t_bright_level = std::min( t_bright_level, ( uint32_t ) 100U );
        m_on_time = PWM_Period * t_bright_level / 100;
    }

protected:
    void callback_pwm()
    {
        if ( m_on_time > 0  && m_on_time >= m_cur_time ) {
            m_dig_out.write( 1 );
        } else {
            m_dig_out.write( 0 );
        }
        m_cur_time = ( m_cur_time + 1 ) % PWM_Period;
    }
};


int main()
{
    PRINTF( "LED demo program started...\n" );

#if 1
    PWMLED leds[8] = {
    	 PWMLED(P4_00),  PWMLED(P4_01),  PWMLED(P4_02),  PWMLED(P4_03), PWMLED(P4_12), PWMLED(P4_13), PWMLED(P4_16), PWMLED(P4_20)
    };

    bool funguje = false;
    int aktualni_led = 0;
    int jas = 0;
    bool zhasinani = false;

    for (int i = 0; i < 8; i++) {
        leds[i].set_jas(0);
    }

    while (1)
    {
        if (g_but_P3_21.read() == 0) {
        	funguje = true;
        }
        if (g_but_P3_20.read() == 0) {
        	funguje = false;
        }

        if (g_but_P3_18.read() == 0) {
            funguje = true;
            aktualni_led = 0;
            jas = 0;
            zhasinani = false;
            for (int i = 0; i < 8; i++) leds[i].set_jas(0);
            delay_ms(200);
        }

        if ( funguje ) {
            if (!zhasinani) {
                jas += 5;
                if (jas >= 100) {
                    jas = 100;
                    leds[aktualni_led].set_jas(100);
                    aktualni_led++;
                    jas = 0;

                    if (aktualni_led >= 8) {
                        aktualni_led = 0;
                        jas = 100;
                        zhasinani = true;
                    }
                } else {
                    leds[aktualni_led].set_jas(jas);
                }
            } else {
                jas -= 5;
                if (jas <= 0) {
                    jas = 0;
                    leds[aktualni_led].set_jas(0);
                    aktualni_led++;
                    jas = 100;

                    if (aktualni_led >= 8) {
                        aktualni_led = 0;
                        jas = 0;
                        zhasinani = false;
                    }
                } else {
                    leds[aktualni_led].set_jas(jas);
                }
            }
            delay_ms(30);
        } else {
            delay_ms(30);
        }
    }
#endif

#if 0
    PWMLED led_r(P0_14);
    PWMLED led_g(P0_15);
    PWMLED led_b(P0_22);

    bool funguje = false;

    int prechod = 0;

    int r_hodnota = 100;
    int g_hodnota = 0;
    int b_hodnota = 0;

    led_r.set_jas(r_hodnota);
    led_g.set_jas(g_hodnota);
    led_b.set_jas(b_hodnota);

    while (1)
    {
        if (g_but_P3_21.read() == 0) {
        	funguje = true;
        }
        if (g_but_P3_20.read() == 0) {
        	funguje = false;
        }

        if (g_but_P3_18.read() == 0) {
            delay_ms(1000);
        }

        if (funguje) {
            if (prechod == 0) {
                g_hodnota += 2;
                if (g_hodnota >= 100) {
                    g_hodnota = 100;
                    prechod = 1;
                }
            }
            else if (prechod == 1) {
                r_hodnota -= 2;
                if (r_hodnota <= 0) {
                    r_hodnota = 0;
                    prechod = 2;
                }
            }
            else if (prechod == 2) {
                r_hodnota += 2;
                if ( r_hodnota >= 100 ) {
                    r_hodnota = 100;
                    prechod = 3;
                }
            }
            else if (prechod == 3) {
                g_hodnota -= 2;
                if (g_hodnota <= 0) {
                    g_hodnota = 0;
                    prechod = 0;
                }
            }

            led_r.set_jas(r_hodnota);
            led_g.set_jas(g_hodnota);
        }

        delay_ms(20);
    }
#endif

    while ( 1 )
    {
        __WFI();
    }
}
