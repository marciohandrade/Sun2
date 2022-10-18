#include "WorldServerPch.hxx"
#include "./CommonStructInWorld.h"

//==================================================================================================

void ns_world::CharStatusDataNode::Initialize()
{
    ZeroMemory(this, sizeof(*this));
    this->char_status_data_tag = this->kCharStatusDataTag;
}

ns_world::CharStatusDataNode* ns_world::CharStatusDataNode::GetContainerObject(
    const STRUCT_FRIEND_INFO* contained_item)
{
    if (contained_item == NULL) {
        return NULL;
    };
    ns_world::CharStatusDataNode* header = \
        CONTAINING_RECORD(contained_item, ns_world::CharStatusDataNode, friend_info);
    if (header->char_status_data_tag != kCharStatusDataTag) {
        return NULL;
    };

    return header;
}

ns_world::CharStatusDataNode* ns_world::CharStatusDataNode::GetContainerObject(
    const STRUCT_BLOCK_INFO* contained_item)
{
    if (contained_item == NULL) {
        return NULL;
    };
    ns_world::CharStatusDataNode* header = \
        CONTAINING_RECORD(contained_item, ns_world::CharStatusDataNode, block_info);
    if (header->char_status_data_tag != kCharStatusDataTag) {
        return NULL;
    };

    return header;
}

ns_world::CharStatusDataNode* ns_world::CharStatusDataNode::GetContainerObject(
    const STRUCT_REVENGE_INFO* contained_item)
{
    if (contained_item == NULL) {
        return NULL;
    };
    ns_world::CharStatusDataNode* header = \
        CONTAINING_RECORD(contained_item, ns_world::CharStatusDataNode, revenge_info);
    if (header->char_status_data_tag != kCharStatusDataTag) {
        return NULL;
    };

    return header;
}

//==================================================================================================


