/*
  ==============================================================================

    SpatInterpolator.cpp
    Created: 26 Nov 2016 12:03:08pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


#include "SpatInterpolator.h"

#include "IModel.h"



using namespace Miam;

// EXPLICIT TEMPLATES INSTANCIATIONS -> are all of these necessary ?
//template class Miam::SpatInterpolator<double>;






// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =


template SpatType SpatInterpolator<double>::GetSpatType();
template<typename T>
SpatType SpatInterpolator<T>::GetSpatType()
{
    return model->GetSpatType();
}


template std::string SpatInterpolator<double>::GetOutputName(size_t);
template<typename T>
std::string SpatInterpolator<T>::GetOutputName(size_t _i)
{
    return model->GetOutputName(_i);
}



// = = = = = = = = = = METHODS = = = = = = = = = =


template SpatInterpolator<double>::SpatInterpolator(IModel* _model);
template<typename T>
SpatInterpolator<T>::SpatInterpolator(IModel* _model)
{
    model = _model;
}



template void SpatInterpolator<double>::__AddDefaultStates();
template<typename T>
void SpatInterpolator<T>::__AddDefaultStates()
{
    for (size_t i = 0 ; i<3 ; i++)
    {
        std::shared_ptr<SpatState<T>> newState(new OutputVolumesState<T>(model->GetOutputsCount()));
        newState->SetName("Effet de spat " + std::to_string(i+1));
        spatStates.push_back(newState);
    }
}





// - - - - - Speakers management - - - - -
template void SpatInterpolator<double>::AddSpeaker();
template<typename T>
void SpatInterpolator<T>::AddSpeaker()
{
    for (size_t i = 0 ; i<spatStates.size() ; i++)
    {
        spatStates[i]->AddOutput();
    }
}
template void SpatInterpolator<double>::RemoveSpeaker(size_t);
template<typename T>
void SpatInterpolator<T>::RemoveSpeaker(size_t _id)
{
    for (size_t i = 0 ; i<spatStates.size() ; i++)
    {
        spatStates[i]->DeleteOutput(_id);
    }
}


