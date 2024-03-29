#ifndef CPU_INSTRUCTIONS_CPY_HH
#define CPU_INSTRUCTIONS_CPY_HH

#include "harpoon/execution/instruction.hh"

namespace c64 {
namespace hw {
namespace cpu {
namespace instructions {
namespace cpy {

static constexpr const char *MNEMONIC = "CPY";

struct immediate {
	static constexpr const std::uint8_t OPCODE = 0xC0;
	static harpoon::execution::instruction factory(harpoon::execution::processing_unit *cpu);
};

struct zero_page {
	static constexpr const std::uint8_t OPCODE = 0xC4;
	static harpoon::execution::instruction factory(harpoon::execution::processing_unit *cpu);
};

struct absolute {
	static constexpr const std::uint8_t OPCODE = 0xCC;
	static harpoon::execution::instruction factory(harpoon::execution::processing_unit *cpu);
};

} // namespace cpy
} // namespace instructions
} // namespace cpu
} // namespace hw
} // namespace c64

#endif
