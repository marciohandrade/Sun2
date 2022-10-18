#include "stdafx.h"
#include <gtest/gtest.h>
#include "DBCharacter.h"
#include <TinyXml/tinyxml.h>
#include <atltime.h>

TEST(DBCharacterTest, MemberSizeCheck)
{
    DBCharacter dbcharacter;
    //dbcharacter.Initialize();

    printf("DBCharacter size is %d \n", \
        sizeof(DBCharacter));

    printf("DBCharacter_Achievements size is %d \n", \
        sizeof(DBCharacter_Achievements)); // 14002
    printf("REPURCHASITEM_TOTAL_INFO : %d\n", \
        sizeof(REPURCHASITEM_TOTAL_INFO /*repurchase_items_*/)); // 226 byte
    printf("CHAR_INVITATEDFRIEND_INFOs : %d\n", \
        sizeof(CHAR_INVITATEDFRIEND_INFOs /*Invitatedfriend_count_*/)); // 1001 byte
    printf("nsSlot::InventoryConfig : %d\n", \
        sizeof(nsSlot::InventoryConfig /*inventory_config_*/)); // 20
    printf("nsQuery::DBQueryProxyCharNode : %d\n", \
        sizeof(nsQuery::DBQueryProxyCharNode /*db_query_node_*/)); // 34992 byte
    ;

    printf("sQUERY_CHARACTER : %d\n", sizeof(sQUERY_CHARACTER));
    
}

void test_DBCharacterTest()
{
    DBCharacter db_character;

    TiXmlDocument doc;
    TiXmlElement* toggle_info = new TiXmlElement("TOGGLE_ITEM");
    doc.LinkEndChild(toggle_info);
    
    //nsSlot::TOGGLE_ITEMS* toggle_item_info = db_character.GetToggleItemInfo();
    //ToggleItemType toggle_item_enum[] = { 
    //    kToggleItem_Badge,
    //    kToggleItem_Badge, // test
    //    kToggleItem_Badge, // test
    //};
    //for (int i = 0; i < sizeof(toggle_item_enum)/sizeof(toggle_item_enum[0]); i++)
    //{
    //    ToggleItemType toggle_item_type = toggle_item_enum[i];

    //    TiXmlElement* item_info = new TiXmlElement("ITEM_INFO");
    //    item_info->SetAttribute("TYPE",  toggle_item_type);
    //    item_info->SetAttribute("EQUIP", toggle_item_info[toggle_item_type].is_equiped_);
    //    item_info->SetAttribute("POS",   toggle_item_info[toggle_item_type].item_pos_);
    //    item_info->SetAttribute("CODE",  toggle_item_info[toggle_item_type].item_code_);

    //    toggle_info->LinkEndChild(item_info);
    //}
    
    int toggle_item_enum[] = { 
        1, 2, 3,
    };
    for (int i = 0; i < sizeof(toggle_item_enum)/sizeof(toggle_item_enum[0]); i++)
    {
        int toggle_item_type = toggle_item_enum[i];

        TiXmlElement* item_info = new TiXmlElement("ITEM_INFO");
        item_info->SetAttribute("TYPE",  toggle_item_type);
        item_info->SetAttribute("EQUIP", toggle_item_type);
        item_info->SetAttribute("POS",   toggle_item_type);
        item_info->SetAttribute("CODE",  toggle_item_type);

        toggle_info->LinkEndChild(item_info);
    }

    //doc.Print();

    TiXmlPrinter printer;
    doc.Accept(&printer);
    const char* xml_string = printer.CStr();
    printf("%s", xml_string);
    printf("%d", strlen(xml_string));

    TiXmlDocument character_info_ex_doc;
    character_info_ex_doc.Parse(xml_string);

    ;{
        TiXmlPrinter printer;
        doc.Accept(&printer);
        const char* xml_string = printer.CStr();
        printf("%s", xml_string);
        printf("%d", strlen(xml_string));
    };

    TiXmlElement* toggle_item_elem = character_info_ex_doc.FirstChildElement("TOGGLE_ITEM");
    if (toggle_item_elem)
    {
        for (TiXmlElement* elem = toggle_item_elem->FirstChildElement("ITEM_INFO");
            elem; elem = elem->NextSiblingElement())
        {
            int toggle_item_type, is_equiped, item_pos, item_code;
            elem->QueryIntAttribute("TYPE",  &toggle_item_type);
            elem->QueryIntAttribute("EQUIP", &is_equiped);
            elem->QueryIntAttribute("POS",   &item_pos);
            elem->QueryIntAttribute("CODE",  &item_code);

            printf("%d, %d, %d, %d\n", toggle_item_type, is_equiped, item_pos, item_code);
        }
    }
}

void test_CTime_String()
{
    CTime current_time  = util::TimeSync::_time64(NULL);
    
    TiXmlDocument doc;
    TiXmlPrinter printer;
    const char* xml_string = NULL;

    char datetime_string[MAX_DATETIME_SIZE+1] = { 0, };
    util::GetSQLDateTime_YYYYMMDDHHMMSSNNN( \
        util::GetDateTime_YYYYMMDDHHMMSS(current_time.GetTime())*1000, datetime_string);
    
    TiXmlElement* daily_quest_reset_time_elem = new TiXmlElement("DAILY_QUEST_RESET_TIME");
    daily_quest_reset_time_elem->SetAttribute("TIME", datetime_string);

    doc.LinkEndChild(daily_quest_reset_time_elem);
    doc.Accept(&printer);
    xml_string = printer.CStr();

    printf("%s\n", xml_string);

    //////////////////////////////////////////////////////////////////////////

    const char* datetime = daily_quest_reset_time_elem->Attribute("TIME");

    struct tm s_tm;
    sscanf(datetime_string, "%4d-%2d-%2d %2d:%2d:%2d",
        &s_tm.tm_year, &s_tm.tm_mon, &s_tm.tm_mday, 
        &s_tm.tm_hour, &s_tm.tm_min, &s_tm.tm_sec);

    ;{
        CTime current_time = util::TimeSync::_time64(NULL);
        CTime last_daily_init_time = CTime(current_time.GetYear(), 
            current_time.GetMonth(),
            current_time.GetDay(), 
            4,
            0,
            0);
        if (last_daily_init_time > current_time) {
            last_daily_init_time -= CTimeSpan(1, 0, 0, 0);
        }

        DWORD64 last_init_time = util::GetDateTime_YYYYMMDDHHMMSS(last_daily_init_time.GetTime());
        printf("%ld", last_init_time);
    };
}

TEST(DBCharacterTest, xml_string_test)
{
    test_DBCharacterTest();
    test_CTime_String();
}