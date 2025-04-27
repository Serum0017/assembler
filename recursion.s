    irmovq $0x5, %rax
    irmovq $0x1, %rbx
    call L1
    nop
    nop
    nop
    irmovq $0x100, %rcx
    halt
L1:
    subq %rbx, %rax
    jle L2
    call L1 # 3f
    jmp L2
    halt
L2:
    ret
    halt