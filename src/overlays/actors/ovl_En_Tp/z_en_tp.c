/*
 * File: z_en_tp.c
 * Overlay: ovl_En_Tp
 * Description: Electric Tailpasaran
 */

#include "z_en_tp.h"
#include "objects/object_tp/object_tp.h"

#define FLAGS 0x00000000

#define THIS ((EnTp*)thisx)

void EnTp_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTp_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTp_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTp_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnTp_Tail_SetupFollowHead(EnTp* this);
void EnTp_Tail_FollowHead(EnTp* this, GlobalContext* globalCtx);
void EnTp_Head_SetupApproachPlayer(EnTp* this);
void EnTp_Head_ApproachPlayer(EnTp* this, GlobalContext* globalCtx);
void EnTp_SetupDie(EnTp* this);
void EnTp_Die(EnTp* this, GlobalContext* globalCtx);
void EnTp_Fragment_SetupFade(EnTp* this);
void EnTp_Fragment_Fade(EnTp* this, GlobalContext* globalCtx);
void EnTp_Head_SetupTakeOff(EnTp* this);
void EnTp_Head_TakeOff(EnTp* this, GlobalContext* globalCtx);
void EnTp_Head_SetupWait(EnTp* this);
void EnTp_Head_Wait(EnTp* this, GlobalContext* globalCtx);
void EnTp_Head_SetupBurrowReturnHome(EnTp* this);
void EnTp_Head_BurrowReturnHome(EnTp* this, GlobalContext* globalCtx);

typedef enum {
    /* 0 */ TAILPASARAN_ACTION_FRAGMENT_FADE,
    /* 1 */ TAILPASARAN_ACTION_DIE,
    /* 2 */ TAILPASARAN_ACTION_TAIL_FOLLOWHEAD,
    /* 4 */ TAILPASARAN_ACTION_HEAD_WAIT = 4,
    /* 7 */ TAILPASARAN_ACTION_HEAD_APPROACHPLAYER = 7,
    /* 8 */ TAILPASARAN_ACTION_HEAD_TAKEOFF,
    /* 9 */ TAILPASARAN_ACTION_HEAD_BURROWRETURNHOME
} TailpasaranAction;

const ActorInit En_Tp_InitVars = {
    ACTOR_EN_TP,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_TP,
    sizeof(EnTp),
    (ActorFunc)EnTp_Init,
    (ActorFunc)EnTp_Destroy,
    (ActorFunc)EnTp_Update,
    (ActorFunc)EnTp_Draw,
};

static ColliderJntSphElementInit D_80B22A90[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x03, 0x08 },
            { 0xFFCFFFFF, 0x01, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 0, { { 0, 0, 0 }, 4 }, 100 },
    },
};

