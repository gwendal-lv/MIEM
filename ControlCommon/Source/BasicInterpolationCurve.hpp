/*
  ==============================================================================

    BasicInterpolationCurve.hpp
    Created: 10 Mar 2019 3:05:14pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <utility> // std::swap depuis C++11

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
namespace bptree = boost::property_tree;

#include "MiamExceptions.h"

#include "InterpolationTypes.h"



namespace Miam
{
    /// \brief Class for fast and basic interpolation of 1-D values between 2 points.
    /// Monotonically increasing functions only;
    ///
    /// Inputs are noted x, outputs are noted y
    ///
    /// \remark Boost of course has some interpolation functionnalities... But here we just need
    /// very simple and basic functionnalities (with custom and simple interface)
    template<typename T>
    class BasicInterpolationCurve
    {
        // ========== ATTRIBUTES ==========
        private :
        
        // CONST attributes cannot be left const (and public...)
        // for easiness of use
        // Because it deletes any possibility for an implicit copy-constructor !
        //public :
        ParamInterpolationType interpolationType;
        T minX, maxX, minY, maxY;
        T deltaX, deltaY;
        T centerX, centerY;
        
        
        
        
        // ========== Getters and Setters ==========
        public :
        T GetMinY() const {return minY;}
        T GetMaxY() const {return maxY;}
        ParamInterpolationType GetInterpolationType() const {return interpolationType;}
        
        static std::vector<ParamInterpolationType> GetAvailableParamInterpolationTypes()
        {
            std::vector<ParamInterpolationType> returnTypes;
            returnTypes.push_back(ParamInterpolationType::Independant_Linear);
            return returnTypes;
        }
        inline static T GetMinOutputRange() { return (T)0.0000001; }
        
        
        
        // ========== METHODS ==========
        public :
        /// \brief Default constructor, with default values corresponding to normalised
        /// input values and scaled interpolated output values.
        BasicInterpolationCurve(ParamInterpolationType _interpolationType,
                                T _minY, T _maxY, T _minX = (T)0.0, T _maxX = (T)1.0)
        :
        interpolationType(_interpolationType),
        // assignation of const attributes, with check for monotonic increase
        minX( (_minX < _maxX) ? _minX : _maxX ), maxX( (_minX < _maxX) ? _maxX : _minX ),
        minY( (_minY < _maxY) ? _minY : _maxY ), maxY( (_minY < _maxY) ? _maxY : _minY )
        {
            assert( (minX < maxX)  && (minY < maxY) ); // monotonically increasing functions only
            
            updateInternalValues();
            
            assert( deltaY > (T) 0.001 );
        }
        private :
        void updateInternalValues()
        {
            // pre-computed values, for computation speed
            deltaX = maxX - minX;
            deltaY = maxY - minY;
            centerX = (maxX + minX) / ((T) 2.0);
            centerY = (maxY + minY) / ((T) 2.0);
        }
        public :
        
        
        /// \brief Constructeur de copie par défaut
        BasicInterpolationCurve(const BasicInterpolationCurve&) = default;
        
        
        static BasicInterpolationCurve GetDefault()
        {
            return BasicInterpolationCurve(ParamInterpolationType::Independant_Linear,
                                           (T)0.0, (T)1.0, (T)0.0, (T)1.0);
        }
        bool IsDefault()
        {
            return (interpolationType == GetDefault().interpolationType)
            && (std::abs(minX - GetDefault().minX) < GetMinOutputRange())
            && (std::abs(minY - GetDefault().minY) < GetMinOutputRange())
            && (std::abs(maxX - GetDefault().maxX) < GetMinOutputRange())
            && (std::abs(maxY - GetDefault().maxY) < GetMinOutputRange());
        }
        
        
        
        /// \brief Computes the interpolated output (y) value that corresponds
        /// to the given input (x) value
        inline T InterpolateValue(T inputX) const
        {
            // security checks (for debug only ???)
            if (inputX < minX)
                return minY;
            else if (inputX > maxX)
                return maxY;
            
            // actual computation if OK
            else
            {
                switch(interpolationType)
                {
                    case ParamInterpolationType::Independant_Linear :
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        return minY + ((inputX - minX) / deltaX) * deltaY;
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                
                    case ParamInterpolationType::Independant_Threshold :
                        return (inputX < centerX) ? minY : maxY;
                
                    default : //
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        return minY + ((inputX - minX) / deltaX) * deltaY;
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                        // interp linéaire seulement.... pour tester pour l'instant
                }
            }
        }
        
        
        
        // - - - - - XML - Boost Property Trees import/export - - - - -
        std::shared_ptr<bptree::ptree> GetTree()
        {
            auto innerTree = std::make_shared<bptree::ptree>();
            innerTree->put("<xmlattr>.type",
                           ParamInterpolationTypes::GetInterpolationName(interpolationType,
                                                                         false,
                                                                         true)
                           );
            innerTree->put("<xmlattr>.minY", minY);
            innerTree->put("<xmlattr>.maxY", maxY);
            
            // valeur en X pas mises, on laisse les valeurs par défaut pour l'instant...
            return innerTree;
        }
        /// \brief Prend les paramètres à partir d'un property tree.
        /// S'il manque un paramètre, on met les valeurs par défaut...
        void SetFromTree(bptree::ptree& innerTree)
        {
            // we try to get and set all params, one by one
            try {
                auto stringType = innerTree.get<std::string>("<xmlattr>.type");
                interpolationType = ParamInterpolationTypes::ParseName(stringType);
                minY = innerTree.get<T>("<xmlattr>.minY");
                maxY = innerTree.get<T>("<xmlattr>.maxY");
            }
            // exceptions if errors happen
            catch (bptree::ptree_error& e)
            {
                throw XmlReadException(std::string("Error when reading XML attributes inside a <curve> node: ") + e.what());
            }
            catch (ParseException& e)
            {
                throw XmlReadException(std::string("Error when reading XML attributes inside a <curve> node: ") + e.what());
            }
            
            
            // update
            updateInternalValues();
        }
        
    };
}
