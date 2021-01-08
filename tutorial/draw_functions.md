# Draw functions

Up: [Contents](contents.md)
Previous: [The rest of the functions in the actor](other_functions.md)

Draw functions behave completely differently from the other functions in an actor. They often use a lot of macros.

For this tutorial we will first look at the `EnJj` draw function, `EnJj_Draw`, then some more complicated examples.

## A first example

Unless it is completely invisible, an actor usually has a draw function as one of the main four actor functions. Hence its prototype looks like

```C
EnJj_Draw(Actor* thisx, GlobalContext* globalCtx);
```

As in Init, Destroy and Update, it is much more convenient to feed mips2c the fake prototype

```C
EnJj_Draw(EnJj* this, GlobalContext* globalCtx);
```

so that it fills out the struct fields from the actuar actor; we then put a

```C
EnJj* this = THIS;
```

in the declarations as before. From now on, the process is rather different from the decompilation process used for the other functions. Here is the output of mips2c after sorting out the actor struct from Init, and with the arguments set back to `Actor* thisx`:
```C
void EnJj_Draw(Actor *thisx, GlobalContext *globalCtx) {
	EnJj* this = THIS;
	
    GraphicsContext *sp4C;
    Gfx *sp3C;
    EnJj *sp18;
    Gfx *temp_v1;
    GraphicsContext *temp_a1;
    s32 temp_a0;

    temp_a1 = globalCtx->state.gfxCtx;
    sp4C = temp_a1;
    Graph_OpenDisps(&sp3C, temp_a1, (const char *) "../z_en_jj.c", 0x36F);
    func_800943C8(globalCtx->state.gfxCtx);
    Matrix_Translate(0.0f, (cosf(this->skelAnime.curFrame * 0.076624215f) * 10.0f) - 10.0f, 0.0f, (u8)1U);
    Matrix_Scale(10.0f, 10.0f, 10.0f, (u8)1U);
    temp_v1 = temp_a1->polyOpa.p;
    temp_a1->polyOpa.p = temp_v1 + 8;
    temp_v1->words.w0 = 0xDB060020;
    temp_a0 = *(&D_80A88CFC + (this->unk_30E * 4));
    temp_v1->words.w1 = (temp_a0 & 0xFFFFFF) + gSegments[(u32) (temp_a0 * 0x10) >> 0x1C] + 0x80000000;
    sp18 = this;
    SkelAnime_DrawFlexOpa(globalCtx, this->skelAnime.skeleton, this->skelAnime.jointTable, (s32) this->skelAnime.dListCount, 0, 0);
    Graph_CloseDisps(&sp3C, globalCtx->state.gfxCtx, (const char *) "../z_en_jj.c", 0x382);
}
```

Notable features are the Open and Close Disps functions, and blocks of the form

```C
temp_v1 = temp_a1->polyOpa.p;
temp_a1->polyOpa.p = temp_v1 + 8;
temp_v1->words.w0 = 0xDB060020;
temp_a0 = *(&D_80A88CFC + (this->unk_30E * 4));
temp_v1->words.w1 = (temp_a0 & 0xFFFFFF) + gSegments[(u32) (temp_a0 * 0x10) >> 0x1C] + 0x80000000;
```

