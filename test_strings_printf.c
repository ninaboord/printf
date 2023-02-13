#include "assert.h"
#include "printf.h"
#include <stddef.h>
#include "strings.h"
#include "uart.h"

// Copied from printf.c
int unsigned_to_base(char *buf, size_t bufsize, unsigned int val, int base, size_t min_width);
int signed_to_base(char *buf, size_t bufsize, int val, int base, size_t min_width);


// unsigned int add_to_buf(unsigned char ascii, const char *buf){

// }

static void test_memset(void)
{
    char buf[12];
    size_t bufsize = sizeof(buf);

    memset(buf, 0x77, bufsize); // fill buffer with repeating value
    for (int i = 0; i < bufsize; i++)
        assert(buf[i] == 0x77); // confirm value

    memset(buf, 0, bufsize); // fill buffer with repeating value
    for (int i = 0; i < bufsize; i++)
        assert(buf[i] == 0); // confirm value

    memset(buf, 0b10010100, bufsize); // fill buffer with repeating value
    for (int i = 0; i < bufsize; i++)
        assert(buf[i] == 0b10010100); // confirm value
    
    memset(buf, 0b00000000, bufsize); // fill buffer with repeating value
    for (int i = 0; i < bufsize; i++)
        assert(buf[i] == 0b00000000); // confirm value

    memset(buf, 0b000000000, bufsize); // fill buffer with repeating value
    for (int i = 0; i < bufsize; i++)
        assert(buf[i] == 0b000000000); // confirm value

    memset(buf, 0b000100001, bufsize); // fill buffer with repeating value
    for (int i = 0; i < bufsize; i++)
        assert(buf[i] == 0b000100001); // confirm value
    
    memset(buf, 'a', bufsize); // fill buffer with repeating value
    for (int i = 0; i < bufsize; i++)
        assert(buf[i] == 'a'); // confirm value

    for (int j = 0; j < 256; j++){
        memset(buf, j, bufsize); // fill buffer with repeating value
        for (int i = 0; i < bufsize; i++)
            assert(buf[i] == j); // confirm value
    }
        
}

static void test_strcmp(void)
{
    assert(strcmp("apple", "apple") == 0);
    assert(strcmp("apple", "applesauce") < 0);
    assert(strcmp("pears", "apples") > 0);
    assert(strcmp(" a", "") > 0);
    assert(strcmp("", "chacha") < 0);
    assert(strcmp("", "") == 0);
    assert(strcmp("a", "b") < 0);
    assert(strcmp("zasas", "2321f444ad4ad000") > 0);
    assert(strcmp("x", "x") == 0);
}

static void test_strlcat(void)
{
    char buf[20];
    size_t bufsize = sizeof(buf);
    memset(buf, 0x77, bufsize); // init contents with known value
    buf[0] = '\0'; // null in first index makes empty string


    assert(strlen(buf) == 0);
    strlcat(buf, "", bufsize); // append empty string
    assert(strlen(buf) == 0);
    assert(strcmp(buf, "") == 0);
    strlcat(buf, "hi", bufsize); // append stirng
    assert(strlen(buf) == 2);
    assert(strcmp(buf, "hi") == 0);


//THESE TESTS ALL PASS
//     assert(strlen(buf) == 0);
//     strlcat(buf, "hithere", bufsize); // append hithere
//     assert(strlen(buf) == 7);
//     assert(strcmp(buf, "hithere") == 0);
//     strlcat(buf, "", bufsize); // append empty string
//     assert(strlen(buf) == 7);
//     assert(strcmp(buf, "hithere") == 0);


//    memset(buf, 0x77, bufsize); // init contents with known value
//     buf[0] = '\0'; // null in first index makes empty string
//     assert(strlen(buf) == 0);
//     strlcat(buf, "CS", bufsize); // append CS
//     assert(strlen(buf) == 2);
//     assert(strcmp(buf, "CS") == 0);
//     strlcat(buf, "107e", bufsize); // append 107e
//     assert(strlen(buf) == 6);
//     assert(strcmp(buf, "CS107e") == 0);



}

