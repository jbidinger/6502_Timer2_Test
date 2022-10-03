; vasm6502_oldstyle -Fbin -dotdir -wdc02 ramtest_symon.s -o ramtest_symon.out
; xxd -a -o 49152 ramtest_symon.out

    .org $c000
        ;Setup stack
        ldx #$ff
        txs    
    ; start ram test here.
HELLOWORLD:
IOBASE   = $8800
IOSTATUS = IOBASE + 1
IOCMD    = IOBASE + 2
IOCTRL   = IOBASE + 3
STRNGA   = $00

;.org $0300

start:  ;cli
    lda #<string
    sta STRNGA
    lda #>string
    sta STRNGA + 1
    pha
    tya
    pha
    jsr ser_init
;        lda #$0b
;        sta IOCMD      ; Set command status
;        lda #$1a
;        sta IOCTRL     ; 0 stop bits, 8 bit word, 2400 baud

init:   ldy #$00       ; Initialize index

loop:   lda IOSTATUS
        lda (STRNGA),y   ; Otherwise, load the string pointer
        beq init       ; If the char is 0, re-init
        jsr ser_printchar
        iny            ; Increment string pointer.
        jmp loop       ; Repeat write.
        pla
        tay
        pla

string: .string "Hello, 6502 world! "

ser_init:
    pha
    lda #$0b
    sta IOCMD      ; Set command status
    lda #$1a
    sta IOCTRL     ; 0 stop bits, 8 bit word, 2400 baud
    pla
    rts

ser_printchar:
        sta $02         ; Need the character back later.
        pha
        tya
        pha
ser_loop
        lda IOSTATUS
        and #$10       ; Is the tx register empty?
        beq ser_loop   ; No, wait for it to empty
        lda $02        ; Otherwise, load the string pointer
        sta IOBASE     ; transmit
        pla
        tay
        pla

hex2ascii:      ; Pass value in a
    cmp #10
    bcc H2A     ; Less than #10
    adc #6      ; Carry is set so this is really a 7
H2A:
    adc #48
    rts

RAMTEST:
    .org $fffc
    .word start
    .word RAMTEST