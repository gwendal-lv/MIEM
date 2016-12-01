/*
  ==============================================================================

    Speaker.cpp
    Created: 30 Nov 2016 3:13:47pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


#include "Speaker.h"

using namespace Miam;


template Speaker<double>::Speaker();
template< typename T >
Speaker<T>::Speaker()
{
    name = "Undefined speaker";
}


