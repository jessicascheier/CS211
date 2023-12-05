input [3:0] numeral_bit;

output e;

wire not_numeral_bit_3;
wire not_numeral_bit_2;
wire not_numeral_bit_1;
wire not_numeral_bit_0;

wire minterm_02_and_06;
wire minterm_08_and_00;

wire not_3_and_not_2;
wire not_1_and_not_0;

assign not_numeral_bit_3 = ~ numeral_bit[3];
assign not_numeral_bit_2 = ~ numeral_bit[2];
assign not_numeral_bit_1 = ~ numeral_bit[1];
assign not_numeral_bit_0 = ~ numeral_bit[0];

assign not_3_and_not_2 = not_numeral_bit_3 & not_numeral_bit_2;
assign not_1_and_not_0 = not_numeral_bit_1 & not_numeral_bit_0;

assign minterm_02_and_06 = not_numeral_bit_0 & numeral_bit[1];
assign minterm_08_and_00 = not_numeral_bit_2 & not_1_and_not_0;

assign e = minterm_02_and_06 | minterm_08_and_00;
