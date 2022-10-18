#pragma  once

#pragma warning(push)
#pragma warning(disable:4146)

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename( "EOF", "adoEOF" ) no_namespace

#pragma warning(pop)


#include <oledb.h>
#include <icrsint.h>
#include <list>


namespace eXtreme
{
	class RecordBinding;

	typedef		void		( *CallbackRecordError )( RecordBinding*, HRESULT, _bstr_t, _bstr_t, _bstr_t );

	class RecordBinding
	{
		friend class DBC;

	private:
		class RecordInfo
		{
		public:
					void*					ptr;
					_bstr_t					rowName;
					DataTypeEnum			type;
					ParameterDirectionEnum	dir;
					long					size;
					_bstr_t					var;
		};

	public:
				RecordBinding();
		virtual	~RecordBinding();

				void				BindVariable( void* ptr, _bstr_t rowName );
				void				BindParameter( void* ptr, _bstr_t rowName,
													DataTypeEnum type, ParameterDirectionEnum dir,
													long size, _bstr_t var );

				bool				IsEndRecord();
				bool				Next();
				bool				Prev();

				long				GetRecordCount();


				CallbackRecordError	m_callbackError;

	private:
				bool				Fetch( _bstr_t& queryStr, _ConnectionPtr conn );
				void				EndFetch();

				bool				BindingVariable();
				void				AttachVariable( _variant_t& value, void* ptr );

				//  Stored Procedure ���� Method
				void				PrepareSP( _bstr_t& spName );
				bool				BindingParameter();
				void				FetchParameter();

		typedef	std::list	< RecordInfo* >		ListRecordInfo;

				_RecordsetPtr		m_recordSet;
				_CommandPtr			m_command;

				ListRecordInfo		m_listRecordInfo;
				int					m_opItemCount;
	};
}