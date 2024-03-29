#ifndef CPU_INSTRUCTIONS_BVS_HH
#define CPU_INSTRUCTIONS_BVS_HH

#include "harpoon/execution/instruction.hh"

namespace c64 {
namespace hw {
namespace cpu {
namespace instructions {
namespace bvs {

static constexpr const char *MNEMONIC = "BVS";

struct relative {
	static constexpr const std::uint8_t OPCODE = 0x70;
	static harpoon::execution::instruction factory(harpoon::execution::processing_unit *cpu);
};

} // namespace bvs
} // namespace instructions
} // namespace cpu
} // namespace hw
} // namespace c64

#endif
