/*****************************
*                            *
*  Genesis Ian M. Palaganas  *
*  2012-32936                *
*  CS 153                    *
*  MP 1                      *
*                            *
*****************************/

#include <stdio.h>
#include <stdlib.h>

#define BLOCK_ROWS 8
#define BLOCK_COLS 8
#define E_BLOCK_COLS 6
#define P_BLOCK_COLS 4
#define PC_BLOCK_COLS 7
#define S_ROWS 4
#define S_COLS 16
#define ITERATIONS 16

//Initialization Functions
void init_ip_block_mapping(int IP[BLOCK_ROWS][BLOCK_COLS], int IP1[BLOCK_ROWS][BLOCK_COLS]);
void init_cipher_block_mapping(int E[BLOCK_ROWS][E_BLOCK_COLS], int P[BLOCK_ROWS][P_BLOCK_COLS],
			int S[BLOCK_ROWS][S_ROWS][S_COLS]);
void init_key_block_mapping(int PC1[BLOCK_ROWS][PC_BLOCK_COLS], int PC2[BLOCK_ROWS][E_BLOCK_COLS]);
void init_s_functions(int S[BLOCK_ROWS][S_ROWS][S_COLS]);
void init_IP(int IP[BLOCK_ROWS][BLOCK_COLS]);
void init_IP1(int IP1[BLOCK_ROWS][BLOCK_COLS]);
void init_E(int E[BLOCK_ROWS][E_BLOCK_COLS]);
void init_P(int P[BLOCK_ROWS][P_BLOCK_COLS]);
void init_PC1(int PC1[BLOCK_ROWS][PC_BLOCK_COLS]);
void init_PC2(int PC2[BLOCK_ROWS][E_BLOCK_COLS]);
void init_SN(int SN[S_ROWS][S_COLS], int n);

//Math Utility Functions
int power(int n, int k);

//String Utility Functions
int strcmp(char *str1, char *str2);

//Conversion Utility Functions
void to_bin_array(int n, int rows, int arr[rows]);
int to_int(int cols, int arr[cols]);

//Block Utility Functions
void to_bin_block(int rows, int cols,int init_array[rows], int block[rows][cols] );
void to_int_array(int rows, int cols, int block[rows][cols], int output[rows]);
void assign_block(int rows, int cols, int o_rows, int o_cols, 
		int orig_block[o_rows][o_cols], int block_mapping[rows][cols], int new_block[rows][cols]);
void split_block_by_row(int rows, int cols, int input[rows][cols], int a[rows/2][cols], int b[rows/2][cols]);
void split_block_by_col(int rows, int cols, int input[rows][cols], int a[rows][cols/2], int b[rows][cols/2]);
void merge_blocks_by_row(int rows, int cols, int output[rows][cols], int a[rows/2][cols], int b[rows/2][cols]);
void merge_blocks_by_col(int rows, int cols, int output[rows][cols], int a[rows][cols/2], int b[rows][cols/2]);
void copy_block_content(int rows, int cols, int block1[rows][cols], int block2[rows][cols]);
void xor_blocks(int rows, int cols, int input1[rows][cols], int input2[rows][cols], int output[rows][cols]);
void left_shift_block(int n, int rows, int cols, int input[rows][cols], int output[rows][cols]);
int even_parity_check_block(int rows, int cols, int input[rows][cols]);
int odd_parity_check_block(int rows, int cols, int input[rows][cols]);


//Encryption Related Functions
void s_function(int in_cols, int out_cols, int s_rows, int s_cols,
		int input[in_cols], int output[out_cols], int S[s_rows][s_cols]);
void ks_function(int n, int rows, int cols, int out_rows, int out_cols, int pc_rows, int pc_cols, int pc2_cols, 
		int key[rows][cols], int out[out_rows][out_cols], int PC1[pc_rows][pc_cols], int PC2[pc_rows][pc2_cols]);
void cipher_function(int rows, int cols,int k_rows, int k_cols, int s_rows, int s_cols, int p_cols,
		int r_block[rows][cols], int key[k_rows][k_cols], int output[rows][cols], 
		int E[rows][k_cols], int S[cols][s_rows][s_cols], int P[k_rows][p_cols]);

