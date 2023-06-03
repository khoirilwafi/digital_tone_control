/*
 * arduino audio processor inteface
 * lcd 16x2 
 * version 1.0
 * 
 */

// rotary encoder reading
int8_t encoder_read()
{
    static uint8_t valid[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};
    prev <<= 2;
    
    if(digitalRead(en_a) == 1){ prev |= 0x02; }
    if(digitalRead(en_b) == 1){ prev |= 0x01; }
    prev &= 0x0f;

    if(valid[prev])
    {
        memo <<= 4;
        memo |= prev;
        
        if(((memo & 0xff) == 0x2b)||((memo & 0xff) == 0xD4)){return -1;}
        if(((memo & 0xff) == 0x17)||((memo & 0xff) == 0xE8)){return  1;}
    }
    
    return 0;
}

// clear all cursor
void cursor_clear(void)
{
    for(uint8_t i=0; i<2; i++)
    {
        lcd.setCursor(7,i);
        lcd.print(" ");
        lcd.setCursor(15,i);
        lcd.print(" ");
    }
}

// display cursor in menu segment
void display_cursor(uint8_t pos)
{
    switch(pos)
    {
        case 0 : lcd.setCursor(7,0);  break;
        case 1 : lcd.setCursor(7,1);  break;
        case 2 : lcd.setCursor(15,0); break;
        case 3 : lcd.setCursor(15,1); break;
    }
    lcd.write((byte)0);
}

// limit value to proper range
bool limit(int8_t *value, int8_t min, uint8_t max)
{
    if(*value > max)
    {
        *value = max;
        return false;
    }
    else if(*value < min)
    {
        *value = min;
        return false;
    }
    else
    {
        *value = *value;
        return true;
    }
}

// display frame page on lcd
void display_page(uint8_t page)
{
    switch(page)
    {
        // input menu
        case 0 :
            lcd.setCursor(0,0);
            lcd.print("Input           ");
            lcd.setCursor(0,1);
            lcd.print("  --        dB  ");

            lcd.setCursor(10,1);
            lcd.print(gain);
            display_mute(mute);   
        break;

        // treble menu
        case 1 :
            lcd.setCursor(0,0);
            lcd.print("Treble      dB  ");
            lcd.setCursor(0,1);
            lcd.print("  --            ");

            lcd.setCursor(9,0);
            lcd.print(treble);
            display_treble_fc(treble_fc); 
        break;

        // middle menu
        case 2 :
            lcd.setCursor(0,0);
            lcd.print("Middle      dB  ");
            lcd.setCursor(0,1);
            lcd.print("                ");

            lcd.setCursor(9,0);
            lcd.print(middle);
            display_middle_qu(middle_qu);
            display_middle_fc(middle_fc);
        break;

        // bass menu
        case 3 :
            lcd.setCursor(0,0);
            lcd.print("Bass        dB  ");
            lcd.setCursor(0,1);
            lcd.print("                ");

            lcd.setCursor(9,0);
            lcd.print(bass);
            display_bass_qu(bass_qu);
            display_bass_fc(bass_fc);
        break;

        // subwoofer menu
        case 4 :
            lcd.setCursor(0,0);
            lcd.print("SubWfr      dB  ");
            lcd.setCursor(0,1);
            lcd.print("  --            ");

            lcd.setCursor(9,0);
            lcd.print(subwoofer);
            display_subwoofer_fc(subwoofer_fc);
        break;

        // volume menu
        case 5 :
            lcd.setCursor(0,0);
            lcd.print("Volume   L:     ");
            lcd.setCursor(0,1);
            lcd.print("    dB   R:     ");

            lcd.setCursor(1,1);
            lcd.print(volume);
            lcd.setCursor(11,0);
            lcd.print(left_att);
            lcd.setCursor(11,1);
            lcd.print(right_att);
        break;
    }
}

