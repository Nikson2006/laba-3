#include <UnitTest++/UnitTest++.h>
#include "table_cipher.h"
#include <string>

std::string s = "MAGMA";

SUITE(KeyTest) {
    TEST(ValidKey) {
        code cipher(3, "MAGMA");
        CHECK_EQUAL("GAMMA", cipher.encryption(s));
    }
    TEST(LongKey) {
        CHECK_THROW(code(1, "MAGMA"), cipher_error);
    }
}

struct KeyB_fixture {
    code * t;
    KeyB_fixture() {
        t = new code(3, "MAGMA");
    }
    ~KeyB_fixture() {
        delete t;
    }
};

SUITE(EncryptTest) {
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        std::string input = "MAGMA";
        CHECK_EQUAL("GAMMA", t->encryption(input));
    }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        std::string input = "magma";
        CHECK_EQUAL("gamma", t->encryption(input));
    }
    TEST_FIXTURE(KeyB_fixture, StringWithWhitspaceAndPunct) {
        std::string input = "MAGMA MAGMA";
        CHECK_EQUAL("GMMAAGMMAA", t->encryption(input));
    }
    TEST_FIXTURE(KeyB_fixture, StringWithNumbers) {
        std::string input = "M7GMA";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        std::string input = "";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, NoAlphaString) {
        std::string input = "2*2=5?";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST(MaxShiftKey) {
        code cipher(8, "FOOTBALL");
        std::string input1 = "LLABTOOF";
        std::string input2 = "FOOTBALL";
        CHECK_EQUAL("FOOTBALL", cipher.transcript(input1, input2));
    }
}

SUITE(DecryptText) {
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        std::string input1 = "GAMMA";
        std::string input2 = "MAGMA";
        CHECK_EQUAL("MAGMA", t->transcript(input1, input2));
    }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        std::string input1 = "magma";
        std::string input2 = "gamma";
        CHECK_EQUAL("gamma", t->transcript(input1, input2));
    }
    TEST_FIXTURE(KeyB_fixture, WhitespaceString) {
        std::string input1 = "GAMMAGAMMA";
        std::string input2 = "GAMMA GAMMA";
        CHECK_THROW(t->transcript(input1, input2), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, DigitsString) {
        std::string input1 = "G7MMA";
        std::string input2 = "G7MMA";
        CHECK_THROW(t->transcript(input1, input2), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, PunctString) {
        std::string input1 = "GAMMA!";
        std::string input2 = "GAMMA!";
        CHECK_THROW(t->transcript(input1, input2), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        std::string input1 = "";
        std::string input2 = "";
        CHECK_THROW(t->transcript(input1, input2), cipher_error);
    }
    TEST(MaxShiftKey) {
        code cipher(8, "Gamegame");
        std::string input1 = "LLABTOOF";
        std::string input2 = "FOOTBALL";
        CHECK_EQUAL("FOOTBALL", cipher.transcript(input1, input2));
    }
}

int main(int argc, char **argv) {
    return UnitTest::RunAllTests();
}