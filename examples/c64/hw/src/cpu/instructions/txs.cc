#include "txs.hh"

#include "../mos_6510.hh"
#include "read_instruction.hh"

namespace c64 {
namespace hw {
namespace cpu {
namespace instructions {
namespace txs {

harpoon::execution::instruction implied::factory(harpoon::execution::processing_unit *cpu) {
	return transfer_register_factory<std::uint8_t, &mos_6510::get_X, &mos_6510::set_SP, false>(
	    cpu, MNEMONIC);
}

} // namespace txs
} // namespace instructions
} // namespace cpu
} // namespace hw
} // namespace c64
