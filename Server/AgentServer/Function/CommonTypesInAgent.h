
//==================================================================================================
// <Common Types In AgentServer>

class UserSession;
class User;
class TempUser;

typedef STLX_HASH_MAP<DWORD, User*>		USER_HASH;
typedef USER_HASH::iterator				USER_HASH_IT;
typedef USER_HASH::value_type			USER_HASH_PAIR;


//==================================================================================================
// instance iterating factility for UserManager::Foreach, Room::Foreach
namespace ns_functor {
;

struct IUserForeachFunctor
{
    virtual bool operator()(User* const user) = 0;
};

}; //end of namespace
//==================================================================================================