//DES Encryption Function
void encrypt(int n, int rows, int cols, int input[rows][cols], int output[rows][cols], int key[rows][cols],
		int IP[BLOCK_ROWS][BLOCK_COLS], int IP1[BLOCK_ROWS][BLOCK_COLS],
		int E[BLOCK_ROWS][E_BLOCK_COLS], int P[BLOCK_ROWS][P_BLOCK_COLS],int S[BLOCK_ROWS][S_ROWS][S_COLS],
		int PC1[BLOCK_ROWS][PC_BLOCK_COLS], int PC2[BLOCK_ROWS][E_BLOCK_COLS]
		);

//DES Decryption Function
void decrypt(int n, int rows, int cols, int input[rows][cols], int output[rows][cols], int key[rows][cols],
		int IP[BLOCK_ROWS][BLOCK_COLS], int IP1[BLOCK_ROWS][BLOCK_COLS],
		int E[BLOCK_ROWS][E_BLOCK_COLS], int P[BLOCK_ROWS][P_BLOCK_COLS],int S[BLOCK_ROWS][S_ROWS][S_COLS],
		int PC1[BLOCK_ROWS][PC_BLOCK_COLS], int PC2[BLOCK_ROWS][E_BLOCK_COLS]
		);

//Debugging Functions
void print_2D(int rows, int cols,int arr[rows][cols]);
void print_2D_to_file(char *file_name, char *msg ,int rows, int cols,int arr[rows][cols]);
void print_to_file(char *file_name, char *msg, int rows, int cols, int arr[rows][cols], int n);

/*<=====================================================MAIN=====================================================>*/

int main(){
	int IP[BLOCK_ROWS][BLOCK_COLS];
	int IP1[BLOCK_ROWS][BLOCK_COLS];
	int E[BLOCK_ROWS][E_BLOCK_COLS];
	int P[BLOCK_ROWS][P_BLOCK_COLS];
	int PC1[BLOCK_ROWS][PC_BLOCK_COLS];
	int PC2[BLOCK_ROWS][E_BLOCK_COLS];
	int S[BLOCK_COLS][S_ROWS][S_COLS];

	int key_block[BLOCK_ROWS][BLOCK_COLS];
	int key[BLOCK_ROWS];

	int msg_block[BLOCK_ROWS][BLOCK_COLS];
	int msg[BLOCK_ROWS];
	
	int o_msg_block[BLOCK_ROWS][BLOCK_COLS];
	int o_msg[BLOCK_ROWS];

	int n;

	int i,j;

	FILE *out = fopen("201232936.txt", "w");
	FILE *in = fopen("input.txt","r");

	char operation[7];

	init_ip_block_mapping(IP,IP1);
	init_cipher_block_mapping(E,P,S);
	init_key_block_mapping(PC1,PC2);

	j = 1;

	//I got lazy
	while(fscanf(in, "%d %s {%d,%d,%d,%d,%d,%d,%d,%d}{%d,%d,%d,%d,%d,%d,%d,%d}", &n, operation, 
		&msg[0], &msg[1], &msg[2], &msg[3], &msg[4], &msg[5], &msg[6], &msg[7],
		&key[0], &key[1], &key[2], &key[3], &key[4], &key[5], &key[6], &key[7]
		) != EOF){

		to_bin_block(BLOCK_ROWS, BLOCK_COLS, key, key_block);
		to_bin_block(BLOCK_ROWS, BLOCK_COLS, msg, msg_block);

		if(!odd_parity_check_block(BLOCK_ROWS, BLOCK_COLS, key_block)){
			printf("Odd Parity Check Failed For %d!\n",j);
			fprintf(out,"%d\n",j);
			goto skip;
		}

		if(strcmp("encrypt",operation) == 0){
			encrypt(ITERATIONS, BLOCK_ROWS, BLOCK_COLS, msg_block, o_msg_block, key_block, IP, IP1, E, P, S, PC1, PC2);	
			to_int_array(BLOCK_ROWS, BLOCK_COLS, o_msg_block, o_msg);
		}
		else if(strcmp("decrypt",operation) == 0){
			decrypt(ITERATIONS, BLOCK_ROWS, BLOCK_COLS, o_msg_block, o_msg_block, key_block, IP, IP1, E, P, S, PC1, PC2);			
			to_int_array(BLOCK_ROWS, BLOCK_COLS, o_msg_blocprink, o_msg);

		}
		else{
			printf("Invalid Operation %s\n",operation);
			return 1;
		}

		fprintf(out,"%d ",j);
		for(i = 0; i < BLOCK_ROWS; i++){
			fprintf(out,"%d",o_msg[i]);
			if(i != BLOCK_ROWS - 1)
				fprintf(out,",");
			else
				fprintf(out,"\n");
		}

skip:
		j++;
	}
	
	fclose(in);
	fclose(out);


	return 0;

}