(This is a particularly simple example, since there's only one of these blocks. We will give a more involved example later.)

Each of these blocks converts into a graphics macro. They are usually (but not always) straightforward, but manually converting them is a pain, and there are sometimes special cases. To deal with them easily, we will use a tool from glank's N64 tools. To install these, follow the instructions [here](https://practicerom.com/public/packages/debian/howto.txt).

For our purposes, we only need one of the programs this provides: `gfxdis.f3dex2`.


Graphics are actually 64-bit on the Nintendo 64. This code block is a result of instructions telling the processor what to do with the graphics pointer. There are two types of graphics pointer,
- polyOpa for solid textures
- polyXlu for translucent textures

Our example is polyOpa, not surprisingly since Jabu is solid.

`words.w0` and `words.w1` contain the actual graphics instruction, in hex format. Usually, `w0` is constant and `w1` contains the arguments. To find out what sort of macro we are dealing with, we use `gfxdis.f3dex2`. `w1` is variable, but we need to give the program a constant placeholder. A common word to use is 12345678, so in this case we run
```
gfxdis.f3dex2 -x -g "POLY_OPA_DISP++" -d DB06002012345678
```

- `-x` uses hex instead of the default qu macros (never mind what those are, OoT doesn't have them)
- `-g` is used to specify which graphics pointer macro to use
- `-d` is for the graphics dword

Our standard now is to use decimal colors. If you have a constant second argument rather than a variable one, you can also use `-dc` to get decimal colors instead of the default hex.

The output looks like
```
gSPSegment(POLY_OPA_DISP++, 0x08, 0x12345678);
```

We can now replace the `0x12345678` by the actual second word. Or we could, if we had worked out what it was.

Firstly, `*(&D_80A88CFC + (this->unk_30E * 4))` is referring to a piece of data we haven't externed yet. It looks like
```
glabel D_80A88CFC
.word 0x06007698, 0x06007A98, 0x06007E98, 0x00000000, 0x00000000
```

The first three words look like pointers to assets in the actor segment, which would make sense if we're looking for textures to draw. The last two words are 0, which is strange. A check in the rest of the actor file shows that `unk_30E` only takes the values `0,1,2`. We conclude that the last two words are just padding, and can be removed. Because this data is used in a graphics macro, it will be either a displaylist or a texture. We therefore set it up to be an array of unknown pointers for now:
```C
extern UNK_PTR D_80A88CFC[];
// static Gfx* D_80A88CFC[] = { 0x06007698, 0x06007A98, 0x06007E98, }
```

It goes through further processing before it is used in the macro: `(temp_a0 & 0xFFFFFF) + gSegments[(u32) (temp_a0 * 0x10) >> 0x1C] + 0x80000000` is a conversion of a segmented address into a VRAM address. We have a macro for this too: `SEGMENTED_TO_VIRTUAL`. So after all this, the second word is
```C
SEGMENTED_TO_VIRTUAL(D_80A88CFC[this->unk_30E]);
```
and the whole macro is
```C
gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80A88CFC[this->unk_30E]));
```
The contents of a desegmentation macro used like this are almost always textures in this context, so we can replace `UNK_PTR` by `u64*`, the type for textures.

You repeat this for every block in the function.

We also have macros for Graph_OpenDisps and Graph_CloseDisps: you can replace
```C
Graph_OpenDisps(&sp3C, temp_a1, (const char *) "../z_en_jj.c", 0x36F);
```
by
```C
OPEN_DISPS(temp_a1, "../z_en_jj.c", 879);
```
(the last argument is a line number, so should be in decimal).

The function may or may not use a temp for `globalCtx->state.gfxCtx`: you have to work it out using the diff.

Once you've replaced all the blocks and the open and close with macros, you proceed with the function as usual.

Two last things: the last argument of the matrix functions tells the compiler whether to use the previously stored matrix or not, so we have the enums `MTXMODE_NEW` and `MTXMODE_APPLY` for these. And the rather weird-looking float `0.076624215f` is, of all things, `(M_PI/41.0f)` (try Wolfram Alpha for these things, and if that doesn't produce anything sensible, ask in Discord).

(The actual reason is probably that the animation is 41 frames long, but you won't necessarily spot this sort of thing for most floats)

After all that, it turns out that
```C
void EnJj_Draw(Actor *thisx, GlobalContext *globalCtx) {
    EnJj *this = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_jj.c", 879);
    func_800943C8(globalCtx->state.gfxCtx);
    Matrix_Translate(0.0f, (cosf(this->skelAnime.curFrame * (M_PI/41.0f)) * 10.0f) - 10.0f, 0.0f, 1);
    Matrix_Scale(10.0f, 10.0f, 10.0f, 1);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80A88CFC[this->unk_30E]));
    SkelAnime_DrawFlexOpa(globalCtx, this->skelAnime.skeleton, this->skelAnime.jointTable,
     this->skelAnime.dListCount, 0, 0, this);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_jj.c", 898);
}
```

matches apart from a couple of stack differences. This can be resolved by giving it `GlobalContext* globalCtx = globalCtx2;` at the top of the function and changing the second argument to `globalCtx2` as usual.

We have enums for the last argument of the matrix functions: `0` is `MTXMODE_NEW`, `1` is `MTXMODE_APPLY`.

Lastly, the penultimate and antepenultimate arguments of `SkelAnime_DrawFlexOpa` are actually pointers to functions, so they should be `NULL` instead of `0`.


## More examples: OverrideLimbDraw and PostLimbDraw








