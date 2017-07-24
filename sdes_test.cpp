
#include "rubric.hh"

#include "sdes.hh"

#include <climits>

int main() {

  Rubric r;

  Bits ones("1111 1111"),
    zeroes("0000 0000"),
    F0("1111 0000"),
    byte("1001 0100");

  std::vector<Bits> all_bytes;
  for (int i = 0; i < (1 << 8); i++) {
    Bits b(8);
    for (int j = 0; j < 8; j++) {
      b[j+1] = (i >> j) & 1;
    }
    all_bytes.push_back(b);
  }
  
  r.criterion("IP and IP-inverse", 1,
	      [&]() {
		Bits ip(sdes_ip(byte));
		TEST_EQUAL("IP 2", 0, ip[1]);
		TEST_EQUAL("IP 6", 1, ip[2]);
		TEST_EQUAL("IP 3", 0, ip[3]);
		TEST_EQUAL("IP 1", 1, ip[4]);
		TEST_EQUAL("IP 4", 1, ip[5]);
		TEST_EQUAL("IP 8", 0, ip[6]);
		TEST_EQUAL("IP 5", 0, ip[7]);
		TEST_EQUAL("IP 7", 0, ip[8]);

		Bits ip_inverse(sdes_ip_inverse(byte));
		TEST_EQUAL("IP 4", 1, ip_inverse[1]);
		TEST_EQUAL("IP 1", 1, ip_inverse[2]);
		TEST_EQUAL("IP 3", 0, ip_inverse[3]);
		TEST_EQUAL("IP 5", 0, ip_inverse[4]);
		TEST_EQUAL("IP 7", 0, ip_inverse[5]);
		TEST_EQUAL("IP 2", 0, ip_inverse[6]);
		TEST_EQUAL("IP 8", 0, ip_inverse[7]);
		TEST_EQUAL("IP 6", 1, ip_inverse[8]);

		for (auto b : all_bytes) {
		  TEST_EQUAL("IP exhaustive inversion", b, sdes_ip_inverse(sdes_ip(b)));
		}
	      });
  
  r.criterion("SW", 1,
	      [&]() {
		TEST_EQUAL("SW ones", ones, sdes_sw(ones));
		TEST_EQUAL("SW zeroes", zeroes, sdes_sw(zeroes));
		TEST_EQUAL("SW F0", "0000 1111", sdes_sw(F0).to_string(true));
		TEST_EQUAL("SW byte", "0100 1001", sdes_sw(byte).to_string(true));
	      });

  r.criterion("S-box", 1,
	      [&]() {
		SBox s({ {0, 1, 2, 3},
			 {1, 2, 3, 0},
			 {2, 3, 0, 1},
			 {3, 0, 1, 2} });

		TEST_EQUAL("0000", "00", s.substitute(Bits("0000")).to_string(true));
		TEST_EQUAL("0001", "01", s.substitute(Bits("0001")).to_string(true));
		TEST_EQUAL("0010", "01", s.substitute(Bits("0010")).to_string(true));
		TEST_EQUAL("0011", "10", s.substitute(Bits("0011")).to_string(true));
		TEST_EQUAL("0100", "10", s.substitute(Bits("0100")).to_string(true));
		TEST_EQUAL("0101", "11", s.substitute(Bits("0101")).to_string(true));
		TEST_EQUAL("0110", "11", s.substitute(Bits("0110")).to_string(true));
		TEST_EQUAL("0111", "00", s.substitute(Bits("0111")).to_string(true));
		TEST_EQUAL("1000", "10", s.substitute(Bits("1000")).to_string(true));
		TEST_EQUAL("1001", "11", s.substitute(Bits("1001")).to_string(true));
		TEST_EQUAL("1010", "11", s.substitute(Bits("1010")).to_string(true));
		TEST_EQUAL("1011", "00", s.substitute(Bits("1011")).to_string(true));
		TEST_EQUAL("1100", "00", s.substitute(Bits("1100")).to_string(true));
		TEST_EQUAL("1101", "01", s.substitute(Bits("1101")).to_string(true));
		TEST_EQUAL("1110", "01", s.substitute(Bits("1110")).to_string(true));
		TEST_EQUAL("1111", "10", s.substitute(Bits("1111")).to_string(true));
	      });

  r.criterion("F (upper-case f)", 2,
	      [&]() {
		// worked example from
		// http://homepage.smc.edu/morgan_david/vpn/assignments/assgt-sdes-encrypt-sample.htm
		TEST_EQUAL("Worked example", "1111", sdes_F(Bits("0110"), Bits("1010 0100")).to_string(true));

		// input/output from solution code
		TEST_EQUAL("0000", "1011", sdes_F(Bits("0000"), byte).to_string(false));
		TEST_EQUAL("0001", "1111", sdes_F(Bits("0001"), byte).to_string(false));
		TEST_EQUAL("0010", "0000", sdes_F(Bits("0010"), byte).to_string(false));
		TEST_EQUAL("0011", "1100", sdes_F(Bits("0011"), byte).to_string(false));
		TEST_EQUAL("0100", "1100", sdes_F(Bits("0100"), byte).to_string(false));
		TEST_EQUAL("0101", "0001", sdes_F(Bits("0101"), byte).to_string(false));
		TEST_EQUAL("0110", "0111", sdes_F(Bits("0110"), byte).to_string(false));
		TEST_EQUAL("0111", "0000", sdes_F(Bits("0111"), byte).to_string(false));
		TEST_EQUAL("1000", "1101", sdes_F(Bits("1000"), byte).to_string(false));
		TEST_EQUAL("1001", "1110", sdes_F(Bits("1001"), byte).to_string(false));
		TEST_EQUAL("1010", "1010", sdes_F(Bits("1010"), byte).to_string(false));
		TEST_EQUAL("1011", "1001", sdes_F(Bits("1011"), byte).to_string(false));
		TEST_EQUAL("1100", "0001", sdes_F(Bits("1100"), byte).to_string(false));
		TEST_EQUAL("1101", "0110", sdes_F(Bits("1101"), byte).to_string(false));
		TEST_EQUAL("1110", "1011", sdes_F(Bits("1110"), byte).to_string(false));
		TEST_EQUAL("1111", "0101", sdes_F(Bits("1111"), byte).to_string(false));
	      });

  r.criterion("f (lower-case f)", 2,
	      [&]() {
		// worked example
		TEST_EQUAL("Worked example", "0001 0110", sdes_f(Bits("1010 0100"), Bits("1110 0110")).to_string(true));

		// input/output from solution code
		TEST_EQUAL("0000 0000", "1011 0000", sdes_f(byte, Bits("0000 0000")).to_string(true));
		TEST_EQUAL("0000 0001", "1111 0001", sdes_f(byte, Bits("0000 0001")).to_string(true));
		TEST_EQUAL("0000 0010", "0000 0010", sdes_f(byte, Bits("0000 0010")).to_string(true));
		TEST_EQUAL("0000 0011", "1100 0011", sdes_f(byte, Bits("0000 0011")).to_string(true));
		TEST_EQUAL("0000 0100", "1100 0100", sdes_f(byte, Bits("0000 0100")).to_string(true));
		TEST_EQUAL("0000 0101", "0001 0101", sdes_f(byte, Bits("0000 0101")).to_string(true));
		TEST_EQUAL("0000 0110", "0111 0110", sdes_f(byte, Bits("0000 0110")).to_string(true));
		TEST_EQUAL("0000 0111", "0000 0111", sdes_f(byte, Bits("0000 0111")).to_string(true));
		TEST_EQUAL("0000 1000", "1101 1000", sdes_f(byte, Bits("0000 1000")).to_string(true));
		TEST_EQUAL("0000 1001", "1110 1001", sdes_f(byte, Bits("0000 1001")).to_string(true));
		TEST_EQUAL("0000 1010", "1010 1010", sdes_f(byte, Bits("0000 1010")).to_string(true));
		TEST_EQUAL("0000 1011", "1001 1011", sdes_f(byte, Bits("0000 1011")).to_string(true));
		TEST_EQUAL("0000 1100", "0001 1100", sdes_f(byte, Bits("0000 1100")).to_string(true));
		TEST_EQUAL("0000 1101", "0110 1101", sdes_f(byte, Bits("0000 1101")).to_string(true));
		TEST_EQUAL("0000 1110", "1011 1110", sdes_f(byte, Bits("0000 1110")).to_string(true));
		TEST_EQUAL("0000 1111", "0101 1111", sdes_f(byte, Bits("0000 1111")).to_string(true));
	      });

  r.criterion("SDES block encryption/decryption", 1,
	      [&]() {
		// input/output from solution code
		const Bits key1("11 0001 1000");
		// encrypt block
		TEST_EQUAL("encrypt 0000 0000", "0001 1000", sdes_encrypt_block(key1, Bits("0000 0000")).to_string(true));
		TEST_EQUAL("encrypt 0000 0001", "1001 1010", sdes_encrypt_block(key1, Bits("0000 0001")).to_string(true));
		TEST_EQUAL("encrypt 0000 0010", "0111 0110", sdes_encrypt_block(key1, Bits("0000 0010")).to_string(true));
		TEST_EQUAL("encrypt 0000 0011", "1010 0011", sdes_encrypt_block(key1, Bits("0000 0011")).to_string(true));
		TEST_EQUAL("encrypt 0000 0100", "0111 1101", sdes_encrypt_block(key1, Bits("0000 0100")).to_string(true));
		TEST_EQUAL("encrypt 0000 0101", "1101 1111", sdes_encrypt_block(key1, Bits("0000 0101")).to_string(true));
		TEST_EQUAL("encrypt 0000 0110", "1101 0011", sdes_encrypt_block(key1, Bits("0000 0110")).to_string(true));
		TEST_EQUAL("encrypt 0000 0111", "0010 0010", sdes_encrypt_block(key1, Bits("0000 0111")).to_string(true));
		TEST_EQUAL("encrypt 0000 1000", "0111 0011", sdes_encrypt_block(key1, Bits("0000 1000")).to_string(true));
		TEST_EQUAL("encrypt 0000 1001", "1000 0010", sdes_encrypt_block(key1, Bits("0000 1001")).to_string(true));
		TEST_EQUAL("encrypt 0000 1010", "1101 1101", sdes_encrypt_block(key1, Bits("0000 1010")).to_string(true));
		TEST_EQUAL("encrypt 0000 1011", "0100 1101", sdes_encrypt_block(key1, Bits("0000 1011")).to_string(true));
		TEST_EQUAL("encrypt 0000 1100", "1101 0110", sdes_encrypt_block(key1, Bits("0000 1100")).to_string(true));
		TEST_EQUAL("encrypt 0000 1101", "0000 0011", sdes_encrypt_block(key1, Bits("0000 1101")).to_string(true));
		TEST_EQUAL("encrypt 0000 1110", "1011 1000", sdes_encrypt_block(key1, Bits("0000 1110")).to_string(true));
		TEST_EQUAL("encrypt 0000 1111", "0000 1100", sdes_encrypt_block(key1, Bits("0000 1111")).to_string(true));
		// decrypt block
		TEST_EQUAL("decrypt 0000 0000", "1110 0001", sdes_decrypt_block(key1, Bits("0000 0000")).to_string(true));
		TEST_EQUAL("decrypt 0000 0001", "0001 0100", sdes_decrypt_block(key1, Bits("0000 0001")).to_string(true));
		TEST_EQUAL("decrypt 0000 0010", "1010 1111", sdes_decrypt_block(key1, Bits("0000 0010")).to_string(true));
		TEST_EQUAL("decrypt 0000 0011", "0000 1101", sdes_decrypt_block(key1, Bits("0000 0011")).to_string(true));
		TEST_EQUAL("decrypt 0000 0100", "0110 0000", sdes_decrypt_block(key1, Bits("0000 0100")).to_string(true));
		TEST_EQUAL("decrypt 0000 0101", "1011 0001", sdes_decrypt_block(key1, Bits("0000 0101")).to_string(true));
		TEST_EQUAL("decrypt 0000 0110", "1110 1110", sdes_decrypt_block(key1, Bits("0000 0110")).to_string(true));
		TEST_EQUAL("decrypt 0000 0111", "0100 1000", sdes_decrypt_block(key1, Bits("0000 0111")).to_string(true));
		TEST_EQUAL("decrypt 0000 1000", "0100 1110", sdes_decrypt_block(key1, Bits("0000 1000")).to_string(true));
		TEST_EQUAL("decrypt 0000 1001", "1101 1010", sdes_decrypt_block(key1, Bits("0000 1001")).to_string(true));
		TEST_EQUAL("decrypt 0000 1010", "1011 0000", sdes_decrypt_block(key1, Bits("0000 1010")).to_string(true));
		TEST_EQUAL("decrypt 0000 1011", "1011 0111", sdes_decrypt_block(key1, Bits("0000 1011")).to_string(true));
		TEST_EQUAL("decrypt 0000 1100", "0000 1111", sdes_decrypt_block(key1, Bits("0000 1100")).to_string(true));
		TEST_EQUAL("decrypt 0000 1101", "1011 1111", sdes_decrypt_block(key1, Bits("0000 1101")).to_string(true));
		TEST_EQUAL("decrypt 0000 1110", "0001 0101", sdes_decrypt_block(key1, Bits("0000 1110")).to_string(true));
		TEST_EQUAL("decrypt 0000 1111", "1111 0010", sdes_decrypt_block(key1, Bits("0000 1111")).to_string(true));

		// encrypt/decrypt strings
		const std::string hello_world_ciphertext({-17, 35, 106, 106, 20, -80, -71, 20, 41, 106, 65});
		TEST_EQUAL("encrypt \"hello world\"", hello_world_ciphertext, sdes_encrypt_string(key1, "hello world"));
		TEST_EQUAL("decrypt \"hello world\"", "hello world", sdes_decrypt_string(key1, sdes_encrypt_string(key1, "hello world")));
		
		// round trip strings of length 2
		for (int a = SCHAR_MIN; a <= SCHAR_MAX; a++) {
		  for (int b = SCHAR_MIN; b <= SCHAR_MAX; b++) {
		    std::string s({char(a), char(b)});
		    TEST_EQUAL("round trip string", s, sdes_decrypt_string(key1, sdes_encrypt_string(key1, s)));
		  }
		}

		// Round trip every possible key-block
		// combination. There are 2^10 keys and 2^8 blocks, so
		// 262,144 combinations, and this loop is fairly slow.
		for (int k = 0; k < (1 << 10); k++) {
		  Bits key2(int_to_bits(k, 10));
		  for (int p = 0; p < (1 << 8); p++) {
		    Bits plaintext(int_to_bits(p)),
		      ciphertext(sdes_encrypt_block(key2, plaintext)),
		      decrypted_ciphertext(sdes_decrypt_block(key2, ciphertext));
		    TEST_EQUAL("round trip block", decrypted_ciphertext, plaintext);
		  }
		}

	      });

  return r.run();
}
