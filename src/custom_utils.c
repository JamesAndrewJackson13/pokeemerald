#include "global.h"
#include "pokemon.h"


u8 getHiddenPowerType(struct Pokemon* mon)
{
    u8 type = ((GetMonData(mon, MON_DATA_HP_IV) & 1) << 0)
        | ((GetMonData(mon, MON_DATA_ATK_IV) & 1) << 1)
        | ((GetMonData(mon, MON_DATA_DEF_IV) & 1) << 2)
        | ((GetMonData(mon, MON_DATA_SPEED_IV) & 1) << 3)
        | ((GetMonData(mon, MON_DATA_SPATK_IV) & 1) << 4)
        | ((GetMonData(mon, MON_DATA_SPDEF_IV) & 1) << 5);

    type = (15 * type) / 63 + 1;
    if (type >= TYPE_MYSTERY)
        type++;
    type |= 0xC0;

    return type;
}