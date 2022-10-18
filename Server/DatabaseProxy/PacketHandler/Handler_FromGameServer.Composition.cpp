////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   CompositionOrDecompositionHandler.cpp
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2010/11/29 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./Handler_FromGameServer.Composition.h"
//
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/shared_ptr.hpp>


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

CompositionOrDecompositionHandler::CompositionOrDecompositionHandler()
{
}

CompositionOrDecompositionHandler::~CompositionOrDecompositionHandler()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Implement
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace util {
namespace internal {
// �Ҹ����� ���ڷ� ��ȯ�Ѵ�.
static int BooleanToNumber(const bool boolean)
{
    return (boolean ? 1 : 0);
}
// ���ڸ� ���ڷ� ��ȯ�Ѵ�.
template <typename NUMBER_T>
static bool StringToNumber(const std::string& str, NUMBER_T& OUT number)
{
    try
    {
        number = boost::lexical_cast<NUMBER_T>(str);
    }
    catch (boost::bad_lexical_cast&)
    {
        return false;
    }
    return true;
}
// �ռ� �Ǵ� ���� ������ ���� ������ ����� ���ڿ��� ���ڵ��Ѵ�.
static bool EncodeUsedItemList(
    const ItemCompositionOrDecompositionHistory& history, 
    std::string& OUT used_item_list_string)
{
    enum { kMaxUsedItemListString = 255 };
    enum { kMaxUsedItemListSize = _countof(history.used_item_list) };
    if (history.used_item_list_size > ItemCompositionOrDecompositionHistory::kMaxUsedItemSize || 
        history.used_item_list_size > kMaxUsedItemListSize)
    {
        return false;
    }
    std::ostringstream oss;
    for (ItemCompositionOrDecompositionHistory::UsedItemListSize i = 0; 
        i < history.used_item_list_size; 
        ++i)
    {
        const ItemCompositionOrDecompositionHistory::UsedItem& used_item = 
            history.used_item_list[i];
        oss << BooleanToNumber(used_item.is_material); //  ��Ῡ��
        oss << ",";
        oss << used_item.item_code; // �������ڵ�
        oss << ",";
        oss << static_cast<int>(used_item.number_of_item); // �����۰���
        oss << ",";
        oss << static_cast<int>(used_item.ig_type); // ����������
        if (i != (history.used_item_list_size-1))
        {
            oss << ";";
        }
    }
    used_item_list_string = oss.str();
    if (used_item_list_string.size() > kMaxUsedItemListString)
    {
        return false;
    }
    return true;
}
// ���� ������ ��� ���ڿ��κ��� �ռ� �Ǵ� ���� ������ ���� ������ ����� ���ڵ��Ѵ�.
static bool DecodeUsedItemList(
    const char* used_item_list_string, 
    ItemCompositionOrDecompositionHistory& OUT history)
{
    typedef boost::char_separator<char> Seperator;
    typedef boost::tokenizer<Seperator> Tokenizer;
    const Seperator used_item_sep(";");
    const Seperator used_item_member_sep(",");

    std::string temp_used_item_list_string(used_item_list_string);
    const Tokenizer used_item_token(temp_used_item_list_string, used_item_sep);
    for (Tokenizer::const_iterator used_item_pos = used_item_token.begin(); 
        used_item_pos != used_item_token.end();
        ++used_item_pos)
    {
        // ���� �������� �����Ѵ�.
        ItemCompositionOrDecompositionHistory::UsedItem used_item;
        {
            enum UsedItemMemberIndex
            {
                kIsMaterial = 0, 
                kItemCode, 
                kNumberOfItem, 
                kIGType, 
                kMaxMemberSize
            };

            const std::string& used_item_string = *used_item_pos;
            const Tokenizer used_item_member_token(used_item_string, used_item_member_sep);
            Tokenizer::const_iterator used_item_member_pos = used_item_member_token.begin();
            
            for (int used_item_member_index = 0; 
                used_item_member_index < kMaxMemberSize; 
                ++used_item_member_index)
            {
                if (used_item_member_pos == used_item_member_token.end())
                {
                    return false;
                }
                const std::string& used_item_member = *used_item_member_pos;
                switch (used_item_member_index)
                {
                case kIsMaterial:
                    {
                        int converted_member = 0;
                        if (!StringToNumber(used_item_member, converted_member))
                        {
                            return false;
                        }
                        used_item.is_material = (converted_member == 1);
                    }
                    break;
                case kItemCode:
                    if (!StringToNumber(used_item_member, used_item.item_code))
                    {
                        return false;
                    }
                    break;
                case kNumberOfItem:
                    {
                        int converted_member = 0;
                        if (!StringToNumber(used_item_member, converted_member))
                        {
                            return false;
                        }
                        used_item.number_of_item = static_cast<DURATYPE>(converted_member);
                    }
                    break;
                case kIGType:
                    {
                        int converted_member = 0;
                        if (!StringToNumber(used_item_member, converted_member))
                        {
                            return false;
                        }
                        typedef ItemCompositionOrDecompositionHistory::UsedItem::IGType IGType;
                        used_item.ig_type = static_cast<IGType>(converted_member);
                    }
                    break;
                default:
                    assert(false);
                    return false;
                }
                ++used_item_member_pos;
            }
        }
        if (!history.AddUsedItem(used_item))
        {
            return false;
        }
    }
    return true;
}
// ������ ������ ���� ����� ���ڵ��Ѵ�.
static bool EncodeUpdateConditionList(
    const MSG_DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD& update_histories_cmd, 
    std::string& OUT condition_list_string)
{
    enum { kMaxConditionListStringSize = 255 };
    enum { kMaxUpdateConditionListSize = 
        sizeof(update_histories_cmd.modify_condition_list) / sizeof(update_histories_cmd.modify_condition_list[0]) };
    typedef MSG_DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD UpdateHistoriesCmd;
    if (update_histories_cmd.condition_list_size > UpdateHistoriesCmd::kMaxConditionListSize || 
        update_histories_cmd.condition_list_size > kMaxUpdateConditionListSize)
    {
        return false;
    }
    std::ostringstream oss;
    for (UpdateHistoriesCmd::ConditionListSize i = 0; i < update_histories_cmd.condition_list_size; ++i)
    {
        const ItemCompositionOrDecompositionHistory::ModifyCondition& modify_condition = 
            update_histories_cmd.modify_condition_list[i];
        oss << modify_condition.history_id; // �������̵�
        oss << ",";
        oss << BooleanToNumber(modify_condition.is_memorize); // ��￩��
        if (i != (update_histories_cmd.condition_list_size-1))
        {
            oss << ";";
        }
    }
    condition_list_string = oss.str();
    if (condition_list_string.size() > kMaxConditionListStringSize)
    {
        return false;
    }
    return true;
}
// ���� ��û�� �����ؼ� ���� �����߿��� �ٸ� ������ ���� �ʴ� �뵵�� ����Ѵ�.
class InterTransactionManager
{
    typedef DWORD UserKey;
    typedef int UserAction;
    struct Transaction // Ʈ�����
    {
        enum { kMaxTransactionWaitTime = 1 * 1000 * 60 }; // Ʈ����� ���ð�
        Transaction() : is_running(false)
        {
            wait_timer.Clear();
        }
        bool Begin()
        {
            if (!is_running)
            {
                is_running = true;
                wait_timer.SetTimer(kMaxTransactionWaitTime);
                return true;
            }
            if (wait_timer.IsExpired(false)) // �������� ��� ��� �ð��� �������� �˻��Ѵ�.
            {
                wait_timer.SetTimer(kMaxTransactionWaitTime);
                return true;
            }
            return false;
        }
        bool is_running;
        util::Timer wait_timer;
    };
    typedef unsigned __int64 TransactionKey;
    typedef boost::shared_ptr<Transaction> TransactionPtr;
    typedef STLX_HASH_MAP<TransactionKey, TransactionPtr> TransactionMap;
private:
    InterTransactionManager() {}
public:
    static InterTransactionManager& GetInstance()
    {
        static InterTransactionManager instance;
        return instance;
    }
    bool BeginTransaction(const UserKey user_key, const UserAction user_action)
    {
        const TransactionKey transaction_key = MakeTransactionKey(user_key, user_action);
        TransactionMap::iterator pos = transaction_map_.find(transaction_key);
        if (pos != transaction_map_.end())
        {
            return (pos->second)->Begin();
        }
        TransactionPtr new_transaction(new Transaction);
        transaction_map_[transaction_key] = new_transaction;
        return new_transaction->Begin();
    }
    void EndTransaction(const UserKey user_key, const UserAction user_action)
    {
        const TransactionKey transaction_key = MakeTransactionKey(user_key, user_action);
        transaction_map_.erase(transaction_key);
    }
private:
    TransactionKey MakeTransactionKey(const UserKey user_key, const UserAction user_action) const
    {
        BOOST_STATIC_ASSERT(sizeof(UserKey) <= (sizeof(TransactionKey) / 2));
        BOOST_STATIC_ASSERT(sizeof(UserAction) <= (sizeof(TransactionKey) / 2));
        TransactionKey user_action_part = (((TransactionKey)user_action) & 0xffffffff) << 16;
        user_action_part = user_action_part << 16;
        return (((TransactionKey)user_key) & 0xffffffff) | user_action_part;
    }
    TransactionMap transaction_map_;
    __DISABLE_COPY(InterTransactionManager);
};
} // util
} // internal


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

