glabel func_80A59014
/* 00404 80A59014 3C068012 */  lui     $a2, %hi(gDbgCamEnabled)
/* 00408 80A59018 8CC6D394 */  lw      $a2, %lo(gDbgCamEnabled)($a2)
/* 0040C 80A5901C 27BDFFA8 */  addiu   $sp, $sp, 0xFFA8           ## $sp = FFFFFFA8
/* 00410 80A59020 AFBF0014 */  sw      $ra, 0x0014($sp)
/* 00414 80A59024 AFA40058 */  sw      $a0, 0x0058($sp)
/* 00418 80A59028 0006302B */  sltu    $a2, $zero, $a2
/* 0041C 80A5902C 00A03825 */  or      $a3, $a1, $zero            ## $a3 = 00000000
/* 00420 80A59030 14C00003 */  bne     $a2, $zero, .L80A59040
/* 00424 80A59034 8CA21C44 */  lw      $v0, 0x1C44($a1)           ## 00001C44
/* 00428 80A59038 90A61D6C */  lbu     $a2, 0x1D6C($a1)           ## 00001D6C
/* 0042C 80A5903C 0006302B */  sltu    $a2, $zero, $a2
.L80A59040:
/* 00430 80A59040 10C00003 */  beq     $a2, $zero, .L80A59050
/* 00434 80A59044 8FA40058 */  lw      $a0, 0x0058($sp)
/* 00438 80A59048 10000002 */  beq     $zero, $zero, .L80A59054
/* 0043C 80A5904C 24E600E0 */  addiu   $a2, $a3, 0x00E0           ## $a2 = 000000E0
.L80A59050:
/* 00440 80A59050 24460024 */  addiu   $a2, $v0, 0x0024           ## $a2 = 00000024
.L80A59054:
/* 00444 80A59054 27A50044 */  addiu   $a1, $sp, 0x0044           ## $a1 = FFFFFFEC
/* 00448 80A59058 0C00B6F4 */  jal     func_8002DBD0
/* 0044C 80A5905C AFA7005C */  sw      $a3, 0x005C($sp)
/* 00450 80A59060 8FAE0058 */  lw      $t6, 0x0058($sp)
/* 00454 80A59064 24010006 */  addiu   $at, $zero, 0x0006         ## $at = 00000006
/* 00458 80A59068 85CF001C */  lh      $t7, 0x001C($t6)           ## 0000001C
/* 0045C 80A5906C 000FC183 */  sra     $t8, $t7,  6
/* 00460 80A59070 33190007 */  andi    $t9, $t8, 0x0007           ## $t9 = 00000000
/* 00464 80A59074 17210004 */  bne     $t9, $at, .L80A59088
/* 00468 80A59078 3C0142C8 */  lui     $at, 0x42C8                ## $at = 42C80000
/* 0046C 80A5907C 44811000 */  mtc1    $at, $f2                   ## $f2 = 100.00
/* 00470 80A59080 10000004 */  beq     $zero, $zero, .L80A59094
/* 00474 80A59084 00000000 */  nop
.L80A59088:
/* 00478 80A59088 3C014348 */  lui     $at, 0x4348                ## $at = 43480000
/* 0047C 80A5908C 44811000 */  mtc1    $at, $f2                   ## $f2 = 200.00
/* 00480 80A59090 00000000 */  nop
.L80A59094:
/* 00484 80A59094 0C296306 */  jal     EnHoll_IsKokiriSetup8              
/* 00488 80A59098 E7A2003C */  swc1    $f2, 0x003C($sp)           
/* 0048C 80A5909C 14400022 */  bne     $v0, $zero, .L80A59128     
/* 00490 80A590A0 C7A2003C */  lwc1    $f2, 0x003C($sp)           
/* 00494 80A590A4 3C01C248 */  lui     $at, 0xC248                ## $at = C2480000
/* 00498 80A590A8 44812000 */  mtc1    $at, $f4                   ## $f4 = -50.00
/* 0049C 80A590AC C7A00048 */  lwc1    $f0, 0x0048($sp)
/* 004A0 80A590B0 3C014348 */  lui     $at, 0x4348                ## $at = 43480000
/* 004A4 80A590B4 4600203C */  c.lt.s  $f4, $f0
/* 004A8 80A590B8 00000000 */  nop
/* 004AC 80A590BC 4502003D */  bc1fl   .L80A591B4
/* 004B0 80A590C0 8FBF0014 */  lw      $ra, 0x0014($sp)
/* 004B4 80A590C4 44813000 */  mtc1    $at, $f6                   ## $f6 = 200.00
/* 004B8 80A590C8 00000000 */  nop
/* 004BC 80A590CC 4606003C */  c.lt.s  $f0, $f6
/* 004C0 80A590D0 C7A00044 */  lwc1    $f0, 0x0044($sp)
/* 004C4 80A590D4 45020037 */  bc1fl   .L80A591B4
/* 004C8 80A590D8 8FBF0014 */  lw      $ra, 0x0014($sp)
/* 004CC 80A590DC 46000005 */  abs.s   $f0, $f0
/* 004D0 80A590E0 3C0142C8 */  lui     $at, 0x42C8                ## $at = 42C80000
/* 004D4 80A590E4 4602003C */  c.lt.s  $f0, $f2
/* 004D8 80A590E8 C7A0004C */  lwc1    $f0, 0x004C($sp)
/* 004DC 80A590EC 45020031 */  bc1fl   .L80A591B4
/* 004E0 80A590F0 8FBF0014 */  lw      $ra, 0x0014($sp)
/* 004E4 80A590F4 44814000 */  mtc1    $at, $f8                   ## $f8 = 100.00
/* 004E8 80A590F8 46000005 */  abs.s   $f0, $f0
/* 004EC 80A590FC 3C014248 */  lui     $at, 0x4248                ## $at = 42480000
/* 004F0 80A59100 4608003C */  c.lt.s  $f0, $f8
/* 004F4 80A59104 00000000 */  nop
/* 004F8 80A59108 4502002A */  bc1fl   .L80A591B4
/* 004FC 80A5910C 8FBF0014 */  lw      $ra, 0x0014($sp)
/* 00500 80A59110 44815000 */  mtc1    $at, $f10                  ## $f10 = 50.00
/* 00504 80A59114 00000000 */  nop
/* 00508 80A59118 4600503C */  c.lt.s  $f10, $f0
/* 0050C 80A5911C 00000000 */  nop
/* 00510 80A59120 45020024 */  bc1fl   .L80A591B4
/* 00514 80A59124 8FBF0014 */  lw      $ra, 0x0014($sp)
.L80A59128:
/* 00518 80A59128 C7B0004C */  lwc1    $f16, 0x004C($sp)
/* 0051C 80A5912C 44809000 */  mtc1    $zero, $f18                ## $f18 = 0.00
/* 00520 80A59130 8FA80058 */  lw      $t0, 0x0058($sp)
/* 00524 80A59134 8FA5005C */  lw      $a1, 0x005C($sp)
/* 00528 80A59138 4612803C */  c.lt.s  $f16, $f18
/* 0052C 80A5913C 9507001C */  lhu     $a3, 0x001C($t0)           ## 0000001C
/* 00530 80A59140 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00534 80A59144 00A12821 */  addu    $a1, $a1, $at
/* 00538 80A59148 45000003 */  bc1f    .L80A59158
/* 0053C 80A5914C 00073A83 */  sra     $a3, $a3, 10
/* 00540 80A59150 10000002 */  beq     $zero, $zero, .L80A5915C
/* 00544 80A59154 00002025 */  or      $a0, $zero, $zero          ## $a0 = 00000000
.L80A59158:
/* 00548 80A59158 24040001 */  addiu   $a0, $zero, 0x0001         ## $a0 = 00000001
.L80A5915C:
/* 0054C 80A5915C 8CA91D38 */  lw      $t1, 0x1D38($a1)           ## 00001D38
/* 00550 80A59160 00075100 */  sll     $t2, $a3,  4
/* 00554 80A59164 00045840 */  sll     $t3, $a0,  1
/* 00558 80A59168 012A1821 */  addu    $v1, $t1, $t2
/* 0055C 80A5916C 006B6021 */  addu    $t4, $v1, $t3
/* 00560 80A59170 81860000 */  lb      $a2, 0x0000($t4)           ## 00000000
/* 00564 80A59174 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00568 80A59178 34211CBC */  ori     $at, $at, 0x1CBC           ## $at = 00011CBC
/* 0056C 80A5917C A1060003 */  sb      $a2, 0x0003($t0)           ## 00000003
/* 00570 80A59180 80AD1CBC */  lb      $t5, 0x1CBC($a1)           ## 00001CBC
/* 00574 80A59184 81070003 */  lb      $a3, 0x0003($t0)           ## 00000003
/* 00578 80A59188 8FA4005C */  lw      $a0, 0x005C($sp)
/* 0057C 80A5918C 11A70008 */  beq     $t5, $a3, .L80A591B0
/* 00580 80A59190 00812821 */  addu    $a1, $a0, $at
/* 00584 80A59194 0C025CA3 */  jal     func_8009728C
/* 00588 80A59198 00E03025 */  or      $a2, $a3, $zero            ## $a2 = 00000000
/* 0058C 80A5919C 10400004 */  beq     $v0, $zero, .L80A591B0     
/* 00590 80A591A0 8FA40058 */  lw      $a0, 0x0058($sp)           
/* 00594 80A591A4 3C0580A6 */  lui     $a1, %hi(EnHoll_NextAction)    ## $a1 = 80A60000
/* 00598 80A591A8 0C296304 */  jal     EnHoll_SetupAction              
/* 0059C 80A591AC 24A59828 */  addiu   $a1, $a1, %lo(EnHoll_NextAction) ## $a1 = 80A59828
.L80A591B0:
/* 005A0 80A591B0 8FBF0014 */  lw      $ra, 0x0014($sp)
.L80A591B4:
/* 005A4 80A591B4 27BD0058 */  addiu   $sp, $sp, 0x0058           ## $sp = 00000000
/* 005A8 80A591B8 03E00008 */  jr      $ra
/* 005AC 80A591BC 00000000 */  nop
