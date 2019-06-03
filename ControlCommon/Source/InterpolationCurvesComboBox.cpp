/*
  ==============================================================================

    InterpolationCurvesComboBox.cpp
    Created: 9 Mar 2019 4:44:22pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "InterpolationCurvesComboBox.h"

#include "BasicInterpolationCurve.hpp"

using namespace Miam;


// =====================================================
// ================= Images manager ====================
// =====================================================

// déclaration du pointeur de singleton
InterpolationCurvesComboBox::ImagesManager* InterpolationCurvesComboBox::ImagesManager::mainInstance = nullptr;

InterpolationCurvesComboBox::ImagesManager::ImagesManager(int _height) :
height(_height)
{
    // création des images dans le constructeur en mode singleton
    // Taille du vecteur prévue par la taille de l'énum
    curveImages.resize((int)ParamInterpolationType::InterpolationTypesCount);
    // Ajout de toutes les images, si elles sont bien définies
    for (int i=0 ; i < (int)ParamInterpolationType::InterpolationTypesCount ; i++)
    {
        Image imageToBeResized;
        switch ((ParamInterpolationType) i )
        {
            // None: clear image
            case ParamInterpolationType::None:
                curveImages[i].reset(new Image(Image::PixelFormat::ARGB, height, height, true));
                break;
            // Normal types, from PNG files
            case ParamInterpolationType::Independant_Threshold:
                imageToBeResized =
                ImageFileFormat::loadFrom(BinaryData::InterpolationCurve_Threshold_80at1_png,
                                          BinaryData::InterpolationCurve_Threshold_80at1_pngSize);
                break;
            case ParamInterpolationType::Independant_Exp:
                imageToBeResized =
                ImageFileFormat::loadFrom(BinaryData::InterpolationCurve_Expscale_80at1_png,
                                          BinaryData::InterpolationCurve_Expscale_80at1_pngSize);
                break;
            case ParamInterpolationType::Independant_Soft1:
                imageToBeResized =
                ImageFileFormat::loadFrom(BinaryData::InterpolationCurve_Soft1_80at1_png,
                                          BinaryData::InterpolationCurve_Soft1_80at1_pngSize);
                break;
            case ParamInterpolationType::Independant_Hard1:
                imageToBeResized =
                ImageFileFormat::loadFrom(BinaryData::InterpolationCurve_Hard1_80at1_png,
                                          BinaryData::InterpolationCurve_Hard1_80at1_pngSize);
                break;
            case ParamInterpolationType::Independant_Soft2:
                imageToBeResized =
                ImageFileFormat::loadFrom(BinaryData::InterpolationCurve_Soft2_80at1_png,
                                          BinaryData::InterpolationCurve_Soft2_80at1_pngSize);
                break;
            case ParamInterpolationType::Independant_Hard2:
                imageToBeResized =
                ImageFileFormat::loadFrom(BinaryData::InterpolationCurve_Hard2_80at1_png,
                                          BinaryData::InterpolationCurve_Hard2_80at1_pngSize);
                break;
            case ParamInterpolationType::Independant_Linear:
                imageToBeResized =
                ImageFileFormat::loadFrom(BinaryData::InterpolationCurve_Linear_80at1_png,
                                          BinaryData::InterpolationCurve_Linear_80at1_pngSize);
                break;
            case ParamInterpolationType::Independant_Log:
                imageToBeResized =
                ImageFileFormat::loadFrom(BinaryData::InterpolationCurve_Logscale_80at1_png,
                                          BinaryData::InterpolationCurve_Logscale_80at1_pngSize);
                break;
            // Unvalid values
            case ParamInterpolationType::Any:
            case ParamInterpolationType::InterpolationTypesCount:
                break;
        }
        // if actual interpolation value, we do the assignation
        if ( ParamInterpolationTypes::IsActualInterpolationType((ParamInterpolationType) i) )
        {
            imageToBeResized = imageToBeResized.rescaled(height*2, height*2, // retina rescale
                                    Graphics::ResamplingQuality::highResamplingQuality);
            curveImages[i].reset(new Image(imageToBeResized));
        }
    }
}

Image& InterpolationCurvesComboBox::ImagesManager::GetCurveImage(ParamInterpolationType interpolationType, int _height)
{
    // création de l'instance singleton, si nécessaire, càd dans 2 cas :
    // 1) si la classe n'avait pas été créée
    // 2) si l'utilisateur demande des images de taille différente
    if (mainInstance == nullptr)
        mainInstance = new ImagesManager(_height);
    else if (_height != mainInstance->height)
    {
        mainInstance = new ImagesManager(_height);
        DBG("[InterpolationCurvesComboBox] Re-construction des images pour une taille différente");
    }
    
    int imageIndex = (int) interpolationType;
    if ((imageIndex >= 0)
        && (imageIndex < (int)ParamInterpolationType::InterpolationTypesCount))
    {
        return *(mainInstance->curveImages[imageIndex].get());
    }
    else
    {
        // The asked interpolation type must be valid
        assert(false);
        return *(mainInstance->curveImages[0].get()); // empty transparent image
    }
}





// ================================================
// ================= Combo Box ====================
// ================================================

InterpolationCurvesComboBox::InterpolationCurvesComboBox (const String &componentName,
                                                          LabelledMatrixComponent* _parentComponent,
                                                          int _selfRow,
                                                          std::shared_ptr<ImageComponent> _imageComponent,
                                                          int _height)
:
ComboBox(componentName),
parentComponent(_parentComponent),
selfRow(_selfRow),
associatedImageComponent(_imageComponent),
height(_height)
{
    // pptés générales
    setEditableText(false);
    setTextWhenNoChoicesAvailable(TRANS("Custom interpolation not available"));
    setScrollWheelEnabled(false); // pour ne pas changer sans faire exprès....
    
    // ajout des items disponibles avec ID personnalisé
    auto availableTypes = BasicInterpolationCurve<double>::GetAvailableParamInterpolationTypes();
    for (int i = 0; i < availableTypes.size() ; i++)
    {
		auto itemShortName = ParamInterpolationTypes::GetInterpolationName(availableTypes[i], true, false);
        addItem(itemShortName, (int)availableTypes[i]);
    }
    
    // auto-listener
    addListener(this);
}


void InterpolationCurvesComboBox::SetSelectedInterpolationType(ParamInterpolationType newType)
{
    // Modification de la combo box
    if (ParamInterpolationTypes::IsActualInterpolationType(newType))
        setSelectedId((int) newType, NotificationType::sendNotification);
	else if (newType == ParamInterpolationType::None)
	{
		setSelectedId(0, NotificationType::sendNotification);
		setText("", NotificationType::dontSendNotification);
	}
    // Actualisation de la copie du choix.... sinon bug.
    lastActualChoice = newType;
    // Internal update
    updateImage();
}


void InterpolationCurvesComboBox::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged != this)
    {
        assert(false); // self-listener only...
        return;
    }
    
    // translation of data, internal save
    lastActualChoice = (ParamInterpolationType) getSelectedId();
    if (ParamInterpolationTypes::IsActualInterpolationType(lastActualChoice))
    {
    }
    else
    {
        lastActualChoice = ParamInterpolationType::None;
        assert(false); // should not happen !!
    }
    // and short name writing
	auto shortName = ParamInterpolationTypes::GetInterpolationName(lastActualChoice, true, true);
    setText(shortName,
            NotificationType::dontSendNotification);
    // Self-update
    updateImage();
    // callback to parent
    parentComponent->OnInterpolationTypeChanged(selfRow, lastActualChoice);
}


void InterpolationCurvesComboBox::updateImage()
{
    associatedImageComponent->setImage(ImagesManager::GetCurveImage(lastActualChoice, height));
}
