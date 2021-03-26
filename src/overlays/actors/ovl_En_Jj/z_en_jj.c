/*
 * File: z_en_jj.c
 * Overlay: ovl_En_Jj
 * Description: Lord Jabu-Jabu
 */

#include "z_en_jj.h"
#include "objects/object_jj/object_jj.h"
#include "overlays/actors/ovl_Eff_Dust/z_eff_dust.h"

#define FLAGS 0x00000030

#define THIS ((EnJj*)thisx)

typedef enum {
    /* 0 */ JABUJABU_EYE_OPEN,
    /* 1 */ JABUJABU_EYE_HALF,
    /* 2 */ JABUJABU_EYE_CLOSED,
    /* 3 */ JABUJABU_EYE_MAX
} EnJjEyeState;

void EnJj_Init(Actor* thisx, GlobalContext* globalCtx);
void EnJj_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnJj_Update(Actor* thisx, GlobalContext* globalCtx);
void EnJj_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnJj_UpdateStaticCollision(Actor* thisx, GlobalContext* globalCtx);
void func_80A87BEC(EnJj* this, GlobalContext* globalCtx);
void func_80A87C30(EnJj* this, GlobalContext* globalCtx);
void func_80A87CEC(EnJj* this, GlobalContext* globalCtx);
void func_80A87EF0(EnJj* this, GlobalContext* globalCtx);

const ActorInit En_Jj_InitVars = {
    ACTOR_EN_JJ,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_JJ,
    sizeof(EnJj),
    (ActorFunc)EnJj_Init,
    (ActorFunc)EnJj_Destroy,
    (ActorFunc)EnJj_Update,
    (ActorFunc)EnJj_Draw,
};

s32 unused = 0;

#include "z_en_jj_cutscene_data.c" EARLY

