/*
  ==============================================================================

    IModel.h
    Created: 31 Dec 2016 9:25:00am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#ifndef IMODEL_H_INCLUDED
#define IMODEL_H_INCLUDED


#include "LockFreeParamChangeSender.h"
#include <iostream>


namespace Miam
{
    class IModel : public LockFreeParamChangeSender
    {
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        IModel();
        virtual ~IModel() {}
        
        
    };
}


#endif  // IMODEL_H_INCLUDED
