#pragma once

#ifdef VERSION_VS60
	#ifdef _DEBUG
		#pragma  comment(lib, "ClientStub_VS60_d.lib")
	#else
		#pragma  comment(lib, "ClientStub_VS60.lib")
	#endif
#elif VERSION_VS2003
	#ifdef _UNICODE
		#ifdef _DEBUG
			#pragma  comment(lib, "ClientStub_VS2003_d.lib")
		#else
			#pragma  comment(lib, "ClientStub_VS2003.lib")
		#endif	
	#else
		#ifdef _DEBUG
			#pragma  comment(lib, "ClientStub_VS2003_MB_d.lib")
		#else
			#pragma  comment(lib, "ClientStub_VS2003_MB.lib")
		#endif	
	#endif
#elif VERSION_VS2005
	#ifdef _WIN64
		#ifdef _UNICODE
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2005_64_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2005_64.lib")
			#endif	
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2005_64_MB_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2005_64_MB.lib")
			#endif	
		#endif
	#else
		#ifdef _UNICODE
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2005_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2005.lib")
			#endif	
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2005_MB_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2005_MB.lib")
			#endif	
		#endif
	#endif
#elif VERSION_VS2005_NOSP
	#ifdef _WIN64
		#ifdef _UNICODE
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2005_NOSP_64_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2005_NOSP_64.lib")
			#endif	
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2005_NOSP_64_MB_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2005_NOSP_64_MB.lib")
			#endif	
		#endif
	#else
		#ifdef _UNICODE
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2005_NOSP_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2005_NOSP.lib")
			#endif	
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2005_NOSP_MB_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2005_NOSP_MB.lib")
			#endif	
		#endif
	#endif
#elif VERSION_VS2008
	#ifdef _WIN64
		#ifdef _R2_
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2008_R2_64_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2008_R2_64.lib")
				#endif	
			#else
			#endif
		#else
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2008_64_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2008_64.lib")
				#endif	
			#else
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2008_64_MB_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2008_64_MB.lib")
				#endif	
			#endif
		#endif
	#else
		#ifdef _R2_
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2008_R2_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2008_R2.lib")
				#endif	
			#else
			#endif
		#else
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2008_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2008.lib")
				#endif	
			#else
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2008_MB_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2008_MB.lib")
				#endif	
			#endif
		#endif
	#endif
#elif VERSION_VS2005_MT
	#ifdef _WIN64
	#else
		#ifdef _UNICODE
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2005_MB_MT_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2005_MB_MT.lib")
			#endif
		#endif
	#endif
#elif VERSION_VS2008_MT
	#ifdef _WIN64
	#else
		#ifdef _UNICODE
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2008_MB_MT_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2008_MB_MT.lib")
			#endif
		#endif
	#endif
#elif VERSION_VS2010
	#ifdef _WIN64
		#ifdef _R2_
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2010_R2_64_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2010_R2_64.lib")
				#endif	
			#else
			#endif
		#else
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2010_64_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2010_64.lib")
				#endif	
			#else
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2010_64_MB_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2010_64_MB.lib")
				#endif	
			#endif
		#endif
	#else
		#ifdef _R2_
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2010_R2_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2010_R2.lib")
				#endif	
			#else
			#endif
		#else
			#ifdef _UNICODE
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2010_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2010.lib")
				#endif	
			#else
				#ifdef _DEBUG
					#pragma  comment(lib, "ClientStub_VS2010_MB_d.lib")
				#else
					#pragma  comment(lib, "ClientStub_VS2010_MB.lib")
				#endif	
			#endif
		#endif
	#endif
#elif VERSION_VS2010_MT
	#ifdef _WIN64
	#else
		#ifdef _UNICODE
		#else
			#ifdef _DEBUG
				#pragma  comment(lib, "ClientStub_VS2010_MB_MT_d.lib")
			#else
				#pragma  comment(lib, "ClientStub_VS2010_MB_MT.lib")
			#endif
		#endif
	#endif
#endif
