#include "Tokenizer.h"
#include <gtest/gtest.h>

class TokenizerTest : public ::testing::Test { };

TEST_F(TokenizerTest, TestBasicCommandInput)
{
    LibRepl::Tokenizer tokenizer("cd /home/user/code/lake");
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 2);
    ASSERT_EQ(tokens[0].type, LibRepl::TokenType::Identifier);
    ASSERT_EQ(tokens[1].type, LibRepl::TokenType::Identifier);
    ASSERT_EQ(tokens[0].value, "cd");
    ASSERT_EQ(tokens[1].value, "/home/user/code/lake");
}

TEST_F(TokenizerTest, TestBasicCommandWithDashParams)
{
    LibRepl::Tokenizer tokenizer("ls -latr");
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 2);
    ASSERT_EQ(tokens[0].type, LibRepl::TokenType::Identifier);
    ASSERT_EQ(tokens[1].type, LibRepl::TokenType::Identifier);
    ASSERT_EQ(tokens[0].value, "ls");
    ASSERT_EQ(tokens[1].value, "-latr");
}

TEST_F(TokenizerTest, TestBasicCommandPipe)
{
    LibRepl::Tokenizer tokenizer("ls -latr | grep hello");
    auto tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 5);
    ASSERT_EQ(tokens[0].type, LibRepl::TokenType::Identifier);
    ASSERT_EQ(tokens[1].type, LibRepl::TokenType::Identifier);
    ASSERT_EQ(tokens[2].type, LibRepl::TokenType::Pipe);
    ASSERT_EQ(tokens[3].type, LibRepl::TokenType::Identifier);
    ASSERT_EQ(tokens[4].type, LibRepl::TokenType::Identifier);
    ASSERT_EQ(tokens[0].value, "ls");
    ASSERT_EQ(tokens[1].value, "-latr");
    ASSERT_EQ(tokens[2].value, "|");
    ASSERT_EQ(tokens[3].value, "grep");
    ASSERT_EQ(tokens[4].value, "hello");
}
