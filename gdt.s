; make the label visible outside this file
global load_gdt
global load_segments

; Set up the memory information of the GDT
; stack: [esp + 4] The GDT info
;        [esp    ] return address
load_gdt:
    lgdt [esp + 4]      ; Load the GDT info
    ret                 ; return to the calling function

load_segments:
    mov ax, 0x10
    mov ds, ax          ; Point data segment to the second descriptor
    mov ss, ax          ; Point stack segment to the second descriptor
    mov es, ax          ; Point the extra segments to the second descriptor
    mov fs, ax
    mov gs, ax
    jmp 0x08:flush_cs   ; Use a jump to point the code segment to the first descriptor

flush_cs:
    ret                 ; Returns back to the C code
