#include "stdafx.h"
#include <gtest/gtest.h>
#include <VendorSearchSystem/VendorSearchSystem.h>

TEST(VendorSearchSystemTest, DISABLED_VendorSearchSystemTest)
{
    VendorSearchSystem vc;
    ASSERT_TRUE(vc.initialize());

    ;{
        int count = 0;
        for (int i = 1; i <= 50; i++) {
            vc.insert( 1000+i, "������ 1",  count, 100000000 - count++);
            //vc.insert( 1000+i, "������ 2",  i, 100000000 - count++);
            vc.insert( 1000+i, "�������� 34", count, 100000000 - count++);
            vc.insert( 1000+i, "�������� 1",  count, 100000000 - count++);
            //vc.insert( 1000+i, "�������� +1", i, 100000000 - count++);
            //vc.insert( 1000+i, "�������� +6", i, 100000000 - count++);
        }

        printf("done insert...\n");
        vc.print_sql_result("select * from vendor_items;");
        vc.print_sql_result("select * from search_result;");
    };
    ;{
        //SEARCH_RESULT result1;
        //EXPECT_TRUE(vc.select("����", 0));
        //EXPECT_EQ(6, result1.row_count);
        vc.print_sql_result("select * from search_result;");
        
        //SEARCH_RESULT result2;
        //EXPECT_TRUE(vc.select("����", 0));
        //EXPECT_EQ(4, result2.row_count);
        vc.print_sql_result("select * from search_result;");
    };
    ;{
        vc.erase( 10001, 1);
    };

    vc.close();
};