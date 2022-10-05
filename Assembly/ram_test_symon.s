; vasm6502_oldstyle -Fbin -dotdir -wdc02 ram_test_symon.s -o ram_test_symon.bin
; xxd -a -o 49152 ram_test_symon.bin

    .org $C000
START:

ZERO_PAGE:
    lda #$FF
    sta $00
    lda $00
    cmp #$FF
    bne OH_FUCK ; We stored a FF got something else back.
    lda #$FF
    sta $01
    lda $01
    cmp #$FF
    bne OH_FUCK ; We stored a FF got something else back.

    jmp ZERO_PAGE

OH_FUCK:
    NOP
    NOP
    NOP
    NOP
    jmp OH_FUCK


    .org $fff7
        jmp START ; Go back to 9000, skipping IO region and read NOPs again.  
    .org $fffA
    .word $FFFF   ; NMIB
    .word START   ; Jump on boot
    .word $FFFF   ; IRQB/BRK