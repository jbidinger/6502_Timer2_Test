; vasm6502_oldstyle -Fbin -dotdir -wdc02 ram_test_hardware.s -o ram_test_hardware.bin
; xxd -a -o 32768 ram_test_hardware.bin
    .org $8000 ; IO

    .org $9000
START:
    nop
    nop
    nop
    lda #$FF
    sta $00
    sta $01
    lda $00
    cmp #$FF
    bne OH_FUCK ; We stored a FF got something else back.
    lda $01
    cmp #$FF
    bne OH_FUCK ; We stored a FF got something else back.
    jmp START
    
OH_FUCK:
    lda #$00
    sta $00
    sta $01
    jmp OH_FUCK


    .org $fff7
        jmp START ; Go back to 9000, skipping IO region and read NOPs again.  
    .org $fffA
    .word $FFFF   ; NMIB
    .word START   ; Jump on boot
    .word $FFFF   ; IRQB/BRK