static void test_strtonum(void)
{
    // test decomal: basic
    int val = strtonum("013", NULL);
    assert(val == 13);

    // test larger decimal: basic
    val = strtonum("81928", NULL);
    assert(val == 81928);

    // test space as invalid char
    val = strtonum(" 81", NULL);
    assert(val == 0);

    // test new invalid chars
    val = strtonum("1&**2938 81", NULL);
    assert(val == 1);

    // test hex
    val = strtonum("8A88888", NULL);
    assert(val == 8);
    
    // test hex
    val = strtonum("A9171", NULL);
    assert(val == 0);

    // test null as invalid char
    val = strtonum("8\088888", NULL);
    assert(val == 8);

    // test null
    val = strtonum("\0", NULL);
    assert(val == 0);

    // original rest test
    const char *input = "107rocks";
    const char *rest = NULL;
    val = strtonum(input, &rest);
    assert(val == 107);
    //rest was modified to point to first non-digit character
    assert(rest == &input[3]);

    // // second rest test
    // const char *input = "0x77eiscool";
    // const char *rest = NULL;
    // val = strtonum(input, &rest);
    // assert(rest == &input[5]);

    val = strtonum("0x\0", NULL);
    assert(val == 0);

    // hex to hex
    val = strtonum("0x77e", NULL);
    assert(val == 0x77e);
    // hex to dec
    val = strtonum("0x77e", NULL);
    assert(val == 1918);
    // 1 hex
    val = strtonum("0x1", NULL);
    assert(val == 1);
    // invalid chars + large hex number
    val = strtonum("0xab1cDEFvlllall", NULL);
    assert(val == 179424751);
    // more invalid chars, further up
    val = strtonum("0xahb1cDEF", NULL);
    assert(val == 10);
    // tests opposite cases for abcdef as other tests
    val = strtonum("0xAB1Cd2f", NULL);
    assert(val == 179424559);
    // tests how large the number can get
    val = strtonum("1000000000", NULL);
    assert(val == 1000000000);


}

