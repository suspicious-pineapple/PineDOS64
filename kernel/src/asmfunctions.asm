

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
lgdt [rdi]
ret

