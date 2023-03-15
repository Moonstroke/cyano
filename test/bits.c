/* SPDX-License-Identifier: CECILL-2.1 */
#include "bits.h"

#include "CUTE/cute.h"



CUTE_TestCase *case_bits;


void test_num_octets(void) {
	fputs("Testing num_octets(size)\n", stderr);
	static const size_t values[] = {
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
	static const size_t expected[] = {
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
	for (unsigned int i = 0; i < (sizeof values / sizeof values[0]); ++i) {
		fprintf(stderr, "num_octets(%zu): expected %zu, got %zu\n", values[i],
		        expected[i], num_octets(values[i]));
	}
	fputc('\n', stderr);
}

void test_get_bit(void) {
	fputs("Testing get_bit(arr, i)\n", stderr);
	/* binary: 1110 1111 0011 0101 0011 1010 0001 0011 */
	static const char bits[] = {
		0xef, 0x35, 0x3a, 0x13
	};
	static const int expected[] = {
		1, 1, 1, 0, 1, 1, 1, 1,
		0, 0, 1, 1, 0, 1, 0, 1,
		0, 0, 1, 1, 1, 0, 1, 0,
		0, 0, 0, 1, 0, 0, 1, 1
	};
	for(unsigned int i = 0; i < 8 * sizeof bits; ++i) {
		fprintf(stderr, "get_bit(%u): expected %d, got %d\n", i, expected[i],
		        get_bit(bits, i));
	}
	fputc('\n', stderr);
}

void test_set_bit(void) {
	fputs("Testing set_bit(arr, i, val)\n", stderr);
	static const char expected[] = {
		0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff,
		0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01, 0x00
	};
	char value = 0;
	for (unsigned int i = 0; i < 8; ++i) {
		fprintf(stderr, "set_bit(0x%02hhx, %u, %d): expected 0x%02hhx", value,
		        i, 1, expected[i]);
		set_bit(&value, i, 1);
		fprintf(stderr, ", got 0x%02hhx\n", value);
	}
	value = 0xff;
	for (unsigned int i = 0; i < 8; ++i) {
		fprintf(stderr, "set_bit(0x%02hhx, %u, %d): expected 0x%02hhx", value,
		        i, 0, expected[8 + i]);
		set_bit(&value, i, 0);
		fprintf(stderr, ", got 0x%02hhx\n", value);
	}
	fputc('\n', stderr);
}

void test_toggle_bit(void) {
	fputs("Testing toggle_bit(arr, i)\n", stderr);
	static const char expected[] = {
		0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff,
		0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01, 0x00
	};
	char value = 0;
	for (unsigned int i = 0; i < 8; ++i) {
		fprintf(stderr, "toggle_bit(0x%02hhx, %u): expected 0x%02hhx", value,
		        i, expected[i]);
		toggle_bit(&value, i);
		fprintf(stderr, ", got 0x%02hhx\n", value);
	}
	value = 0xff;
	for (unsigned int i = 0; i < 8; ++i) {
		fprintf(stderr, "toggle_bit(0x%02hhx, %u): expected 0x%02hhx",
		        value, i, expected[8 + i]);
		toggle_bit(&value, i);
		fprintf(stderr, ", got 0x%02hhx\n", value);
	}
	fputc('\n', stderr);
}

void test_copy_bits(void) {
	fputs("Testing copy_bits(src, src_offset, dest, dest_offset, length)\n",
	      stderr);
	static const char src[] = {
		0xca, 0xfe, 0xba, 0xbe, 0xde, 0xad, 0xbe, 0xef
		/* 1100 1010 1111 1110
		   1011 1010 1011 1110
		   1101 1110 1010 1101
		   1011 1110 1110 1111 */
	};
	char dest[sizeof src] = {0};
	static const size_t indices[] = {
		/* src_offset, dest_offset, length */
		0, 0, 0, /* Corner case: empty copy */
		3, 2, 6,
		8, 8, 16,
		24, 26, 9,
		37, 37, 3,
		40, 41, 5,
		46, 46, 12,
		60, 59, 4
	};
	static const char *expected[] = {
		"0000000000000000000000000000000000000000000000000000000000000000",
		"0001010100000000000000000000000000000000000000000000000000000000",
		"0001010111111110101110100000000000000000000000000000000000000000",
		"0001010111111110101110100010111110100000000000000000000000000000",
		"0001010111111110101110100010111110100110000000000000000000000000",
		"0001010111111110101110100010111110100110010101000000000000000000",
		"0001010111111110101110100010111110100110010101011011111011000000",
		"0001010111111110101110100010111110100110010101011011111011011110"
	};
	for (unsigned int i = 0; i < sizeof expected / sizeof *expected; ++i) {
		size_t src_offset = indices[3 * i];
		size_t dest_offset = indices[3 * i + 1];
		size_t length = indices[3 * i + 2];
		fprintf(stderr, "copy_bits(src, %zu, dest, %zu, %zu):\nexpected %s\n",
		        src_offset, dest_offset, length, expected[i]);
		copy_bits(src, src_offset, dest, dest_offset, length);
		fputs("got      ", stderr);
		print_bits(dest, 0, sizeof dest * 8, stderr);
		fputc('\n', stderr);
	}
	fputc('\n', stderr);
}

void build_case_bits(void) {
	case_bits = CUTE_newTestCase("Tests for bit manipulation functions", 5);
	CUTE_addCaseTest(case_bits, CUTE_makeTest(test_num_octets));
	CUTE_addCaseTest(case_bits, CUTE_makeTest(test_get_bit));
	CUTE_addCaseTest(case_bits, CUTE_makeTest(test_set_bit));
	CUTE_addCaseTest(case_bits, CUTE_makeTest(test_toggle_bit));
	CUTE_addCaseTest(case_bits, CUTE_makeTest(test_copy_bits));
}
