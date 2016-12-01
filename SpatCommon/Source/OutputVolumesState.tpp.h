/*
  ==============================================================================

    OutputVolumesState.cpp
    Created: 26 Nov 2016 12:09:44pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


#include <algorithm>

//#include "OutputVolumesState.h"


using namespace Miam;



// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =

//template double OutputVolumesState<double>::GetVolume(size_t);
template<typename T>
T OutputVolumesState<T>::GetVolume(size_t i)
{
    return outputVolumes[i];
}


//template void OutputVolumesState<double>::SetVolume(size_t, double);
template<typename T>
void OutputVolumesState<T>::SetVolume(size_t i, T volume)
{
	// pour éviter le warning "formal parameters"
	i = i;
	volume = volume;
}

//template size_t OutputVolumesState<double>::GetOutputsCount();
template<typename T>
size_t OutputVolumesState<T>::GetOutputsCount()
{
    return outputVolumes.size();
}



// = = = = = = = = = = METHODS = = = = = = = = = =

// - - - - - Construction and Destruction - - - - -

//template OutputVolumesState<double>::OutputVolumesState(size_t);
template<typename T>
OutputVolumesState<T>::OutputVolumesState(size_t outputsCount) :
    SpatState<T>() // default base constructor
{
    for (size_t i = 0 ; i<outputsCount ; i++)
        AddOutput();
}


// - - - - - Output channels (speakers) : add, delete, swap, ... - - - - -

//template void OutputVolumesState<double>::AddOutput();
template<typename T>
void OutputVolumesState<T>::AddOutput()
{
    outputVolumes.resize(outputVolumes.size()+1);
}
//template void OutputVolumesState<double>::DeleteOutput(size_t);
template<typename T>
void OutputVolumesState<T>::DeleteOutput(size_t i)
{
    outputVolumes.erase(outputVolumes.begin()+i);
}
//template void OutputVolumesState<double>::SwapOutputs(size_t, size_t);
template<typename T>
void OutputVolumesState<T>::SwapOutputs(size_t i, size_t j)
{
    std::iter_swap(outputVolumes.begin()+i, outputVolumes.begin()+j);
}


