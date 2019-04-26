function [opinionResults] = GetMethodsOpinionsStats(globalParams, experiments)
%GETMETHODSOPINIONSSTATS Summary of this function goes here
%   Detailed explanation goes here


% 1  ==  fader
% 1.5 == none
% 2  ==  interpolation

% copie des données des expériences, dans des tableaux séparés pour
% affichage statique direct (box plot, etc...)
validI = 1;
for i=1:globalParams.experimentsCount
    if experiments(i).isValid
        opinionResults.fastest(validI) = experiments(i).methodsOpinion.fastest;
        opinionResults.mostPrecise(validI) = experiments(i).methodsOpinion.mostPrecise;
        opinionResults.mostIntuitive(validI) = experiments(i).methodsOpinion.mostIntuitive;
        opinionResults.preferred(validI) = experiments(i).methodsOpinion.preferred;
        
        validI = validI + 1;
    end
end



% extraction des pourcentages de préférences

% -> par type de méthode
opinionResults.interpolation.fastestTotal = sum( (abs(opinionResults.fastest - 2)) < 0.1 );
opinionResults.fader.fastestTotal = sum( (abs(opinionResults.fastest - 1)) < 0.1 );
opinionResults.none.fastestTotal = sum( (abs(opinionResults.fastest - 1.5)) < 0.1 );

opinionResults.interpolation.preciseTotal =  sum( (abs(opinionResults.mostPrecise - 2)) < 0.1 );
opinionResults.fader.preciseTotal = sum( (abs(opinionResults.mostPrecise - 1)) < 0.1 );
opinionResults.none.preciseTotal = sum( (abs(opinionResults.mostPrecise - 1.5)) < 0.1 );

opinionResults.interpolation.intuitiveTotal =  sum( (abs(opinionResults.mostIntuitive - 2)) < 0.1 );
opinionResults.fader.intuitiveTotal = sum( (abs(opinionResults.mostIntuitive - 1)) < 0.1 );
opinionResults.none.intuitiveTotal = sum( (abs(opinionResults.mostIntuitive - 1.5)) < 0.1 );

opinionResults.interpolation.preferredTotal =  sum( (abs(opinionResults.preferred - 2)) < 0.1 );
opinionResults.fader.preferredTotal = sum( (abs(opinionResults.preferred - 1)) < 0.1 );
opinionResults.none.preferredTotal = sum( (abs(opinionResults.preferred - 1.5)) < 0.1 );

% -> et aussi par critère

end

