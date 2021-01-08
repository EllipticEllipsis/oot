/*
 * File: z_en_jj.c
 * Overlay: ovl_En_Jj
 * Description: Lord Jabu-Jabu
 */

#include "z_en_jj.h"

#define FLAGS 0x00000030

#define THIS ((EnJj*)thisx)

void EnJj_Init(Actor* thisx, GlobalContext* globalCtx);
void EnJj_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnJj_Update(Actor* thisx, GlobalContext* globalCtx);
void EnJj_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A87F44(Actor* thisx, GlobalContext* globalCtx);
void func_80A87BEC(EnJj* this, GlobalContext* globalCtx);
void func_80A87C30(EnJj* this, GlobalContext* globalCtx);
void func_80A87CEC(EnJj* this, GlobalContext* globalCtx);
void func_80A87EF0(EnJj* this, GlobalContext* globalCtx);


const ActorInit En_Jj_InitVars = {
    ACTOR_EN_JJ,
    ACTORTYPE_ITEMACTION,
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

static ColliderCylinderInit sCylinderInit =
{
    { COLTYPE_UNK10, 0x00, 0x09, 0x39, 0x10, COLSHAPE_CYLINDER },
    { 0x00, { 0x00000000, 0x00, 0x00 }, { 0x00000004, 0x00, 0x00 }, 0x00, 0x01, 0x01 },
    { 170, 150, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 87, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 3300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1100, ICHAIN_STOP),
};

static Vec3f D_80A88CF0 = { -1589.0f, 53.0f, -43.0f };

static u64* D_80A88CFC[] = { 0x06007698, 0x06007A98, 0x06007E98, };

extern UNK_TYPE D_06000A1C;
extern UNK_TYPE D_06001830;
extern AnimationHeader D_06001F4C;
extern FlexSkeletonHeader D_0600B9A8;
extern UNK_TYPE D_0600BA8C;

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func_80A87800.s")
void func_80A87800(EnJj* this, EnJjActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/EnJj_Init.s")
void EnJj_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnJj* this = THIS;

    s32 sp4C = 0;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    ActorShape_Init(&this->dyna.actor.shape, 0.0f, NULL, 0.0f);

    switch (this->dyna.actor.params) {
        case -1:
            SkelAnime_InitFlex(globalCtx, &this->skelAnime, &D_0600B9A8, &D_06001F4C, this->limbDrawTable,
                               this->transitionDrawTable, 22);
            Animation_PlayLoop(&this->skelAnime, &D_06001F4C);
            this->unk_30A = 0;
            this->unk_30E = 0;
            this->unk_30F = 0;
            this->unk_310 = 0;
            this->unk_311 = 0;
            if ((gSaveContext.eventChkInf[3] & 0x400) != 0) {
                func_80A87800(this, func_80A87BEC);
            } else {
                func_80A87800(this, func_80A87C30);
            }
            this->childActor = (DynaPolyActor*)Actor_SpawnAsChild(
                &globalCtx->actorCtx, &this->dyna.actor, globalCtx, ACTOR_EN_JJ, this->dyna.actor.posRot.pos.x - 10.0f,
                this->dyna.actor.posRot.pos.y, this->dyna.actor.posRot.pos.z, 0, this->dyna.actor.posRot.rot.y, 0, 0);
            DynaPolyInfo_SetActorMove(&this->dyna, 0);
            DynaPolyInfo_Alloc(&D_06000A1C, &sp4C);
            this->dyna.dynaPolyId =
                DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp4C);
            Collider_InitCylinder(globalCtx, &this->collider);
            Collider_SetCylinder(globalCtx, &this->collider, &this->dyna.actor, &sCylinderInit);
            this->dyna.actor.colChkInfo.mass = 0xFF;
            break;
        case 0:
            DynaPolyInfo_SetActorMove(&this->dyna, 0);
            DynaPolyInfo_Alloc(&D_06001830, &sp4C);
            this->dyna.dynaPolyId =
                DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp4C);
            func_8003ECA8(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
            this->dyna.actor.update = func_80A87F44;
            this->dyna.actor.draw = NULL;
            Actor_SetScale(&this->dyna.actor, 0.087f);
            break;
        case 1:
            DynaPolyInfo_SetActorMove(&this->dyna, 0);
            DynaPolyInfo_Alloc(&D_0600BA8C, &sp4C);
            this->dyna.dynaPolyId =
                DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp4C);
            this->dyna.actor.update = func_80A87F44;
            this->dyna.actor.draw = NULL;
            Actor_SetScale(&this->dyna.actor, 0.087f);
            break;
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/EnJj_Destroy.s")
void EnJj_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnJj* this = THIS;

    switch (this->dyna.actor.params) {
        case -1:
            DynaPolyInfo_Free(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
            Collider_DestroyCylinder(globalCtx, &this->collider);
            break;
        case 0:
        case 1:
            DynaPolyInfo_Free(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
            break;
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func_80A87B1C.s")
void func_80A87B1C(EnJj* this) {
    s8 new_var;

    if (this->unk_30F > 0) {
        this->unk_30F--;
    } else {
        this->unk_30E++;
        if (this->unk_30E >= 3) {
            this->unk_30E = 0;
            if (this->unk_310 > 0) {
                this->unk_310--;
            } else {
                this->unk_30F = Rand_S16Offset(20, 20);
                this->unk_310 = this->unk_311;
            }
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func_80A87B9C.s")
void func_80A87B9C(EnJj* this, GlobalContext* globalCtx) {
    DynaPolyActor* child = this->childActor;

    if (this->unk_308 >= -5200) {
        this->unk_308 -= 102;
        if (this->unk_308 < -2600) {
            func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, child->dynaPolyId);
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func_80A87BEC.s")
void func_80A87BEC(EnJj* this, GlobalContext* globalCtx) {
    if (this->dyna.actor.xzDistToLink < 300.0f) {
        func_80A87800(this, func_80A87B9C);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func_80A87C30.s")
void func_80A87C30(EnJj* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;

    if ((Math_Vec3f_DistXZ(&D_80A88CF0, &player->actor.posRot.pos) < 300.0f) &&
        (globalCtx->isPlayerDroppingFish(globalCtx) != 0)) {
        this->unk_30C = 100;
        func_80A87800(this, func_80A87CEC);
    }
    this->collider.dim.pos.x = -1245;
    this->collider.dim.pos.y = 20;
    this->collider.dim.pos.z = -48;
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func_80A87CEC.s")
void func_80A87CEC(EnJj* this, GlobalContext* globalCtx) {
    DynaPolyActor* child = this->childActor;
    if (this->unk_30C > 0) {
        this->unk_30C--;
    } else {
        func_80A87800(this, func_80A87EF0);
        globalCtx->csCtx.segment = &D_80A88164;
        gSaveContext.cutsceneTrigger = 1;
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, child->dynaPolyId);
        func_8005B1A4(ACTIVE_CAM);
        gSaveContext.eventChkInf[3] |= 0x400;
        func_80078884(NA_SE_SY_CORRECT_CHIME);
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func_80A87D94.s")
void func_80A87D94(EnJj* this, GlobalContext* globalCtx) {
    switch (globalCtx->csCtx.npcActions[2]->action) {
        case 1:
            if ((this->unk_30A & 2) != 0) {
                this->unk_30E = 0;
                this->unk_30F = Rand_S16Offset(20, 20);
                this->unk_310 = 0;
                this->unk_311 = 0;
                this->unk_30A ^= 2;
            }
            break;
        case 2:
            this->unk_30A |= 1;
            if ((this->unk_30A & 8) == 0) {
                this->unk_304 = Actor_SpawnAsChild(&globalCtx->actorCtx, &this->dyna.actor, globalCtx, ACTOR_EFF_DUST,
                                                   -1100.0f, 105.0f, -27.0f, 0, 0, 0, 0);
                this->unk_30A |= 8;
            }
            break;
        case 3:
            if ((this->unk_30A & 2) == 0) {
                this->unk_30E = 0;
                this->unk_30F = 0;
                this->unk_310 = 1;
                this->unk_311 = 0;
                this->unk_30A |= 2;
            }
            break;
    }
    if ((this->unk_30A & 1) != 0) {
        Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_JABJAB_BREATHE - SFX_FLAG);
        if (this->unk_308 >= -5200) {
            this->unk_308 -= 102;
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func_80A87EF0.s")
void func_80A87EF0(EnJj* this, GlobalContext* globalCtx) {
    Actor* temp_a0;

    if (!(this->unk_30A & 4)) {
        this->unk_30A |= 4;
        temp_a0 = this->unk_304;
        if (temp_a0 != NULL) {
            Actor_Kill(temp_a0);
            this->dyna.actor.child = NULL;
        }
    }
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/func_80A87F44.s")
void func_80A87F44(Actor* thisx, GlobalContext* globalCtx) {
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
    func_80A87B1C(this);
    SkelAnime_Update(&this->skelAnime);
    Actor_SetScale(&this->dyna.actor, 0.087f);
    this->skelAnime.jointTable[10].z = this->unk_308;
}

// #pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Jj/EnJj_Draw.s")
void EnJj_Draw(Actor *thisx, GlobalContext *globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnJj *this = THIS;
    // s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_jj.c", 879);
    func_800943C8(globalCtx->state.gfxCtx);
    Matrix_Translate(0.0f, (cosf(this->skelAnime.curFrame * (M_PI/41.0f)) * 10.0f) - 10.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(10.0f, 10.0f, 10.0f, MTXMODE_APPLY);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80A88CFC[this->unk_30E]));
    SkelAnime_DrawFlexOpa(globalCtx, this->skelAnime.skeleton, this->skelAnime.jointTable,
     this->skelAnime.dListCount, 0, 0, this);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_jj.c", 898);
}

