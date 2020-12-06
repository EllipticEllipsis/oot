#include "z_en_bom_bowl_pit.h"
#include "vt.h"
#include "overlays/actors/ovl_En_Bom_Chu/z_en_bom_chu.h"

#define FLAGS 0x00000010

#define THIS ((EnBomBowlPit*)thisx)

void EnBomBowlPit_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlPit_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlPit_Update(Actor* thisx, GlobalContext* globalCtx);

void EnBomBowlPit_SetupDetectHit(EnBomBowlPit* this, GlobalContext* globalCtx);
void EnBomBowlPit_DetectHit(EnBomBowlPit* this, GlobalContext* globalCtx);
void EnBomBowlPit_CameraDollyIn(EnBomBowlPit* this, GlobalContext* globalCtx);
void EnBomBowlPit_SpawnPrize(EnBomBowlPit* this, GlobalContext* globalCtx);
void EnBomBowlPit_SetupGivePrize(EnBomBowlPit* this, GlobalContext* globalCtx);
void EnBomBowlPit_GivePrize(EnBomBowlPit* this, GlobalContext* globalCtx);
void EnBomBowlPit_WaitTillPrizeGiven(EnBomBowlPit* this, GlobalContext* globalCtx);
void EnBomBowlPit_Reset(EnBomBowlPit* this, GlobalContext* globalCtx);

static s32 sGetItemIds[] = { GI_BOMB_BAG_30, GI_HEART_PIECE, GI_BOMBCHUS_10, GI_BOMBS_1, GI_RUPEE_PURPLE };

const ActorInit En_Bom_Bowl_Pit_InitVars = {
    ACTOR_EN_BOM_BOWL_PIT,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnBomBowlPit),
    (ActorFunc)EnBomBowlPit_Init,
    (ActorFunc)EnBomBowlPit_Destroy,
    (ActorFunc)EnBomBowlPit_Update,
    NULL,
};

void EnBomBowlPit_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBomBowlPit* this = THIS;

    this->actionFunc = EnBomBowlPit_SetupDetectHit;
}

void EnBomBowlPit_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnBomBowlPit_SetupDetectHit(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (this->start != 0) {
        this->start = this->status = 0;
        this->actionFunc = EnBomBowlPit_DetectHit;
    }
}

void EnBomBowlPit_DetectHit(EnBomBowlPit* this, GlobalContext* globalCtx) {
    EnBomChu* chu;
    Vec3f chuPosDiff;

    if (globalCtx->cameraPtrs[0]->setting == 0x15) {
        chu = (EnBomChu*)globalCtx->actorCtx.actorList[ACTORTYPE_EXPLOSIVES].first;

        while (chu != NULL) {
            if ((&chu->actor == &this->actor) || (chu->actor.id != ACTOR_EN_BOM_CHU)) {
                chu = (EnBomChu*)chu->actor.next;
                continue;
            }

            chuPosDiff.x = chu->actor.posRot.pos.x - this->actor.posRot.pos.x;
            chuPosDiff.y = chu->actor.posRot.pos.y - this->actor.posRot.pos.y;
            chuPosDiff.z = chu->actor.posRot.pos.z - this->actor.posRot.pos.z;

            if (((fabsf(chuPosDiff.x) < 40.0f) || (BREG(2))) && ((fabsf(chuPosDiff.y) < 40.0f) || (BREG(2))) &&
                ((fabsf(chuPosDiff.z) < 40.0f) || (BREG(2)))) {
                func_8002DF54(globalCtx, NULL, 8);
                chu->unk_150 = 1;

                this->camId = Gameplay_CreateSubCamera(globalCtx);
                Gameplay_ChangeCameraStatus(globalCtx, 0, 1);
                Gameplay_ChangeCameraStatus(globalCtx, this->camId, 7);

                this->unk_1C8.x = this->unk_1C8.y = this->unk_1C8.z = 0.1f;
                this->unk_1A4.x = this->unk_1A4.y = this->unk_1A4.z = 0.1f;

                this->unk_180.x = this->unk_168.x = globalCtx->view.lookAt.x;
                this->unk_180.y = this->unk_168.y = globalCtx->view.lookAt.y;
                this->unk_180.z = this->unk_168.z = globalCtx->view.lookAt.z;

                this->unk_18C.x = this->unk_174.x = globalCtx->view.eye.x;
                this->unk_18C.y = this->unk_174.y = globalCtx->view.eye.y;
                this->unk_18C.z = this->unk_174.z = globalCtx->view.eye.z;

                this->unk_1BC.x = 20.0f;
                this->unk_1BC.y = 100.0f;
                this->unk_1BC.z = -800.0f;

                this->unk_198.x = 20.0f;
                this->unk_198.y = 50.0f;
                this->unk_198.z = -485.0f;

                this->unk_1B0.x = fabsf(this->unk_18C.x - this->unk_198.x) * 0.02f;
                this->unk_1B0.y = fabsf(this->unk_18C.y - this->unk_198.y) * 0.02f;
                this->unk_1B0.z = fabsf(this->unk_18C.z - this->unk_198.z) * 0.02f;

                this->unk_1D4.x = fabsf(this->unk_180.x - this->unk_1BC.x) * 0.02f;
                this->unk_1D4.y = fabsf(this->unk_180.y - this->unk_1BC.y) * 0.02f;
                this->unk_1D4.z = fabsf(this->unk_180.z - this->unk_1BC.z) * 0.02f;

                func_800C04D8(globalCtx, this->camId, &this->unk_180, &this->unk_18C);
                this->actor.textId = 0xF;
                func_8010B680(globalCtx, this->actor.textId, NULL);
                this->unk_154 = 5;
                func_80078884(NA_SE_EV_HIT_SOUND);
                func_8002DF54(globalCtx, NULL, 8);
                this->status = 1;
                this->actionFunc = EnBomBowlPit_CameraDollyIn;
                break;
            } else {
                chu = (EnBomChu*)chu->actor.next;
            }
        }
    }
}

