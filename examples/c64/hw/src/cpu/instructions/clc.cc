#include "clc.hh"

#include "../mos_6510.hh"
#include "instruction_step.hh"

namespace c64 {
namespace hw {
namespace cpu {
namespace instructions {
namespace clc {

harpoon::execution::instruction implied::factory(harpoon::execution::processing_unit *cpu) {
	return set_flag_factory<&mos_6510::set_flag_C, false>(cpu, MNEMONIC);
}

} // namespace clc
} // namespace instructions
} // namespace cpu
} // namespace hw
} // namespace c64
