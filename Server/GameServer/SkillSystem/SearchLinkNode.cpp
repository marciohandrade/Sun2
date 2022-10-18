#include "stdafx.h"
#include "./SearchLinkNode.h"

//==================================================================================================
namespace ns_object {
//==================================================================================================
;

static void CompileAssertCheck()
{
    SearchNode::LinkNode link_node;
    ZeroMemory(&link_node, sizeof(link_node));
    BOOST_STATIC_ASSERT(
        sizeof(link_node.select.object_node) == sizeof(link_node.select.character_node) &&
        sizeof(link_node.select.object_node) == sizeof(link_node.select.player_node) &&
        sizeof(link_node.select.object_node) == sizeof(link_node.select.npc_node) &&
        sizeof(link_node.select.object_node) == sizeof(link_node.select.item_node));
};

//==================================================================================================

#if SUN_CODE_BACKUP // sample
struct SearchNodeIterator //: public SearchNodeConstIterator
{
    SearchNode::LinkNode* ClimbToNext(const SearchNodeIterator& end_it);
    SearchNode::LinkNode* node;
};

inline SearchNode::LinkNode* SearchNodeIterator::ClimbToNext(const SearchNodeIterator& end_it)
{
    while (this->node != end_it.node)
    {
        const SearchNode::LinkNode* cur_node = node;
        node = static_cast<SearchNode::LinkNode*>(node->next);

        if (cur_node->select.root_node.object_type != NULL) {
            return const_cast<SearchNode::LinkNode*>(cur_node);
        }
    }
    return NULL;
}
#endif

//==================================================================================================
}; //end of namespace
//==================================================================================================
