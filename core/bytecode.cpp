
#include "bytecode.h"

#include <set>
#include <algorithm>

namespace mary {

const ScrOpcodeClass gScrOpcodeClass[SCR_OPCODE_MAX] = {
	{ "nop",     0 },
	{ "equ",    -1 },
	{ "addequ", -1 },
	{ "subequ", -1 },
	{ "mulequ", -1 },
	{ "divequ", -1 },
	{ "modequ", -1 },
	{ "add",    -1 },
	{ "sub",    -1 },
	{ "mul",    -1 },
	{ "div",    -1 },
	{ "mod",    -1 },
	{ "and",    -1 },
	{ "or",     -1 },
	{ "inc",     0 },
	{ "dec",     0 },
	{ "neg",     0 },
	{ "not",     0 },
	{ "cmp",    -1 },
	{ "pushm",  +1, 4 },
	{ "popm",   -1, 4 },
	{ "dup",    +1 },
	{ "pop",    -1 },
	{ "push",   +1, 4 },
	{ "b",       0, 4, true,  true },
	{ "blt",    -1, 4, true },
	{ "ble",    -1, 4, true },
	{ "beq",    -1, 4, true },
	{ "bne",    -1, 4, true },
	{ "bge",    -1, 4, true },
	{ "bgt",    -1, 4, true },
	{ "bi",     -1, 0, true,  true },
	{ "end",     0, 0, false, true },
	{ "call",    0, 4 },
	{ "push16", +1, 2 },
	{ "push8",  +1, 1 },
	{ "switch", -1, 4 },
};

const ScrOpcodeClass gScrOpcodeErrClass = { "err?", 0, 0, false, true };

struct ScrInsDecoder
{
	constexpr ScrInsDecoder(Span<const byte_type> range)
		: range(range), it(range.begin()) {}

	bool at_end(void) const
	{
		return it == range.end();
	}

	ScrIns decode()
	{
		ScrIns result;

		// TODO: err if reaching past end

		result.offset = baseOffset + std::distance(range.begin(), it);
		result.opcode = *it++;

		if (result.opcode & 0x80)
		{
			result.opcode &= 0x7F;
			result.xUsed = true;
		}

		if (!result.is_valid())
		{
			using namespace std::string_literals;

			throw std::runtime_error(
				"failed to decode instruction at "s + std::to_string(result.offset)); // TODO: better error
		}

		for (unsigned i = 0; i < result.operand_size(); ++i)
		{
			result.operand += (*it++) << (i*8);
		}

		return result;
	}

	std::size_t baseOffset = 0;

protected:
	Span<const byte_type> range;
	Span<const byte_type>::const_iterator it;
};

std::vector<ScrIns> decode_script(Span<const byte_type> bytes)
{
	std::vector<ScrIns> result;

	ScrInsDecoder decoder(bytes);

	while (!decoder.at_end())
		result.push_back(decoder.decode());

	return result;
}

std::vector<byte_type> encode_script(Span<const ScrIns> script)
{
	std::vector<byte_type> result(get_script_length(script));
	auto baseOffset = get_script_offset(script);

	for (auto& ins : script)
	{
		auto index = ins.offset - baseOffset;

		result[index] = ins.opcode + (ins.xUsed ? 0x80 : 0);

		for (unsigned i = 0; i < ins.operand_size(); ++i)
			result[index + 1 + i] = 0xFF & (ins.operand >> (i*8));
	}

	return result;
}

ScrAnalysis analyse_script(Span<const ScrIns> script)
{
	ScrAnalysis result;

	std::set<std::size_t> slicePoints;

	// Step 1: Find slice points

	for (auto& ins : script)
	{
		if (ins.is_jump())
		{
			// jumps generate:
			// a slice after themselves
			// a slice before the jump target
			// a label before the jump target

			if (ins.has_operand())
			{
				slicePoints.insert(ins.operand);
				result.labels.set(ins.operand, make_label_name(ins.operand));
			}

			slicePoints.insert(ins.offset + ins.encoded_size());
		}
		else if (ins.is_end())
		{
			// ends generate slices after themselves

			slicePoints.insert(ins.offset + ins.encoded_size());
		}
	}

	// Step 2: Slice

	auto scrIt   = script.begin();
	auto sliceIt = slicePoints.begin();

	while (scrIt != script.end())
	{
		auto itStart = scrIt;

		if (sliceIt != slicePoints.end())
		{
			auto sliceOffset = *sliceIt++;

			scrIt = std::find_if(itStart, script.end(), [sliceOffset] (auto& ins)
			{
				return ins.offset >= sliceOffset;
			});
		}
		else
		{
			scrIt = script.end();
		}

		result.linearChunks.set(itStart->offset, { itStart, scrIt });
	}

	return result;
}

} // namespace mary