static ColliderJntSphInit D_80B22AB4 = {
    {
        COLTYPE_HIT1,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    D_80B22A90,
};

typedef enum {
    /* 00 */ TAILPASARAN_DMGEFF_NONE, // No effect
    /* 01 */ TAILPASARAN_DMGEFF_DEKUNUT,
    /* 14 */ TAILPASARAN_DMGEFF_SHOCKING = 14, // Kills the Tailpasaran but shocks Player
    /* 15 */ TAILPASARAN_DMGEFF_INSULATING // Kills the Tailpasaran and does not shock Player
} TailpasaranDamageEffect;

static DamageTable D_80B22AC4 = {
    /* Deku nut      */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_DEKUNUT),
    /* Deku stick    */ DMG_ENTRY(2, TAILPASARAN_DMGEFF_INSULATING),
    /* Slingshot     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Explosive     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Boomerang     */ DMG_ENTRY(1, TAILPASARAN_DMGEFF_INSULATING),
    /* Normal arrow  */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Hammer swing  */ DMG_ENTRY(2, TAILPASARAN_DMGEFF_SHOCKING),
    /* Hookshot      */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Kokiri sword  */ DMG_ENTRY(1, TAILPASARAN_DMGEFF_SHOCKING),
    /* Master sword  */ DMG_ENTRY(2, TAILPASARAN_DMGEFF_SHOCKING),
    /* Giant's Knife */ DMG_ENTRY(4, TAILPASARAN_DMGEFF_SHOCKING),
    /* Fire arrow    */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Ice arrow     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Light arrow   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Unk arrow 1   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Ice magic     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Light magic   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Shield        */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, TAILPASARAN_DMGEFF_SHOCKING),
    /* Giant spin    */ DMG_ENTRY(4, TAILPASARAN_DMGEFF_SHOCKING),
    /* Master spin   */ DMG_ENTRY(2, TAILPASARAN_DMGEFF_SHOCKING),
    /* Kokiri jump   */ DMG_ENTRY(2, TAILPASARAN_DMGEFF_SHOCKING),
    /* Giant jump    */ DMG_ENTRY(8, TAILPASARAN_DMGEFF_SHOCKING),
    /* Master jump   */ DMG_ENTRY(4, TAILPASARAN_DMGEFF_SHOCKING),
    /* Unknown 1     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, TAILPASARAN_DMGEFF_SHOCKING),
    /* Unknown 2     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
};

// sInitChain
static InitChainEntry D_80B22AE4[] = {
    ICHAIN_F32(targetArrowOffset, 10, ICHAIN_STOP),
};

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B20DE0.s")
void EnTp_SetupAction(EnTp* this, EnTpActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/EnTp_Init.s")
#ifdef NON_MATCHING
// Regalloc, some stack, and compiler refuses to put zero in temp_s4; all seems to be related
void EnTp_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnTp* this = THIS;

    EnTp* now;
    EnTp* next;
    s32 i;
    // s64 new_var;
    s64 temp_s4;

    Actor_ProcessInitChain(&this->actor, D_80B22AE4);
    this->actor.targetMode = 3;
    this->actor.colChkInfo.damageTable = &D_80B22AC4;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.14f);
    this->unk_150 = 0;
    this->actor.colChkInfo.health = 1;
    now = this;
    this->alpha = 255;
    Collider_InitJntSph(globalCtx, &this->collider);
    Collider_SetJntSph(globalCtx, &this->collider, &this->actor, &D_80B22AB4, this->colliderItems);

    if (this->actor.params <= TAILPASARAN_HEAD) {
        this->actor.naviEnemyId = 6;
        this->timer = 0;
        this->collider.base.acFlags |= AC_HARD;
        this->collider.elements->dim.modelSphere.radius = this->collider.elements->dim.worldSphere.radius = 8;
        EnTp_Head_SetupWait(this);
        this->actor.focus.pos = this->actor.world.pos;
        this->actor.flags |= 0x15;
        Actor_SetScale(&this->actor, 1.5f);

        for (i = 0; i <= 6; i++) {
            temp_s4 = 0;
            // new_var = 0;
            next = (EnTp*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_TP, this->actor.world.pos.x,
                                      this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, temp_s4);

            if (next != NULL) {
                now->actor.child = &next->actor;
                next->actor.parent = &now->actor;
                next->kiraSpawnTimer = i + 1;
                next->head = this;
                Actor_SetScale(&next->actor, 0.3f);

                if (i == 2) {
                    next->actor.flags |= 0x15;
                    next->unk_150 = 1; // Why?
                }

                next->timer = next->unk_15C = i * -5;
                next->horizontalVariation = 6.0f - (i * 0.75f);
                now = next;
            }
        }
    } else if (this->actor.params == TAILPASARAN_TAIL) {
        EnTp_Tail_SetupFollowHead(this);
    } else {
        EnTp_Fragment_SetupFade(this);
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/EnTp_Init.s")
#endif

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/EnTp_Destroy.s")
void EnTp_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnTp* this = THIS;

    Collider_DestroyJntSph(globalCtx, &this->collider);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B21084.s")
void EnTp_Tail_SetupFollowHead(EnTp* this) {
    this->actionIndex = TAILPASARAN_ACTION_TAIL_FOLLOWHEAD;
    EnTp_SetupAction(this, EnTp_Tail_FollowHead);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B210B0.s")
void EnTp_Tail_FollowHead(EnTp* this, GlobalContext* globalCtx) {
    s16 sp36;
    s16 phase;

    if (this->actor.params == TAILPASARAN_TAIL_DYING) {
        this->actionIndex = TAILPASARAN_ACTION_DIE;

        if (this->actor.parent == NULL) {
            EnTp_SetupDie(this);
        }
    } else {
        if (this->unk_150 != 0) {
            this->actor.flags |= 1;
        }

        if (this->head->unk_150 != 0) {
            this->actor.speedXZ = this->red = this->actor.velocity.y = this->heightPhase = 0.0f;
            if (this->actor.world.pos.y < this->head->actor.home.pos.y) {
                this->actor.flags &= ~1;
            }

            this->actor.world.pos = this->actor.parent->prevPos;
        } else {
            Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.parent->world.pos.y - 4.0f, 1.0f, 1.0f, 0.0f);
            sp36 = this->head->actor.shape.rot.y + 0x4000;
            phase = 2000 * (this->head->unk_15C + this->timer);
            this->actor.world.pos.x = this->actor.home.pos.x + Math_SinS(phase) * (Math_SinS(sp36) * this->horizontalVariation);
            this->actor.world.pos.z = this->actor.home.pos.z + Math_SinS(phase) * (Math_CosS(sp36) * this->horizontalVariation);
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B2128C.s")
void EnTp_Head_SetupApproachPlayer(EnTp* this) {
    this->actionIndex = TAILPASARAN_ACTION_HEAD_APPROACHPLAYER;
    this->timer = 200;
    EnTp_SetupAction(this, EnTp_Head_ApproachPlayer);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B212C0.s")
void EnTp_Head_ApproachPlayer(EnTp* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;

    Math_SmoothStepToF(&this->actor.world.pos.y, player->actor.world.pos.y + 30.0f, 1.0f, 0.5f, 0.0f);
    Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                           &D_801333E8);

    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~AT_HIT;
        if (&player->actor == this->collider.base.at) {
            this->timer = 1;
        }
    }

    if (this->red < 255) {
        this->red += 15;
    }

    if (Math_CosF(this->heightPhase) == 0.0f) {
        this->extraHeightVariation = 2.0f * Rand_ZeroOne();
    }

    this->actor.world.pos.y += Math_CosF(this->heightPhase) * (2.0f + this->extraHeightVariation);
    this->heightPhase += 0.2f;
    Math_SmoothStepToF(&this->actor.speedXZ, 2.5f, 0.1f, 0.2f, 0.0f);
    this->timer--;

    if (this->timer != 0) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 750, 0);
        this->actor.shape.rot.y = this->actor.world.rot.y;
    } else {
        EnTp_Head_SetupBurrowReturnHome(this);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B21454.s")
void EnTp_SetupDie(EnTp* this) {
    Actor* now;

    this->timer = 2;
    if (this->actor.params <= TAILPASARAN_HEAD) {
        for (now = this->actor.child; now != NULL; now = now->child) {
            now->params = TAILPASARAN_TAIL_DYING;
            now->colChkInfo.health = 0;
        }

        this->timer = 13;
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_TAIL_DEAD);
    }
    this->actionIndex = TAILPASARAN_ACTION_DIE;
    EnTp_SetupAction(this, EnTp_Die);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B214CC.s")
void EnTp_Die(EnTp* this, GlobalContext* globalCtx) {
    EnTp* now;
    s16 phi_s1;
    s32 pad;
    Vec3f effectVelAccel = { 0.0f, 0.5f, 0.0f };
    Vec3f effectPos = { 0.0f, 0.0f, 0.0f };

    this->timer--;

    if (this->timer <= 0) {
        if (this->actor.params == TAILPASARAN_HEAD_DYING) {
            effectPos.x = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.x;
            effectPos.z = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.z;
            effectPos.y = ((Rand_ZeroOne() - 0.5f) * 5.0f) + this->actor.world.pos.y;
            EffectSsDeadDb_Spawn(globalCtx, &effectPos, &effectVelAccel, &effectVelAccel, 100, 0, 255, 255, 255, 255, 0,
                                 0, 255, 1, 9, 1);

            effectPos.x = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.x;
            effectPos.z = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.z;
            effectPos.y = ((Rand_ZeroOne() - 0.5f) * 5.0f) + this->actor.world.pos.y;
            EffectSsDeadDb_Spawn(globalCtx, &effectPos, &effectVelAccel, &effectVelAccel, 100, 0, 255, 255, 255, 255, 0,
                                 0, 255, 1, 9, 1);
            Item_DropCollectibleRandom(globalCtx, &this->actor, &this->actor.world.pos, 0x50);
        } else {
            for (phi_s1 = 0; phi_s1 < 1; phi_s1++) {
                now =
                    (EnTp*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_TP, this->actor.world.pos.x,
                                       this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, TAILPASARAN_FRAGMENT);

                if (now != NULL) {
                    Actor_SetScale(&now->actor, this->actor.scale.z * 0.5f);
                    now->red = this->red;
                }
            }
        }

        if (this->actor.child != NULL) {
            this->actor.child->parent = NULL;
            this->actor.child->params = TAILPASARAN_TAIL_DYING;
            this->actor.child->colChkInfo.health = 0;
        }

        this->unk_150 = 2;
        Actor_Kill(&this->actor);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B217FC.s")
void EnTp_Fragment_SetupFade(EnTp* this) {
    this->actionIndex = TAILPASARAN_ACTION_FRAGMENT_FADE;
    this->actor.world.pos.x += ((Rand_ZeroOne() - 0.5f) * 5.0f);
    this->actor.world.pos.y += ((Rand_ZeroOne() - 0.5f) * 5.0f);
    this->actor.world.pos.z += ((Rand_ZeroOne() - 0.5f) * 5.0f);
    this->actor.velocity.x = (Rand_ZeroOne() - 0.5f) * 1.5f;
    this->actor.velocity.y = (Rand_ZeroOne() - 0.5f) * 1.5f;
    this->actor.velocity.z = (Rand_ZeroOne() - 0.5f) * 1.5f;
    this->actor.flags &= ~1;
    EnTp_SetupAction(this, EnTp_Fragment_Fade);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B21900.s")
void EnTp_Fragment_Fade(EnTp* this, GlobalContext* globalCtx) {
    func_8002D7EC(&this->actor);
    this->alpha -= 20;

    if (this->alpha < 20) {
        this->alpha = 0;
        Actor_Kill(&this->actor);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B2194C.s")
void EnTp_Head_SetupTakeOff(EnTp* this) {
    this->timer = (s32)((Rand_ZeroOne() * 15.0f) + 40.0f);
    this->actionIndex = TAILPASARAN_ACTION_HEAD_TAKEOFF;
    EnTp_SetupAction(this, EnTp_Head_TakeOff);
}

/**
 * Flies up and loops around until it makes for Player
 */
// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B219A8.s")
void EnTp_Head_TakeOff(EnTp* this, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = PLAYER;

    Math_SmoothStepToF(&this->actor.speedXZ, 2.5f, 0.1f, 0.2f, 0.0f);
    Math_SmoothStepToF(&this->actor.world.pos.y, player->actor.world.pos.y + 85.0f + this->horizontalVariation, 1.0f,
                       this->actor.speedXZ * 0.25f, 0.0f);
    Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                           &D_801333E8);

    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~AT_HIT;
        if (&player->actor == this->collider.base.at) {
            this->unk_15C = 1;
        }
    }

    if (this->red != 0) {
        this->red -= 15;
    }

    if (Math_CosF(this->heightPhase) == 0.0f) {
        this->extraHeightVariation = Rand_ZeroOne() * 4.0f;
    }

    this->actor.world.pos.y += (Math_CosF(this->heightPhase) * ((this->actor.speedXZ * 0.25f) + this->extraHeightVariation));
    this->actor.world.rot.y += this->unk_164;
    this->heightPhase += 0.2f;

    if (this->timer != 0) {
        this->timer--;
    }

    Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos), 1, 750,
                       0);

    if (this->timer == 0) {
        EnTp_Head_SetupApproachPlayer(this);
    }

    this->actor.shape.rot.y = this->actor.world.rot.y;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B21B90.s")
