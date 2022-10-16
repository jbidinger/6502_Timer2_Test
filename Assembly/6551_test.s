; vasm6502_oldstyle.exe -Fbin -dotdir -wdc02 6551_test.s -o 6551_test.out
; xxd -a -o 32768 6551_test.out

  ; IO sections are masked by the PLD. ROM really starts at $9000
  ; All the way up to $8900 but the PLD only maps 3 right now.
  ; $8000    ; IO0
  ; $8100    ; IO1
  ; $8200    ; IO2

IOBASE   = $8000
IOSTATUS = IOBASE + 1 ; 65C51
IOCMD    = IOBASE + 2
IOCTRL   = IOBASE + 3
STRNGA   = $00        ; Store the address of the string to print here

CR = $0d
LF = $0A
SPACE = $20
INPUT_BUFFER = $0200
INPUT_BUFFER_LEN = $FF


  .org $8000
    ; IO Region
  .org $9000
START:
    ;Setup stack
    ldx #$ff
    txs
    jsr ser_init

MAIN_LOOP:
  jsr HELLO
READ_LINE_WECHO:
  jsr ser_getline
  jsr PRINT_LINE_HEX
  jmp MAIN_LOOP

PRINT_LINE_HEX:
  ldx #$00
plh_loop:
  lda INPUT_BUFFER,X
  cmp #CR
  beq plh_exit
  jsr PRHEXBYTE
  lda #SPACE
  jsr CHAROUT
  inx
  jmp plh_loop
plh_exit:
  lda #CR
  jsr CHAROUT
  lda #LF
  jsr CHAROUT
  rts

HELLO:
  jsr PRIMM
	.string "6502>"
	rts

ser_init:
    pha
    lda #$0b       ; 00001011
    sta IOCMD      ; Set command status
    lda #$1a
    sta IOCTRL     ; 0 stop bits, 8 bit word, 2400 baud
    pla
    rts

CHAROUT:
ser_printchar:
    pha
ser_loop:
    lda IOSTATUS
    and #$10       ; Is the tx register empty?
    beq ser_loop   ; No, wait for it to empty
    pla            ; Otherwise, load the string pointer
    sta IOBASE     ; transmit
    rts

ser_getline:      ; x returns length of string
    pha
    ldx #$00      ; Zero index into buffer
sgl_loop:
    jsr ser_getchar ; Get next character in A
    
    cmp #CR
    bne sgl_notcr
    sta INPUT_BUFFER,x
    inx
    jsr ser_printchar
    lda #LF
    jsr ser_printchar
sgl_return:
    pla
    rts
sgl_notcr:    
    ; TODO: add checking for backspace, arrows, etc. later.
    sta INPUT_BUFFER,x
    inx
    jsr ser_printchar
    jmp sgl_loop;

ser_getchar:
    lda IOSTATUS
    and #$08        ; Receive data register full
    beq ser_getchar ; not full yet. wait.
    lda IOBASE
    rts

hex2ascii:      ; Pass value in a
    and #$0f
    cmp #10
    bcc H2A     ; Less than #10
    adc #6      ; Carry is set so this is really a 7
H2A:
    adc #48
    rts

PRHEXBYTE:
  pha
  lsr
  lsr
  lsr
  lsr
  jsr hex2ascii
  jsr CHAROUT
  pla
PRHEX:
  jsr hex2ascii
  jsr CHAROUT
  rts


; Inline printing routine from http://6502.org/source/io/primm.htm
; Example for PRIMM:
;	JSR PRIMM
;	.BYTE "This will be printed!",$00
;	RTS
PRIMM:
    PHA     		; save A
    TYA			    ; copy Y
    PHA  			  ; save Y
    TXA			    ; copy X
    PHA  			  ; save X
    TSX			    ; get stack pointer
    LDA $0104,X		; get return address low byte (+4 to
          				;   correct pointer)
    STA $BC		  	; save in page zero
    LDA $0105,X		; get return address high byte (+5 to
                  ;   correct pointer)
    STA $BD			  ; save in page zero
    LDY #$01		  ; set index (+1 to allow for return
                  ;   address offset)
PRIM2:
    LDA ($BC),Y		; get byte from string
    BEQ PRIM3		; exit if null (end of text)

    JSR CHAROUT		; else display character
    INY			; increment index
    BNE PRIM2		; loop (exit if 256th character)

PRIM3:
    TYA			; copy index
    CLC			; clear carry
    ADC $BC			; add string pointer low byte to index
    STA $0104,X		; put on stack as return address low byte
          ; (+4 to correct pointer, X is unchanged)
    LDA #$00		; clear A
    ADC $BD		; add string pointer high byte
    STA $0105,X		; put on stack as return address high byte
          ; (+5 to correct pointer, X is unchanged)
    PLA			; pull value
    TAX  			; restore X
    PLA			; pull value
    TAY  			; restore Y
    PLA  			; restore A
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