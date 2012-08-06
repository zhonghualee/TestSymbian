//const TUid KUPPayIFUid = {0xE0009666};
const TUid KUPPayIFUid = {0x2005F90C};

inline CUPPayPlugin::CUPPayPlugin()
	{
	}

inline CUPPayPlugin::~CUPPayPlugin()
	{
	// Destroy any instance variables and then
	// inform the framework that this specific 
	// instance of the interface has been destroyed.
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

inline CUPPayPlugin* CUPPayPlugin::NewL()
	{
	//KUPPayUid same as interface_uid of rss
         return REINTERPRET_CAST(CUPPayPlugin*, 
                REComSession::CreateImplementationL(KUPPayUid, 
                _FOFF(CUPPayPlugin,iDtor_ID_Key)));
    }

inline void CUPPayPlugin::ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray)
	{
	REComSession::ListImplementationsL(KUPPayIFUid, aImplInfoArray);
	}

