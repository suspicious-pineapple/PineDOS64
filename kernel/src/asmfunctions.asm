

global get_cr3
get_cr3:
mov rax, cr3
ret


global set_cr3
set_cr3:
mov rax, rdi
mov cr3, rax
ret



global load_gdt
load_gdt:
   mov   [gdtr_desc], DI
   mov   [gdtr_desc+2], RSI
   lgdt  [gdtr_desc]
ret

;global load_gdt
;load_gdt:
;lgdt [rdi]

ret

global load_idt
load_idt:
mov [idtr_desc], DI
mov [idtr_desc+2], RSI
lidt [idtr_desc]
ret




global load_tss
load_tss:
ltr di
ret



section .data
gdtr_desc DW 0 ; For limit storage
     DQ 0 ; For base storage

section .data
idtr_desc DW 0 ; For limit storage
     DQ 0 ; For base storage