void EnTp_Head_SetupWait(EnTp* this) {
    this->actionIndex = TAILPASARAN_ACTION_HEAD_WAIT;
    this->unk_150 = 0;
    this->actor.shape.rot.x = -0x4000;
    this->timer = 60;
    this->unk_15C = 0;
    this->actor.speedXZ = 0.0f;
    EnTp_SetupAction(this, EnTp_Head_Wait);
}

/**
 * Awaken and rise from the ground when Player is closer than 200
 */
// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B21BDC.s")
void EnTp_Head_Wait(EnTp* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;
    s16 yaw;

    this->unk_15C--;

    if (this->actor.xzDistToPlayer < 200.0f) {
        if (this->collider.base.atFlags & AT_HIT) {
            this->collider.base.atFlags &= ~AT_HIT;
            if (&player->actor == this->collider.base.at) {
                this->timer = 0;
            }
        }

        if (this->timer != 0) {
            this->timer--;

            Math_SmoothStepToS(&this->actor.shape.rot.x, 0, 1, 500, 0);
            Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 1500, 0);

            yaw = Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos) + 0x4000;
            Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.home.pos.y + 30.0f, 0.3f, 1.0f, 0.3f);
            this->actor.world.pos.x =
                this->actor.home.pos.x + (Math_SinS(2000 * this->unk_15C) * (Math_SinS(yaw) * this->horizontalVariation));
            this->actor.world.pos.z =
                this->actor.home.pos.z + (Math_SinS(2000 * this->unk_15C) * (Math_CosS(yaw) * this->horizontalVariation));
        } else {
            this->actor.shape.rot.x = 0;
            this->unk_150 = 1;
            EnTp_Head_SetupTakeOff(this);
        }
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.x, -0x4000, 1, 500, 0);

        if (Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.home.pos.y, 0.3f, 1.5f, 0.3f) == 0.0f) {
            this->timer = 60;
        } else {
            yaw = Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos);
            this->actor.world.pos.x =
                this->actor.home.pos.x + (Math_SinS(2000 * this->unk_15C) * (Math_SinS(yaw) * 6.0f));
            this->actor.world.pos.z =
                this->actor.home.pos.z + (Math_SinS(2000 * this->unk_15C) * (Math_CosS(yaw) * 6.0f));
        }
    }

    this->actor.shape.rot.y = this->actor.world.rot.y;

    if (this->actor.world.pos.y != this->actor.home.pos.y) {
        Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B21EE8.s")
void EnTp_Head_SetupBurrowReturnHome(EnTp* this) {
    this->actionIndex = TAILPASARAN_ACTION_HEAD_BURROWRETURNHOME;
    this->timer = 0;
    EnTp_SetupAction(this, EnTp_Head_BurrowReturnHome);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B21F18.s")
void EnTp_Head_BurrowReturnHome(EnTp* this, GlobalContext* globalCtx) {
    static Vec3f bubbleAccel = { 0.0f, -0.5f, 0.0f };
    static Color_RGBA8 bubblePrimColor = { 255, 255, 255, 255 };
    static Color_RGBA8 bubbleEnvColor = { 150, 150, 150, 0 };
    Vec3f bubbleVelocity;
    Vec3f bubblePos;
    s32 closeToFloor;
    EnTp* now;
    s16 temp_v0;

    closeToFloor = false;
    temp_v0 = this->timer;
    this->unk_15C--;

    if ((temp_v0 != 0) || ((this->actor.home.pos.y - this->actor.world.pos.y) > 60.0f)) {
        this->timer = temp_v0 - 1; // Required to match
        temp_v0 = this->timer;

        if (temp_v0 == 0) {
            EnTp_Head_SetupWait(this);

            for (now = (EnTp*)this->actor.child; now != NULL; now = (EnTp*)now->actor.child) {
                now->unk_15C = now->timer;
            }
        } else {
            if (this->actor.shape.rot.x != -0x4000) {
                this->timer = 80;
                this->actor.velocity.y = 0.0f;
                this->actor.speedXZ = 0.0f;
                this->actor.world.pos = this->actor.home.pos;
                this->actor.shape.rot.x = -0x4000;

                for (now = (EnTp*)this->actor.child; now != NULL; now = (EnTp*)now->actor.child) {
                    now->actor.velocity.y = 0.0f;
                    now->actor.speedXZ = 0.0f;
                    now->actor.world.pos = this->actor.home.pos;
                    now->actor.world.pos.y = this->actor.home.pos.y - 80.0f;
                }
            }

            this->actor.world.pos.y = this->actor.home.pos.y - this->timer;
        }
    } else {
        if (this->actor.shape.rot.x != 0x4000) {
            this->actor.shape.rot.x -= 0x400;
        }

        if (this->red != 0) {
            this->red -= 15;
        }

        this->actor.speedXZ = 2.0f * Math_CosS(this->actor.shape.rot.x);
        this->actor.velocity.y = Math_SinS(this->actor.shape.rot.x) * -2.0f;

        if ((this->actor.world.pos.y - this->actor.floorHeight) < 20.0f) {
            closeToFloor = true;
        }

        if (this->actor.world.pos.y != this->actor.home.pos.y) {
            Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }

        if (closeToFloor && ((globalCtx->gameplayFrames & 1) != 0)) {
            bubblePos = this->actor.world.pos;
            bubblePos.y = this->actor.floorHeight;

            bubbleVelocity.x = Rand_CenteredFloat(5.0f);
            bubbleVelocity.y = (Rand_ZeroOne() * 3.5f) + 1.5f;
            bubbleVelocity.z = Rand_CenteredFloat(5.0f);

            EffectSsDtBubble_SpawnCustomColor(globalCtx, &bubblePos, &bubbleVelocity, &bubbleAccel, &bubblePrimColor,
                                              &bubbleEnvColor, Rand_S16Offset(100, 50), 20, 0);
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/func _80B221E8.s")
void EnTp_UpdateDamage(EnTp* this, GlobalContext* globalCtx) {
    s32 phi_s2;
    s32 phi_s4;
    EnTp* temp_s0; // Can eliminate this and just use phi_s0, but they're used differently
    EnTp* phi_s0;

    if ((this->collider.base.acFlags & AC_HIT) && (this->actionIndex >= TAILPASARAN_ACTION_TAIL_FOLLOWHEAD)) {
        phi_s4 = phi_s2 = 0;

        if (this->actor.params <= TAILPASARAN_HEAD) {
            phi_s2 = 1;
        }

        this->collider.base.acFlags &= ~AC_HIT;
        func_8003573C(&this->actor, &this->collider, 1);
        this->damageEffect = this->actor.colChkInfo.damageEffect;

        if (this->actor.colChkInfo.damageEffect != TAILPASARAN_DMGEFF_NONE) {
            if (this->actor.colChkInfo.damageEffect == TAILPASARAN_DMGEFF_DEKUNUT) {
                phi_s4 = 1;
            }

            // Head is invincible
            if (phi_s2 == 0) {
                Actor_ApplyDamage(&this->actor);
            }

            if (this->actor.colChkInfo.health == 0) {
                this->actor.flags &= ~1;
                temp_s0 = this->head;

                if (temp_s0->actor.params <= TAILPASARAN_HEAD) {
                    EnTp_SetupDie(temp_s0);
                    temp_s0->damageEffect = this->actor.colChkInfo.damageEffect;
                    temp_s0->actor.params = TAILPASARAN_HEAD_DYING;
                }
            } else {
                if (phi_s4 != 0) {
                    this->actor.freezeTimer = 80;
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
                    if (phi_s2 != 0) {
                        Actor_SetColorFilter(&this->actor, 0, 0xFF, 0, 0x50);
                    } else {
                        Actor_SetColorFilter(&this->actor, 0, 0xFF, 0x2000, 0x50);
                    }
                }

                for (phi_s0 = (EnTp*)this->actor.parent; phi_s0 != NULL; phi_s0 = (EnTp*)phi_s0->actor.parent) {
                    phi_s0->collider.base.acFlags &= ~AC_HIT;

                    if (phi_s4 != 0) {
                        phi_s0->actor.freezeTimer = 80;
                        Audio_PlayActorSound2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);

                        if (phi_s2 != 0) {
                            Actor_SetColorFilter(&phi_s0->actor, 0, 0xFF, 0, 0x50);
                        } else {
                            Actor_SetColorFilter(&phi_s0->actor, 0, 0xFF, 0x2000, 0x50);
                        }
                    }
                }

                for (phi_s0 = (EnTp*)this->actor.child; phi_s0 != NULL; phi_s0 = (EnTp*)phi_s0->actor.child) {
                    phi_s0->collider.base.acFlags &= ~AC_HIT;
                    if (phi_s4 != 0) {
                        phi_s0->actor.freezeTimer = 80;

                        if (phi_s2 != 0) {
                            Actor_SetColorFilter(&phi_s0->actor, 0, 0xFF, 0, 0x50);
                        } else {
                            Actor_SetColorFilter(&phi_s0->actor, 0, 0xFF, 0x2000, 0x50);
                        }
                    }
                }
            }
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/EnTp_Update.s")
void EnTp_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnTp* this = THIS;
    Vec3f kiraVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f kiraAccel = { 0.0f, -0.6f, 0.0f };
    Vec3f kiraPos;
    Color_RGBA8 kiraPrimColor = { 0, 0, 255, 255 };
    Color_RGBA8 kiraEnvColor = { 0, 0, 0, 0 };
    Player* player = PLAYER;
    s16 yawToWall;

    if (player->stateFlags1 & 0x4000000) { // Shielding
        this->damageEffect = TAILPASARAN_DMGEFF_NONE;
    }

    if (this->actor.colChkInfo.health != 0) {
        EnTp_UpdateDamage(this, globalCtx);
    }

    this->actionFunc(this, globalCtx);

    if (this->actor.params <= TAILPASARAN_HEAD) {
        Actor_MoveForward(&this->actor);

        if (this->actionIndex != TAILPASARAN_ACTION_HEAD_BURROWRETURNHOME) {
            Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 0.0f, 15.0f, 10.0f, 5);
        }

        // Turn away from wall
        if ((this->actor.speedXZ != 0.0f) && (this->actor.bgCheckFlags & 8)) {
            yawToWall = this->actor.wallYaw - this->actor.world.rot.y;

            if (ABS(yawToWall) > 0x4000) {
                if (yawToWall >= 0) {
                    this->actor.world.rot.y -= 500;
                } else {
                    this->actor.world.rot.y += 500;
                }

                this->actor.shape.rot.y = this->actor.world.rot.y;
            }
        }

        this->actor.shape.rot.z += 0x800;

        if (this->actor.shape.rot.z == 0) {
            Audio_PlaySoundGeneral(NA_SE_EN_TAIL_CRY, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }

        if (this->actionIndex >= TAILPASARAN_ACTION_TAIL_FOLLOWHEAD) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
        }
    }

    if (this->actor.params != TAILPASARAN_TAIL_DYING) {
        this->kiraSpawnTimer--;
        this->kiraSpawnTimer &= 7;
    }

    this->actor.focus.pos = this->actor.world.pos;

    if (this->damageEffect == TAILPASARAN_DMGEFF_SHOCKING) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
    }

    if ((this->kiraSpawnTimer & 7) == 0) {
        kiraPrimColor.r = this->red;
        kiraAccel.x = -this->actor.velocity.x * 0.25f;
        kiraAccel.y = -this->actor.velocity.y * 0.25f;
        kiraAccel.z = -this->actor.velocity.z * 0.25f;
        kiraPos.x = ((Rand_ZeroOne() - 0.5f) * 25.0f) + this->actor.world.pos.x;
        kiraPos.y = ((Rand_ZeroOne() - 0.5f) * 20.0f) + this->actor.world.pos.y;
        kiraPos.z = ((Rand_ZeroOne() - 0.5f) * 25.0f) + this->actor.world.pos.z;
        EffectSsKiraKira_SpawnSmall(globalCtx, &kiraPos, &kiraVelocity, &kiraAccel, &kiraPrimColor, &kiraEnvColor);
    }

    if ((this->actionIndex >= TAILPASARAN_ACTION_TAIL_FOLLOWHEAD) && (this->actor.colChkInfo.health != 0)) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Tp/EnTp_Draw.s")
void EnTp_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnTp* this = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_tp.c", 1451);

    if (this->unk_150 != 2) {
        if ((thisx->params <= TAILPASARAN_HEAD) || (thisx->params == TAILPASARAN_HEAD_DYING)) {
            func_80093D18(globalCtx->state.gfxCtx);

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_tp.c", 1459),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gTailpasaranHeadDL);

            Matrix_Translate(0.0f, 0.0f, 8.0f, MTXMODE_APPLY);
        } else {
            func_80093D84(globalCtx->state.gfxCtx);
            func_800D1FD4(&globalCtx->mf_11DA0);

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, this->red, 0, 255, this->alpha);
            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetCombineLERP(POLY_XLU_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT,
                              TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT,
                              TEXEL0, ENVIRONMENT);
            gDPPipeSync(POLY_XLU_DISP++);
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(gTailpasaranTailSegmentTex));
            gDPPipeSync(POLY_XLU_DISP++);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_tp.c", 1480),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gTailpasaranTailSegmentDL);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_tp.c", 1495);

    if ((thisx->params <= TAILPASARAN_TAIL) || (thisx->params == TAILPASARAN_TAIL_DYING)) {
        Collider_UpdateSpheres(0, &this->collider);
    }
}
