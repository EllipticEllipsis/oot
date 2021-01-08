#ifndef _Z_EN_TR_H_
#define _Z_EN_TR_H_

#include "ultra64.h"
#include "global.h"

struct EnTr;

typedef void (*EnTrActionFunc)(struct EnTr*, GlobalContext*);

typedef struct EnTr {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ SkelAnime skelAnime;
    /* 0x0190 */ Vec3s jointTable[27];
    /* 0x0232 */ Vec3s morphTable[27];
    /* 0x02D4 */ s16 unk_2D4;
    /* 0x02D6 */ u16 unk_2D6;
    /* 0x02D8 */ s16 unk_2D8;
    /* 0x02DA */ char unk_2DA[0x2];
    /* 0x02DC */ EnTrActionFunc actionFunc;
    /* 0x02E0 */ s16 unk_2E0;
    /* 0x02E2 */ s16 unk_2E2;
    /* 0x02E4 */ AnimationHeader* unk_2E4;
} EnTr; // size = 0x02E8

extern const ActorInit En_Tr_InitVars;

#endif
