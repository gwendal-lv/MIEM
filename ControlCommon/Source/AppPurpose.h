/*
  ==============================================================================

    AppType.h
    Created: 28 Feb 2018 3:48:03pm
    Author:  Gwendal

  ==============================================================================
*/

#pragma once


namespace Miam
{
    
    enum class AppPurpose
    {
        Spatialisation = 1,
        
        GenericController = 2,
        
        Multi = 4,
        
        None = 8,
    };
    
    
    class App
    {
        private :
        // Si problème de link (symbole non-défini), il ne faut pas oublier de déclarer et d'initialiser cet
        // attribut qqpart dans le code !! Dans le Presenter final par exemple
        static AppPurpose appPurpose;
        
        public :
        static AppPurpose GetPurpose() {return appPurpose;}
    };
    
}
