/*
 * comunication function for arduino to tda7419
 * digital processor
 * version 1.0
 * 
 */

// send command to processor
void send_command(uint8_t address, uint8_t value)
{
    i2c.beginTransmission(0x44);
    i2c.write(address);
    i2c.write(value);
    i2c.endTransmission();

    // debugging
    // Serial.print(address, HEX); Serial.print(" -> "); Serial.println(value, BIN);
}

// set gain 
void set_gain(int8_t gain)
{
    send_command(0x40, 0x81 | (gain<<3));
}

// set mute
void set_mute(int8_t mute)
{
    if(mute == 1)
    {
        send_command(0x42, 0xEE);
    }
    else
    {
        send_command(0x42, 0xEF);
    }
}

// set volume
void set_volume(int8_t volume)
{
    if(volume < 0)
    {
        send_command(0x43, 16-volume);
    }
    else
    {
        send_command(0x43, volume);
    }    
}

// set treble
void set_treble(int8_t att, int8_t fc)
{
    if(att < 0)
    {
       send_command(0x44, 0x80 | (fc<<5) | abs(att));
    }
    else
    {
       send_command(0x44, 0x80 | (fc<<5) | (att+16));
    }
}

// set middle
void set_middle(int8_t att, int8_t qu)
{
    if(att < 0)
    {
       send_command(0x45, (qu<<5) | abs(att));
    }
    else
    {
       send_command(0x45, (qu<<5) | (att+16));
    }
}

// set bass
void set_bass(int8_t att, int8_t qu)
{
    if(att < 0)
    {
       send_command(0x46, (qu<<5) | abs(att));
    }
    else
    {
       send_command(0x46, (qu<<5) | (att+16));
    }
}

// set frequency
void set_frekuensi(int8_t sub_fc, int8_t mid_fc, int8_t bass_fc)
{
    send_command(0x48, 0xC0 | (bass_fc<<4) | (mid_fc<<2) | sub_fc);
}

// set left attenuator
void set_left_att(int8_t att)
{
    if(att < 0)
    {
        send_command(0x4C, 16-att);
    }
    else
    {
        send_command(0x4C, att);
    }
}

// set right attenuator
void set_right_att(int8_t att)
{
    if(att < 0)
    {
        send_command(0x4D, 16-att);
    }
    else
    {
        send_command(0x4D, att);
    }
}

// set subwoofer attenuator
void set_subwoofer(int8_t att)
{
    if(att < 0)
    {
        send_command(0x4F, 16-att);
    }
    else
    {
        send_command(0x4F, att);
    }
}

// save parameter to eeprom
void save_to_eeprom(uint8_t address, int8_t value)
{
    EEPROM.write(address, (uint8_t)value);
}

void init_processor(void)
{
    // send constant data to processor
    // this parameter not cange
    
    send_command(0x41, 0x40);   // loudness
    send_command(0x42, 0xEB);   // soft mute - clock generator
    send_command(0x43, 0x5F);   // volume master
    send_command(0x47, 0x01);   // second source selector
    send_command(0x49, 0xF7);   // gain effect
    send_command(0x4A, 0x00);   // left front
    send_command(0x4B, 0x00);   // right front
    send_command(0x4E, 0x00);   // mixing
    send_command(0x4F, 0x5F);   // subwoofer
    send_command(0x50, 0x9C);   // spectrum analizer
    send_command(0x51, 0x00);   // testing

    // reading eeprom value
    gain         = (int8_t)EEPROM.read(0);
    treble       = (int8_t)EEPROM.read(1);
    middle       = (int8_t)EEPROM.read(2);
    bass         = (int8_t)EEPROM.read(3);
    subwoofer    = -80;
    volume       = -80;
    mute                                                                                                                                                                                                                                                                                                                                                                                                                                     = (int8_t)EEPROM.read(6);
    treble_fc    = (int8_t)EEPROM.read(7);
    middle_fc    = (int8_t)EEPROM.read(8);
    middle_qu    = (int8_t)EEPROM.read(9);
    bass_fc      = (int8_t)EEPROM.read(10);
    bass_qu      = (int8_t)EEPROM.read(11);
    subwoofer_fc = (int8_t)EEPROM.read(12);
    left_att     = (int8_t)EEPROM.read(13);
    right_att    = (int8_t)EEPROM.read(14);

    // send parameter from eeprom
    set_mute(1); 
    set_gain(gain);
    set_volume(volume);
    set_treble(treble, treble_fc);
    set_middle(middle, middle_qu);
    set_bass(bass, bass_qu);
    set_frekuensi(subwoofer_fc, middle_fc, bass_fc);
    set_left_att(left_att);
    set_right_att(right_att);
    set_subwoofer(subwoofer);
    set_mute(mute);
}

