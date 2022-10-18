#pragma once

class State;

class NPCStateFactory
{
public:		NPCStateFactory();
			~NPCStateFactory();

			State*				Alloc( DWORD dwStateID );
			VOID				Free( State* pState );

			util::PoolRouter<STATE_ID_UPPERBOUND>		m_Router;
};





