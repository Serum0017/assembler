main:
    nop
    cmovl %rax, %r14
    irmovq $0xabcd, %rdx
    rmmovq %rsi, 0x41c(%rsp)
    mrmovq -0x41c(%rsi), %rsp
L0:
    xorq %rbp, %r14
    jle .L1
    jmp .L0
    call .L0
    addq %rsi, %rdi 
    subq %rsi, %rdi
    andq %rsi, %rdi
    pushq %rax
    popq %rax
    ret
L1: