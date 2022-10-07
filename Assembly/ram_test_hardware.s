; vasm6502_oldstyle -Fbin -dotdir -wdc02 ram_test_hardware.s -o ram_test_hardware.bin
; xxd -a -o 32768 ram_test_hardware.bin
    .org $8000 ; IO
    .string "There be deamons here!"

    .org $9000
START:
    lda #$FF
    sta $00
    sta $01
    lda $00
    cmp #$FF
    bne OH_FUCK ; We stored a FF got something else back.
    lda $01
    cmp #$FF
    bne OH_FUCK ; We stored a FF got something else back.

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
    lda #$00
    sta $00
    sta $01
    jmp OH_FUCK

;    .org $fff7
;        jmp START ; Go back to 9000, skipping IO region and read NOPs again.  
    .org $fffA
    .word $0000   ; NMIB
    .word START   ; Jump on boot
    .word $0000   ; IRQB/BRK