void EnBomBowlPit_CameraDollyIn(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (this->camId != 0) {
        Math_SmoothScaleMaxF(&this->unk_180.x, this->unk_1BC.x, this->unk_1C8.x, this->unk_1D4.x);
        Math_SmoothScaleMaxF(&this->unk_180.y, this->unk_1BC.y, this->unk_1C8.y, this->unk_1D4.y);
        Math_SmoothScaleMaxF(&this->unk_180.z, this->unk_1BC.z, this->unk_1C8.z, this->unk_1D4.z);
        Math_SmoothScaleMaxF(&this->unk_18C.x, this->unk_198.x, this->unk_1A4.x, this->unk_1B0.x);
        Math_SmoothScaleMaxF(&this->unk_18C.y, this->unk_198.y, this->unk_1A4.y, this->unk_1B0.y);
        Math_SmoothScaleMaxF(&this->unk_18C.z, this->unk_198.z, this->unk_1A4.z, this->unk_1B0.z);
    }

    func_800C04D8(globalCtx, this->camId, &this->unk_180, &this->unk_18C);

    if ((this->unk_154 == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
    }

    if ((fabsf(this->unk_18C.x - this->unk_198.x) < 5.0f) && (fabsf(this->unk_18C.y - this->unk_198.y) < 5.0f) &&
        (fabsf(this->unk_18C.z - this->unk_198.z) < 5.0f) && (fabsf(this->unk_180.x - this->unk_1BC.x) < 5.0f) &&
        (fabsf(this->unk_180.y - this->unk_1BC.y) < 5.0f) && (fabsf(this->unk_180.z - this->unk_1BC.z) < 5.0f)) {
        func_80106CCC(globalCtx);
        this->timer = 30;
        this->actionFunc = EnBomBowlPit_SpawnPrize;
    }
}

void EnBomBowlPit_SpawnPrize(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (this->timer == 0) {
        this->exItem = (EnExItem*)Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_EX_ITEM,
                                                     this->actor.posRot.pos.x, this->actor.posRot.pos.y,
                                                     this->actor.posRot.pos.z - 70.0f, 0, 0, 0, this->prizeIndex);
        if (this->exItem != NULL) {
            this->actionFunc = EnBomBowlPit_SetupGivePrize;
        }
    }
}

void EnBomBowlPit_SetupGivePrize(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (this->unk_156 != 0) {
        switch (this->prizeIndex) {
            case 0:
                gSaveContext.itemGetInf[1] |= 2;
                break;
            case 1:
                gSaveContext.itemGetInf[1] |= 4;
        }

        Gameplay_ClearCamera(globalCtx, this->camId);
        Gameplay_ChangeCameraStatus(globalCtx, 0, 7);
        func_8002DF54(globalCtx, NULL, 8);
        this->actionFunc = EnBomBowlPit_GivePrize;
    }
}

void EnBomBowlPit_GivePrize(EnBomBowlPit* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;

    func_8002DF54(globalCtx, NULL, 7);
    this->getItemId = sGetItemIds[this->prizeIndex];

    if ((this->getItemId == GI_BOMB_BAG_30) && (CUR_CAPACITY(UPG_BOMB_BAG) == 30)) {
        this->getItemId = GI_BOMB_BAG_40;
    }

    player->stateFlags1 &= ~0x20000000;
    this->actor.parent = NULL;
    func_8002F434(&this->actor, globalCtx, this->getItemId, 2000.0f, 1000.0f);
    player->stateFlags1 |= 0x20000000;
    this->actionFunc = EnBomBowlPit_WaitTillPrizeGiven;
}

void EnBomBowlPit_WaitTillPrizeGiven(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (Actor_HasParent(&this->actor, globalCtx)) {
        this->actionFunc = EnBomBowlPit_Reset;
    } else {
        func_8002F434(&this->actor, globalCtx, this->getItemId, 2000.0f, 1000.0f);
    }
}

void EnBomBowlPit_Reset(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
        // Normal termination/completion
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 正常終了 ☆☆☆☆☆ \n" VT_RST);
        if (this->getItemId == GI_HEART_PIECE) {
            gSaveContext.healthAccumulator = 0x140;
            // Ah recovery! (?)
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ あぁ回復！ ☆☆☆☆☆ \n" VT_RST);
        }
        this->unk_156 = 0;
        this->status = 2;
        this->actionFunc = EnBomBowlPit_SetupDetectHit;
    }
}

void EnBomBowlPit_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBomBowlPit* this = THIS;

    this->actionFunc(this, globalCtx);

    if (this->timer != 0) {
        this->timer--;
    }
}
