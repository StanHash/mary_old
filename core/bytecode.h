#ifndef MARY_SCRIPT_BYTECODE_H
#define MARY_SCRIPT_BYTECODE_H

#include "core/types.h"

#include <string>
#include <vector>

namespace mary {

enum
{
	// Script opcode constants

	SCR_OPCODE_NOP    = 0x00,
	SCR_OPCODE_EQU    = 0x01,
	SCR_OPCODE_ADDEQU = 0x02,
	SCR_OPCODE_SUBEQU = 0x03,
	SCR_OPCODE_MULEQU = 0x04,
	SCR_OPCODE_DIVEQU = 0x05,
	SCR_OPCODE_MODEQU = 0x06,
	SCR_OPCODE_ADD    = 0x07,
	SCR_OPCODE_SUB    = 0x08,
	SCR_OPCODE_MUL    = 0x09,
	SCR_OPCODE_DIV    = 0x0A,
	SCR_OPCODE_MOD    = 0x0B,
	SCR_OPCODE_AND    = 0x0C,
	SCR_OPCODE_OR     = 0x0D,
	SCR_OPCODE_INC    = 0x0E,
	SCR_OPCODE_DEC    = 0x0F,
	SCR_OPCODE_NEG    = 0x10,
	SCR_OPCODE_NOT    = 0x11,
	SCR_OPCODE_CMP    = 0x12,
	SCR_OPCODE_PUSHM  = 0x13,
	SCR_OPCODE_EQUIMM = 0x14,
	SCR_OPCODE_DUP    = 0x15,
	SCR_OPCODE_POP    = 0x16,
	SCR_OPCODE_PUSH   = 0x17,
	SCR_OPCODE_B      = 0x18,
	SCR_OPCODE_BLT    = 0x19,
	SCR_OPCODE_BLE    = 0x1A,
	SCR_OPCODE_BEQ    = 0x1B,
	SCR_OPCODE_BNE    = 0x1C,
	SCR_OPCODE_BGE    = 0x1D,
	SCR_OPCODE_BGT    = 0x1E,
	SCR_OPCODE_BI     = 0x1F,
	SCR_OPCODE_END    = 0x20,
	SCR_OPCODE_CALL   = 0x21,
	SCR_OPCODE_PUSH16 = 0x22,
	SCR_OPCODE_PUSH8  = 0x23,
	SCR_OPCODE_SWITCH = 0x24,

	SCR_OPCODE_MAX,
};

struct ScrOpcodeInfo
{
	std::string mnemonic;

	int spDiff = 0;
	unsigned operandSize = 0;

	bool isJump = false;
	bool isEnd = false;
};

extern const ScrOpcodeInfo gScrOpcodeInfo[SCR_OPCODE_MAX];
extern const ScrOpcodeInfo gScrOpcodeErrInfo;

inline
const ScrOpcodeInfo& get_opcode_info(unsigned opcodeClass)
{
	return opcodeClass < SCR_OPCODE_MAX ? gScrOpcodeInfo[opcodeClass] : gScrOpcodeErrInfo;
}

struct ScrIns
{
	inline
	bool is_valid() const
	{
		return opcode < SCR_OPCODE_MAX;
	}

	inline
	const std::string& mnemonic() const
	{
		return get_opcode_info(opcode).mnemonic;
	}

	inline
	unsigned operand_size() const
	{
		return get_opcode_info(opcode).operandSize;
	}

	inline
	bool is_jump() const
	{
		return get_opcode_info(opcode).isJump;
	}

	inline
	bool is_end() const
	{
		return get_opcode_info(opcode).isEnd;
	}

	inline
	bool has_operand() const
	{
		return operand_size();
	}

	inline
	unsigned encoded_size() const
	{
		return 1 + operand_size();
	}

	std::size_t offset = 0;
	std::uint32_t operand = 0;
	std::uint8_t opcode = 0;
	bool xUsed = false; // TODO: figure out
};

std::vector<ScrIns> decode_script(const byte_type* begin, const byte_type* end);
std::vector<byte_type> encode_script(const std::vector<ScrIns>& script);

std::vector<std::vector<ScrIns>> slice_script(const std::vector<ScrIns>& script); // TODO: maybe generate a more detailed analysis?

inline
unsigned get_script_offset(const std::vector<ScrIns>& script)
{
	return script.empty() ? 0 : script.front().offset;
}

inline
unsigned get_script_length(const std::vector<ScrIns>& script)
{
	return script.empty() ? 0 : (script.back().offset + script.back().encoded_size() - script.front().offset);
}

} // namespace mary

#endif // MARY_SCRIPT_BYTECODE_H
