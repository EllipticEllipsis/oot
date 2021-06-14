#include "z_en_dekubaba.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000005

#define THIS ((EnDekubaba*)thisx)

void EnDekubaba_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDekubaba_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDekubaba_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDekubaba_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809E5ABC(EnDekubaba* this);
void func_809E5D28(EnDekubaba* this);
void func_809E64F4(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E65A0(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E6A04(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E6ED4(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E7104(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E738C(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E7458(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E77E4(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E78DC(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E79EC(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E7A88(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E7BB0(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E7F14(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E80D8(EnDekubaba* this, GlobalContext* globalCtx);

void func_809E858C(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E86B8(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E89E4(EnDekubaba* this, GlobalContext* globalCtx);
void func_809E8AD8(EnDekubaba* this, GlobalContext* globalCtx);

extern AnimationHeader D_06000208;
extern AnimationHeader D_060002B8;
extern Gfx D_060010F0[]; // leaf base
extern Gfx D_06001330[]; // mid third of stem
extern Gfx D_06001628[]; // lower third of stem
extern Gfx D_06001828[]; // upper third of stem
extern SkeletonHeader D_06002A40;
extern Gfx D_06003070[]; // deku stick drop

static Vec3f D_809E8EA0 = { 0.0f, 0.0f, 0.0f };

const ActorInit En_Dekubaba_InitVars = {
    ACTOR_EN_DEKUBABA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DEKUBABA,
    sizeof(EnDekubaba),
    (ActorFunc)EnDekubaba_Init,
    (ActorFunc)EnDekubaba_Destroy,
    (ActorFunc)EnDekubaba_Update,
    (ActorFunc)EnDekubaba_Draw,
};

static ColliderJntSphElementInit D_809E8ECC[7] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_HARD,
            BUMP_ON,
            OCELEM_ON,
        },
        { 1, { { 0, 100, 1000 }, 15 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 51, { { 0, 0, 1500 }, 8 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 52, { { 0, 0, 500 }, 8 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 53, { { 0, 0, 1500 }, 8 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 54, { { 0, 0, 500 }, 8 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 55, { { 0, 0, 1500 }, 8 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 56, { { 0, 0, 500 }, 8 }, 100 },
    },
};

static ColliderJntSphInit D_809E8FC8 = {
    {
        COLTYPE_HIT6,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    7,
    D_809E8ECC,
};

static CollisionCheckInfoInit D_809E8FD8 = { 2, 25, 25, MASS_IMMOVABLE };

static DamageTable D_809E8FE0 = {
    /* Deku nut      */ DMG_ENTRY(0, 0x1),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(2, 0xE),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(2, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
    /* Fire arrow    */ DMG_ENTRY(4, 0x2),
    /* Ice arrow     */ DMG_ENTRY(2, 0x0),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, 0x2),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0xF),
    /* Giant spin    */ DMG_ENTRY(4, 0xF),
    /* Master spin   */ DMG_ENTRY(2, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(2, 0xF),
    /* Giant jump    */ DMG_ENTRY(8, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static DamageTable D_809E9000 = {
    /* Deku nut      */ DMG_ENTRY(0, 0x1),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(2, 0xE),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x1),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
    /* Fire arrow    */ DMG_ENTRY(4, 0x2),
    /* Ice arrow     */ DMG_ENTRY(2, 0x0),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, 0x2),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0xF),
    /* Giant spin    */ DMG_ENTRY(4, 0xF),
    /* Master spin   */ DMG_ENTRY(2, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(2, 0xF),
    /* Giant jump    */ DMG_ENTRY(8, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry D_809E9020[] = {
    ICHAIN_F32(targetArrowOffset, 1500, ICHAIN_STOP),
};

static Color_RGBA8 D_809E9024 = { 105, 255, 105, 255 };
static Color_RGBA8 D_809E9028 = { 150, 250, 150, 0 };

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/EnDekubaba_Init.s")
void EnDekubaba_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDekubaba* this = THIS;

    s32 i;

    Actor_ProcessInitChain(&this->actor, D_809E9020);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 22.0f);
    SkelAnime_Init(globalCtx, &this->skelAnime, &D_06002A40, &D_060002B8, this->jointTable, this->morphTable, 8);
    Collider_InitJntSph(globalCtx, &this->collider);
    Collider_SetJntSph(globalCtx, &this->collider, &this->actor, &D_809E8FC8, this->colliderElements);

    if (this->actor.params == 1) {
        this->unk_230 = 2.5f;

        for (i = 0; i < D_809E8FC8.count; i++) {
            this->collider.elements[i].dim.worldSphere.radius = this->collider.elements[i].dim.modelSphere.radius =
                (D_809E8ECC[i].dim.modelSphere.radius * 2.50f);
        }

        if (gSaveContext.linkAge != 0) {
            D_809E9000.table[0x1B] = 4;
        }
        CollisionCheck_SetInfo(&this->actor.colChkInfo, &D_809E9000, &D_809E8FD8);
        this->actor.colChkInfo.health = 4;
        this->actor.naviEnemyId = 8;
        this->actor.targetMode = 2;
    } else {
        this->unk_230 = 1.0f;

        for (i = 0; i < D_809E8FC8.count; i++) {
            this->collider.elements[i].dim.worldSphere.radius = this->collider.elements[i].dim.modelSphere.radius;
        }
        if (gSaveContext.linkAge != 0) {
            D_809E8FE0.table[0x1B] = 4;
        }
        CollisionCheck_SetInfo(&this->actor.colChkInfo, &D_809E8FE0, &D_809E8FD8);
        this->actor.naviEnemyId = 7;
        this->actor.targetMode = 1;
    }

    func_809E5ABC(this);
    this->timer = 0;
    this->unk_234 = 0;
    this->unk_170 = this->actor.home.pos;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/EnDekubaba_Destroy.s")
void EnDekubaba_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnDekubaba* this = THIS;

    Collider_DestroyJntSph(globalCtx, &this->collider);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E5A38.s")
void func_809E5A38(EnDekubaba* this) {
    s32 i;

    for (i = 1; i < 7; i++) {
        this->collider.elements[i].info.bumperFlags &= ~1;
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E5ABC.s")
void func_809E5ABC(EnDekubaba* this) {
    s32 i;
    ColliderJntSphElement* element;

    this->actor.shape.rot.x = -0x4000;
    this->unk_1CA[0] = this->unk_1CA[1] = this->unk_1CA[2] = this->actor.shape.rot.x;

    this->actor.world.pos.x = this->actor.home.pos.x;
    this->actor.world.pos.z = this->actor.home.pos.z;
    this->actor.world.pos.y = this->actor.home.pos.y + (14.0f * this->unk_230);

    Actor_SetScale(&this->actor, this->unk_230 * 0.01f * 0.5f);

    this->collider.base.colType = COLTYPE_HARD;
    this->collider.base.acFlags |= 4;
    this->timer = 45;

    for (i = 1; i < 7; i++) {
        element = &this->collider.elements[i];
        element->dim.worldSphere.center.x = this->actor.world.pos.x;
        element->dim.worldSphere.center.y = (s16)this->actor.world.pos.y - 7;
        element->dim.worldSphere.center.z = this->actor.world.pos.z;
    }

    this->actionFunc = func_809E64F4;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E5D28.s")
void func_809E5D28(EnDekubaba* this) {
    s32 i;

    Animation_Change(&this->skelAnime, &D_060002B8, Animation_GetLastFrame(&D_060002B8) * 0.06666667f, 0.0f,
                     Animation_GetLastFrame(&D_060002B8), 2, 0.0f);

    this->timer = 15;

    for (i = 2; i < 7; i++) {
        this->collider.elements[i].info.ocElemFlags |= 1;
    }

    this->collider.base.colType = COLTYPE_HIT6;
    this->collider.base.acFlags &= ~4;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_DUMMY482);
    this->actionFunc = func_809E65A0;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E5E58.s")
void func_809E5E58(EnDekubaba* this) {
    s32 i;

    Animation_Change(&this->skelAnime, &D_060002B8, -1.5f, Animation_GetLastFrame(&D_060002B8), 0.0f, 2, -3.0f);

    this->timer = 15;

    for (i = 2; i < 7; i++) {
        this->collider.elements[i].info.ocElemFlags &= ~1;
    }

    this->actionFunc = func_809E6A04;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E5F44.s")
void func_809E5F44(EnDekubaba* this) {
    this->timer = Animation_GetLastFrame(&D_060002B8) * 2;
    Animation_MorphToLoop(&this->skelAnime, &D_060002B8, -3.0f);
    this->actionFunc = func_809E6ED4;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E5F9C.s")
void func_809E5F9C(EnDekubaba* this) {
    this->timer = 8;
    this->skelAnime.playSpeed = 0.0f;
    this->actionFunc = func_809E738C;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E5FBC.s")
void func_809E5FBC(EnDekubaba* this) {
    Animation_PlayOnce(&this->skelAnime, &D_06000208);
    this->timer = 0;
    this->actionFunc = func_809E7104;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E6000.s")
void func_809E6000(EnDekubaba* this) {
    Animation_Change(&this->skelAnime, &D_06000208, 1.0f, 15.0f, Animation_GetLastFrame(&D_06000208), 2, -3.0f);
    this->timer = 0;
    this->actionFunc = func_809E7458;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E6078.s")
void func_809E6078(EnDekubaba* this) {
    this->timer = 9;
    this->collider.base.acFlags |= 1;
    this->skelAnime.playSpeed = -1.0f;
    this->actionFunc = func_809E77E4;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E60A8.s")
void func_809E60A8(EnDekubaba* this, s32 arg1) {
    Animation_MorphToPlayOnce(&this->skelAnime, &D_06000208, -5.0f);
    this->timer = arg1;
    this->collider.base.acFlags &= ~1;
    Actor_SetScale(&this->actor, this->unk_230 * 0.01f);

    if (arg1 == 2) {
        Actor_SetColorFilter(&this->actor, 0, 0x9B, 0, 0x3E);
    } else {
        Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 0x2A);
    }

    this->actionFunc = func_809E78DC;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E6170.s")
void func_809E6170(EnDekubaba* this) {
    this->timer = 0;
    this->skelAnime.playSpeed = 0.0f;
    this->actor.gravity = -0.8f;
    this->actor.velocity.y = 4.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y + 0x8000;
    this->collider.base.acFlags &= ~1;
    this->actor.speedXZ = this->unk_230 * 3.0f;
    this->actor.flags |= 0x30;
    this->actionFunc = func_809E7BB0;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E61E0.s")
void func_809E61E0(EnDekubaba* this) {
    Animation_Change(&this->skelAnime, &D_060002B8, -1.5f, Animation_GetLastFrame(&D_060002B8), 0.0f, 2, -3.0f);
    this->collider.base.acFlags &= ~1;
    this->actionFunc = func_809E7F14;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E6264.s")
void func_809E6264(EnDekubaba* this) {
    s32 i;

    for (i = 1; i < 7; i++) {
        this->collider.elements[i].info.bumperFlags |= 1;
    }

    if (this->timer == 1) {
        Animation_Change(&this->skelAnime, &D_060002B8, 4.0f, 0.0f, Animation_GetLastFrame(&D_060002B8), 0, -3.0f);
        this->timer = 40;
    } else {
        Animation_Change(&this->skelAnime, &D_060002B8, 0.0f, 0.0f, Animation_GetLastFrame(&D_060002B8), 0, -3.0f);
        this->timer = 60;
    }

    this->actor.world.pos.x = this->actor.home.pos.x;
    this->actor.world.pos.y = this->actor.home.pos.y + (60.0f * this->unk_230);
    this->actor.world.pos.z = this->actor.home.pos.z;
    this->actionFunc = func_809E79EC;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E63EC.s")
void func_809E63EC(EnDekubaba* this) {
    this->unk_1C8 = -0x6000;
    this->unk_1CA[2] = -0x5000;
    this->unk_1CA[1] = -0x4800;

    func_809E5A38(this);
    Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 0x23);
    this->collider.base.acFlags &= ~1;
    this->actionFunc = func_809E7A88;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E645C.s")
void func_809E645C(EnDekubaba* this, GlobalContext* globalCtx) {
    Actor_SetScale(&this->actor, 0.03f);
    this->actor.shape.rot.x += -0x4000;
    this->actor.shape.yOffset = 1000.0f;
    this->actor.gravity = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.shape.shadowScale = 3.0f;
    Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &this->actor, ACTORCAT_MISC);
    this->actor.flags &= ~0x20;
    this->timer = 200;
    this->actionFunc = func_809E80D8;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E64F4.s")
void func_809E64F4(EnDekubaba* this, GlobalContext* globalCtx) {
    if (this->timer != 0) {
        this->timer--;
    }

    this->actor.world.pos.x = this->actor.home.pos.x;
    this->actor.world.pos.z = this->actor.home.pos.z;
    this->actor.world.pos.y = this->actor.home.pos.y + (14.0f * this->unk_230);

    if ((this->timer == 0) && (this->actor.xzDistToPlayer < 200.0f * this->unk_230) &&
        (fabsf(this->actor.yDistToPlayer) < 30.0f * this->unk_230)) {
        func_809E5D28(this);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E65A0.s")
void func_809E65A0(EnDekubaba* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;
    f32 phi_f12;
    f32 sp5C;
    f32 sp58;
    f32 temp_f18;

    if (this->timer != 0) {
        this->timer--;
    }

    SkelAnime_Update(&this->skelAnime);

    this->actor.scale.x = this->actor.scale.y = this->actor.scale.z =
        this->unk_230 * 0.01f * (0.5f + (((15 - this->timer) * 0.5f) / 15.0f));
    Math_ScaledStepToS(&this->actor.shape.rot.x, 0x1800, 0x800);

    sp5C = (sinf(CLAMP_MAX((15 - this->timer) * 0.06666667f, 0.7f) * M_PI) * 32.0f) + 14.0f;

    if (this->actor.shape.rot.x < -0x38E3) {
        phi_f12 = 0.0f;
    } else if (this->actor.shape.rot.x < -0x238E) {
        Math_ScaledStepToS(&this->unk_1CA[0], -0x5555, 0x38E);
        phi_f12 = Math_CosS(this->unk_1CA[0]) * 20.0f;
    } else if (this->actor.shape.rot.x < -0xE38) {
        Math_ScaledStepToS(&this->unk_1CA[0], -0xAAA, 0x38E);
        Math_ScaledStepToS(&this->unk_1CA[1], -0x5555, 0x38E);
        Math_ScaledStepToS(&this->unk_1CA[2], -0x5555, 0x222);

        phi_f12 = (20.0f * (Math_CosS(this->unk_1CA[0]) + Math_CosS(this->unk_1CA[1]))) +
                  (((sp5C - (20.0f * (-Math_SinS(this->unk_1CA[0]) - Math_SinS(this->unk_1CA[1])))) *
                    Math_CosS(this->unk_1CA[2])) /
                   -Math_SinS(this->unk_1CA[2]));
    } else {
        Math_ScaledStepToS(&this->unk_1CA[0], -0xAAA, 0x38E);
        Math_ScaledStepToS(&this->unk_1CA[1], -0x31C7, 0x222);
        Math_ScaledStepToS(&this->unk_1CA[2], -0x5555, 0x222);

        phi_f12 = (20.0f * (Math_CosS(this->unk_1CA[0]) + Math_CosS(this->unk_1CA[1]))) +
                  (((sp5C - (20.0f * (-Math_SinS(this->unk_1CA[0]) - Math_SinS(this->unk_1CA[1])))) *
                    Math_CosS(this->unk_1CA[2])) /
                   -Math_SinS(this->unk_1CA[2]));
    }

    if (this->timer < 10) {
        Math_ApproachS(&this->actor.shape.rot.y, Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos), 2,
                       0xE38);
        if (temp_f18) {} // One way of fake-matching
    }

    this->actor.world.pos.y = this->actor.home.pos.y + (sp5C * this->unk_230);
    sp58 = Math_SinS(this->actor.shape.rot.y) * (phi_f12 * this->unk_230);
    temp_f18 = Math_CosS(this->actor.shape.rot.y) * (phi_f12 * this->unk_230);
    this->actor.world.pos.x = this->actor.home.pos.x + sp58;
    this->actor.world.pos.z = this->actor.home.pos.z + temp_f18;

    EffectSsHahen_SpawnBurst(globalCtx, &this->actor.home.pos, this->unk_230 * 3.0f, 0, this->unk_230 * 12.0f,
                             this->unk_230 * 5.0f, 1, -1, 0xA, NULL);

    if (this->timer == 0) {
        if (Math_Vec3f_DistXZ(&this->actor.home.pos, &player->actor.world.pos) < (240.0f * this->unk_230)) {
            func_809E5F9C(this);
        } else {
            func_809E5E58(this);
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E6A04.s")
void func_809E6A04(EnDekubaba* this, GlobalContext* globalCtx) {
    f32 horizontalScale;
    f32 sp58;
    f32 xShift;
    f32 zShift;

    if (this->timer != 0) {
        this->timer--;
    }

    SkelAnime_Update(&this->skelAnime);

    this->actor.scale.x = this->actor.scale.y = this->actor.scale.z =
        this->unk_230 * 0.01f * (0.5f + this->timer * 0.033333335f);
    Math_ScaledStepToS(&this->actor.shape.rot.x, -0x4000, 0x300);

    sp58 = (sinf(CLAMP_MAX(this->timer * 0.033f, 0.7f) * M_PI) * 32.0f) + 14.0f;

    if (this->actor.shape.rot.x < -0x38E3) {
        horizontalScale = 0.0f;
    } else if (this->actor.shape.rot.x < -0x238E) {
        Math_ScaledStepToS(&this->unk_1CA[0], -0x4000, 0x555);
        horizontalScale = Math_CosS(this->unk_1CA[0]) * 20.0f;
    } else if (this->actor.shape.rot.x < -0xE38) {
        Math_ScaledStepToS(&this->unk_1CA[0], -0x5555, 0x555);
        Math_ScaledStepToS(&this->unk_1CA[1], -0x4000, 0x555);
        Math_ScaledStepToS(&this->unk_1CA[2], -0x4000, 0x333);

        horizontalScale = (20.0f * (Math_CosS(this->unk_1CA[0]) + Math_CosS(this->unk_1CA[1]))) +
                          (((sp58 - (20.0f * (-Math_SinS(this->unk_1CA[0]) - Math_SinS(this->unk_1CA[1])))) *
                            Math_CosS(this->unk_1CA[2])) /
                           -Math_SinS(this->unk_1CA[2]));
    } else {
        Math_ScaledStepToS(&this->unk_1CA[0], -0x5555, 0x555);
        Math_ScaledStepToS(&this->unk_1CA[1], -0x5555, 0x333);
        Math_ScaledStepToS(&this->unk_1CA[2], -0x4000, 0x333);

        horizontalScale = (20.0f * (Math_CosS(this->unk_1CA[0]) + Math_CosS(this->unk_1CA[1]))) +
                          (((sp58 - (20.0f * (-Math_SinS(this->unk_1CA[0]) - Math_SinS(this->unk_1CA[1])))) *
                            Math_CosS(this->unk_1CA[2])) /
                           -Math_SinS(this->unk_1CA[2]));
    }

    this->actor.world.pos.y = this->actor.home.pos.y + (sp58 * this->unk_230);
    xShift = Math_SinS(this->actor.shape.rot.y) * (horizontalScale * this->unk_230);
    zShift = Math_CosS(this->actor.shape.rot.y) * (horizontalScale * this->unk_230);
    this->actor.world.pos.x = this->actor.home.pos.x + xShift;
    this->actor.world.pos.z = this->actor.home.pos.z + zShift;

    EffectSsHahen_SpawnBurst(globalCtx, &this->actor.home.pos, this->unk_230 * 3.0f, 0, this->unk_230 * 12.0f,
                             this->unk_230 * 5.0f, 1, -1, 0xA, NULL);

    if (this->timer == 0) {
        func_809E5ABC(this);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E6DCC.s")
void func_809E6DCC(EnDekubaba* this) {
    f32 sp2C = (Math_CosS(this->unk_1CA[0]) + Math_CosS(this->unk_1CA[1]) + Math_CosS(this->unk_1CA[2])) * 20.0f;

    this->actor.world.pos.x = this->actor.home.pos.x + (Math_SinS(this->actor.shape.rot.y) * (sp2C * this->unk_230));
    this->actor.world.pos.y =
        this->actor.home.pos.y -
        ((Math_SinS(this->unk_1CA[0]) + Math_SinS(this->unk_1CA[1]) + Math_SinS(this->unk_1CA[2])) * 20.0f *
         this->unk_230);
    this->actor.world.pos.z = this->actor.home.pos.z + (Math_CosS(this->actor.shape.rot.y) * (sp2C * this->unk_230));
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E6ED4.s")
void func_809E6ED4(EnDekubaba* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;

    SkelAnime_Update(&this->skelAnime);
    if (Animation_OnFrame(&this->skelAnime, 0.0f) || Animation_OnFrame(&this->skelAnime, 12.0f)) {
        if (this->actor.params == 1) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_DEKU_MOUTH);
        } else {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_DEKU_JR_MOUTH);
        }
    }

    if (this->timer != 0) {
        this->timer--;
    }

    Math_ApproachS(&this->actor.shape.rot.y, Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos), 2,
                   (this->timer % 5) * 0x222);

    if (this->timer < 10) {
        this->unk_1CA[0] += 0x16C;
        this->unk_1CA[1] += 0x16C;
        this->unk_1CA[2] += 0xB6;
        this->actor.shape.rot.x += 0x222;
    } else if (this->timer < 20) {
        this->unk_1CA[0] += -0x16C;
        this->unk_1CA[1] += 0x111;
        this->actor.shape.rot.x += 0x16C;
    } else if (this->timer < 30) {
        this->unk_1CA[1] += -0x111;
        this->actor.shape.rot.x += -0xB6;
    } else {
        this->unk_1CA[1] += -0xB6;
        this->unk_1CA[2] += 0xB6;
        this->actor.shape.rot.x -= 0x16C;
    }

    func_809E6DCC(this);

    if (240.0f * this->unk_230 < Math_Vec3f_DistXZ(&this->actor.home.pos, &player->actor.world.pos)) {
        func_809E5E58(this);
    } else if ((this->timer == 0) || (this->actor.xzDistToPlayer < 80.0f * this->unk_230)) {
        func_809E5F9C(this);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E7104.s")
void func_809E7104(EnDekubaba* this, GlobalContext* globalCtx) {
    s32 allStepsDone;
    s16 curFrame10;
    Vec3f velocity;

    SkelAnime_Update(&this->skelAnime);

    if (this->timer == 0) {
        if (Animation_OnFrame(&this->skelAnime, 1.0f) != 0) {
            if (this->actor.params == 1) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_DEKU_ATTACK);
            } else {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_DEKU_JR_ATTACK);
            }
        }

        Math_ScaledStepToS(&this->actor.shape.rot.x, 0, 0x222);

        curFrame10 = this->skelAnime.curFrame * 10.0f;
        allStepsDone = Math_ScaledStepToS(this->unk_1CA, -0xE38, curFrame10 + 0x38E) & 1;
        allStepsDone &= Math_ScaledStepToS(&this->unk_1CA[1], -0xE38, curFrame10 + 0x71C);
        allStepsDone &= Math_ScaledStepToS(&this->unk_1CA[2], -0xE38, curFrame10 + 0xE38);

        if (allStepsDone) {
            Animation_PlayLoopSetSpeed(&this->skelAnime, &D_060002B8, 4.0f);
            velocity.x = Math_SinS(this->actor.shape.rot.y) * 5.0f;
            velocity.y = 0.0f;
            velocity.z = Math_CosS(this->actor.shape.rot.y) * 5.0f;

            func_8002829C(globalCtx, &this->actor.world.pos, &velocity, &D_809E8EA0, &D_809E9024, &D_809E9028, 1,
                          this->unk_230 * 100.0f);
            this->timer = 1;
            this->collider.base.acFlags |= 1;
        }
    } else if (this->timer > 10) {
        func_809E6000(this);
    } else {
        this->timer++;

        if ((this->timer >= 4) && !func_8002E084(&this->actor, 0x16C)) {
            Math_ApproachS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xF, 0x71C);
        }

        if (Animation_OnFrame(&this->skelAnime, 0.0f) || Animation_OnFrame(&this->skelAnime, 12.0f)) {
            if (this->actor.params == 1) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_DEKU_MOUTH);
            } else {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_DEKU_JR_MOUTH);
            }
        }
    }

    func_809E6DCC(this);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E738C.s")
void func_809E738C(EnDekubaba* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;

    if (this->timer != 0) {
        this->timer--;
    }

    Math_SmoothStepToS(&this->actor.shape.rot.x, 0x1800, 2, 0xE38, 0x71C);
    Math_ApproachS(&this->actor.shape.rot.y, Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos), 2, 0xE38);
    Math_ScaledStepToS(&this->unk_1CA[0], 0xAAA, 0x444);
    Math_ScaledStepToS(&this->unk_1CA[1], -0x4718, 0x888);
    Math_ScaledStepToS(&this->unk_1CA[2], -0x6AA4, 0x888);
    if (this->timer == 0) {
        func_809E5FBC(this);
    }
    func_809E6DCC(this);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E7458.s")
void func_809E7458(EnDekubaba* this, GlobalContext* globalCtx) {
    Vec3f dustPos;
    f32 xIncr;
    f32 zIncr;
    s32 i;

    SkelAnime_Update(&this->skelAnime);

    if (this->timer == 0) {
        Math_ScaledStepToS(&this->actor.shape.rot.x, -0x93E, 0x38E);
        Math_ScaledStepToS(this->unk_1CA, -0x888, 0x16C);
        Math_ScaledStepToS(&this->unk_1CA[1], -0x888, 0x16C);
        if (Math_ScaledStepToS(&this->unk_1CA[2], -0x888, 0x16C)) {
            xIncr = Math_SinS(this->actor.shape.rot.y) * 30.0f * this->unk_230;
            zIncr = Math_CosS(this->actor.shape.rot.y) * 30.0f * this->unk_230;
            dustPos = this->actor.home.pos;

            for (i = 0; i < 3; i++) {
                func_800286CC(globalCtx, &dustPos, &D_809E8EA0, &D_809E8EA0, this->unk_230 * 500.0f,
                              this->unk_230 * 50.0f);
                dustPos.x += xIncr;
                dustPos.z += zIncr;
            }

            this->timer = 1;
        }
    } else if (this->timer == 11) {
        Math_ScaledStepToS(&this->actor.shape.rot.x, -0x93E, 0x200);
        Math_ScaledStepToS(&this->unk_1CA[0], -0xAAA, 0x200);
        Math_ScaledStepToS(&this->unk_1CA[2], -0x5C71, 0x200);

        if (Math_ScaledStepToS(&this->unk_1CA[1], 0x238C, 0x200)) {
            this->timer = 12;
        }
    } else if (this->timer == 18) {
        Math_ScaledStepToS(&this->actor.shape.rot.x, 0x2AA8, 0xAAA);

        if (Math_ScaledStepToS(this->unk_1CA, 0x1554, 0x5B0)) {
            this->timer = 25;
        }

        Math_ScaledStepToS(&this->unk_1CA[1], -0x38E3, 0xAAA);
        Math_ScaledStepToS(&this->unk_1CA[2], -0x5C71, 0x2D8);
    } else if (this->timer == 25) {
        Math_ScaledStepToS(&this->actor.shape.rot.x, -0x5550, 0xAAA);

        if (Math_ScaledStepToS(this->unk_1CA, -0x6388, 0x93E)) {
            this->timer = 26;
        }

        Math_ScaledStepToS(&this->unk_1CA[1], -0x3FFC, 0x4FA);
        Math_ScaledStepToS(&this->unk_1CA[2], -0x238C, 0x444);
    } else if (this->timer == 26) {
        Math_ScaledStepToS(&this->actor.shape.rot.x, 0x1800, 0x93E);

        if (Math_ScaledStepToS(this->unk_1CA, -0x1555, 0x71C)) {
            this->timer = 27;
        }

        Math_ScaledStepToS(&this->unk_1CA[1], -0x38E3, 0x2D8);
        Math_ScaledStepToS(&this->unk_1CA[2], -0x5C71, 0x5B0);
    } else if (this->timer >= 27) {
        this->timer++;

        if (this->timer > 30) {
            if (this->actor.xzDistToPlayer < 80.0f * this->unk_230) {
                func_809E5F9C(this);
            } else {
                func_809E5F44(this);
            }
        }
    } else {
        this->timer++;

        if (this->timer == 10) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_DEKU_SCRAPE);
        }

        if (this->timer >= 12) {
            Math_ScaledStepToS(&this->unk_1CA[2], -0x5C71, 0x88);
        }
    }
    func_809E6DCC(this);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E77E4.s")
void func_809E77E4(EnDekubaba* this, GlobalContext* globalCtx) {
    s32 anyStepsDone;

    SkelAnime_Update(&this->skelAnime);

    if (this->timer > 8) {
        anyStepsDone = Math_SmoothStepToS(&this->actor.shape.rot.x, 0x1800, 1, 0x11C6, 0x71C);
        anyStepsDone |= Math_SmoothStepToS(this->unk_1CA, -0x1555, 1, 0xAAA, 0x71C);
        anyStepsDone |= Math_SmoothStepToS(&this->unk_1CA[1], -0x38E3, 1, 0xE38, 0x71C);
        anyStepsDone |= Math_SmoothStepToS(&this->unk_1CA[2], -0x5C71, 1, 0x11C6, 0x71C);

        if (!anyStepsDone) {
            this->timer = 8;
        }
    } else {
        if (this->timer != 0) {
            this->timer--;
        }

        if (this->timer == 0) {
            func_809E5F44(this);
        }
    }
    func_809E6DCC(this);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E78DC.s")
void func_809E78DC(EnDekubaba* this, GlobalContext* globalCtx) {
    s32 allStepsDone;

    SkelAnime_Update(&this->skelAnime);

    allStepsDone = Math_ScaledStepToS(&this->actor.shape.rot.x, -0x4000, 0xE38) & 1;
    allStepsDone &= Math_ScaledStepToS(&this->unk_1CA[0], -0x4000, 0xE38);
    allStepsDone &= Math_ScaledStepToS(&this->unk_1CA[1], -0x4000, 0xE38);
    allStepsDone &= Math_ScaledStepToS(&this->unk_1CA[2], -0x4000, 0xE38);

    if (allStepsDone) {
        if (this->actor.colChkInfo.health == 0) {
            func_809E61E0(this);
        } else {
            this->collider.base.acFlags |= 1;
            if (this->timer == 0) {
                if (this->actor.xzDistToPlayer < 80.0f * this->unk_230) {
                    func_809E5F9C(this);
                } else {
                    func_809E6078(this);
                }
            } else {
                func_809E6264(this);
            }
        }
    }
    func_809E6DCC(this);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E79EC.s")
void func_809E79EC(EnDekubaba* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->skelAnime);

    if (this->timer != 0) {
        this->timer--;
    }

    if (this->timer == 0) {
        func_809E5A38(this);

        if (this->actor.xzDistToPlayer < (80.0f * this->unk_230)) {
            func_809E5F9C(this);
        } else {
            func_809E6078(this);
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E7A88.s")
void func_809E7A88(EnDekubaba* this, GlobalContext* globalCtx) {
    s16 temp_v0;
    s32 phi_v1;

    SkelAnime_Update(&this->skelAnime);
    Math_ScaledStepToS(&this->actor.shape.rot.x, this->unk_1CA[0], 0x71C);
    Math_ScaledStepToS(&this->unk_1CA[0], this->unk_1CA[1], 0x71C);
    Math_ScaledStepToS(&this->unk_1CA[1], this->unk_1CA[2], 0x71C);

    if (Math_ScaledStepToS(&this->unk_1CA[2], this->unk_1C8, 0x71C)) {
        this->unk_1C8 = -16384.0f - (this->unk_1C8 + 0x4000) * 0.8f;
    }
    temp_v0 = this->unk_1C8 + 0x4000;
    phi_v1 = ABS(temp_v0);

    if (phi_v1 < 0x100) {
        this->collider.base.acFlags |= 1;
        if (this->actor.xzDistToPlayer < 80.0f * this->unk_230) {
            func_809E5F9C(this);
        } else {
            func_809E6078(this);
        }
    }
    func_809E6DCC(this);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E7BB0.s")
void func_809E7BB0(EnDekubaba* this, GlobalContext* globalCtx) {
    s32 i;
    Vec3f sp78;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 sp68;

    Math_StepToF(&this->actor.speedXZ, 0.0f, this->unk_230 * 0.1f);

    if (this->timer == 0) {
        Math_ScaledStepToS(&this->actor.shape.rot.x, 0x4800, 0x71C);
        Math_ScaledStepToS(&this->unk_1CA[0], 0x4800, 0x71C);
        Math_ScaledStepToS(&this->unk_1CA[1], 0x4800, 0x71C);

        EffectSsHahen_SpawnBurst(globalCtx, &this->actor.world.pos, this->unk_230 * 3.0f, 0, this->unk_230 * 12.0f,
                                 this->unk_230 * 5.0f, 1, -1, 10, NULL);

        if ((this->actor.scale.x > 0.005f) && ((this->actor.bgCheckFlags & 2) || (this->actor.bgCheckFlags & 8))) {
            this->actor.scale.z = 0.0f;
            this->actor.scale.y = 0.0f;
            this->actor.scale.x = 0.0f;
            this->actor.speedXZ = 0.0f;
            this->actor.flags &= -6;
            EffectSsHahen_SpawnBurst(globalCtx, &this->actor.world.pos, this->unk_230 * 3.0f, 0, this->unk_230 * 12.0f,
                                     this->unk_230 * 5.0f, 15, -1, 10, NULL);
        }

        if (this->actor.bgCheckFlags & 2) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_DODO_M_GND);
            this->timer = 1;
        }
    } else if (this->timer == 1) {
        sp78 = this->actor.world.pos;

        temp_f24 = Math_SinS(this->actor.shape.rot.x) * 20.0f;
        sp68 = Math_CosS(this->actor.shape.rot.x);
        temp_f20 = Math_SinS(this->actor.shape.rot.y) * (-20.0f * sp68);
        sp68 = Math_CosS(this->actor.shape.rot.x);
        temp_f22 = (-20.0f * sp68) * Math_CosS(this->actor.shape.rot.y);

        for (i = 0; i < 4; i++) {
            func_800286CC(globalCtx, &sp78, &D_809E8EA0, &D_809E8EA0, 500, 50);
            sp78.x += temp_f20;
            sp78.y += temp_f24;
            sp78.z += temp_f22;
        }

        func_800286CC(globalCtx, &this->actor.home.pos, &D_809E8EA0, &D_809E8EA0, this->unk_230 * 500.0f,
                      this->unk_230 * 100.0f);
        func_809E645C(this, globalCtx);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E7F14.s")
void func_809E7F14(EnDekubaba* this, GlobalContext* globalCtx) {
    Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y, this->unk_230 * 5.0f);

    if (Math_StepToF(&this->actor.scale.x, this->unk_230 * 0.1f * 0.01f, this->unk_230 * 0.1f * 0.01f)) {
        func_800286CC(globalCtx, &this->actor.home.pos, &D_809E8EA0, &D_809E8EA0, this->unk_230 * 500.0f,
                      this->unk_230 * 100.0f);
        if (this->actor.dropFlag == 0) {
            Item_DropCollectible(globalCtx, &this->actor.world.pos, ITEM00_NUTS);

            if (this->actor.params == 1) {
                Item_DropCollectible(globalCtx, &this->actor.world.pos, ITEM00_NUTS);
                Item_DropCollectible(globalCtx, &this->actor.world.pos, ITEM00_NUTS);
            }
        } else {
            Item_DropCollectibleRandom(globalCtx, &this->actor, &this->actor.world.pos, 0x30);
        }
        Actor_Kill(&this->actor);
    }

    this->actor.scale.y = this->actor.scale.z = this->actor.scale.x;
    this->actor.shape.rot.z += 0x1C70;
    EffectSsHahen_SpawnBurst(globalCtx, &this->actor.home.pos, this->unk_230 * 3.0f, 0, this->unk_230 * 12.0f,
                             this->unk_230 * 5.0f, 1, -1, 10, NULL);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E80D8.s")
void func_809E80D8(EnDekubaba* this, GlobalContext* globalCtx) {
    if (this->timer != 0) {
        this->timer--;
    }

    if (Actor_HasParent(&this->actor, globalCtx) || (this->timer == 0)) {
        Actor_Kill(&this->actor);
        return;
    }

    func_8002F554(&this->actor, globalCtx, 7);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E8140.s")
void func_809E8140(EnDekubaba* this, GlobalContext* globalCtx) {
    Vec3f* sp38;
    f32 sp34;
    s32 phi_s0;

    if ((this->collider.base.acFlags & 2)) {
        this->collider.base.acFlags &= ~2;
        func_8003573C(&this->actor, &this->collider, 1);

        if ((this->collider.base.colType != COLTYPE_HARD) &&
            ((this->actor.colChkInfo.damageEffect != 0) || (this->actor.colChkInfo.damage != 0))) {

            phi_s0 = this->actor.colChkInfo.health - this->actor.colChkInfo.damage;

            if (this->actionFunc != func_809E79EC) {
                if ((this->actor.colChkInfo.damageEffect == 0xE) || (this->actor.colChkInfo.damageEffect == 1)) {
                    if (this->actor.colChkInfo.damageEffect == 0xE) {
                        phi_s0 = this->actor.colChkInfo.health;
                    }

                    func_809E60A8(this, 2);
                } else if (this->actionFunc == func_809E7458) {
                    if (phi_s0 <= 0) {
                        phi_s0 = 1;
                    }

                    func_809E60A8(this, 1);
                } else {
                    func_809E60A8(this, 0);
                }
            } else if ((this->actor.colChkInfo.damageEffect == 0xE) || (this->actor.colChkInfo.damageEffect == 0xF)) {
                if (phi_s0 > 0) {
                    func_809E63EC(this);
                } else {
                    func_809E6170(this);
                }
            } else if (this->actor.colChkInfo.damageEffect != 1) {
                func_809E60A8(this, 0);
            } else {
                return;
            }

            if (phi_s0 < 0) {
                this->actor.colChkInfo.health = 0;
            } else {
                this->actor.colChkInfo.health = phi_s0;
            }

            if (this->actor.colChkInfo.damageEffect == 2) {
                sp38 = &this->actor.world.pos;
                sp34 = (this->unk_230 * 70.0f);

                for (phi_s0 = 0; phi_s0 < 4; phi_s0++) {
                    EffectSsEnFire_SpawnVec3f(globalCtx, &this->actor, sp38, sp34, 0, 0, phi_s0);
                }
            }
        } else {
            return;
        }
    } else if ((globalCtx->actorCtx.unk_02 != 0) && (this->collider.base.colType != COLTYPE_HARD) &&
               (this->actionFunc != func_809E79EC) && (this->actionFunc != func_809E78DC) &&
               (this->actor.colChkInfo.health != 0)) {
        this->actor.colChkInfo.health--;
        this->actor.dropFlag = 0;
        func_809E60A8(this, 1);
    } else {
        return;
    }

    if (this->actor.colChkInfo.health != 0) {
        if (this->timer == 2) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
        } else {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_DEKU_DAMAGE);
        }
    } else {
        func_80032C7C(globalCtx, &this->actor);
        if (this->actor.params == 1) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_DEKU_DEAD);
        } else {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_DEKU_JR_DEAD);
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/EnDekubaba_Update.s")
void EnDekubaba_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDekubaba* this = THIS;

    if (this->collider.base.atFlags & 2) {
        this->collider.base.atFlags &= ~2;
        func_809E6078(this);
    }

    func_809E8140(this, globalCtx);
    this->actionFunc(this, globalCtx);

    if (this->actionFunc == func_809E7BB0) {
        Actor_MoveForward(&this->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 10.0f, this->unk_230 * 15.0f, 10.0f, 5);
    } else if (this->actionFunc != func_809E80D8) {
        Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 0.0f, 0.0f, 0.0f, 4);
        if (this->unk_234 == NULL) {
            this->unk_234 = this->actor.floorPoly;
        }
    }
    if (this->actionFunc == func_809E7104) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
        this->actor.flags |= 0x1000000;
    }

    if (this->collider.base.acFlags & 1) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
    }

    if (this->actionFunc != func_809E80D8) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E858C.s")
void func_809E858C(EnDekubaba* this, GlobalContext* globalCtx) {
    f32 horizontalScale;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2445);

    horizontalScale = this->unk_230 * 0.01f;

    Matrix_Translate(this->actor.home.pos.x, this->actor.home.pos.y + (-6.0f * this->unk_230), this->actor.home.pos.z,
                     MTXMODE_NEW);
    Matrix_RotateRPY(this->unk_1CA[0], this->actor.shape.rot.y, 0, MTXMODE_APPLY);
    Matrix_Scale(horizontalScale, horizontalScale, horizontalScale, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2461),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, D_06001330);

    Actor_SetFocus(&this->actor, 0.0f);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2468);
}

static Gfx* D_809E902C[] = { D_06001330, D_06001628, D_06001828 };

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E86B8.s")
void func_809E86B8(EnDekubaba* this, GlobalContext* globalCtx) {
    MtxF mtx;
    s32 i;
    f32 horizontalStepSize;
    f32 spA4;
    f32 scale;
    s32 stalkSections;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2487);

    if (this->actionFunc == func_809E7BB0) {
        stalkSections = 2;
    } else {
        stalkSections = 3;
    }

    scale = this->unk_230 * 0.01f;
    Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Matrix_Get(&mtx);
    if (this->actor.colorFilterTimer != 0) {
        spA4 = this->unk_230 * 20.0f;
        this->unk_164.x = this->actor.world.pos.x;
        this->unk_164.y = this->actor.world.pos.y - spA4;
        this->unk_164.z = this->actor.world.pos.z;
    }

    for (i = 0; i < stalkSections; i++) {
        mtx.wy += 20.0f * Math_SinS(this->unk_1CA[i]) * this->unk_230;
        horizontalStepSize = 20.0f * Math_CosS(this->unk_1CA[i]) * this->unk_230;
        mtx.wx -= horizontalStepSize * Math_SinS(this->actor.shape.rot.y);
        mtx.wz -= horizontalStepSize * Math_CosS(this->actor.shape.rot.y);

        Matrix_Put(&mtx);
        Matrix_RotateRPY(this->unk_1CA[i], this->actor.shape.rot.y, 0, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2533),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(POLY_OPA_DISP++, D_809E902C[i]);

        Collider_UpdateSpheres(0x33 + 2 * i, &this->collider);
        Collider_UpdateSpheres(0x34 + 2 * i, &this->collider);

        if (i == 0) {
            if (this->actionFunc != func_809E7A88) {
                this->actor.focus.pos.x = mtx.wx;
                this->actor.focus.pos.y = mtx.wy;
                this->actor.focus.pos.z = mtx.wz;
            } else {
                this->actor.focus.pos.x = this->actor.home.pos.x;
                this->actor.focus.pos.y = this->actor.home.pos.y + (40.0f * this->unk_230);
                this->actor.focus.pos.z = this->actor.home.pos.z;
            }
        }

        if ((i < 2) && (this->actor.colorFilterTimer != 0)) {
            this->unk_14C[i].x = mtx.wx;
            this->unk_14C[i].y = mtx.wy - spA4;
            this->unk_14C[i].z = mtx.wz;
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2569);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E89E4.s")
void func_809E89E4(EnDekubaba* this, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2579);

    Matrix_RotateRPY(this->unk_1CA[2], this->actor.shape.rot.y, 0, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2586),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, D_06001828);

    Collider_UpdateSpheres(0x37, &this->collider);
    Collider_UpdateSpheres(0x38, &this->collider);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2596);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E8AD8.s")
void func_809E8AD8(EnDekubaba* this, GlobalContext* globalCtx) {
    MtxF mtx;
    f32 horizontalScale;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2692);
    func_80094044(globalCtx->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0x00, 0x00, 0x00, 0xFF);

    func_80038A28(this->unk_234, this->actor.home.pos.x, this->actor.home.pos.y, this->actor.home.pos.z, &mtx);
    Matrix_Mult(&mtx, MTXMODE_NEW);

    horizontalScale = this->unk_230 * 0.15f;
    Matrix_Scale(horizontalScale, 1.0f, horizontalScale, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2710),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gCircleShadowDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2715);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/func_809E8C0C.s")
void func_809E8C0C(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnDekubaba* this = THIS;

    if (limbIndex == 1) {
        Collider_UpdateSpheres(limbIndex, &this->collider);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dekubaba/EnDekubaba_Draw.s")
void EnDekubaba_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDekubaba* this = THIS;
    f32 scale;

    if (1) {}
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2752);
    func_80093D18(globalCtx->state.gfxCtx);

    if (this->actionFunc != func_809E80D8) {
        SkelAnime_DrawOpa(globalCtx, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, func_809E8C0C, this);

        if (this->actionFunc == func_809E64F4) {
            func_809E858C(this, globalCtx);
        } else {
            func_809E86B8(this, globalCtx);
        }

        scale = this->unk_230 * 0.01f;
        Matrix_Translate(this->actor.home.pos.x, this->actor.home.pos.y, this->actor.home.pos.z, MTXMODE_NEW);
        Matrix_RotateY(this->actor.home.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2780),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, D_060010F0);

        if (this->actionFunc == func_809E7BB0) {
            func_809E89E4(this, globalCtx);
        }

        if (this->unk_234 != 0) {
            func_809E8AD8(this, globalCtx);
        }
    } else if ((this->timer > 40) || (this->timer & 1)) {
        Matrix_Translate(0.0f, 0.0f, 200.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2797),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, D_06003070);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2804);
}
