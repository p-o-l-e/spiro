#pragma once

typedef enum 
{
    NOTE_OFF           = 0x80,
    NOTE_ON            = 0x90,
    POLY_PRESSURE      = 0xA0,
    CONTROL_CHANGE     = 0xB0,
    PROGRAM_CHANGE     = 0xC0,
    CHANNEL_PRESSURE   = 0xD0, // Aftertouch
    PITCH_BEND         = 0xE0

} MidiMessage;
