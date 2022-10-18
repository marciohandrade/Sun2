#include "stdafx.h"
#include <gtest/gtest.h>

//////////////////////////////////////////////////////////////////////////
//
class MolaLibTest : public ::testing::Test
{
protected:
    // per-test-case set-up
    static void SetUpTestCase() {}
    // per-test-case tear-down
    static void TearDownTestCase() {}

    // per-test set-up
    virtual void SetUp() {}
    // per-test tear-down
    virtual void TearDown() {}
};

TEST(MolaLibTest, MolaLibraryValidateHeapTest)
{

}