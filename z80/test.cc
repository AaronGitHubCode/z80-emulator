#include <cassert>
#include <cstdio>

#include "./z80.h"

static z80 cpu;

int main(void) {
    cpu.Add(Register::REG_A, 10);
    cpu.Move(Register::REG_B, Register::REG_A);
    cpu.Substraction(Register::REG_B, 3);
    assert(cpu.b == 7);
    return 0;
}