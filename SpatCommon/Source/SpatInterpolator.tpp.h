/*
  ==============================================================================

    SpatInterpolator.cpp
    Created: 26 Nov 2016 12:03:08pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/


//#include "SpatInterpolator.h"

#include "SpatModel.h"



using namespace Miam;




// = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =


//template SpatType SpatInterpolator<double>::GetSpatType();
template<typename T>
SpatType SpatInterpolator<T>::GetSpatType()
{
    return model->GetSpatType();
}

//template std::string SpatInterpolator<double>::GetOutputName(size_t);
template<typename T>
std::string SpatInterpolator<T>::GetOutputName(size_t _i)
{
    return model->GetOutputName(_i);
}

template<typename T>
void SpatInterpolator<T>::SetInputOuputChannelsCount(int _inputsCount, int _outputsCount)
{
    // At first : update of internal data
    inputsCount = _inputsCount;
    outputsCount = _outputsCount;
    
    // Config transmission to the individual states
    for (size_t i=0 ; i<spatStates.size() ; i++)
    {
        // Dynamic cast of the state to a MatrixState only for now
        if (std::shared_ptr<MatrixState<T>> matrixState = std::dynamic_pointer_cast<MatrixState<T>>(spatStates[i]))
        {
            matrixState->SetInputOuputChannelsCount(inputsCount,outputsCount);
        }
        // Else : behavior not implemented
        else
            throw std::runtime_error("in/out channels count modification is not implemented yet for this spatialization state");
    }
}



// = = = = = = = = = = METHODS = = = = = = = = = =


//template SpatInterpolator<double>::SpatInterpolator(IModel* _model);
template<typename T>
SpatInterpolator<T>::SpatInterpolator(SpatModel* _model)
{
    model = _model;
}



//template void SpatInterpolator<double>::__AddDefaultStates();
template<typename T>
void SpatInterpolator<T>::__AddDefaultStates()
{
    for (size_t i = 0 ; i<3 ; i++)
    {
        std::shared_ptr<MatrixState<T>> newState = std::make_shared<MatrixState<T>>();
        newState->SetName("Matrice " + std::to_string(i+1));
        spatStates.push_back(newState);
    }
}





// - - - - - Speakers management - - - - -
//template void SpatInterpolator<double>::AddSpeaker();
template<typename T>
void SpatInterpolator<T>::AddSpeaker()
{
    for (size_t i = 0 ; i<spatStates.size() ; i++)
    {
        spatStates[i]->AddOutput();
    }
}
//template void SpatInterpolator<double>::RemoveSpeaker(size_t);
template<typename T>
void SpatInterpolator<T>::RemoveSpeaker(size_t _id)
{
    for (size_t i = 0 ; i<spatStates.size() ; i++)
    {
        spatStates[i]->DeleteOutput(_id);
    }
}


