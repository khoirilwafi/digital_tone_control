/*
 * DIGITAL AUDIO PROCESSOR
 * Arduino digital audio processor
 * 3 band equalizer with subwoofer filter
 * 
 * Muhammad Khoiril Wafi
 * DIY Tech Engineering
 * Indonesia
 * 
 */

#include <SoftwareWire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

// peripheral setting
SoftwareWire i2c(13,12);
LiquidCrystal lcd(2,4,5,6,7,8);

// encoder variable
#define en_a    10
#define en_b    11
#define en_sw   9

static uint8_t prev = 0;
static uint8_t memo = 0;
int8_t mod;

// database variable
int8_t gain, treble, middle, bass, subwoofer, volume;
int8_t mute, treble_fc, middle_fc, middle_qu, bass_fc;
int8_t bass_qu, subwoofer_fc, left_att, right_att;

// system interface variable
int8_t cursor_pos = 0;
int8_t page = 0;
bool edit = true;
bool display_refresh = true;
bool cursor_on_screen = true;

// millis tick for cursor blink
unsigned long previous_tick = 0;
unsigned long current_tick = 0;

// cursor character
uint8_t cursor_icon[] = 
{
    0x02, 0x06, 0x0E, 0x1E,
    0x0E, 0x06, 0x02, 0x00
};

void setup(void)
{
    // init peripheral
    // Serial.begin(9600);
    lcd.begin(16,2);
    i2c.begin();

    // setting pin
    pinMode(en_a,  INPUT_PULLUP);
    pinMode(en_b,  INPUT_PULLUP);
    pinMode(en_sw, INPUT_PULLUP);  

    // create cursor icon
    lcd.createChar(0,cursor_icon);

    // cek koneksi
    i2c.beginTransmission(0x44);
    if(i2c.endTransmission() != 0)
    {
        lcd.setCursor(0,0); lcd.print(" dsp error :(   ");
        lcd.setCursor(0,1); lcd.print(" 44h not found  ");
        while(1);
    }

    // init processor
    init_processor();

    // splash screen
    lcd.setCursor(0,0); lcd.print(" Digital  Audio ");
    lcd.setCursor(0,1); lcd.print("   Processing   ");
    delay(3000);
    lcd.clear();

    // display home screen
    display_page(0);
    display_cursor(0);
}

void loop(void)
{
    // reading encoder push button
    if(digitalRead(en_sw) == 0)
    {
        while(digitalRead(en_sw) == 0);
        if(edit == true)
        {
            display_cursor(cursor_pos);
            edit = false;
        }
        else edit = true;
    }

    // read encoder position
    mod = encoder_read();

    // if encoder change 
    if(mod != 0)
    {
        setting_menu();
    }

    // blink cursor if edit parameter
    if(edit == true)
    {
        current_tick = millis();
        if(current_tick - previous_tick >= 400)
        {
            previous_tick = current_tick;
            if(cursor_on_screen == true)
            {
                cursor_clear();
                cursor_on_screen = false;
            }
            else
            {
                display_cursor(cursor_pos);
                cursor_on_screen = true;
            }
        }
    }

    // end of program
}

