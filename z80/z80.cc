#include "./z80.h"

/* TODO: Tener en cuenta memoria RAM Stack (SP) */

z80::z80(void)
: 
a(0), b(0), c(0), d(0), e(0), f(0), h(0), l(0), i(0), r(0), ixh(0), ixl(0), iyh(0), iyl(0), 
af(0), bc(0), de(0), hl(0), pc(0), sp(0), ix(0), iy(0), interrupts(false)
{
}

std::tuple<int8_t*, int8_t*> z80::GetCpuRegisters(Register regA, Register regB) noexcept {
    int8_t *a, *b;

    switch (regA) {
        case REG_A:
            a = &this->a;
            break;
        case REG_B:
            a = &this->b;
            break;    
        case REG_C:
            a = &this->c;
            break;
        case REG_D:
            a = &this->d;
            break;
        case REG_E:
            a = &this->e;
            break;
        case REG_F:
            a = &this->f;
            break;
        case REG_H:
            a = &this->h;
            break;
        case REG_I:
            a = &this->i;
            break;
        case REG_L:
            a = &this->l;
            break;
        case REG_R:
            a = &this->r;
            break;
    }

    switch (regB) {
        case REG_A:
            b = &this->a;
            break;
        case REG_B:
            b = &this->b;
            break;    
        case REG_C:
            b = &this->c;
            break;
        case REG_D:
            b = &this->d;
            break;
        case REG_E:
            b = &this->e;
            break;
        case REG_F:
            b = &this->f;
            break;
        case REG_H:
            b = &this->h;
            break;
        case REG_I:
            b = &this->i;
            break;
        case REG_L:
            b = &this->l;
            break;
        case REG_R:
            b = &this->r;
            break;
    }

    return std::make_tuple(a, b);
}

int8_t* z80::GetCpuRegister(Register reg) noexcept {
    int8_t* a;

    switch (reg) {
        case REG_A:
            a = &this->a;
            break;
        case REG_B:
            a = &this->b;
            break;    
        case REG_C:
            a = &this->c;
            break;
        case REG_D:
            a = &this->d;
            break;
        case REG_E:
            a = &this->e;
            break;
        case REG_F:
            a = &this->f;
            break;
        case REG_H:
            a = &this->h;
            break;
        case REG_I:
            a = &this->i;
            break;
        case REG_L:
            a = &this->l;
            break;
        case REG_R:
            a = &this->r;
            break;
    }

    return a;
}

void z80::AbsoluteJump(int16_t* mem) noexcept {
    auto currentPosition = &stack[sp];
    sp = CMP_MEM(mem, currentPosition) ? sp + (mem - currentPosition) : sp - (mem - currentPosition);
}

void z80::RelativeJump(int16_t* mem) {
    auto currentPosition = &stack[sp];
    if ((mem - currentPosition) > 128)
        throw new std::exception();
    sp = CMP_MEM(mem, currentPosition) ? sp + (mem - currentPosition) : sp - (mem - currentPosition);
}

void z80::Return(void) noexcept {
    pc = stack.back();
}

/* TODO: Mirar como actúa el condicional de la instrucción RET. */
void z80::Return(Flag flag) noexcept {
    pc = stack.back();
}

void z80::Move(Register regA, Register regB) noexcept {
    auto registers = GetCpuRegisters(regA, regB);
    auto *a = std::get<0>(registers), *b = std::get<1>(registers);
    *a = *b;
}

void z80::Move(Register reg, int8_t data) noexcept {
    auto registerA = GetCpuRegister(reg);
    *registerA = data;
}

void z80::Ldd(void) noexcept {
    /** TODO: ¿? 
     * 
     * LD (DE), (HL)
    */
    
    --de;
    --hl;
    --bc;
}

void z80::And(Register regA, Register regB) noexcept {
    auto registers = GetCpuRegisters(regA, regB);
    auto *a = std::get<0>(registers), *b = std::get<1>(registers);
    *a &= *b;
}

void z80::And(Register reg, int8_t data) noexcept {
    auto registerA = GetCpuRegister(reg);
    *registerA &= data;
}

void z80::Or(Register regA, Register regB) noexcept {
    auto registers = GetCpuRegisters(regA, regB);
    auto *a = std::get<0>(registers), *b = std::get<1>(registers);
    *a |= *b;
}

void z80::Or(Register reg, int8_t data) noexcept {
    auto registerA = GetCpuRegister(reg);
    *registerA |= data;
}

void z80::Add(Register regA, Register regB) noexcept {
    auto registers = GetCpuRegisters(regA, regB);
    auto *a = std::get<0>(registers), *b = std::get<1>(registers);
    *a += *b;
    sp += 16;
}

void z80::Add(Register reg, int8_t data) noexcept {
    auto registerA = GetCpuRegister(reg);
    *registerA += data;
}

void z80::Substraction(Register regA, Register regB) noexcept {
    auto registers = GetCpuRegisters(regA, regB);
    auto *a = std::get<0>(registers), *b = std::get<1>(registers);
    *a -= *b;
}

void z80::Substraction(Register reg, int8_t data) noexcept {
    auto registerA = GetCpuRegister(reg);
    *registerA -= data;
}

void z80::Increment(Register reg) noexcept {
    auto registerA = GetCpuRegister(reg);
    ++*registerA;
}

void z80::Decrement(Register reg) noexcept {
    auto registerA = GetCpuRegister(reg);
    --*registerA;
}

void z80::Exchange(Register regA, Register regB) noexcept {
    auto registers = GetCpuRegisters(regA, regB);
    auto *a = std::get<0>(registers), *b = std::get<1>(registers);
    const auto aux = *a;
    *a = *b;
    *b = aux;
}

void z80::Push(Register reg) noexcept {
    auto registerA = GetCpuRegister(reg);
    

}

void z80::Pop(Register reg) noexcept {
    auto registerA = GetCpuRegister(reg);


}

template <int8_t id> void z80::Out(Port<id> port, Register reg) noexcept {
    auto registerA = GetCpuRegister(reg);
    port.data = *registerA;
}

template <int8_t id> void z80::In(Register reg, Port<id> port) noexcept {
    auto registerA = GetCpuRegister(reg);
    *registerA = port.data;
}

void z80::IMode(Mode mode) noexcept {
    this->mode = mode;
}

void z80::DisableInterrupts(void) noexcept {
    interrupts = false;
}

void z80::EnableInterrupts(void) noexcept {
    interrupts = true;
}