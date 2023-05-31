#include <Command.hpp>
#include <helper.hpp>

#include <gmock/gmock.h>
#include <vector>

static char  buffer[20] = {0};
static int   int0       = 0;
static float float0     = 0.0f;
static char  char0      = '0';

static helper::PrintManagerMock mock_print;

void one_int(CommandBase& cmd, int data) {
    (void)cmd;
    int0 = data;
}

TEST(COMMAND, command_int) {
    Command     my_int("one_int", "desc", one_int);
    std::size_t parse_depth = 0;

    my_int.parse(&mock_print, (char*)"one_int 5", parse_depth);

    EXPECT_EQ(int0, 5);
    EXPECT_EQ(parse_depth, 1);

    std::memset(buffer, 0, sizeof(buffer));
    std::size_t len = my_int.get_values_info(buffer);
    EXPECT_STREQ(buffer, "i");
    EXPECT_EQ(len, 1);
}

void int_float(CommandBase& cmd, int data0, float data1) {
    (void)cmd;
    int0   = data0;
    float0 = data1;
}

TEST(COMMAND, command_int_float) {
    Command my_int("my_int", "desc", int_float);

    std::size_t parse_depth = 0;

    my_int.parse(&mock_print, (char*)"my_int 5 2.54", parse_depth);

    EXPECT_EQ(int0, 5);
    EXPECT_EQ(float0, 2.54f);
    EXPECT_EQ(parse_depth, 1);

    std::memset(buffer, 0, sizeof(buffer));
    std::size_t len = my_int.get_values_info(buffer);
    EXPECT_STREQ(buffer, "if");
    EXPECT_EQ(len, 2);
}

void int_float_char(CommandBase& cmd, int data0, float data1, char data2) {
    (void)cmd;
    int0   = data0;
    float0 = data1;
    char0  = data2;
}

TEST(COMMAND, command_int_float_char) {
    Command my_int("my_int", "desc", int_float_char);

    std::size_t parse_depth = 0;

    my_int.parse(&mock_print, (char*)"my_int 5 2.54 i", parse_depth);

    EXPECT_EQ(int0, 5);
    EXPECT_EQ(float0, 2.54f);
    EXPECT_EQ(char0, 'i');
    EXPECT_EQ(parse_depth, 1);

    std::memset(buffer, 0, sizeof(buffer));
    std::size_t len = my_int.get_values_info(buffer);
    EXPECT_STREQ(buffer, "ifc");
    EXPECT_EQ(len, 3);
}

TEST(COMMAND, command_int_float_char_missing_arg) {
    Command my_int("my_int", "desc", int_float_char);

    std::size_t parse_depth = 0;

    my_int.parse(&mock_print, (char*)"my_int 6 2.57", parse_depth);

    EXPECT_EQ(int0, 6);
    EXPECT_EQ(float0, 2.57f);
    EXPECT_EQ(char0, '\0');
    EXPECT_EQ(parse_depth, 1);
}

void double_callback(CommandBase& cmd, double d0, double d1, double d2) {
    (void)cmd;
    (void)d0;
    d0 = d1 = d2;
}

TEST(COMMAND, command_double) {
    Command double_command("my_int", "desc", double_callback);

    std::memset(buffer, 0, sizeof(buffer));
    std::size_t len = double_command.get_values_info(buffer);
    EXPECT_STREQ(buffer, "ddd");
    EXPECT_EQ(len, 3);
}

void my_command_callback(CommandBase& cmd, const char* data) {
    (void)cmd;
    strcpy(buffer, data);
}

TEST(COMMAND, command_char) {
    std::memset(buffer, 0, sizeof(buffer));
    Command     my_command("my_command", "desc", my_command_callback);
    std::size_t parse_depth = 0;
    my_command.parse(&mock_print, (char*)"my_command command", parse_depth);
    EXPECT_STREQ(buffer, "command");
    EXPECT_EQ(parse_depth, 1);

    std::memset(buffer, 0, sizeof(buffer));
    std::size_t len = my_command.get_values_info(buffer);
    EXPECT_STREQ(buffer, "c*");
    EXPECT_EQ(len, 2);
}

void void_callback(CommandBase& cmd) {
    (void)cmd;
    int0 = 10;
}

TEST(COMMAND, command_void) {
    int0 = 0;
    Command     my_command("my_command", "desc", void_callback);
    std::size_t parse_depth = 0;
    my_command.parse(&mock_print, (char*)"my_command", parse_depth);
    EXPECT_EQ(int0, 10);
    EXPECT_EQ(parse_depth, 1);

    std::memset(buffer, 0, sizeof(buffer));
    std::size_t len = my_command.get_values_info(buffer);
    EXPECT_STREQ(buffer, "v");
    EXPECT_EQ(len, 1);
}

TEST(COMMAND, getInfo) {
    int0 = 0;
    Command my_command("my_command", "desc", void_callback);

    helper::PrintManagerMock mock_print;

    my_command.get_info(mock_print);

    EXPECT_STREQ(mock_print.get().c_str(), "my_command\tdesc\n\r");
}

TEST(COMMAND, command_int_float_char_missing) {
    Command my_int("missing", "desc", int_float_char);

    std::size_t parse_depth = 0;

    my_int.parse(&mock_print, (char*)"missing", parse_depth);

    EXPECT_EQ(int0, 0);
    EXPECT_EQ(float0, 0.0f);
    EXPECT_EQ(char0, '\0');
    EXPECT_EQ(parse_depth, 1);
}

bool gat_argument_count_called = false;
void gat_argument_count(CommandBase& cmd, int i, float j, char k) {
    (void)i;
    (void)j;
    (void)k;
    EXPECT_EQ(cmd.get_args_count(), 3);
    gat_argument_count_called = true;
}

TEST(COMMAND, gat_argument_count) {
    int0 = 0;
    Command     my_command("my_command", "desc", gat_argument_count);
    std::size_t parse_depth   = 0;
    gat_argument_count_called = false;
    my_command.parse(&mock_print, (char*)"my_command 7 3.43 hello", parse_depth);

    EXPECT_TRUE(gat_argument_count_called);
}
