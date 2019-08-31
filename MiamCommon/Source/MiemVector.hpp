/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