void CompositionOrDecompositionHandler::HandleInsertHistory( 
    ChildServerSession* const server_session, 
    const MSG_BASE* const message, 
    const MessageSize message_size) const
{
    const MSG_DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD* const insert_history_cmd = 
        static_cast<const MSG_DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD* const>(message);

#ifdef _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK
    // �߱������� ������� �ʴ´�.
    return;
#endif
    
    // ��Ŷ ũ�⸦ �˻��Ѵ�.
    if (insert_history_cmd->GetSize() != message_size) 
    {
        assert(false);
        return;
    }
    
    // ��û�ڸ� �˻��Ѵ�.
    const DWORD user_key = message->m_dwKey;
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (!requester)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��û�ڰ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            return;
        }
        if (!requester->GetConnectedChar())
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�������� ĳ���Ͱ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            return;
        }
    }
    
    const ItemCompositionOrDecompositionHistory& history = 
        insert_history_cmd->new_composition_or_decomposition_history;
    
    // ���� ������ ����� ���ڵ��Ѵ�.
    std::string used_item_list_string;
    if (!util::internal::EncodeUsedItemList(history, used_item_list_string))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|���� ������ ��� ���ڵ��� �����߽��ϴ�.|UserKey = %u|"), 
               __FUNCTION__, 
               user_key);
        return;
    }
    const char* used_item_list_c_str = used_item_list_string.c_str();

    // Ʈ������� �����Ѵ�.
    if (!util::internal::InterTransactionManager::GetInstance().BeginTransaction(
        user_key, DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|Ʈ����� ���ۿ� �����߽��ϴ�.|UserKey = %u|"), 
               __FUNCTION__, 
               user_key);
        return;
    }

    // ���� ���ڿ��� �����.
    STRING_SAFER_QUERY512 query_string;
    query_string.MakeString(
        _T("{?=call S_CompositionOrDecompositionHistory_Insert(%u,%u,%u,%I64d,'%s')}"), 
        requester->GetUserGuid(), // 1. �����
        static_cast<int>(history.request_type), // 2. ��û����
        util::internal::BooleanToNumber(history.is_composition), // 3. �ռ�����
        static_cast<__int64>(history.history_date_time), // 4. ��������
        used_item_list_c_str); // 5. ���Ⱦ����۸��
    
    // ������ �����Ѵ�.
    CompositionOrDecompositionHistoryInsertQuery* const query = 
        CompositionOrDecompositionHistoryInsertQuery::ALLOC();
    {
        query->SetUserKey(message->m_dwKey);
        query->SetQuery(query_string);
        query->SetVoidObject(server_session);
    }
    requester->DBQuery(DG_OTHER, DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_DBR, query);
}