// display middle quality factor
void display_middle_qu(uint8_t middle_qu)
{
    lcd.setCursor(0,1);
    
    switch(middle_qu)
    {
        case 0 : lcd.print("0.50Q"); break;
        case 1 : lcd.print("0.75Q"); break;
        case 2 : lcd.print("1.00Q"); break;
        case 3 : lcd.print("1.25Q"); break;
    }
}

// display bass quality factor
void display_bass_qu(uint8_t bass_qu)
{
    lcd.setCursor(0,1);
    switch(bass_qu)
    {
        case 0 : lcd.print("1.00Q"); break;
        case 1 : lcd.print("1.25Q"); break;
        case 2 : lcd.print("1.50Q"); break;
        case 3 : lcd.print("2.00Q"); break;
    }
}

// display treble frequency center
void display_treble_fc(uint8_t fc)
{
    lcd.setCursor(9,1);
    switch(fc)
    {
        case 0 : lcd.print("10.0k"); break;
        case 1 : lcd.print("12.5K"); break;
        case 2 : lcd.print("15.0k"); break;
        case 3 : lcd.print("17.5k"); break;
    }
}

// display middle frequency center
void display_middle_fc(uint8_t fc)
{
    lcd.setCursor(9,1);
    switch(fc)
    {
        case 0 : lcd.print("500Hz"); break;
        case 1 : lcd.print("1.00k"); break;
        case 2 : lcd.print("1.50k"); break;
        case 3 : lcd.print("2.50k"); break;
    }
}

// display bass frequency center
void display_bass_fc(uint8_t fc)
{
    lcd.setCursor(9,1);
    switch(fc)
    {
        case 0 : lcd.print("60.Hz"); break;
        case 1 : lcd.print("80.Hz"); break;
        case 2 : lcd.print("100Hz"); break;
        case 3 : lcd.print("200Hz"); break;
    }
}

// display subwoofer frequency center
void display_subwoofer_fc(uint8_t fc)
{
    lcd.setCursor(9,1);
    switch(fc)
    {
        case 0 : lcd.print("Flat "); break;
        case 1 : lcd.print("80.Hz"); break;
        case 2 : lcd.print("120Hz"); break;
        case 3 : lcd.print("160Hz"); break;
    }
}

// display mute status
void display_mute(uint8_t mute)
{
    lcd.setCursor(10,0);
    if(mute == 0)
    {
        lcd.print("Pass");
    }
    else
    {
        lcd.print("Mute");
    }
}

