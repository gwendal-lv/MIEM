function [] = DisplayExpeResults(globalParams, expeResults, expeId)
%DISPLAYEXPERESULTS Summary of this function goes here
%   Detailed explanation goes here

perfIndexMaxDisplayValue = 1.0;
errorYMax = 1; % valeurs entre -1 et +1..... erreur max en norme 1 ?
timeMaxDisplayValue = 35.0; % fin max de l'expérience

xTicksValues = 1 : globalParams.synthsCount;
xTicksLabels = cell(1, globalParams.synthsCount);
for i=1 : globalParams.synthsCount
    if (i <= globalParams.trialSynthsCount) % trial synth
        xTicksLabels{i} = strcat('T', int2str(i));
    else % normal synth, as indexed in the C++ experiment code
        xTicksLabels{i} = i - globalParams.synthIdOffset; % curly braces to access value
    end
end


resultsFigure = figure('Name', strcat('Duration, error and performance index of experiment #', num2str(expeId)));


synthsIds = 1:globalParams.synthsCount;

subplot(3, 1, 1);
plot(synthsIds, expeResults(expeId).T(:,1), 's', 'Color', globalParams.colors(1,:));
hold on;
plot(synthsIds, expeResults(expeId).T(:,2), 'd', 'Color', globalParams.colors(2,:));
xlim([0 (globalParams.synthsCount+1)]);
xticks(xTicksValues);
xticklabels(xTicksLabels);
legend('Faders', 'interpolation', 'Location', 'southwest');
ylim([0, timeMaxDisplayValue]);
ylabel('Search duration T [s]');
title( "Characteristics T, E and P for experiment i = " + num2str(expeId));

subplot(3, 1, 2);
plot(synthsIds, expeResults(expeId).E(:,1), 's', 'Color', globalParams.colors(1,:));
hold on;
plot(synthsIds, expeResults(expeId).E(:,2), 'd', 'Color', globalParams.colors(2,:));
ylabel('Parametric error E');
ylim([0, errorYMax]);
xlim([0 (globalParams.synthsCount+1)]);
xticks(xTicksValues);
xticklabels(xTicksLabels);

subplot(3, 1, 3);
plot(synthsIds, expeResults(expeId).P(:,1), 's', 'Color', globalParams.colors(1,:));
hold on;
plot(synthsIds, expeResults(expeId).P(:,2), 'd', 'Color', globalParams.colors(2,:));
ylabel('Performance score P');
ylim([0, perfIndexMaxDisplayValue]);
xlim([0 (globalParams.synthsCount+1)]);
xticks(xTicksValues);
xticklabels(xTicksLabels);
xlabel("j = Synth ID");


% valeur de taille par défaut
 %set(resultsFigure, 'Position', [0 0 560 250]);


end

