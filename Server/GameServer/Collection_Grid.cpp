#include "stdafx.h"
#include "Collection.h"
#include "GameZone.h"
#include "GameField.h"
#include "Sector.h"
#include "CollectionManager.h"

// 섹터안에 필요한 케릭에게만 전달.
void Collection::OnEnterObject(const Sector& IN sector)
{
    // CHANGES: f110415.6L, at this point in time, the usage case of 'OnEnterObject' only has
    // notification to users so that if the sector has no users,
    // it is no need to process contiuously.
    if (sector.GetPlayerNum() == 0) {
        return;
    };
    //
    MSG_CG_COLLECT_ACTIVE_BRD msg_brd;
    msg_brd.m_dwObjectKey  = GetObjectKey();
    msg_brd.m_CollectionID = m_pMapObjectInfo->CollectionID;

    GameField* const game_field = GetField();
    // (CHANGES) (f100504.1L) change the interface specification to support a complex condition
    // the replacement facility for SendXXPacketToSector<IsSendPlayer>
    struct CheckCollectionInfoReceiver : public IObjectFinder
    {
        CheckCollectionInfoReceiver(CollectionManager* collection_manager, WzID collection_id)
            : collection_manager_(collection_manager),
            collection_id_(collection_id)
        {}

        virtual bool operator()(Object* object) const
        {
            // must be player
            assert(object->IsEqualObjectKind(PLAYER_OBJECT));
            Player* player = static_cast<Player*>(object);
            return collection_manager_->ChkCondision(player, collection_id_) ==
                   RC::RC_COLLECT_SUCCESS;
        }

        CollectionManager* const collection_manager_;
        const WzID collection_id_;
        //
    private:
        CheckCollectionInfoReceiver(const CheckCollectionInfoReceiver&)
            : collection_manager_(NULL), collection_id_(0){};
        void operator=(const CheckCollectionInfoReceiver&){};
    } // instance
    recevier_filter(game_field->GetCollectManeger(), m_pMapObjectInfo->CollectionID);
    //
    game_field->SendPacketToSector(sector.GetSectorIndex(),
                                   &msg_brd, sizeof(msg_brd),
                                   &recevier_filter);
#if SUN_CODE_BACKUP
    GameField* const pGameField = GetField();
    IsSendPlayer op(pGameField->GetCollectManeger(), m_pMapObjectInfo->CollectionID);
    WORD size = sizeof(MSG_CG_COLLECT_ACTIVE_BRD);

    pGameField->SendPacketToSector(sector.GetSectorIndex(), &msg_brd, sizeof(MSG_CG_COLLECT_ACTIVE_BRD), op);
#endif
}

VOID Collection::OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle )
{
	NonCharacter::OnEnterField( rField, rwzVec, iAngle );
}

VOID Collection::OnLeaveField()
{
	NonCharacter::OnLeaveField();
	Release();
}