/*<===================================================END  MAIN===================================================>*/


//Initialize IP Mappings for blocks
void init_ip_block_mapping(int IP[BLOCK_ROWS][BLOCK_COLS], int IP1[BLOCK_ROWS][BLOCK_COLS]){

	init_IP(IP);
	init_IP1(IP1);
	

}

//Initialize Mappings involved with Cipher Function
void init_cipher_block_mapping(int E[BLOCK_ROWS][E_BLOCK_COLS], int P[BLOCK_ROWS][P_BLOCK_COLS],
		int S[BLOCK_ROWS][S_ROWS][S_COLS]){

	init_E(E);
	init_P(P);
	init_s_functions(S);

}

//Initialize Mappings involved with Key Schedule
void init_key_block_mapping(int PC1[BLOCK_ROWS][PC_BLOCK_COLS], int PC2[BLOCK_ROWS][E_BLOCK_COLS]){
	
	init_PC1(PC1);
	init_PC2(PC2);

}

//Initialize S Functions
void init_s_functions(int S[BLOCK_ROWS][S_ROWS][S_COLS]){

	int i;

	for(i = 1; i <= BLOCK_ROWS; i++)
		init_SN(S[i - 1], i);

}

//Initialize Initial Permutation Mapping
void init_IP(int IP[BLOCK_ROWS][BLOCK_COLS]){

	int ip_val = 58;
	int i, j;

	for(i = 0; i < BLOCK_ROWS; i++){
	
		if(i == 4)
			ip_val = 57;

		int ip_holder = ip_val;

		for(j = 0; j < BLOCK_COLS; j++){
			
			IP[i][j] = ip_holder;
			ip_holder -= 8;
		
		}

		ip_val += 2;
	}

}

//Initiliaze Inverse of Initial Permutation Mapping
void init_IP1(int IP1[BLOCK_ROWS][BLOCK_COLS]){

	int ip1_vals[BLOCK_COLS] = {40, 8, 48, 16, 56, 24, 64, 32};
	int i,j;

	for(i = 0; i < BLOCK_ROWS; i++){
		for(j = 0; j < BLOCK_COLS; j++){
			IP1[i][j] = ip1_vals[j] - i;
		}
	}

}

//Initialize E Mapping
void init_E(int E[BLOCK_ROWS][E_BLOCK_COLS]){

	int e_val = 0;
	int i,j;

	for(i = 0; i < BLOCK_ROWS; i++){
		 
		 int e_holder = e_val + (4 * i);
		 
		 for(j = 0; j < E_BLOCK_COLS; j++){
		 	E[i][j] = (e_holder % 32 == 0)?32:e_holder % 32;
		 	e_holder++;
		 }
	}

}

//Initialize P Mapping
void init_P(int P[BLOCK_ROWS][P_BLOCK_COLS]){

	int i,j;

	int holder[BLOCK_ROWS][P_BLOCK_COLS] = { 
			{ 16,  7, 20, 21 }, 
			{ 29, 12, 28, 17 }, 
			{  1, 15, 23, 26 },
			{  5, 18, 31, 10 },
			{  2,  8, 24, 14 },
			{ 32, 27,  3,  9 },
			{ 19, 13, 30,  6 },
			{ 22, 11,  4, 25 } 
		};

	copy_block_content(BLOCK_ROWS,P_BLOCK_COLS,P,holder);

}

