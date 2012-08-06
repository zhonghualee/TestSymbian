

#ifndef _C_UPPAY_INTERFACEDEFINITION_
#define _C_UPPAY_INTERFACEDEFINITION_

#include <e32base.h>
#include <badesca.h>
#include <aknappui.h>
#include <ECom.h>
#include "UPPayObserver.h"

// UID of this interface
// Due to self signing the UID cannot be from the protected range.
// Because of that all UIDs are 0xE... in S60 3rd Edition.
//const TUid KUPPayUid = {0xE0009888};
const TUid KUPPayUid = {0x2005F90B};

class CUPPayPlugin : public CBase
{
public:
	// Instantiates an object of this type
	static CUPPayPlugin* NewL();

	// Destructor.
	virtual ~CUPPayPlugin();

	// Request a list of all available implementations which
	// satisfy this given interface.
	static void ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray);

	// Pure interface method
	// Representative of a method provided on the interface by
	// the interface definer.
	virtual void startPay(const CAknAppUi* aAppUi, MUPPayObserver* aObserver, const TDesC8* aPaa,const TDesC& asysProvide,const TDesC& aspId, TUint32 aIapId) = 0;
	virtual void test(const CAknAppUi* aAppUi,MUPPayObserver* aObserver, const TDesC8* aPaa,const TDesC& asysProvide,const TDesC& aspId, TUint32 aIapId) = 0;

protected:
	inline CUPPayPlugin();

private:
	// Unique instance identifier key
	TUid iDtor_ID_Key;
};

#include "UPPay.inl"

#endif // _C_UPPAY_INTERFACEDEFINITION_

