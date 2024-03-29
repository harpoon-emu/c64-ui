#ifndef CPU_INSTRUCTIONS_BEQ_HH
#define CPU_INSTRUCTIONS_BEQ_HH

#include "harpoon/execution/instruction.hh"

namespace c64 {
namespace hw {
namespace cpu {
namespace instructions {
namespace beq {

static constexpr const char *MNEMONIC = "BEQ";

struct relative {
	static constexpr const std::uint8_t OPCODE = 0xF0;
	static harpoon::execution::instruction factory(harpoon::execution::processing_unit *cpu);
};

} // namespace beq
} // namespace instructions
} // namespace cpu
} // namespace hw
} // namespace c64

#endif
