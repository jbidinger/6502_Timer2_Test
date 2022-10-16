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
    ; At this point we have a couple bytes in zero page tested.
    ; Use them to store the address we are testing.

TEST_START
    lda #$02
    sta $00 
    lda #$00
    sta $01
    ldy #$00
    lda #$FF
TEST_LOOP:
    sta ($00),y
    lda ($00)
    cmp #$FF
    bne OH_FUCK
    iny             ; Counting on this to wrap from #$FF to #$00
    cpy #$00        ; Did we wrap?
    bne TEST_LOOP; More on this page
    inc $01     ; Next Page. $00 always stays zero. We index with Y
    ldx $01
    cpx #$80    ; Stop at the end of RAM (32k in this case)
    bne TEST_LOOP
    jmp TEST_START

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