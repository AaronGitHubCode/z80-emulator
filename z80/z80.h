#pragma once

#include <array>
#include <tuple>
#include <exception>

#include <cstdint>
#include <cstring>

#define CMP_MEM(POSA, POSB) ( POSA > POSB )

static constexpr unsigned short STACK_SIZE = static_cast<unsigned short>(65536);

enum Register {
    REG_A,
    REG_B,
    REG_C,
    REG_D,
    REG_E,
    REG_F,
    REG_H,
    REG_L,
    REG_I,
    REG_R,
    REG_IXH,
    REG_IXL,
    REG_IYH,
    REG_IYL,
    REG_AF,
    REG_BC,
    REG_DE,
    REG_HL,
    REG_IX,
    REG_IY,
    REG_SP,
    REG_PC
};

enum Flag {
    S_FLAG,
    Z_FLAG,
    F5_FLAG,
    H_FLAG,
    F3_FLAG,
    N_FLAG,
    C_FLAG
};

enum Mode : int {
    MODE_0 = 0,
    MODE_1 = 1,
    MODE_2 = 2
};

class CpuOpException : std::exception {
    const char* message;

    public:
        CpuOpException(const char* message) : message(message) {};
        virtual const char* what() const throw() override;
};

template<int8_t ID>
struct Port {
    static constexpr int8_t id = ID;
    int8_t data;
};

struct z80 {
    // Registros de 8 bits
    int8_t a, b, c, d, e, f, h, l, i, r, ixh, ixl, iyh, iyl;
    
    // Registros de 16 bits
    int16_t af, bc, de, hl, ix, iy, pc, sp;

    bool interrupts;

    Mode mode;

    // z80 ports

    /**
     * TODO: Añadir más puertos
     */
    Port<0x00> link;
    Port<0x01> keyboard;
    Port<0x02> statusPort;
    Port<0x03> interruptMask;
    Port<0x04> memoryMapInterrupt;
    Port<0x05> ramPage;
    Port<0x06> memoryPageA;
    Port<0x07> memoryPageB;
    Port<0x08> linkAssistEnable;

    std::array<int16_t, 65536> stack;

    z80(void);

    void AbsoluteJump(int16_t*) noexcept;
    void RelativeJump(int16_t*);

    void Compare(Register, Register) noexcept;

    void Return(void) noexcept;
    void Return(Flag) noexcept;

    void Move(Register, Register) noexcept;
    void Move(Register, int8_t) noexcept;

    void Ldd(void) noexcept;

    void Add(Register, Register) noexcept;
    void Add(Register, int8_t) noexcept;

    void Substraction(Register, Register) noexcept;
    void Substraction(Register, int8_t) noexcept;

    void Increment(Register) noexcept;

    void Decrement(Register) noexcept;

    void Exchange(Register, Register) noexcept;

    void And(Register, Register) noexcept;
    void And(Register, int8_t) noexcept;

    void Otdr(void) noexcept;

    /** TODO: Implementar más puertos */
    template<int8_t id>
    void Out(Port<id>, Register) noexcept;

    template<int8_t id>
    void In(Register, Port<id>) noexcept;

    void IMode(Mode) noexcept;

    void DisableInterrupts(void) noexcept;
    void EnableInterrupts(void) noexcept;

    void Or(Register, Register) noexcept;
    void Or(Register, int8_t) noexcept;

    void Xor(Register, Register) noexcept;
    void Xor(Register, int8_t) noexcept;

    void Push(Register) noexcept;
    void Pop(Register) noexcept;

private:
    std::tuple<int8_t*, int8_t*> GetCpuRegisters(Register, Register) noexcept;
    int8_t* GetCpuRegister(Register) noexcept;
};