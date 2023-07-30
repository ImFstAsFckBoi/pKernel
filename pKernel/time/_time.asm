SECTION .data
SECTION	.text
    GLOBAL timestamp
    ALIGN 16
    BITS 32

timestamp:
    rdtsc   ; Sets EAX:EDX to timestamp
    ret     ; Returns EAX:EDX since
            ; return value is QWORD





