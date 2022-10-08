; vasm6502_oldstyle.exe -Fbin -dotdir -wdc02 6551_test.s -o 6551_test.out
; xxd -a -o 32768 6551_test.out

  ; IO sections are masked by the PLD. ROM really starts at $9000
  ; All the way up to $8900 but the PLD only maps 3 right now.
  ; $8000    ; IO0
  ; $8100    ; IO1
  ; $8200    ; IO2

IOBASE   = $8000
IOSTATUS = IOBASE + 1
IOCMD    = IOBASE + 2
IOCTRL   = IOBASE + 3
STRNGA   = $00
SAVEA    = $02

  .org $8000
    ; IO Region
  .org $9000
    ;Setup stack
    ldx #$ff
    txs
START:
    pha
    tya
    pha
    lda #<string
    sta STRNGA
    lda #>string
    sta STRNGA + 1
    jsr ser_init
init:   
    ldy #$00       ; Initialize index

loop: 
    lda (STRNGA),y ; Otherwise, load the string pointer
    beq init       ; If the char is 0, re-init
    jsr ser_printchar
    iny            ; Increment string pointer.
    jmp loop       ; Repeat write.
    pla
    tay
    pla
    rts

string: .string "Hello, 6502 world! ",$0A,$0D

ser_init:
    pha
    lda #$0b       ; 00001011
    sta IOCMD      ; Set command status
    lda #$1a
    sta IOCTRL     ; 0 stop bits, 8 bit word, 2400 baud
    pla
    rts

ser_printchar:
        sta SAVEA      ; Need the character in "A" back later.
        pha
ser_loop:
        lda IOSTATUS
        and #$10       ; Is the tx register empty?
        beq ser_loop   ; No, wait for it to empty
        lda SAVEA      ; Otherwise, load the string pointer
        sta IOBASE     ; transmit
        pla
        rts

hex2ascii:      ; Pass value in a
    cmp #10
    bcc H2A     ; Less than #10
    adc #6      ; Carry is set so this is really a 7
H2A:
    adc #48
    rts

  .org $fff0    ; Jump table so other things can find and use these subroutines
  .word ser_printchar
  .word hex2ascii   ; Does not preserve registers.

  .org $fff7
    jmp START ; Go back to 9000, skipping IO region and read NOPs again.  
  .org $fffA
  .word $FFFF   ; NMIB
  .word START   ; Jump on boot
  .word $FFFF   ; IRQB/BRK