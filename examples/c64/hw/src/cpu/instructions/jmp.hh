#ifndef CPU_INSTRUCTIONS_JMP_HH
#define CPU_INSTRUCTIONS_JMP_HH

#include "harpoon/execution/instruction.hh"

namespace c64 {
namespace hw {
namespace cpu {
namespace instructions {
namespace jmp {

static constexpr const char *MNEMONIC = "JMP";

struct absolute {
	static constexpr const std::uint8_t OPCODE = 0x4C;
	static harpoon::execution::instruction factory(harpoon::execution::processing_unit *cpu);
};

struct indirect {
	static constexpr const std::uint8_t OPCODE = 0x6C;
	static harpoon::execution::instruction factory(harpoon::execution::processing_unit *cpu);
};

} // namespace jmp
} // namespace instructions
} // namespace cpu
} // namespace hw
} // namespace c64

#endif