static void test_to_base(void)
{
    char buf[20];
    size_t bufsize = sizeof(buf);

    memset(buf, 0x77, bufsize); // init contents with known value

    // // test get int size helper with decimal and hex
    // int n = get_int_size(0x7e, 16);
    // assert(n == 2);

    // int n = get_int_size(154, 16);
    // assert(n == 3);

    // n = get_int_size(0x7efff, 16);
    // assert(n == 5);

    // n = get_int_size(0x0, 16);
    // assert(n == 1);

    // // unsigned tests
    // int n = unsigned_to_base(buf, bufsize, 23, 10, 4);
    // assert(strcmp(buf, "0023") == 0);
    // assert(n == 4);

    // // test min_size = 0 and val = 0
    // int n = unsigned_to_base(buf, bufsize, 0, 10, 0);
    // assert(strcmp(buf, "0") == 0);
    // assert(n == 1);

    // // test min_size = 0 and val != 0
    // unsigned_to_base(buf, bufsize, 35, 10, 0);
    // assert(strcmp(buf, "35") == 0);

    // // test min_size != 0 and hex
    // int n = unsigned_to_base(buf, bufsize, 35, 16, 4);
    // assert(strcmp(buf, "0023")== 0);
    // assert(n == 4);

    // // test min_size = val and dec
    // unsigned_to_base (buf, bufsize, 35, 10, 2);
    // assert(strcmp(buf, "35") == 0);

    // // test large min_size = val and dec
    // unsigned_to_base (buf, bufsize, 35, 10, 10);
    // assert(strcmp(buf, "0000000035") == 0);

    // // test hex with a-f digits
    // unsigned_to_base (buf, bufsize, 0x7feabc, 16, 0);
    // assert(strcmp(buf, "7feabc") == 0);

    // // test hex with a-f digits with larger minsize
    // int n = unsigned_to_base (buf, bufsize, 0x7feabc, 16, 9);
    // assert(strcmp(buf, "0007feabc") == 0);
    // assert(n == 9);

    // below tests are for if buf is too small
    // char buf[3];
    // size_t bufsize = sizeof(buf);

    // memset(buf, 0x77, bufsize);

    // min_width < int_size
    // int n =  unsigned_to_base(buf, bufsize, 2001, 10, 2);
    // assert(strcmp(buf, "20") == 0);
    // assert(n == 4);

    // same as above with hex: min_width = int_size
    // int n =  unsigned_to_base(buf, bufsize, 0x7feabc, 16, 6);
    // assert(strcmp(buf, "7f") == 0);
    // assert(n == 6);

    // hex, min_width < int_size
    // int n =  unsigned_to_base(buf, bufsize, 0x7feabc, 16, 5);
    // assert(strcmp(buf, "7f") == 0);
    // assert(n == 6);

    // // // same as above test but with hex
    // int n =  unsigned_to_base(buf, bufsize, 0xe5f, 16, 6);
    // assert(strcmp(buf, "00") == 0);
    // assert(n == 6);

    // int_size < min_width with zeros only
    // int n =  unsigned_to_base(buf, bufsize, 1, 10, 3);
    // assert(strcmp(buf, "00") == 0);
    // assert(n == 3);

    // // above with a nonzero digit
    // int n =  unsigned_to_base(buf, bufsize, 1, 10, 2);
    // assert(strcmp(buf, "01") == 0);
    // assert(n == 2);

    // int n =  unsigned_to_base (buf, bufsize, 0x7feabc, 16, 9);
    // assert(strcmp(buf, "00") == 0);
    // assert(n == 9);

    // signed tests
    // int n = signed_to_base(buf, 5, -9999, 10, 6);
    // assert(strcmp(buf, "-099") == 0);
    // assert(n == 6);

    // num with buffer zeros
    // int n = signed_to_base(buf, 5, -49, 10, 6);
    // assert(strcmp(buf, "-00049") == 0);
    // assert(n == 6);

    // int n = signed_to_base(buf, 7, -49, 10, 6);
    // assert(strcmp(buf, "-00049") == 0);
    // assert(n == 6);

    // hex test
    // int n = signed_to_base(buf, bufsize, -0x6c2f2, 16, 1);
    // assert(strcmp(buf, "-6c2f2") == 0);
    // assert(n == 6);

    // min_width = 0
    // int n = signed_to_base(buf, bufsize, -0x6c2f2, 16, 0);
    // assert(strcmp(buf, "-6c2f2") == 0);
    // assert(n == 6);

    // min_width = 0 
    // int n = signed_to_base(buf, bufsize, -1, 16, 0);
    // assert(strcmp(buf, "-1") == 0);
    // assert(n == 2);

    // minus 0 input
    // int n = signed_to_base(buf, bufsize, -0, 16, 0);
    // assert(strcmp(buf, "0") == 0);
    // assert(n == 1);

    // make sure signed int works just like unsigned int when val is nonegative 
    // int n =  unsigned_to_base(buf, 3, 0x7feabc, 16, 9);
    // assert(strcmp(buf, "00") == 0);
    // assert(n == 9);
}

