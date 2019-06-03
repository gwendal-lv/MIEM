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
        
        // For log-curve interpolation ; will actually compute an interpolation
        // along an "a.exp(b.x) + c" curve, to produce a log-scale effect
        T a_logInterp, b_logInterp, c_logInterp;
        
        // For a soft-curve interpolation based on a x |-> x^2 curve, form:
        // y = m.(x-xmin)^2 + ymin
        T m_soft;
        // For a hard-curve interpolation along curve:
        // y = m.sqrt(x-xmin) + ymin
        T m_hard;
        
        // For more controlled soft and hard curves, the linearity is controlled by
        // alpha coefficient.
        // If alpha is 0, pure soft/hard curves. When alpha is close to 1, the soft/hard curves
        // become closer and closer to the linear curve.
        T alpha_s = (T) 0.3;
        T alpha_h = (T) 0.1;
        // The better-controlled soft/hard curves need a, b, (and c) coeffs
        // soft : y = a.(x-xmin)^2 + b.(x-xmin) + ymin
        // hard : y = a.sqrt(alpha_h + b.(x-xmin)) + c
        T a_soft, b_soft;
        T a_hard, b_hard, c_hard;
        
        
        // ========== Getters and Setters ==========
        public :
        T GetMinY() const {return minY;}
        T GetMaxY() const {return maxY;}
        ParamInterpolationType GetInterpolationType() const {return interpolationType;}
        
        static std::vector<ParamInterpolationType> GetAvailableParamInterpolationTypes()
        {
            std::vector<ParamInterpolationType> returnTypes;
            returnTypes.push_back(ParamInterpolationType::Independant_Threshold);
            returnTypes.push_back(ParamInterpolationType::Independant_Hard2);
            returnTypes.push_back(ParamInterpolationType::Independant_Hard1);
            returnTypes.push_back(ParamInterpolationType::Independant_Linear);
            returnTypes.push_back(ParamInterpolationType::Independant_Soft1);
            returnTypes.push_back(ParamInterpolationType::Independant_Soft2);
            returnTypes.push_back(ParamInterpolationType::Independant_Log);
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
            
            // Contrainte éventuellement supprimée car elle n'a pas beaucoup de sens...
            // les float/double vont automatiquement s'adapter aux ordres de grandeur...
            assert( deltaY > (GetMinOutputRange() * ((T)0.75) ) );
        }
        /// \brief Constructeur de copie par défaut
        BasicInterpolationCurve(const BasicInterpolationCurve&) = default;
        private :
        /// \brief Pré-calcul des coefficients pour toutes les interpolations
        void updateInternalValues()
        {
            // pre-computed generic values, for computation speed of linear interpolation
            // (re-used in other interps)
            deltaX = maxX - minX;
            deltaY = maxY - minY;
            centerX = (maxX + minX) / ((T) 2.0);
            centerY = (maxY + minY) / ((T) 2.0);
            
            // log-scale interp is also prepared here
            // If any exception comes from the internal code, we transform the interpolation
            // into a basic linear one
            try {
                double a, b, c; // local copies
                CoefficientsComputation::LogInterpolationCoeffs(minY, maxY, &a, &b, &c);
                a_logInterp = (T)a;
                b_logInterp = (T)b;
                c_logInterp = (T)c;
            }
            catch (std::exception& ) // assez mal de tout catcher...
            {
                // En débug, il faut absolument vérifier ce qui a déclenché l'exception !
                assert(false);
                // En release on laisse couler pour la stabilité... interp linéaire forcée
                if (interpolationType == ParamInterpolationType::Independant_Log)
                    interpolationType = ParamInterpolationType::Independant_Linear;
                a_logInterp = (T) 0.0;
                b_logInterp = (T) 0.0;
                c_logInterp = (T) 0.0;
            }
            
            // soft and hard curves and based on quadratic or square-root functions; very
            // simple formulae
            m_soft = (deltaY / (deltaX * deltaX));
            m_hard = (deltaY / std::sqrt(deltaX));
            
            // better-controller soft/hard curves, closer to a linear curve
            a_soft = (deltaY * ((T)1.0-alpha_s)*(1.0-alpha_s) )
                    / (deltaX * deltaX * ((T)1.0 - alpha_s*alpha_s) );
            b_soft = ((T)2.0 * alpha_s * ((T)1.0 - alpha_s) * deltaY )
                    / ( ((T)1.0 - alpha_s*alpha_s) * deltaX );
            a_hard = (deltaY / ((T)1.0 - std::sqrt(alpha_h)));
            b_hard = (((T)1.0 - alpha_h) / deltaX);
            c_hard = (minY - ((deltaY*std::sqrt(alpha_h)) / ((T)1.0 - sqrt(alpha_h))));
        }
        
        
        
        public :
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
                        return minY + ((inputX - minX) / deltaX) * deltaY;
                        
                    // - - - hard/soft 2 (hardest and softest curves) - - -
                    case ParamInterpolationType::Independant_Soft2 :
                        return m_soft * (inputX - minX)*(inputX - minX) + minY;
                        
                    case ParamInterpolationType::Independant_Hard2 :
                        return m_hard * sqrt(inputX - minX) + minY;
                        
                    // - - - hard/soft 1 : more linear, formulae is a bit more complex - - -
                    case ParamInterpolationType::Independant_Soft1 :
                        return a_soft * (inputX - minX)*(inputX - minX)
                        + b_soft * (inputX - minX) + minY;
                        
                    case ParamInterpolationType::Independant_Hard1 :
                        return a_hard * std::sqrt(alpha_h + b_hard*(inputX - minX)) + c_hard;
                        
                    // - - - Log-scale for audio frequencies - - - 
                    case ParamInterpolationType::Independant_Log :
                        // fonction qui prend un x normalisé en entrée
                        // (pourrait être optimisé...)
                        return a_logInterp * std::exp(b_logInterp * ((inputX - minX) / deltaX))
                               + c_logInterp;
                
                    case ParamInterpolationType::Independant_Threshold :
                        return (inputX < centerX) ? minY : maxY;
                
                    default : // interp linéaire par défaut... ou 0.0 en débug