//Initialize Permutation Choice 1 Mapping
void init_PC1(int PC1[BLOCK_ROWS][PC_BLOCK_COLS]){

	int val = 57;
	int i,j;

	for(i = 0; i < BLOCK_ROWS; i++){
		
		for(j = 0; j < PC_BLOCK_COLS; j++){
			PC1[i][j] = val;
			if(val == 7)
				val = 62;
			else if(val == 6)
				val = 61;
			else if(val == 5)
				val = 28;
			else if( val - 8 < 0)
				val = 64 + (val - 7) ;
			else
				val -= 8;
		}

		if(i == 3)
			val = 63;
	}

	//print_2D_to_file("PC1.txt", "PC1", BLOCK_ROWS, PC_BLOCK_COLS, PC1);

}

//Initialize Permutation Choice 2 Mapping
void init_PC2(int PC2[BLOCK_ROWS][E_BLOCK_COLS]){

	int i,j;


	int holder[BLOCK_ROWS][E_BLOCK_COLS] = {
		{ 14, 17, 11, 24,  1,  5 },
		{  3, 28, 15,  6, 21, 10 },
		{ 23, 19, 12,  4, 26,  8 },
		{ 16,  7, 27, 20, 13,  2 },
		{ 41, 52, 31, 37, 47, 55 },
		{ 30, 40, 51, 45, 33, 48 },
		{ 44, 49, 39, 56, 34, 53 },
		{ 46, 42, 50, 36, 29, 32 }
	};

	copy_block_content(BLOCK_ROWS,E_BLOCK_COLS,PC2,holder);

}

//Initilize Sn function
void init_SN(int S[S_ROWS][S_COLS], int n){


	int i,j;
	
	int holder[BLOCK_ROWS][S_ROWS][S_COLS] = {
		{
			{ 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,   7 }, 
			{  0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,   8 }, 
			{  4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,   0 },
			{ 15,  12, 8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6,  13 }
		},

		{
			{ 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10 },
			{  3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5 },
			{  0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15 },
			{ 13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9 }

		},

		{
			{ 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8 },
			{ 13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1 },
			{ 13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7 },
			{  1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12 }
		},

		{
 			{  7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15 },
			{ 13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9 },
			{ 10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4 },
			{  3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14 }
		},

		{
			{  2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9 },
			{ 14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6 },
			{  4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14 },
			{ 11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3 }			
		},

		{
			{ 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11 },
			{ 10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8 },
			{  9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6 },
			{  4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13 }
		},

		{
			{  4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1 },
			{ 13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6 },
			{  1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2 },
			{  6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12 }
		},

		{
			{ 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7 },
			{  1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2 },
			{  7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8 },
			{  2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11 }	
		}

	};

	copy_block_content(S_ROWS,S_COLS,S,holder[n-1]);

}

//Compute for the kth power of the number n
int power(int n, int k){
	if(k == 0)
		return 1;
	else
		return power(n, k-1) * n;
}

//Compare 2 strings
int strcmp(char *str1, char *str2){

	if(*str1 > *str2)
		return 1;
	else if(*str1 < *str2)
		return -1;
	else
		return 0;
}

//Convert an integer n into binary and store each bit to an array arr
void to_bin_array(int n, int rows, int arr[rows]){

	int i = 1, j = 0;

	for(j = 0; j < rows; j++)
		arr[j] = 0;

	while(n != 0){
		arr[rows - i] = n % 2;
		n /= 2;
		i++;
	}

}

//Convert a binary array arr to an integer
int to_int(int cols, int arr[cols]){

	int i, ret = 0;

	for(i = 1; i <= cols; i++){
		ret += power(2,cols-i) * arr[i-1];
	}
	

	return ret;

}

//For each integer in init_array convert to binary then store to an 2D array block
void to_bin_block(int rows, int cols,int init_array[rows], int block[rows][cols]){

	int i;

	for(i = 0; i < rows; i++)
		to_bin_array(init_array[i],rows,block[i]);

}

//For each byte in block convert to int then store in output
void to_int_array(int rows, int cols, int block[rows][cols], int output[rows]){

	int i;

	for(i = 0; i < rows; i++)
		output[i] = to_int(cols, block[i]);

}

//Using an assigned mapping block_mapping and an input block orig_block make a new block new_block
void assign_block(int rows, int cols, int o_rows, int o_cols, 
		int orig_block[o_rows][o_cols], int block_mapping[rows][cols], int new_block[rows][cols]){

	int i, j;
	int y_ind, x_ind, mapping;		

	for(i = 0; i < rows; i++)
		for(j = 0; j < cols; j++){
			mapping = block_mapping[i][j] - 1;
			y_ind = mapping / o_cols;
			x_ind = mapping % o_cols;
			new_block[i][j] = orig_block[y_ind][x_ind];
		}


}

