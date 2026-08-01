#ifndef V8DATAMODEL_BRICKCOLOR_H
#define V8DATAMODEL_BRICKCOLOR_H

#include "decomp.h"

namespace RBX {

// SIZE 0x04
class BrickColor
{
public:
	enum Number
	{
		lego_1 = 1,
		lego_2 = 2,
		lego_3 = 3,
		lego_5 = 5,
		lego_6 = 6,
		lego_9 = 9,
		lego_11 = 11,
		lego_12 = 12,
		lego_18 = 18,
		lego_21 = 21,
		lego_22 = 22,
		lego_23 = 23,
		lego_24 = 24,
		lego_25 = 25,
		lego_26 = 26,
		lego_27 = 27,
		lego_28 = 28,
		lego_29 = 29,
		lego_36 = 36,
		lego_37 = 37,
		lego_38 = 38,
		lego_39 = 39,
		lego_40 = 40,
		lego_41 = 41,
		lego_42 = 42,
		lego_43 = 43,
		lego_44 = 44,
		lego_45 = 45,
		lego_47 = 47,
		lego_48 = 48,
		lego_49 = 49,
		lego_50 = 50,
		lego_100 = 100,
		lego_101 = 101,
		lego_102 = 102,
		lego_103 = 103,
		lego_104 = 104,
		lego_105 = 105,
		lego_106 = 106,
		lego_107 = 107,
		lego_108 = 108,
		lego_110 = 110,
		lego_111 = 111,
		lego_112 = 112,
		lego_113 = 113,
		lego_115 = 115,
		lego_116 = 116,
		lego_118 = 118,
		lego_119 = 119,
		lego_120 = 120,
		lego_121 = 121,
		lego_123 = 123,
		lego_124 = 124,
		lego_125 = 125,
		lego_126 = 126,
		lego_127 = 127,
		lego_128 = 128,
		lego_131 = 131,
		lego_133 = 133,
		lego_134 = 134,
		lego_135 = 135,
		lego_136 = 136,
		lego_137 = 137,
		lego_138 = 138,
		lego_140 = 140,
		lego_141 = 141,
		lego_143 = 143,
		lego_145 = 145,
		lego_146 = 146,
		lego_147 = 147,
		lego_148 = 148,
		lego_149 = 149,
		lego_150 = 150,
		lego_151 = 151,
		lego_153 = 153,
		lego_154 = 154,
		lego_157 = 157,
		lego_158 = 158,
		lego_168 = 168,
		lego_176 = 176,
		lego_178 = 178,
		lego_179 = 179,
		lego_180 = 180,
		lego_190 = 190,
		lego_191 = 191,
		lego_192 = 192,
		lego_193 = 193,
		lego_194 = 194,
		lego_195 = 195,
		lego_196 = 196,
		lego_198 = 198,
		lego_199 = 199,
		lego_200 = 200,
		lego_208 = 208,
		lego_209 = 209,
		lego_210 = 210,
		lego_211 = 211,
		lego_212 = 212,
		lego_213 = 213,
		lego_216 = 216,
		lego_217 = 217,
		lego_218 = 218,
		lego_219 = 219,
		lego_220 = 220,
		lego_221 = 221,
		lego_222 = 222,
		lego_223 = 223,
		lego_224 = 224,
		lego_225 = 225,
		lego_226 = 226,
		lego_232 = 232,
		lego_268 = 268
	};

	BrickColor() : number(lego_1) {}
	BrickColor(Number number) : number(number) {}

	Number number; // 0x00
};

DECOMP_SIZE_ASSERT(BrickColor, 0x04)

} // namespace RBX

#endif // V8DATAMODEL_BRICKCOLOR_H