static void test_snprintf(void)
{
    char buf[100];
    size_t bufsize = sizeof(buf);


    memset(buf, 0x77, bufsize); // init contents with known value

    // Start off simple...
    // snprintf(buf, bufsize, "Hello, world!");
    // assert(strcmp(buf, "Hello, world!") == 0);

    // snprintf(buf, bufsize, "h%%w");
    // assert(strcmp(buf, "h%w") == 0);

    // // Decimal
    // snprintf(buf, bufsize, "%d", 45);
    // assert(strcmp(buf, "45") == 0);

    // inside str
    // snprintf(buf, bufsize, "HOLLA%d", 2023);
    // assert(strcmp(buf, "HOLLA2023") == 0);

    // snprintf(buf, bufsize, "HOLLA%d", 2023);
    // assert(strcmp(buf, "HOLLA2023") == 0);

    // snprintf(buf, bufsize, "%dHO%dLLA%d", 2023, 2024, 2025);
    // assert(strcmp(buf, "2023HO2024LLA2025") == 0);

    // // Hexadecimal with width
    // snprintf(buf, bufsize, "%04x", 0xef);
    // assert(strcmp(buf, "00ef") == 0);

    // hex and dec with width and strings
    // snprintf(buf, bufsize, "hello%03xcool%03d%s%07xggg", 0x3f, 123, "YOOO", 0xfff);
    // assert(strcmp(buf, "hello03fcool123YOOO0000fffggg") == 0);

    // hex with no width
    // snprintf(buf, bufsize, "%x", 0xef);
    // assert(strcmp(buf, "ef") == 0);

    // strings and hex
    // snprintf(buf, bufsize, "NINA%xBOORD", 0x8272ef);
    // assert(strcmp(buf, "NINA8272efBOORD") == 0);

    // chars and hex
    // snprintf(buf, bufsize, "%c%c%x%c", 's', 't', 0xa, 'n');
    // assert(strcmp(buf, "stan") == 0);

    // // Pointer
    // snprintf(buf, bufsize, "%p", (void *) 0x20200004);
    // assert(strcmp(buf, "0x20200004") == 0);

    // Character
    // snprintf(buf, bufsize, "%c", 'A');
    // assert(strcmp(buf, "A") == 0);

    // last index
    // snprintf(buf, bufsize, "NIN%c", 'A');
    // assert(strcmp(buf, "NINA") == 0);

    // middle index
    // snprintf(buf, bufsize, "NI%cA", 'N');
    // assert(strcmp(buf, "NINA") == 0);

    // first index
    // snprintf(buf, bufsize, "%cINA", 'N');
    // assert(strcmp(buf, "NINA") == 0);

    // char is a percent; do double check that returning correct val
    // int a = snprintf(buf, bufsize, "per%c", '%');
    // assert(strcmp(buf, "per%") == 0);
    // assert(a == 4);

    // multiple chars and with %%
    // snprintf(buf, bufsize, "%c%c = 100%% fre%ch", 'C', 'S', 's');
    // assert(strcmp(buf, "CS = 100% fresh") == 0);

    // String
    // snprintf(buf, bufsize, "%s", "binky");
    // assert(strcmp(buf, "binky") == 0);

    // snprintf(buf, bufsize, "%s%s%s%s%s%s", "i", "love", "when", "my", "tests", "pass");
    // assert(strcmp(buf, "ilovewhenmytestspass") == 0);

    // snprintf(buf, bufsize, "%s %s %s %s %s %s", "i", "love", "when", "my", "tests", "pass");
    // assert(strcmp(buf, "i love when my tests pass") == 0);

    // // Format string with intermixed codes
    // snprintf(buf, bufsize, "CS%d%c!", 107, 'e');
    // assert(strcmp(buf, "CS107e!") == 0);

    // // Test return value
    // assert(snprintf(buf, bufsize, "Hello") == 5);
    // assert(snprintf(buf, 2, "Hello") == 5);
}

// This function just here as code to disassemble for extension
int sum(int n)
{
    int result = 6;
    for (int i = 0; i < n; i++) {
        result += i * 3;
    }
    return result + 729;
}

void test_disassemble(void)
{
    const unsigned int add = 0xe0843005;
    const unsigned int sub = 0xe24bd00c;
    const unsigned int mov = 0xe3a0006b;
    const unsigned int bne = 0x1afffffa;

    // If you have not implemented the extension, core printf
    // will output address not disassembled followed by I
    // e.g.  "... disassembles to 0x07ffffd4I"
    printf("Encoded instruction %x disassembles to %pI\n", add, &add);
    printf("Encoded instruction %x disassembles to %pI\n", sub, &sub);
    printf("Encoded instruction %x disassembles to %pI\n", mov, &mov);
    printf("Encoded instruction %x disassembles to %pI\n", bne, &bne);

    unsigned int *fn = (unsigned int *)sum; // diassemble instructions from sum function
    for (int i = 0; i < 10; i++) {
        printf("%p:  %x  %pI\n", &fn[i], fn[i], &fn[i]);
    }
}


void main(void)
{
    uart_init();
    uart_putstring("Start execute main() in tests/test_strings_printf.c\n");
//    test_memset();
//     test_strcmp();
//    test_strlcat();
//     test_strtonum();
//     test_to_base();
//    test_snprintf();
    //test_disassemble();
    uart_putstring("Successfully finished executing main() in tests/test_strings_printf.c\n");
    uart_putchar(EOT);
}