//Split block to 2 arrays a and b row-wise
void split_block_by_row(int rows, int cols, int input[rows][cols], int a[rows/2][cols], int b[rows/2][cols]){

	int new_row = rows/2;
	int i,j;

	for(i = 0; i < new_row; i++)
		for(j = 0; j < cols; j++)
			a[i][j] = input[i][j];

	for(i = 0; i < new_row; i++)
		for(j = 0; j < cols; j++)
			b[i][j] = input[new_row + i][j];

}

//Split input to 2 blocks a and b col-wise
void split_block_by_col(int rows, int cols, int input[rows][cols], int a[rows][cols/2], int b[rows][cols/2]){

	int new_col = cols/2;
	int i,j;

	for(i = 0; i < rows; i++)
		for(j = 0; j < new_col; j++)
			a[i][j] = input[i][j];

	for(i = 0; i < rows; i++)
		for(j = 0; j < new_col; j++)
			b[i][j] = input[i][new_col + j];


}

//Merge 2 blocks a and b into output row-wise
void merge_blocks_by_row(int rows, int cols, int output[rows][cols], int a[rows/2][cols], int b[rows/2][cols]){
	
	int new_row = rows/2;
	int i,j;

	for(i = 0; i < new_row; i++)
		for(j = 0; j < cols; j++)
			output[i][j] = a[i][j];

	for(i = 0; i < new_row; i++)
		for(j = 0; j < cols; j++)
			output[new_row + i][j] = b[i][j];

}

//Merge 2 blocks a and b into output col-wise
void merge_blocks_by_col(int rows, int cols, int output[rows][cols], int a[rows][cols/2], int b[rows][cols/2]){
	
	int new_col = cols/2;
	int i,j;

	for(i = 0; i < rows; i++)
		for(j = 0; j < new_col; j++)
			output[i][j] = a[i][j];

	for(i = 0; i < rows; i++)
		for(j = 0; j < new_col; j++)
			output[i][new_col + j] = b[i][j];

}


//Copy the contents of block2 into block1
void copy_block_content(int rows, int cols, int block1[rows][cols], int block2[rows][cols]){
	int i,j;
	for(i = 0; i < rows; i++)
		for(j = 0; j < cols; j++)
			block1[i][j] = block2[i][j];
}

//Do an XOR operation on the whole blocks input1 and input2 then store output in output
void xor_blocks(int rows, int cols, int input1[rows][cols], int input2[rows][cols], int output[rows][cols]){

	int i,j;

	for(i = 0; i < rows; i++)
		for(j = 0; j < cols; j++)
			output[i][j] = input1[i][j] ^ input2[i][j];

}

//Left shift the whole block of input by n bits
void left_shift_block(int n, int rows, int cols, int input[rows][cols], int output[rows][cols]){

	int holder[rows][cols];
	int size = rows * cols;
	int pos_holder = 1 + n;
	int i,j;

	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			holder[i][j] = pos_holder;
			pos_holder += 1;
			pos_holder = (pos_holder > size)?1:pos_holder;
		}
	}

	assign_block(rows,cols,rows,cols,input,holder,output);

}

//Check for even parity for each row in the block
int even_parity_check_block(int rows, int cols, int input[rows][cols]){

	int i,j;
	int parity_bit;
	int sum_holder;
	int ret = 1;

	for(i = 0; i < rows; i++){
		parity_bit = input[i][cols-1];
		sum_holder = 0;

		for(j = 0; j < cols - 1; j++)
			sum_holder += input[i][j];
		
		ret = ret && (sum_holder % 2 == parity_bit);
	}

	return ret;

}

//Check for odd parity for each row in the block
int odd_parity_check_block(int rows, int cols, int input[rows][cols]){

	int i,j;
	int parity_bit;
	int sum_holder;
	int ret = 1;

	for(i = 0; i < rows; i++){
		parity_bit = input[i][cols-1];
		sum_holder = 0;

		for(j = 0; j < cols - 1; j++)
			sum_holder += input[i][j];
		ret = ret && (sum_holder % 2 != parity_bit);
	}

	return ret;

}

