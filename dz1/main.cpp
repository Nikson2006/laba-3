#include <UnitTest++/UnitTest++.h>
#include "modAlphaCipher.h"

SUITE(KeyTest)
{
    TEST(ValidKey) {
        CHECK_EQUAL("БЕРБЕ",modAlphaCipher("БЕР").encrypt("ААААА"));
    }
    TEST(LongKey) {
        CHECK_EQUAL("БЕРНИ",modAlphaCipher("БЕРНИК").encrypt("ААААА"));
    }
    TEST(LowCaseKey) {
        CHECK_EQUAL("БЕРБЕ",modAlphaCipher("бер").encrypt("ААААА"));
    }
    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cp("Ф1"),cipher_error);
    }
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cp("Ф,И"),cipher_error);
    }
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cp("Ф И"),cipher_error);
    }
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cp(""),cipher_error);
    }
    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cp("ААА"),cipher_error);
    }
}

struct KeyB_fixture {
    modAlphaCipher * p;
    KeyB_fixture()
    {
        p = new modAlphaCipher("К");
    }
    ~KeyB_fixture()
    {
        delete p;
    }
};

SUITE(EncryptTest)
{
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK_EQUAL("ЪЫУМПЭЧУЫ", p->encrypt("ПРИВЕТМИР"));
    }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_EQUAL("ЪЫУМПЭЧУЫ", p->encrypt("приветмир"));
    }
    TEST_FIXTURE(KeyB_fixture, StringWithWhitspaceAndPunct) {
        CHECK_EQUAL("ЪЫУМПЭЧУЫ", p->encrypt("ПРИВЕТ,МИР!"));
    }
    TEST_FIXTURE(KeyB_fixture, StringWithNumbers) {
        CHECK_EQUAL("ЬШЩМЁЧНЩОЩЧ", p->encrypt("СНОВЫМ2025ГОДОМ"));
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(""), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt("2*2=5"), cipher_error);
    }
    TEST(MaxShiftKey) {
        CHECK_EQUAL("УТСАНКЫЦЗЙ", modAlphaCipher("Я").encrypt("ФУТБОЛЬЧИК"));
    }
}

SUITE(DecryptText)
{
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK_EQUAL("ПРИВЕТМИР", p->decrypt("ЪЫУМПЭЧУЫ"));
    }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_THROW(p->decrypt("ъыумпэчуы"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, WhitespaceString) {
        CHECK_THROW(p->decrypt("ЪЫУ МПЭ ЧУЫ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, DigitsString) {
        CHECK_THROW(p->decrypt("ЬШЩМЁЧ2025НЩОЩЧ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, PunctString) {
        CHECK_THROW(p->decrypt("ЪЫУ,МПЭ!ЧУЫ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(""), cipher_error);
    }
    TEST(MaxShiftKey) {
        CHECK_EQUAL("ФУТБОЛЬЧИК", modAlphaCipher("Я").decrypt("УТСАНКЫЦЗЙ"));
    }
}

int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}