void CompositionOrDecompositionHandler::HandleInsertHistoryDBR( 
    ChildServerSession* const server_session, 
    const MSG_BASE* const message, 
    const MessageSize message_size) const
{
    __UNUSED(server_session);
    __UNUSED(message_size);
    
    // ������ ��´�.
    const MSG_DBPROXY_RESULT* const db_result = static_cast<const MSG_DBPROXY_RESULT* const>(message);
    CompositionOrDecompositionHistoryInsertQuery* query = 
        static_cast<CompositionOrDecompositionHistoryInsertQuery*>(db_result->m_pData);

    // Ʈ������� �����Ѵ�.
    const DWORD user_key = query->GetUserKey();
    util::internal::InterTransactionManager::GetInstance().EndTransaction(
        user_key, DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD);

    // ���� ���� ���� ���� �����Ѵ�.
    bool dummy_loop = false;
    do 
    {
        // ��û�ڸ� �˻��Ѵ�.
        const DBUser* const requester = g_DBUserManager.FindUser(user_key);
        {
            if (!requester)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|��û�ڰ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                       __FUNCTION__, 
                       user_key);
                break;
            }
            if (!requester->GetConnectedChar())
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|�������� ĳ���Ͱ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                       __FUNCTION__, 
                       user_key);
                break;
            }
        }

        // ������ �����ߴ��� �˻��Ѵ�.
        if (!query->IsSuccess())
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���� ������ ���� �߽��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            break;
        }
    } while (dummy_loop);
    CompositionOrDecompositionHistoryInsertQuery::FREE(query);
}

