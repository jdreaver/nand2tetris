#pragma once

/*
 * Enumeration of possible errors when parsing ASM.
 */
enum asm_parse_error {
	ASM_PARSE_ERROR_NO_ERROR,
	ASM_PARSE_ERROR_UNEXPECTED_EOF,
	ASM_PARSE_ERROR_EXTRA_INPUT,

	ASM_PARSE_ERROR_A_INSTRUCTION_MISSING_AT_SYMBOL,
	ASM_PARSE_ERROR_A_INSTRUCTION_ADDRESS_TOO_LARGE,
};

// TODO: Function to pretty print each type of asm_parse_error
