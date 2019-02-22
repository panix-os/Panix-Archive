#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../kernel/util/util.h"

/* Can't do this with a loop because we need the address
 * of the function names */
void isrInstall() {
    setIdtGate(0, (uint32_t)isr0);
    setIdtGate(1, (uint32_t)isr1);
    setIdtGate(2, (uint32_t)isr2);
    setIdtGate(3, (uint32_t)isr3);
    setIdtGate(4, (uint32_t)isr4);
    setIdtGate(5, (uint32_t)isr5);
    setIdtGate(6, (uint32_t)isr6);
    setIdtGate(7, (uint32_t)isr7);
    setIdtGate(8, (uint32_t)isr8);
    setIdtGate(9, (uint32_t)isr9);
    setIdtGate(10, (uint32_t)isr10);
    setIdtGate(11, (uint32_t)isr11);
    setIdtGate(12, (uint32_t)isr12);
    setIdtGate(13, (uint32_t)isr13);
    setIdtGate(14, (uint32_t)isr14);
    setIdtGate(15, (uint32_t)isr15);
    setIdtGate(16, (uint32_t)isr16);
    setIdtGate(17, (uint32_t)isr17);
    setIdtGate(18, (uint32_t)isr18);
    setIdtGate(19, (uint32_t)isr19);
    setIdtGate(20, (uint32_t)isr20);
    setIdtGate(21, (uint32_t)isr21);
    setIdtGate(22, (uint32_t)isr22);
    setIdtGate(23, (uint32_t)isr23);
    setIdtGate(24, (uint32_t)isr24);
    setIdtGate(25, (uint32_t)isr25);
    setIdtGate(26, (uint32_t)isr26);
    setIdtGate(27, (uint32_t)isr27);
    setIdtGate(28, (uint32_t)isr28);
    setIdtGate(29, (uint32_t)isr29);
    setIdtGate(30, (uint32_t)isr30);
    setIdtGate(31, (uint32_t)isr31);

    setIdt(); // Load with ASM
}

/* To print the message which defines every exception */
char* exceptionMessages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isrHandler(registers_t r) {
    kprint("received interrupt: ");
    char s[3];
    int_to_ascii(r.interruptNumber, s);
    kprint(s);
    kprint("\n");
    kprint(exceptionMessages[r.interruptNumber]);
    kprint("\n");
}