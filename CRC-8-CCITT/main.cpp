#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

vector <bool> poly;


void cout_bit_array(bool* array, int K) {
	char BufferOut[10000] = "";
	int ind_buf = 0;

	BufferOut[ind_buf] = '['; ind_buf++;
	for (int i = 0; i < K; i++) {
		BufferOut[ind_buf] = array[i] ? '1' : '0'; ind_buf++;
		BufferOut[ind_buf] = ','; BufferOut[ind_buf + 1] = ' '; ind_buf += 2;
	}

	BufferOut[ind_buf-2] = ']';
	BufferOut[ind_buf - 1] = ' ';
	BufferOut[ind_buf] = ' ';

	cout << BufferOut << endl;
}


void cout_bit_array(vector <bool> array) {
	char BufferOut[10000] = "";
	int ind_buf = 0;

	BufferOut[ind_buf] = '['; ind_buf++;
	for (int i = 0; i < array.size(); i++) {
		BufferOut[ind_buf] = array[i] ? '1' : '0'; ind_buf++;
		BufferOut[ind_buf] = ','; BufferOut[ind_buf + 1] = ' '; ind_buf += 2;
	}

	BufferOut[ind_buf - 2] = ']';
	BufferOut[ind_buf - 1] = ' ';
	BufferOut[ind_buf] = ' ';

	cout << BufferOut << endl;
}


bool* gen_random_bit_array(int len_array) {
	bool* bit_array = new bool[len_array];
	for (int i = 0; i < len_array; i++) {
		cin >> bit_array[i];
	}
	return bit_array;
}


bool* CRC_kod(bool* stak, bool* data, int K, int W) {
	for (int i = 0; i < K + W; i++) {
		bool bit = stak[0];

		for (int j = 1; j < W; j++)
			stak[j - 1] = stak[j];
		stak[W - 1] = data[i];

		if (bit) {
			for (int j = 0; j < W; j++) {
				stak[j] = stak[j] ^ poly.at(j + 1);
			}
		}
	}

	return stak;
}


bool* CRC_8_CCITT(bool* data_bit, int K) {
	cout << "»спользовать полином CRC-8-CCIITT или задать свой?" << endl;
	cout << "1 - задать полином, 2 - использовать CRC-8-CCIITT: ";
	int v; cin >> v;
	int W; bool* stak = NULL;

	if(v == 2) {
		W = 8;
		stak = new bool[W];
		poly = { 1, 0, 0, 0, 0, 0, 1, 1, 1 };

		for (int i = 0; i < W; i++)
			stak[i] = 0;
	}
	else {
		cout << "¬ведите размер полинома: "; cin >> W;

		cout << "¬ведите полином: " << endl;
		for (int i = 0; i < W; i++) {
			bool bit;
			cin >> bit;
			poly.push_back(bit);
		}

		W--;
		stak = new bool[W];
		for (int i = 0; i < W; i++)
			stak[i] = 0;
	}

	bool* new_data = new bool[K + W];

	for (int i = 0; i < K; i++) {
		new_data[i] = data_bit[i];
	}

	for (int i = K; i < K + W; i++) {
		new_data[i] = 0;
	}

	stak = CRC_kod(stak, new_data, K, W);

	cout_bit_array(new_data, W+K);
	cout_bit_array(stak, W);

	return stak;
}


int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	bool* bit_array = 0;
	bool* CRC = 0;
	int K;
	cout << "¬ведите длину последовательности: ";
	cin >> K;

	while (true) {
		cout << "1 - ввести последовательность, 2 - сгенерировать," << endl;
		cout << "3 - проверить контрольную сумму: ";
		int v; cin >> v;


		if (v == 1)
			bit_array = gen_random_bit_array(K);
		else if (v == 2) {
			bit_array = new bool[K];
			for (int i = 0; i < K; i++) {
				bit_array[i] = rand() % 2;
			}
			cout << "—генерирована последовательность бит:" << endl;
			cout_bit_array(bit_array, K);

			CRC = CRC_8_CCITT(bit_array, K);
		}
		else if (v == 3) {
			int len_CRC = _msize(CRC) / sizeof(*CRC);
			bool* new_data = new bool[len_CRC + K];

			for (int i = 0; i < K; i++) {
				new_data[i] = bit_array[i];
			}
			for (int i = K; i < K + len_CRC; i++) {
				new_data[i] = CRC[i - K];
			}

			bool* stak = new bool[len_CRC];
			for (int i = 0; i < len_CRC; i++)
				stak[i] = 0;
			
			CRC = CRC_kod(stak, new_data, K, len_CRC);

			cout_bit_array(CRC, len_CRC);
			return 0;
		}
		else
			return 0;
	}
}