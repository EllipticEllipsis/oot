#ifndef _Z_EN_BILI_H_
#define _Z_EN_BILI_H_

#include "ultra64.h"
#include "global.h"

struct EnBili;

typedef void (*EnBiliActionFunc)(struct EnBili*, GlobalContext*);

typedef enum {
    /* 0 */ EN_BILI_LIMB_NONE,
    /* 1 */ EN_BILI_LIMB_ROOT,
    /* 2 */ EN_BILI_LIMB_INNER_MEMBRANE,
    /* 3 */ EN_BILI_LIMB_OUTER_MEMBRANE,
    /* 4 */ EN_BILI_LIMB_ORALARMS,
    /* 5 */ EN_BILI_LIMB_MAX
} EnBiliLimb;

typedef struct EnBili {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ SkelAnime skelAnime;
    /* 0x0190 */ EnBiliActionFunc actionFunc;
    /* 0x0194 */ u8 oralArmsTexIndex;
    /* 0x0195 */ u8 playFlySound;
    /* 0x0196 */ s16 timer;
    /* 0x0198 */ Vec3s jointTable[EN_BILI_LIMB_MAX];
    /* 0x01B6 */ Vec3s morphTable[EN_BILI_LIMB_MAX];
    /* 0x01D4 */ ColliderCylinder collider;
} EnBili; // size = 0x0220

extern const ActorInit En_Bili_InitVars;

typedef enum {
    /* -1 */ EN_BILI_TYPE_NORMAL = -1,
    /*  0 */ EN_BILI_TYPE_VALI_SPAWNED,
    /*  1 */ EN_BILI_TYPE_DYING
} EnBiliType;

#endif
