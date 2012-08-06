/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Handle soft key events
*
*/



#ifndef BRCTLSAMPLEAPPSOFTKEYSOBSERVER_H
#define BRCTLSAMPLEAPPSOFTKEYSOBSERVER_H

// INCLUDES
#include <BrCtlSoftkeysObserver.h>
#include <BrCtlInterface.h>

// FORWARD DECLARATIONS 
class CBrCtlSampleAppContainer;

// CLASS DECLARATION

/**
* CBrCtlSampleAppSoftkeysObserver class.
* This class inherits from the MBrCtlSoftkeysObserver interface that handles requests 
* from the Browser Control to change the softkeys. 
*/
class CBrCtlSampleAppSoftkeysObserver : public CBase, public MBrCtlSoftkeysObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CBrCtlSampleAppSoftkeysObserver* NewL(CBrCtlSampleAppContainer* aContainer);

        /**
        * Destructor.
        */
        ~CBrCtlSampleAppSoftkeysObserver();
       
        /**
        * Browser Control requests to update a softkey
        * @since 2.8
        * @param aKeySoftkey Update the left softkey or the right softkey
        * @param aLabel The label associated with the softkey update
        * @param aCommandId The command to use if the softkey is selected by the user
        * @param aBrCtlSoftkeyChangeReason The reason for the softkey change
        * @return void
        */
        void UpdateSoftkeyL(TBrCtlKeySoftkey /*aKeySoftkey*/,
                            const TDesC& /*aLabel*/,
                            TUint32 /*aCommandId*/,
                            TBrCtlSoftkeyChangeReason /*aBrCtlSoftkeyChangeReason*/) ;
        void UpdateHistorySoftKey();
        void UpdateNormalSoftKey();
    private:
	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(CBrCtlSampleAppContainer* aContainer);


    private:   // data
        // Pointer to the container class associated with this observer
        CBrCtlSampleAppContainer* iContainer;
    };

#endif      // BRCTLSAMPLEAPPSOFTKEYSOBSERVER_H
            
// End of File

