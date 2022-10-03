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
        lda #$0b
        sta IOCMD      ; Set command status
        lda #$1a
        sta IOCTRL     ; 0 stop bits, 8 bit word, 2400 baud

init:   ldy #$00       ; Initialize index

loop:   lda IOSTATUS
        and #$10       ; Is the tx register empty?
        beq loop       ; No, wait for it to empty
;        lda string,x   ; Otherwise, load the string pointer
        lda (STRNGA),y   ; Otherwise, load the string pointer
        beq init       ; If the char is 0, re-init
        sta IOBASE     ; Otherwise, transmit

        iny            ; Increment string pointer.
        jmp loop       ; Repeat write.
        pla
        txa
        pla

string: .byte "Hello, 6502 world! ", 0

RAMTEST:
    .org $fffc
    .word start
    .word RAMTEST