// menu setting running
void setting_menu(void)
{
    // edit parameter function
    if(edit == true)
    {
        switch(cursor_pos)
        {
            // SECTOR 0
            case 0 :
                page += mod;
                if(limit(&page,0,5))
                {
                    lcd.clear();
                    display_page(page);
                } 
            break;

            // SECTOR 1
            case 1 :
                switch(page)
                {
                    // setting middle
                    case 2 :
                        middle_qu += mod;
                        if(limit(&middle_qu,0,3))
                        {
                            display_middle_qu(middle_qu);
                            set_middle(middle, middle_qu);
                            save_to_eeprom(9,middle_qu);
                        }
                    break;

                    // setting bass quality factor
                    case 3 :
                        bass_qu += mod;
                        if(limit(&bass_qu,0,3))
                        {
                            display_bass_qu(bass_qu);
                            set_bass(bass, bass_qu);
                            save_to_eeprom(11,bass_qu);
                        }
                    break;

                    // setting volume
                    case 5 :
                        volume += mod;
                        if(limit(&volume,-80,15))
                        {
                            lcd.setCursor(1,1); lcd.print("   ");
                            lcd.setCursor(1,1); lcd.print(volume);
                            set_volume(volume);
                        }
                    break;
                }
            break;

            // SECTOR 2 
            case 2 :
                switch(page)
                {
                    // setting mute status
                    case 0 :
                        mute += mod;
                        if(limit(&mute,0,1))
                        {
                            display_mute(mute);
                            set_mute(mute);
                            save_to_eeprom(6,mute);
                        }
                    break;

                    // setting treble
                    case 1 :
                        treble += mod;
                        if(limit(&treble,-15,15))
                        {
                            lcd.setCursor(9,0); lcd.print("   ");
                            lcd.setCursor(9,0); lcd.print(treble);
                            set_treble(treble, treble_fc);
                            save_to_eeprom(1,treble);
                        }
                    break;

                    // setting middle
                    case 2 :
                        middle += mod;
                        if(limit(&middle,-15,15))
                        {
                            lcd.setCursor(9,0); lcd.print("   ");
                            lcd.setCursor(9,0); lcd.print(middle);
                            set_middle(middle, middle_qu);
                            save_to_eeprom(2,middle);
                        }
                    break;

                    // setting bass
                    case 3 :
                        bass += mod;
                        if(limit(&bass,-15,15))
                        {
                            lcd.setCursor(9,0);
                            lcd.print("   ");
                            lcd.setCursor(9,0);
                            lcd.print(bass);
                            set_bass(bass, bass_qu);
                            save_to_eeprom(3,bass);
                        }
                    break;

                    // setting subwoofer
                    case 4 :
                        subwoofer += mod;
                        if(limit(&subwoofer,-80,15))
                        {
                            lcd.setCursor(9,0); lcd.print("   ");
                            lcd.setCursor(9,0); lcd.print(subwoofer);
                            set_subwoofer(subwoofer);
                        }
                    break;

                    // setting left attenuator
                    case 5 :
                        left_att += mod;
                        if(limit(&left_att,-80,15))
                        {
                            lcd.setCursor(11,0);
                            lcd.print("   ");
                            lcd.setCursor(11,0);
                            lcd.print(left_att);
                            set_left_att(left_att);
                            save_to_eeprom(13,left_att);
                        }
                    break;
                }
            break;

            // SECTOR 3
            case 3 :
                switch(page)
                {
                    // setting gain 
                    case 0 :
                        gain += mod;
                        if(limit(&gain,0,15))
                        {
                            lcd.setCursor(10,1);
                            lcd.print("  ");
                            lcd.setCursor(10,1);
                            lcd.print(gain);
                            set_gain(gain);
                            save_to_eeprom(0,gain);
                        }
                    break;

                    // setting treble frequency center
                    case 1 :
                        treble_fc += mod;
                        if(limit(&treble_fc,0,3))
                        {
                            display_treble_fc(treble_fc);
                            set_treble(treble, treble_fc);
                            save_to_eeprom(7,treble_fc);
                        }
                    break;

                    // setting middle frequency center
                    case 2 :
                        middle_fc += mod;
                        if(limit(&middle_fc,0,3))
                        {
                            display_middle_fc(middle_fc);
                            set_frekuensi(subwoofer_fc, middle_fc, bass_fc);
                            save_to_eeprom(8,middle_fc);
                        }
                    break;

                    // setting bass frequency center
                    case 3 :
                        bass_fc += mod;
                        if(limit(&bass_fc,0,3))
                        {
                            display_bass_fc(bass_fc);
                            set_frekuensi(subwoofer_fc, middle_fc, bass_fc);
                            save_to_eeprom(10,bass_fc);
                        }
                    break;

                    // setting subwoofer frequency center
                    case 4 :
                        subwoofer_fc += mod;
                        if(limit(&subwoofer_fc,0,3))
                        {
                            display_subwoofer_fc(subwoofer_fc);
                            set_frekuensi(subwoofer_fc, middle_fc, bass_fc);
                            save_to_eeprom(12,subwoofer_fc);
                        }
                    break;

                    // setting right attenuator
                    case 5 :
                        right_att += mod;
                        if(limit(&right_att,-80,15))
                        {
                            lcd.setCursor(11,1);
                            lcd.print("   ");
                            lcd.setCursor(11,1);
                            lcd.print(right_att);
                            set_right_att(right_att);
                            save_to_eeprom(14,right_att);
                        }
                    break;
                }
            break;
        }
    }

    // not edit -> change cursor position
    else
    {
        cursor_pos += mod;
        if(limit(&cursor_pos, 0,3))
        {
            cursor_clear();
            display_cursor(cursor_pos);
        }
    }
}







