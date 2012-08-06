/*
* ==============================================================================
*  Name        : BrCtlSampleAppCommandObserver.h
*  Part of     : BrCtlSampleApp
*  Interface   : Browser Control API
*  Description : Handle load progress events
*  Version     : %Version%
*
*  Copyright (c) 2005 Nokia Corporation.
*  This material, including documentation and any related 
*  computer programs, is protected by copyright controlled by 
*  Nokia Corporation. All rights are reserved. Copying, 
*  including reproducing, storing, adapting or translating, any 
*  or all of this material requires the prior written consent of 
*  Nokia Corporation. This material also contains confidential 
*  information which may not be disclosed to others without the 
*  prior written consent of Nokia Corporation.
* ==============================================================================
*/


#ifndef BRCTLSAMPLEAPPCOMMANDOBSERVER_H
#define BRCTLSAMPLEAPPCOMMANDOBSERVER_H

// INCLUDES
#include <BrCtlInterface.h>

// FORWARD DECLARATIONS
class CBrCtlSampleAppContainer;

// CLASS DECLARATION

/**
* CBrCtlSampleAppCommandObserver class.
* This class inherits from the MBrCtlLoadEventObserver interface. 
* This class is used to receive load progress events.
*/
class CBrCtlSampleAppCommandObserver : public MBrCtlCommandObserver
    {
    public:  // Constructors and destructor
    
        /**
        * Two-phased constructor.
        */
        static CBrCtlSampleAppCommandObserver* NewL(CBrCtlSampleAppContainer* aContainer);
        
        ~CBrCtlSampleAppCommandObserver();

    public:
        virtual void HandleCommandL(TBrCtlDefs::TBrCtlClientCommands aCommand, const CArrayFix<TPtrC>& aAttributesNames,
                                     const CArrayFix<TPtrC>& aAttributeValues);
        /**
        * Register a command observer.
        */                            
        void AddCommandObserver();
        
        /**
        * Unregister a command observer.
        */   
        void RemoveCommandObserver();

    private:
    
    	/**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(CBrCtlSampleAppContainer* aContainer);
        
    private:   // data
        // Pointer to the browser control interface
        CBrCtlInterface* iBrCtlInterface;
        // Pointer to the container class associated with this observer
        CBrCtlSampleAppContainer* iContainer;
    };

#endif      // BRCTLSAMPLEAPPCOMMANDOBSERVER_H
            
// End of File

