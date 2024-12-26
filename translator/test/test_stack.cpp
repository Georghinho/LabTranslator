#include "stack.h"
#include "translator.h"
#include <gtest.h>


// ========================
// TESTS FOR STACK
// ========================

TEST(Stack, canCreateNonEmptyStack) {
    ASSERT_NO_THROW(Stack<int> S(10));
}

TEST(Stack, cannotCreateStackWithNegativeLength) {
    ASSERT_ANY_THROW(Stack<int> S(-5));
}

TEST(Stack, cannotCreateStackWithNullLength) {
    ASSERT_ANY_THROW(Stack<int> S(0));
}

TEST(Stack, canPushAndPopElement) {
    Stack<int> S(4);
    S.push(1);
    S.push(3);
    S.push(8);

    Stack<int> expectedStack(2);
    expectedStack.push(1);
    expectedStack.push(3);

    S.pop();
    ASSERT_EQ(S, expectedStack);
}

TEST(Stack, popReturnsLastElement) {
    Stack<int> S(4);
    S.push(5);
    S.push(6);
    S.push(7);

    ASSERT_EQ(S.pop(), 7);
}

TEST(Stack, cannotPopFromEmptyStack) {
    Stack<int> S(2);
    ASSERT_ANY_THROW(S.pop());
}

TEST(Stack, canPopOnlyElement) {
    Stack<int> S(1);
    S.push(9);
    ASSERT_NO_THROW(S.pop());
}

TEST(Stack, cannotPushToFullStack) {
    Stack<int> S(4);
    S.push(4);
    S.push(2);
    S.push(1);
    S.push(3);
    ASSERT_ANY_THROW(S.push(6));
}

TEST(Stack, canGetLastElement) {
    Stack<int> S(4);
    S.push(10);
    S.push(20);
    S.push(30);
    ASSERT_EQ(S.top(), 30);
}

TEST(Stack, cannotGetLastElementInEmptyStack) {
    Stack<int> S(4);
    ASSERT_ANY_THROW(S.top());
}

TEST(Stack, canGetSize) {
    Stack<int> S(4);
    S.push(1);
    S.push(2);
    ASSERT_EQ(S.size(), 2);
}

TEST(Stack, canGetSizeInEmptyStack) {
    Stack<int> S(4);
    ASSERT_EQ(S.size(), 0);
}

TEST(Stack, emptyStackReturnsTrue) {
    Stack<int> S(4);
    ASSERT_TRUE(S.empty());
}

TEST(Stack, nonEmptyStackReturnsFalse) {
    Stack<int> S(4);
    S.push(5);
    ASSERT_FALSE(S.empty());
}

// ========================
// TESTS FOR TRANSLATOR
// ========================

TEST(Translator, canCalculateWithIntegers) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("10 + 30"), 40);
}

TEST(Translator, canCalculateWithUnaryMinus) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("- 5 + 30"), 25);
}

TEST(Translator, canCalculateWithNegativeNumbers) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("-10 - 20"), -30);
    ASSERT_EQ(translator.evaluate("-2.5 - 20.5"), -23.0);
}

TEST(Translator, canMultiply) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("4 * 25"), 100);
    ASSERT_EQ(translator.evaluate("4 * ( - 25)"), -100);
    ASSERT_LE(translator.evaluate("2.0 * 3.0"), 6.0);
}

TEST(Translator, canMultiplyByZero) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("5 * 0"), 0);
    ASSERT_EQ(translator.evaluate("7.5 * 0"), 0);
}

TEST(Translator, canDivide) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("36 / 6"), 6);
    ASSERT_EQ(translator.evaluate("15.5 / 5"), 3.1);
}

TEST(Translator, cannotDivideByZero) {
    Translator translator;
    ASSERT_ANY_THROW(translator.evaluate("15 / 0"));
    ASSERT_ANY_THROW(translator.evaluate("15.5 / 0"));
}

TEST(Translator, canDeterminePriorityPlusAndMultiply) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("3 + 3 * 3"), 12);
}

TEST(Translator, canDeterminePriorityMinusAndMultiply) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("4 - 2 * 3"), -2);
}

TEST(Translator, canDeterminePriorityPlusAndDivide) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("6 + 6 / 2"), 9);
}

TEST(Translator, canDeterminePriorityMinusAndDivide) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("8 - 4 / 2"), 6);
}

TEST(Translator, canDeterminePriorityWithParentheses) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("3 * ( 2 + 5 )"), 21);
    ASSERT_EQ(translator.evaluate("45 / ( 5 + 5 )"), 4.5);
}

TEST(Translator, cannotCalculateUnmatchedParentheses) {
    Translator translator;
    ASSERT_ANY_THROW(translator.evaluate("3 + ( ( 12 / 4"));
    ASSERT_ANY_THROW(translator.evaluate("3 + ( ( 12 / 4 )"));
    ASSERT_ANY_THROW(translator.evaluate("3 + 12 / 4 ) )"));
    ASSERT_ANY_THROW(translator.evaluate("3 + ( 12 / 4 ) )"));
}

TEST(Translator, cannotCalculateDotBeforeNumberWithoutZero) {
    Translator translator;
    ASSERT_ANY_THROW(translator.evaluate(".75 + 2000"));
}

TEST(Translator, canCalculateMixedIntegersAndDoubles) {
    Translator translator;
    ASSERT_EQ(translator.evaluate("2.5 + 40"), 42.5);
}