s32 usused2[] = { 0, 0 };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000004, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 170, 150, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 87, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 3300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1100, ICHAIN_STOP),
};

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func _80A87800.s")
void EnJj_SetupAction(EnJj* this, EnJjActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/EnJj_Init.s")
void EnJj_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnJj* this = THIS;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    ActorShape_Init(&this->dyna.actor.shape, 0.0f, NULL, 0.0f);

    switch (this->dyna.actor.params) {
        case JABUJABU_MAIN:
            SkelAnime_InitFlex(globalCtx, &this->skelAnime, &gJabuJabuSkel, &gJabuJabuAnim, this->limbDrawTable,
                               this->transitionDrawTable, 22);
            Animation_PlayLoop(&this->skelAnime, &gJabuJabuAnim);
            this->unk_30A = 0;
            this->eyeIndex = 0;
            this->blinkTimer = 0;
            this->extraBlinkCounter = 0;
            this->extraBlinkTotal = 0;
            if (gSaveContext.eventChkInf[3] & 0x400) { // Fish given
                EnJj_SetupAction(this, func_80A87BEC);
            } else {
                EnJj_SetupAction(this, func_80A87C30);
            }
            this->bodyCollisionActor = (DynaPolyActor*)Actor_SpawnAsChild(
                &globalCtx->actorCtx, &this->dyna.actor, globalCtx, ACTOR_EN_JJ, this->dyna.actor.world.pos.x - 10.0f,
                this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z, 0, this->dyna.actor.world.rot.y, 0, JABUJABU_COLLISION);
            DynaPolyActor_Init(&this->dyna, 0);
            CollisionHeader_GetVirtual(&gJabuJabuHeadCol, &colHeader);
            this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, colHeader);
            Collider_InitCylinder(globalCtx, &this->collider);
            Collider_SetCylinder(globalCtx, &this->collider, &this->dyna.actor, &sCylinderInit);
            this->dyna.actor.colChkInfo.mass = MASS_IMMOVABLE;
            break;

        case JABUJABU_COLLISION:
            DynaPolyActor_Init(&this->dyna, 0);
            CollisionHeader_GetVirtual(&gJabuJabuBodyCol, &colHeader);
            this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, colHeader);
            func_8003ECA8(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
            this->dyna.actor.update = EnJj_UpdateStaticCollision;
            this->dyna.actor.draw = NULL;
            Actor_SetScale(&this->dyna.actor, 0.087f);
            break;

        case JABUJABU_UNUSED_COLLISION:
            DynaPolyActor_Init(&this->dyna, 0);
            CollisionHeader_GetVirtual(&gJabuJabuUnusedCol, &colHeader);
            this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, colHeader);
            this->dyna.actor.update = EnJj_UpdateStaticCollision;
            this->dyna.actor.draw = NULL;
            Actor_SetScale(&this->dyna.actor, 0.087f);
            break;
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/EnJj_Destroy.s")
void EnJj_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnJj* this = THIS;

    switch (this->dyna.actor.params) {
        case JABUJABU_MAIN:
            DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
            Collider_DestroyCylinder(globalCtx, &this->collider);
            break;
            
        case JABUJABU_COLLISION:
        case JABUJABU_UNUSED_COLLISION:
            DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
            break;
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func _80A87B1C.s")
/**
 * Blink routine. Blinks at the end of each randomised blinkTimer cycle. If extraBlinkCounter is not zero, blink that many more times before resuming random blinkTimer cycles. 
 * extraBlinkTotal can be set to a positive number to blink that many extra times at the end of every blinkTimer cycle, but the actor always sets it to zero, so only one multiblink happens when extraBlinkCounter is nonzero.
 */
void EnJj_Blink(EnJj* this) {
    if (this->blinkTimer > 0) {
        this->blinkTimer--;
    } else {
        this->eyeIndex++;
        if (this->eyeIndex >= 3) {
            this->eyeIndex = 0;
            if (this->extraBlinkCounter > 0) {
                this->extraBlinkCounter--;
            } else {
                this->blinkTimer = Rand_S16Offset(20, 20);
                this->extraBlinkCounter = this->extraBlinkTotal;
            }
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func _80A87B9C.s")
void func_80A87B9C(EnJj* this, GlobalContext* globalCtx) {
    DynaPolyActor* child = this->bodyCollisionActor;

    if (this->unk_308 >= -5200) {
        this->unk_308 -= 102;
        if (this->unk_308 < -2600) {
            func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, child->bgId);
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func _80A87BEC.s")
void func_80A87BEC(EnJj* this, GlobalContext* globalCtx) {
    if (this->dyna.actor.xzDistToPlayer < 300.0f) {
        EnJj_SetupAction(this, func_80A87B9C);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func _80A87C30.s")
void func_80A87C30(EnJj* this, GlobalContext* globalCtx) {
    static Vec3f D_80A88CF0 = { -1589.0f, 53.0f, -43.0f };
    Player* player = PLAYER;

    if ((Math_Vec3f_DistXZ(&D_80A88CF0, &player->actor.world.pos) < 300.0f) &&
        globalCtx->isPlayerDroppingFish(globalCtx)) {
        this->unk_30C = 100;
        EnJj_SetupAction(this, func_80A87CEC);
    }
    this->collider.dim.pos.x = -1245;
    this->collider.dim.pos.y = 20;
    this->collider.dim.pos.z = -48;
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func _80A87CEC.s")
void func_80A87CEC(EnJj* this, GlobalContext* globalCtx) {
    DynaPolyActor* child = this->bodyCollisionActor;

    if (this->unk_30C > 0) {
        this->unk_30C--;
    } else {
        EnJj_SetupAction(this, func_80A87EF0);
        globalCtx->csCtx.segment = &D_80A88164;
        gSaveContext.cutsceneTrigger = 1;
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, child->bgId);
        func_8005B1A4(ACTIVE_CAM);
        gSaveContext.eventChkInf[3] |= 0x400;
        func_80078884(NA_SE_SY_CORRECT_CHIME);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func _80A87D94.s")
void func_80A87D94(EnJj* this, GlobalContext* globalCtx) {
    switch (globalCtx->csCtx.npcActions[2]->action) {
        case 1:
            if (this->unk_30A & 2) {
                this->eyeIndex = 0;
                this->blinkTimer = Rand_S16Offset(20, 20);
                this->extraBlinkCounter = 0;
                this->extraBlinkTotal = 0;
                this->unk_30A ^= 2;
            }
            break;
        case 2:
            this->unk_30A |= 1;
            if (!(this->unk_30A & 8)) {
                this->dust = Actor_SpawnAsChild(&globalCtx->actorCtx, &this->dyna.actor, globalCtx, ACTOR_EFF_DUST,
                                                   -1100.0f, 105.0f, -27.0f, 0, 0, 0, EFF_DUST_TYPE_0);
                this->unk_30A |= 8;
            }
            break;
        case 3:
            if (!(this->unk_30A & 2)) {
                this->eyeIndex = 0;
                this->blinkTimer = 0;
                this->extraBlinkCounter = 1;
                this->extraBlinkTotal = 0;
                this->unk_30A |= 2;
            }
            break;
    }
    if (this->unk_30A & 1) {
        Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_JABJAB_BREATHE - SFX_FLAG);
        if (this->unk_308 >= -5200) {
            this->unk_308 -= 102;
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func _80A87EF0.s")
void func_80A87EF0(EnJj* this, GlobalContext* globalCtx) {
    Actor* dust;

    if (!(this->unk_30A & 4)) {
        this->unk_30A |= 4;
        dust = this->dust;
        if (dust != NULL) {
            Actor_Kill(dust);
            this->dyna.actor.child = NULL;
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func _80A87F44.s")
void EnJj_UpdateStaticCollision(Actor* thisx, GlobalContext* globalCtx) {
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/EnJj_Update.s")
void EnJj_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnJj* this = THIS;

    if ((globalCtx->csCtx.state != 0) && (globalCtx->csCtx.npcActions[2] != NULL)) {
        func_80A87D94(this, globalCtx);
    } else {
        this->actionFunc(this, globalCtx);
        if (this->skelAnime.curFrame == 41.0f) {
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_JABJAB_GROAN);
        }
    }
    EnJj_Blink(this);
    SkelAnime_Update(&this->skelAnime);
    Actor_SetScale(&this->dyna.actor, 0.087f);
    this->skelAnime.jointTable[10].z = this->unk_308;
}

static u64* sEyeTextures[] = {
    gJabuJabuEyeOpenTex,
    gJabuJabuEyeHalfTex,
    gJabuJabuEyeClosedTex,
};

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/EnJj_Draw.s")
void EnJj_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnJj* this = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_jj.c", 879);

    func_800943C8(globalCtx->state.gfxCtx);
    Matrix_Translate(0.0f, (cosf(this->skelAnime.curFrame * (M_PI / 41.0f)) * 10.0f) - 10.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(10.0f, 10.0f, 10.0f, MTXMODE_APPLY);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[this->eyeIndex]));
    SkelAnime_DrawFlexOpa(globalCtx, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          0, 0, this);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_jj.c", 898);
}