void CompositionOrDecompositionHandler::HandleSelectHistories( 
    ChildServerSession* const server_session, 
    const MSG_BASE* const message, 
    const MessageSize message_size) const
{
    const MSG_DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN* const select_histories_syn = 
        static_cast<const MSG_DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN* const>(message);

#ifdef _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK
    return;
#endif // _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK
    
    // ��Ŷ ũ�⸦ �˻��Ѵ�.
    if (select_histories_syn->GetSize() != message_size) 
    {
        assert(false);
        return;
    }

    // ��û�ڸ� �˻��Ѵ�.
    const DWORD user_key = message->m_dwKey;
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (!requester)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��û�ڰ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            return;
        }
        if (!requester->GetConnectedChar())
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�������� ĳ���Ͱ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            return;
        }
    }

    // Ʈ������� �����Ѵ�.
    if (!util::internal::InterTransactionManager::GetInstance().BeginTransaction(
        user_key, DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|Ʈ����� ���ۿ� �����߽��ϴ�.|UserKey = %u|"), 
               __FUNCTION__, 
               user_key);
        return;
    }

    // ���� ���ڿ��� �����.
    STRING_SAFER_QUERY128 query_string;
    query_string.MakeString(
        _T("{?=call S_CompositionOrDecompositionHistory_Select(%u,%u,%u)}"), 
        requester->GetUserGuid(), // 1. �����
        static_cast<int>(select_histories_syn->request_type), // 2. ��û����
        util::internal::BooleanToNumber(select_histories_syn->is_composition)); // 3. �ռ�����

    // ������ �����Ѵ�.
    CompositionOrDecompositionHistorySelectQuery* const query = 
        CompositionOrDecompositionHistorySelectQuery::ALLOC();
    {
        query->SetUserKey(message->m_dwKey);
        query->SetQuery(query_string);
        query->SetVoidObject(server_session);
    }
    requester->DBQuery(DG_OTHER, DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR, query);
}

void CompositionOrDecompositionHandler::HandleSelectHistoriesDBR( 
    ChildServerSession* const server_session, 
    const MSG_BASE* const message, 
    const MessageSize message_size) const
{
    __UNUSED(server_session);
    __UNUSED(message_size);
    
    // ������ ��´�.
    const MSG_DBPROXY_RESULT* const db_result = static_cast<const MSG_DBPROXY_RESULT* const>(message);
    CompositionOrDecompositionHistorySelectQuery* query = 
        static_cast<CompositionOrDecompositionHistorySelectQuery*>(db_result->m_pData);
    
    // Ʈ������� �����Ѵ�.
    const DWORD user_key = query->GetUserKey();
    util::internal::InterTransactionManager::GetInstance().EndTransaction(
        user_key, DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN);

    // ��û�ڸ� �˻��Ѵ�.
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (!requester)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��û�ڰ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            CompositionOrDecompositionHistorySelectQuery::FREE(query);
            return;
        }
        if (!requester->GetConnectedChar())
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�������� ĳ���Ͱ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            CompositionOrDecompositionHistorySelectQuery::FREE(query);
            return;
        }
    } 

    // ������ �����ؼ� ������.
    MSG_DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK response;
    response.Init();
    bool dummy_loop = false;
    do 
    {
        // ������ �����ߴ��� �˻��Ѵ�.
        if (!query->IsSuccess())
        {
            response.result_code = RC::RC_ITEM_FAILED_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���� ������ ���� �߽��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key); 
            break;
        }

        // �������� �˻��Ѵ�.
        ULONG number_of_result_row = query->GetResultRowNum();
        {
            enum { kMaxHistoryListSize = sizeof(response.history_list) / sizeof(response.history_list[0]) };
            if (number_of_result_row > kMaxHistoryListSize)
            {
                number_of_result_row = kMaxHistoryListSize;
            }
        }
        
        // ������ �����Ѵ�.
        for (ULONG result_row_index = 0; result_row_index < number_of_result_row; ++result_row_index)
        {
            ItemCompositionOrDecompositionHistory& response_history = 
                response.history_list[response.history_list_size];
            {
                const CompositionOrDecompositionHistorySelectQuery::ResultSet& result_row = 
                    query->result_set[result_row_index];
                
                response_history.history_id = result_row.history_id;
                response_history.request_type = result_row.request_type;
                response_history.is_memorized = (result_row.is_memorized == 1);
                response_history.is_composition = (result_row.is_composition == 1);
                response_history.history_date_time = result_row.history_date_time;
                
                if (!util::internal::DecodeUsedItemList(
                    result_row.used_item_list_string, response_history))
                {
                    // ��� ������ ���ڵ��� �������� ��� ��ü ���а� �ƴ϶� ���� ������ ��� ó���Ѵ�.
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|���� ������ ��� ���ڵ��� ���� �߽��ϴ�.|UserKey = %u, HistoryID = %u|"), 
                           __FUNCTION__, 
                           user_key, 
                           result_row.history_id);
                    continue;
                }
            }
            ++response.history_list_size;
        }
        response.result_code = RC::RC_ITEM_SUCCESS_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES;
    } while (dummy_loop);
    // NOTE: f110429.8L, report about data layouts, low data density.
    requester->SendPacketToGameServer(&response, response.GetSize());
    CompositionOrDecompositionHistorySelectQuery::FREE(query);
}

