#pragma once



#define MAX_GUIDANCE_PARTY_NUM			8		// ???̴????? ???? ?ִ? ??Ƽ ????
#define MAX_GUIDANCE_MEMBER_NUM			80		// ???̴????? ???? ?ִ? ???? ??

#define PARTY_MASTER_SUMMON_AROUND_DIST 2		// ??Ƽ?? ??ó ??ȯ ?Ÿ?

enum eGUIDANCE_TYPE
{
	eGUIDANCE_NORMAL,
	eGUIDANCE_MAX
};

enum eITEM_DISTRIBUTION_TYPE
{
	eITEM_DISTRIBUTION_DEFAULT,				//????ȹ??(?⺻??)
	eITEM_DISTRIBUTION_FIFO_PRIORITY,		//???ʴ??? ȹ??(????????..)
	eITEM_DISTRIBUTION_MASTER_PRIORITY,		//??Ƽ???? ȹ??
	eITEM_DISTRIBUTION_RANDOM,				//????ȹ??
	eITEM_DISTRIBUTION_CLASS_PRIORITY,		//?????? ?켱 ???? ȹ??
    eITEM_DISTRIBUTION_DICE_AND_DEFAULT,    //?ֻ??? ???? ȹ?? ????     _NA_003033_20110831_DICECAST_ITEM_ROUTING
    eITEM_DISTRIBUTION_DICE_AND_FIFO,       //?ֻ??? ???ʴ??? ȹ?? ???? _NA_003033_20110831_DICECAST_ITEM_ROUTING
	eITEM_DISTRIBUTION_MAX,
};

enum 
{
    eITEM_DISTRIBUTION_MAX_DICE_BEFORE = eITEM_DISTRIBUTION_DICE_AND_DEFAULT,
};