//Using an in_cols bit input derive a out_cols bit output using S
void s_function(int in_cols, int out_cols, int s_rows, int s_cols,
		int input[in_cols], int output[out_cols], int S[s_rows][s_cols]){

	int row_bin[2] = { input[0], input[in_cols - 1] };
	int col_bin[in_cols - 2];
	int row, col, value, holder[out_cols];

	int i;

	for(i = 1; i < in_cols - 1; i++)
		col_bin[i - 1] = input[i];

	row = to_int(2,row_bin);
	col = to_int(in_cols - 2, col_bin);
	value = S[row][col];

	to_bin_array(value, out_cols, holder);

	for(i = 0; i < out_cols; i++){
		output[i] = holder[i];
	}

}

//From Key key and n derive Kn using KS(key,n) then store it in out
void ks_function(int n, int rows, int cols, int out_rows, int out_cols, int pc_rows, int pc_cols, int pc2_cols, 
		int key[rows][cols], int out[out_rows][out_cols], int PC1[pc_rows][pc_cols], int PC2[pc_rows][pc2_cols]){

	int holder[pc_rows][pc_cols];
	int new_row = rows/2;
	int iteration[ITERATIONS];
	int c[new_row][pc_cols];
	int c2[new_row][pc_cols];
	int d[new_row][pc_cols];
	int d2[new_row][pc_cols];
	int i,j,k;	

	for(i = 0; i < ITERATIONS; i++)
		iteration[i] = 2;

	iteration[0]  = 1;
	iteration[1]  = 1;
	iteration[8]  = 1;
	iteration[15] = 1;

	assign_block(pc_rows,pc_cols,rows,cols,key,PC1,holder);
	split_block_by_row(pc_rows, pc_cols, holder, c, d);

	left_shift_block(iteration[0], new_row, pc_cols, c, c2);
	left_shift_block(iteration[0], new_row, pc_cols, d, d2);

	copy_block_content(new_row, pc_cols, c, c2);
	copy_block_content(new_row, pc_cols, d, d2);

	for(k = 1; k < n; k++){

		left_shift_block(iteration[k], new_row, pc_cols, c, c2);
		left_shift_block(iteration[k], new_row, pc_cols, d, d2);

		copy_block_content(new_row, pc_cols, c, c2);
		copy_block_content(new_row, pc_cols, d, d2);

	}

	merge_blocks_by_row(pc_rows, pc_cols, holder, c, d);
	assign_block(out_rows,out_cols,pc_rows,pc_cols,holder,PC2,out);	

}


//Using the R block r_block and Kn block k_block get output block output using encryption function f(R, Kn)  
void cipher_function(int rows, int cols,int k_rows, int k_cols, int s_rows, int s_cols, int p_cols,
		int r_block[rows][cols], int key[k_rows][k_cols], int output[rows][cols], 
		int E[rows][k_cols], int S[cols][s_rows][s_cols], int P[k_rows][p_cols]){

	int r_after_E[k_rows][k_cols];
	int r_xor_k[k_rows][k_cols];
	int s_holder[rows][cols/2];
	int i,j;

	assign_block(k_rows,k_cols,rows,cols,r_block, E, r_after_E);
	xor_blocks(k_rows, k_cols, r_after_E, key, r_xor_k);
	
	
	for(i = 0; i < k_rows; i++){
		s_function(k_cols, cols/2, s_rows, s_cols, r_xor_k[i], s_holder[i], S[i] );
	}

	assign_block(rows, cols, rows, cols, s_holder, P, output);


}

