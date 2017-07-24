
test: sdes_test
	./sdes_test

sdes_test: sdes_key.hh sdes.hh rubric.hh sdes_test.cpp
	clang++ --std=c++11 sdes_test.cpp -o sdes_test

clean:
	rm -f sdes_test
