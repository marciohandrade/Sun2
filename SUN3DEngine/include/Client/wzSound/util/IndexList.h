/****************************************************************************************/
/*  IndexList.h																			*/
/*                                                                                      */
/*  Description:    Used to manage an indexed list of items								*/
/*                                                                                      */
/*  The contents of this file are subject to the Genesis3D Public License               */
/*  Version 1.01 (the "License"); you may not use this file except in                   */
/*  compliance with the License. You may obtain a copy of the License at                */
/*  http://www.genesis3d.com                                                            */
/*                                                                                      */
/*  Software distributed under the License is distributed on an "AS IS"                 */
/*  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See                */
/*  the License for the specific language governing rights and limitations              */
/*  under the License.                                                                  */
/*                                                                                      */
/*  Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved           */
/*                                                                                      */
/****************************************************************************************/
#ifndef INDEXLIST_H
#define INDEXLIST_H
#pragma warning( disable : 4068 )


#ifdef __cplusplus
	extern "C" {
#endif


////////////////////////////////////////////////////////////////////////////////////////
//	IndexList struct
////////////////////////////////////////////////////////////////////////////////////////
typedef struct IndexList	IndexList;


////////////////////////////////////////////////////////////////////////////////////////
//	Prototypes
////////////////////////////////////////////////////////////////////////////////////////

//	Get the size of a list.
//
////////////////////////////////////////////////////////////////////////////////////////
int IndexList_GetListSize(
	IndexList	*List );	// list whose size we want

//	Get an elements data.
//
////////////////////////////////////////////////////////////////////////////////////////
void * IndexList_GetElement(
	IndexList	*List,		// list in which the element exists
	int			Number );	// element number

//	Remove an elements data from the list.
//
////////////////////////////////////////////////////////////////////////////////////////
void IndexList_DeleteElement(
	IndexList	*List,		// list in which the element exists
	int			Number );	// element number

//	Add an elements data to the list.
//
////////////////////////////////////////////////////////////////////////////////////////
void IndexList_AddElement(
	IndexList	*List,		// list in which to add the elements data
	int			Number,		// element number
	void		*Data );	// pointer to add

//	Get an empty slot index in the list.
//
////////////////////////////////////////////////////////////////////////////////////////
int IndexList_GetEmptySlot(
	IndexList	*List );	// list in which we want an empyt slot

//	Destroy a list.
//
////////////////////////////////////////////////////////////////////////////////////////
void IndexList_Destroy(
	IndexList	**List );	// list to destroy

//	Create a new list.
//
////////////////////////////////////////////////////////////////////////////////////////
IndexList * IndexList_Create(
	int	StartSize,	// starting size of the list
	int	IncSize );	// how much to enlarge the list when it gets full


#ifdef __cplusplus
	}
#endif
#pragma warning ( default : 4068 )

#endif
