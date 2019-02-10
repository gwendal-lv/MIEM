/*
  ==============================================================================

    UserQuestionsManager.h
    Created: 9 Feb 2019 6:23:39pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "UserQuestions.h"
#include "UserFinalQuestions.h"

class UserQuestions;
class UserFinalQuestions;

class UserQuestionsManager
{
    public :
    
    virtual ~UserQuestionsManager() {}
    
    // Callbacks
    virtual void OnFirstQuestionsAnswered(UserQuestions* sender) = 0;
    virtual void OnFinalQuestionsAnswered(UserFinalQuestions* sender) = 0;
    
    // Methods for retrieving the data, and formatting it to XML and Boost Property Trees
    
};