void CompositionOrDecompositionHandler::HandleUpdateHistories( 
    ChildServerSession* const server_session, 
    const MSG_BASE* const message, 
    const MessageSize message_size) const
{
    const MSG_DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD* const update_histories_cmd = 
        static_cast<const MSG_DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD* const>(message);

#ifdef _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK
    return;
#endif // _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK

    // ��Ŷ ũ�⸦ �˻��Ѵ�.
    if (update_histories_cmd->GetSize() != message_size) 
    {
        assert(false);
        return;
    }

    // ��û�ڸ� �˻��Ѵ�.
    const DWORD user_key = message->m_dwKey;
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (!requester)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��û�ڰ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            return;
        }
        if (!requester->GetConnectedChar())
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�������� ĳ���Ͱ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            return;
        }
    }

    // ���� ���� ����� ���ڵ��Ѵ�.
    std::string condition_list_string;
    if (!util::internal::EncodeUpdateConditionList(*update_histories_cmd, condition_list_string))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|���� ���� ��� ���ڵ��� �����߽��ϴ�.|UserKey = %u|"), 
               __FUNCTION__, 
               user_key);
        return;
    }
    const char* condition_list_c_str = condition_list_string.c_str();

    // Ʈ������� �����Ѵ�.
    if (!util::internal::InterTransactionManager::GetInstance().BeginTransaction(
        user_key, DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|Ʈ����� ���ۿ� �����߽��ϴ�.|UserKey = %u|"), 
               __FUNCTION__, 
               user_key);
        return;
    }

    // ���� ���ڿ��� �����.
    STRING_SAFER_QUERY512 query_string;
    query_string.MakeString(
        _T("{?=call S_CompositionOrDecompositionHistory_Update('%s')}"), 
        condition_list_c_str); // 1. ���� ���� ���
    
    // ������ �����Ѵ�.
    CompositionOrDecompositionHistoryUpdateQuery* const query = 
        CompositionOrDecompositionHistoryUpdateQuery::ALLOC();
    {
        query->SetUserKey(message->m_dwKey);
        query->SetQuery(query_string);
        query->SetVoidObject(server_session);
    }
    requester->DBQuery(DG_OTHER, DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR, query);
}

void CompositionOrDecompositionHandler::HandleUpdateHistoriesDBR( 
    ChildServerSession* const server_session, 
    const MSG_BASE* const message, 
    const MessageSize message_size) const
{
    __UNUSED(server_session);
    __UNUSED(message_size);

    // ������ ��´�.
    const MSG_DBPROXY_RESULT* const db_result = static_cast<const MSG_DBPROXY_RESULT* const>(message);
    CompositionOrDecompositionHistoryUpdateQuery* query = 
        static_cast<CompositionOrDecompositionHistoryUpdateQuery*>(db_result->m_pData);

    // Ʈ������� �����Ѵ�.
    const DWORD user_key = query->GetUserKey();
    util::internal::InterTransactionManager::GetInstance().EndTransaction(
        user_key, DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD);

    // ���� ���� ���� ���� �����Ѵ�.
    bool dummy_loop = false;
    do 
    {
        // ��û�ڸ� �˻��Ѵ�.
        const DBUser* const requester = g_DBUserManager.FindUser(user_key);
        {
            if (!requester)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|��û�ڰ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                       __FUNCTION__, 
                       user_key);
                break;
            }
            if (!requester->GetConnectedChar())
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|�������� ĳ���Ͱ� �������� �ʽ��ϴ�.|UserKey = %u|"), 
                       __FUNCTION__, 
                       user_key);
                break;
            }
        }

        // ������ �����ߴ��� �˻��Ѵ�.
        if (!query->IsSuccess())
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���� ������ ���� �߽��ϴ�.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            break;
        }
    } while (dummy_loop);
    CompositionOrDecompositionHistoryUpdateQuery::FREE(query);
}