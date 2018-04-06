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

#include "InterpolationTypes.h"


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
        
        /// \brief Renvoie le type d'interpolation actuellement calculée, ce qui est une
        /// donnée partagée par le Presenter et le Modèle ;
        /// doit donc rester thread-safe y compris pour les override potentiels
        virtual InterpolationType GetInterpolatorType_Atomic ()
        { return InterpolationType::Matrix_Linear; } // par défaut
        
    };
}


#endif  // IMODEL_H_INCLUDED
