#include "stdafx.h"

#include <gtest/gtest.h>

#include "TempUser.h" // Header file from WorldServer project
#include "ActiveUser.h"

TEST(SampleTest, SimpleAssignment)
{
    TempUser temp_user; // WorldServer User object
    ActiveUser active_user;

	int a = 3;
	EXPECT_EQ( a, 3 );
}
