.globl rdrand

.text
rdrand:
    RDRAND %ax
    JNC rdrand

    ret
