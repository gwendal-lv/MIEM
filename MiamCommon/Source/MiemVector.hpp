/*
  ==============================================================================

    MiemVector.hpp
    Created: 27 Nov 2018 3:38:55pm
    Author:  Gwendal

  ==============================================================================
*/

#pragma once

#include <vector>

namespace Miam
{
    /// \brief Sur-couche de la classe std::vector, qui permet d'effectuer
    /// ou non des vérifications supplémentaires
    template<typename T>
    class Vector : public std::vector<T>
    {
        // - - - - - surcharge des opérateurs d'accès sans vérification - - - - -
        // pour rajouter des vérifications forcées en debug
        public :
        inline T& operator[]( size_t pos )
        {
#ifdef __MIAM_DEBUG
#ifdef __MIEM_DEBUG_TRACE
            testTrace(pos);
#endif
            return std::vector<T>::at(pos);
#else
            return std::vector<T>::operator[](pos);
#endif
        }
        inline const T& operator[]( size_t pos ) const
        {
#ifdef __MIAM_DEBUG
#ifdef __MIEM_DEBUG_TRACE
            testTrace(pos);
#endif
            return std::vector<T>::at(pos);
#else
            return std::vector<T>::operator[](pos);
#endif
        }
        
        
        // - - - tracker d'erreur sur un indice particulier - - -
#ifdef __MIEM_DEBUG_TRACE
        private :
        int indexToTrack = -1;
        inline void testTrace(size_t pos)
        {
            if (indexToTrack > -1)
                if (pos == indexToTrack)
                    std::cout << "[MIEM TRACE] access to tracked value" << std::endl;
        }
        public :
        void SetIndexToTrack(int _indexToTrack) {indexToTrack = _indexToTrack;}
#endif
    };
}
