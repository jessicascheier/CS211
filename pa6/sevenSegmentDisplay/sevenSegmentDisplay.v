input [3:0] numeral_bit;

output a;
output b;
output c;
output d;
output e;
output f;
output g;

wire not_numeral_bit_3;
wire not_numeral_bit_2;
wire not_numeral_bit_1;
wire not_numeral_bit_0;

wire maxterm_01;
wire a_3_2;
wire a_1_0;
wire not_maxterm_01;
wire aa_3_2;
wire aa_1_0;
wire maxterm_04;
wire not_maxterm_04;

wire b_3_2;
wire b_1_0;
wire bb_3_2;
wire bb_1_0;
wire b_maxterm_05;
wire not_b_maxterm_05;
wire not_b_maxterm_06;
wire b_maxterm_06;

wire maxterm_02;
wire not_maxterm_02;
wire c_3_2;
wire c_1_0;

wire d_3_2;
wire d_1_0;
wire d_maxterm_07;
wire maxterm_01_and_04;
wire not_d_maxterm_07;

wire minterm_02_and_06;
wire minterm_08_and_00;
wire not_3_and_not_2;
wire not_1_and_not_0;

wire f_3_2;
wire f_1_0;
wire f_maxterm_03;
wire not_f_maxterm_03;
wire maxterm_01_and_02;
wire maxterm_03_and_07;

wire g_3_2;
wire g_1_0;
wire g_maxterm_00;
wire not_g_maxterm_00;
wire maxterm_01_and_07;

assign not_numeral_bit_3 = ~ numeral_bit[3];
assign not_numeral_bit_2 = ~ numeral_bit[2];
assign not_numeral_bit_1 = ~ numeral_bit[1];
assign not_numeral_bit_0 = ~ numeral_bit[0];

assign a_3_2 = not_numeral_bit_3 & not_numeral_bit_2; // opposite of binary for 1 (maxterm)
assign a_1_0 = not_numeral_bit_1 & numeral_bit[0];
assign maxterm_01 = a_3_2 & a_1_0;
assign not_maxterm_01 = ~ maxterm_01;
assign aa_3_2 = not_numeral_bit_3 & numeral_bit[2]; // opposite of binary for 4 (maxterm)
assign aa_1_0 = not_numeral_bit_1 & not_numeral_bit_0;
assign maxterm_04 = aa_3_2 & aa_1_0;
assign not_maxterm_04 = ~ maxterm_04;
assign a = not_maxterm_01 & not_maxterm_04;

assign b_3_2 = not_numeral_bit_3 & numeral_bit[2]; // opposite binary for 5 (maxterm)
assign b_1_0 = not_numeral_bit_1 & numeral_bit[0];
assign b_maxterm_05 = b_3_2 & b_1_0;
assign not_b_maxterm_05 = ~ b_maxterm_05;
assign bb_3_2 = not_numeral_bit_3 & numeral_bit[2]; // opposite of binary for 6 (maxterm)
assign bb_1_0 = numeral_bit[1] & not_numeral_bit_0; // note: maxterms are the numbers that make segment evaluate to 0,
assign b_maxterm_06 = bb_3_2 & bb_1_0; //              so if it's everything ELSE, it's a 1,
assign not_b_maxterm_06 = ~ b_maxterm_06; //            that's why you NOT them both and & them at the end bc they both can't happen
assign b = not_b_maxterm_05 & not_b_maxterm_06;

assign c_3_2 = not_numeral_bit_3 & not_numeral_bit_2;
assign c_1_0 = numeral_bit[1] & not_numeral_bit_0;
assign maxterm_02 = c_3_2 & c_1_0;
assign not_maxterm_02 = ~ maxterm_02;
assign c = not_maxterm_02;

assign d_3_2 = not_numeral_bit_3 & numeral_bit[2];
assign d_1_0 = numeral_bit[1] & numeral_bit[0];
assign d_maxterm_07 = d_3_2 & d_1_0;
assign not_d_maxterm_07 = ~ d_maxterm_07;
assign maxterm_01_and_04 = not_maxterm_01 & not_maxterm_04;
assign d = maxterm_01_and_04 & not_d_maxterm_07;

assign not_3_and_not_2 = not_numeral_bit_3 & not_numeral_bit_2;
assign not_1_and_not_0 = not_numeral_bit_1 & not_numeral_bit_0;
assign minterm_02_and_06 = not_numeral_bit_0 & numeral_bit[1];
assign minterm_08_and_00 = not_numeral_bit_2 & not_1_and_not_0; // note: minterms are the opposite of maxterms,
assign e = minterm_02_and_06 | minterm_08_and_00; // they are the only terms that make segment evaluate to 1

assign f_3_2 = not_numeral_bit_3 & not_numeral_bit_2;
assign f_1_0 = numeral_bit[1] & numeral_bit[0];
assign f_maxterm_03 = f_3_2 & f_1_0;
assign not_f_maxterm_03 = ~ f_maxterm_03;
assign maxterm_01_and_02 = not_maxterm_01 & not_maxterm_02;
assign maxterm_03_and_07 = not_f_maxterm_03 & not_d_maxterm_07;
assign f = maxterm_01_and_02 & maxterm_03_and_07;

assign g_3_2 = not_numeral_bit_3 & not_numeral_bit_2;
assign g_1_0 = not_numeral_bit_1 & not_numeral_bit_0;
assign g_maxterm_00 = g_3_2 & g_1_0;
assign not_g_maxterm_00 = ~ g_maxterm_00;
assign maxterm_01_and_07 = not_maxterm_01 & not_d_maxterm_07;
assign g = maxterm_01_and_07 & not_g_maxterm_00;