//Encrypts 64-bit input using DES then stores it in output with key
void encrypt(int n, int rows, int cols, int input[rows][cols], int output[rows][cols], int key[rows][cols],
		int IP[BLOCK_ROWS][BLOCK_COLS], int IP1[BLOCK_ROWS][BLOCK_COLS],
		int E[BLOCK_ROWS][E_BLOCK_COLS], int P[BLOCK_ROWS][P_BLOCK_COLS],int S[BLOCK_ROWS][S_ROWS][S_COLS],
		int PC1[BLOCK_ROWS][PC_BLOCK_COLS], int PC2[BLOCK_ROWS][E_BLOCK_COLS]
		){

	int holder[rows][cols];
	int new_row = rows/2;
	int l[new_row][cols];
	int r[new_row][cols];
	int l2[new_row][cols];
	int r2[new_row][cols];
	int holder2[new_row][cols];
	int kn[rows][E_BLOCK_COLS];
	int i,j, k;

	assign_block(rows, cols, rows, cols, input, IP, holder);
	split_block_by_row(rows, cols, holder, l, r);

	for(i = 0; i < n; i++){
		
		ks_function(i + 1, rows, cols, rows, E_BLOCK_COLS, rows, PC_BLOCK_COLS, E_BLOCK_COLS, key, kn, PC1, PC2);
		cipher_function(new_row, cols, rows, E_BLOCK_COLS, S_ROWS,S_COLS, P_BLOCK_COLS, r, kn, holder2,E,S,P);
		xor_blocks(new_row, cols, l, holder2, r2);
		copy_block_content(new_row,cols,l,r);
		copy_block_content(new_row,cols,r,r2);
	}

	merge_blocks_by_row(rows, cols, holder, r, l);

	assign_block(rows, cols, rows, cols, holder, IP1, output);
		

}

//Decrypts 64-bit input using DES then stores it in output with key
void decrypt(int n, int rows, int cols, int input[rows][cols], int output[rows][cols], int key[rows][cols],
		int IP[BLOCK_ROWS][BLOCK_COLS], int IP1[BLOCK_ROWS][BLOCK_COLS],
		int E[BLOCK_ROWS][E_BLOCK_COLS], int P[BLOCK_ROWS][P_BLOCK_COLS],int S[BLOCK_ROWS][S_ROWS][S_COLS],
		int PC1[BLOCK_ROWS][PC_BLOCK_COLS], int PC2[BLOCK_ROWS][E_BLOCK_COLS]
		){

	int holder[rows][cols];
	int new_row = rows/2;
	int l[new_row][cols];
	int r[new_row][cols];
	int l2[new_row][cols];
	int r2[new_row][cols];
	int holder2[new_row][cols];
	int kn[rows][E_BLOCK_COLS];
	int i,j, k;

	assign_block(rows, cols, rows, cols, input, IP, holder);

	split_block_by_row(rows,cols,holder,r,l);

	for(i = n - 1; i >= 0; i--){
		ks_function(i + 1, rows, cols, rows, E_BLOCK_COLS, rows, PC_BLOCK_COLS, E_BLOCK_COLS, key, kn, PC1, PC2);
		cipher_function(new_row, cols, rows, E_BLOCK_COLS, S_ROWS,S_COLS, P_BLOCK_COLS, l, kn, holder2,E,S,P);
		xor_blocks(new_row, cols, r, holder2, l2);
		copy_block_content(new_row,cols,r,l);
		copy_block_content(new_row,cols,l,l2);
	}

	merge_blocks_by_row(rows, cols, holder, l,r);

	assign_block(rows, cols, rows, cols, holder, IP1, output);


}


//Debugging functions
void print_2D(int rows, int cols,int arr[rows][cols]){
    int i, j;
    for (i = 0; i < rows; ++i){

        for (j = 0; j < cols; ++j)
            printf("%d ", arr[i][j]);
        printf("\n");
    }
}

void print_2D_to_file(char *file_name, char *msg ,int rows, int cols,int arr[rows][cols]){
    
    FILE *fp = fopen(file_name, "a");

    fprintf(fp, "%s\n", msg);
    int i, j;
    for (i = 0; i < rows; ++i){

        for (j = 0; j < cols; ++j)
            fprintf(fp,"%d ", arr[i][j]);
        
        fprintf(fp,"\n");
    }

    fclose(fp);
}

void print_to_file(char *file_name, char *msg, int rows, int cols, int arr[rows][cols], int n){
	
	FILE *fp = fopen(file_name, "a");

    fprintf(fp, "%s%d: ", msg, n);
    int i, j;
    for (i = 0; i < rows; ++i){

        for (j = 0; j < cols; ++j)
            fprintf(fp,"%d",arr[i][j]);
        fprintf(fp, " ");
    }
    fprintf(fp, "\n");
    fclose(fp);

}