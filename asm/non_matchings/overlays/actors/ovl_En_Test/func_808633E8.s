glabel func_808633E8
/* 03D98 808633E8 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 03D9C 808633EC AFB00018 */  sw      $s0, 0x0018($sp)           
/* 03DA0 808633F0 AFA50024 */  sw      $a1, 0x0024($sp)           
/* 03DA4 808633F4 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 03DA8 808633F8 240E0001 */  addiu   $t6, $zero, 0x0001         ## $t6 = 00000001
/* 03DAC 808633FC 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 03DB0 80863400 A48E001C */  sh      $t6, 0x001C($a0)           ## 0000001C
/* 03DB4 80863404 8607001C */  lh      $a3, 0x001C($s0)           ## 0000001C
/* 03DB8 80863408 8FA60024 */  lw      $a2, 0x0024($sp)           
/* 03DBC 8086340C 0C00CC17 */  jal     BodyBreak_SpawnParts              
/* 03DC0 80863410 260507F0 */  addiu   $a1, $s0, 0x07F0           ## $a1 = 000007F0
/* 03DC4 80863414 1040000D */  beq     $v0, $zero, .L8086344C     
/* 03DC8 80863418 8FA40024 */  lw      $a0, 0x0024($sp)           
/* 03DCC 8086341C 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 03DD0 80863420 26060024 */  addiu   $a2, $s0, 0x0024           ## $a2 = 00000024
/* 03DD4 80863424 0C007E50 */  jal     Item_DropCollectibleRandom
              
/* 03DD8 80863428 240700D0 */  addiu   $a3, $zero, 0x00D0         ## $a3 = 000000D0
/* 03DDC 8086342C 8E020118 */  lw      $v0, 0x0118($s0)           ## 00000118
/* 03DE0 80863430 10400004 */  beq     $v0, $zero, .L80863444     
/* 03DE4 80863434 00000000 */  nop
/* 03DE8 80863438 844F0018 */  lh      $t7, 0x0018($v0)           ## 00000018
/* 03DEC 8086343C 25F8FFFF */  addiu   $t8, $t7, 0xFFFF           ## $t8 = FFFFFFFF
/* 03DF0 80863440 A4580018 */  sh      $t8, 0x0018($v0)           ## 00000018
.L80863444:
/* 03DF4 80863444 0C00B55C */  jal     Actor_Kill
              
/* 03DF8 80863448 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
.L8086344C:
/* 03DFC 8086344C 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 03E00 80863450 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 03E04 80863454 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 03E08 80863458 03E00008 */  jr      $ra                        
/* 03E0C 8086345C 00000000 */  nop