#include "gitversion.h"
#include "types.h"

#include "mock_hardware_api.h"
#include "mock_hardware_api_private.h"

#include <string.h>


void hid_parse_frame(u8* data, u8 sz)
{
}

void process_keypress(uint8_t key, uint8_t mod_key, bool is_held_key)
{
}

void hardware_serializePreset(tt_serializer_t* stream, uint8_t preset_num)
{
}

void hardware_deserializePreset(tt_deserializer_t* stream, uint8_t preset_num, bool clearExisting)
{
}

void hardware_afterVRAMUpdate()
{
}

void hardware_afterInit()
{
}

void hardware_afterStep()
{
}

void hardware_getVersion(char* buffer)
{
    strcpy(buffer, git_version);
}

double hardware_getClockPeriod()
{
    return 0.001;
}
