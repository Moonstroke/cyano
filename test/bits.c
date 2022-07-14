#include "bits.h"

#include <stdbool.h>



static void _test_NUM_OCTETS(void) {
	static const unsigned int values[] = {
		-1,
		0,
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		8,
		9,
		127,
		257,
		0x70000000,
		0x7fffffff,
		0xffffffff
	};
	static const unsigned int expected[] = {
		0,
		0,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		2,
		16,
		33,
		0x0e000000,
		0x10000000,
		0x20000000
	};
	for (unsigned int i = 0; i < (sizeof values / sizeof *values); ++i) {
		fprintf(stderr, "NUM_OCTETS(%u): expected %u, got %u\n", values[i],
		        expected[i], NUM_OCTETS(values[i]));
	}
}

static void _test_GET_BIT(void) {
	// binary: 1110 1111 0011 0101 0011 1010 0001 0011
	static const unsigned char bits[] = {
		0xef, 0x35, 0x3a, 0x13
	};
	static const bool expected[] = {
		true, true, true, false, true, true, true, true,
		false, false, true, true, false, true, false, true,
		false, false, true, true, true, false, true, false,
		false, false, false, true, false, false, true, true
	};
	for(unsigned int i = 0; i < 8 * sizeof bits; ++i) {
		fprintf(stderr, "GET_BIT(%u): expected %d, got %d\n", i, expected[i],
		        GET_BIT(bits, i));
	}
}

static void _test_SET_BIT(void) {
	// TODO
}

static void _test_TOGGLE_BIT(void) {
	// TODO
}

static void _test_copy_bits(void) {
	// TODO
}

void tests_bits(void) {
	fputs("Testing NUM_OCTETS(size)\n", stderr);
	_test_NUM_OCTETS();
	fputc('\n', stderr);

	fputs("Testing GET_BIT(arr, i)\n", stderr);
	_test_GET_BIT();
	fputc('\n', stderr);

	fputs("Testing SET_BIT(arr, i, val)\n", stderr);
	_test_SET_BIT();
	fputc('\n', stderr);

	fputs("Testing TOGGLE_BIT(arr, i)\n", stderr);
	_test_TOGGLE_BIT();
	fputc('\n', stderr);

	fputs("Testing copy_bits(src, src_offset, dest, dest_offset, length)\n",
	      stderr);
	_test_copy_bits();
	fputc('\n', stderr);
}