#ifdef __MIAM_DEBUG
                        return (T) 0.0;
#else
                        return minY + ((inputX - minX) / deltaX) * deltaY;
#endif
                }
            }
        }
        
        
        
        // - - - - - XML - Boost Property Trees import/export - - - - -
        public :
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
        
        
        
        
        
        
        // =============================================================
        // =============================================================
        // =============================================================
        // =============================================================
        // - - - - - Computation of interpolation coefficients - - - - -
        // =============================================================
        // =============================================================
        
        // (Generated from Matlab code)
        class CoefficientsComputation
        {
            public :
            //
            // LOGINTERPOLATIONCOEFFS Computes the coeffs for a log-curve interpolation
            //  function of form : y = a.exp(b.x) + c
            //  considering that x is a normalized input, and
            //  for input y values given at points x=0, x=0.5, x=1
            // Arguments    : double minY
            //                double maxY
            //                double *a
            //                double *b
            //                double *c
            // Return Type  : void
            //
            static void LogInterpolationCoeffs(double minY, double maxY, double *a, double *b,
                                        double *c)
            {
                double y2;
                double ym;
                double a_eq2;
                double b_eq2;
                double delta_eq2;
                double B_roots[2];
                int i;
                double b_test[2];
                int numberOk;
                int validCoeffsIndex;
                double c_test[2];
                int iterationsCount;
                double a_test[2];
                
                //  default values for illegal inputs
                *a = 0.0;
                *b = 0.0;
                *c = 0.0;
                
                //  Interpolation log : il faut faire un peu d'analyse et de calcul. R�solution qui fonctionne pour une interpolation croissante uniquement
                //  On recherche a, b et c tels que (y - minY) = a.exp(bx) + c, avec n�cessairement b>0, et imm�diatement a = -c pour la correspondance en 0
                //  On va passer d'abord par la r�solution d'une �quation du second degr� en B = exp(b)
                //
                //  REMARQUE : le code g�n�r� n'a plus forc�ment de sens "physique"
                //  � l'int�rieur, car Matlab effectue des optimisations (variables
                //  temporaires supprim�es, boucles modifi�es, etc...)
                y2 = maxY - minY;
                
                //  ok, calcul coh�rent avec l'affichage eq8 dans Live 10
                ym = minY * std::pow(10.0, std::log10(maxY / minY) / 2.0) - minY;
                a_eq2 = ym * ym;
                b_eq2 = -ym * ym - (ym - y2) * (ym - y2);
                delta_eq2 = b_eq2 * b_eq2 - 4.0 * a_eq2 * ((ym - y2) * (ym - y2));
                
                //  warning n'est pas compatible avec Matlab Codegen (mettre assert() dans le
                //  code C final)
                if (!(delta_eq2 < 0.0)) {
                    //  racines en vecteur colonne
                    B_roots[0] = (-b_eq2 - std::sqrt(delta_eq2)) / (2.0 * a_eq2);
                    B_roots[1] = (-b_eq2 + std::sqrt(delta_eq2)) / (2.0 * a_eq2);
                    
                    //  Il est probable que les 2 racines soient valables. On doit calculer les
                    //  a,b,c jusqu'au bout puis v�rifier l'application de l'�quation avec une
                    //  marge d'erreur (qui r�duit)
                    //  - - - - - Calcul des coefficients a, b, c - - - - -
                    for (i = 0; i < 2; i++) {
                        b_test[i] = std::log(B_roots[i]);
                        a_eq2 = y2 / (B_roots[i] - 1.0);
                        c_test[i] = -a_eq2;
                        a_test[i] = a_eq2;
                    }
                    
                    //  - - - - - Test - - - - -
                    numberOk = 2;
                    validCoeffsIndex = -2;
                    iterationsCount = 0;
                    a_eq2 = (maxY - minY) / 10.0;
                    
                    //  fera 5% d'erreur pour premi�re it�ration (division par 2 � chaque fois
                    //  on sortira de la boucle si 0 ou 1 valeur ne sont pas OK...
                    while ((numberOk > 1) && (iterationsCount < 20)) {
                        numberOk = 0;
                        a_eq2 /= 2.0;
                        
                        //  pour chaque jeu de r�ponses, on v�rifie les 3 points qui ont
                        //  servi � construire les syst�mes d'�quation
                        for (i = 0; i < 2; i++) {
                            //  always true at the moment...
                            if ((std::abs(a_test[i] + c_test[i]) < a_eq2) && (std::abs((a_test[i] *
                                                                                        std::exp(b_test[i] * 0.5) + c_test[i]) - ym) < a_eq2) && (std::
                                                                                                                                                  abs((a_test[i] * std::exp(b_test[i]) + c_test[i]) - y2) < a_eq2)) {
                                validCoeffsIndex = i;
                                numberOk++;
                            }
                        }
                        
                        iterationsCount++;
                    }
                    
                    //  �criture des bons coeffs, si OK
                    if (numberOk == 1) {
                        *a = a_test[validCoeffsIndex];
                        *b = b_test[validCoeffsIndex];
                        *c = c_test[validCoeffsIndex] + minY;
                    } else {
                        //  sinon on prend les premiers venus...
                        *a = a_test[0];
                        *b = b_test[0];
                        *c = c_test[0] + minY;
                    }
                }

            }
        };
    };
}
