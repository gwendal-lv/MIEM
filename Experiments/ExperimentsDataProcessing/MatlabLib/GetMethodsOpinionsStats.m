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
        opinionResults.fastestByExperiment(validI) = experiments(i).methodsOpinion.fastest;
        opinionResults.mostPreciseByExperiment(validI) = experiments(i).methodsOpinion.mostPrecise;
        opinionResults.mostIntuitiveByExperiment(validI) = experiments(i).methodsOpinion.mostIntuitive;
        opinionResults.preferredByExperiment(validI) = experiments(i).methodsOpinion.preferred;
        
        validI = validI + 1;
    end
end



% calcul des totaux de préférences

% -> par type de méthode
% (la structure de données permet aussi de retrouver par critère)
opinionResults.interpolation.fastestTotal = sum( (abs(opinionResults.fastestByExperiment - 2)) < 0.1 );
opinionResults.fader.fastestTotal = sum( (abs(opinionResults.fastestByExperiment - 1)) < 0.1 );
opinionResults.none.fastestTotal = sum( (abs(opinionResults.fastestByExperiment - 1.5)) < 0.1 );

opinionResults.interpolation.preciseTotal =  sum( (abs(opinionResults.mostPreciseByExperiment - 2)) < 0.1 );
opinionResults.fader.preciseTotal = sum( (abs(opinionResults.mostPreciseByExperiment - 1)) < 0.1 );
opinionResults.none.preciseTotal = sum( (abs(opinionResults.mostPreciseByExperiment - 1.5)) < 0.1 );

opinionResults.interpolation.intuitiveTotal =  sum( (abs(opinionResults.mostIntuitiveByExperiment - 2)) < 0.1 );
opinionResults.fader.intuitiveTotal = sum( (abs(opinionResults.mostIntuitiveByExperiment - 1)) < 0.1 );
opinionResults.none.intuitiveTotal = sum( (abs(opinionResults.mostIntuitiveByExperiment - 1.5)) < 0.1 );

opinionResults.interpolation.preferredTotal =  sum( (abs(opinionResults.preferredByExperiment - 2)) < 0.1 );
opinionResults.fader.preferredTotal = sum( (abs(opinionResults.preferredByExperiment - 1)) < 0.1 );
opinionResults.none.preferredTotal = sum( (abs(opinionResults.preferredByExperiment - 1.5)) < 0.1 );



end

