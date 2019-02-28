function [] = DisplayExpeResults(globalParams, expeResults, expeId)
%DISPLAYEXPERESULTS Summary of this function goes here
%   Detailed explanation goes here

perfIndexMaxDisplayValue = 0.5;
errorYMax = 0.5;
timeMaxDisplayValue = 30.0;

figure('Name', strcat('Duration, error and performance index of experiment #', num2str(expeId)));

% Inutile de reconstruire des données d'affichage :
% on affiche juste des points, comme ça les valeurs négatives ne seront pas
% à l'écran (on fera un ylim)
synthsIds = 1:globalParams.synthsCount;

subplot(3, 1, 1);
plot(synthsIds, expeResults(expeId).T(:,1), '+r');
hold on;
plot(synthsIds, expeResults(expeId).T(:,2), '*b');
xlim([0 (globalParams.synthsCount+1)]);
xticks([1:(globalParams.synthsCount+0)]);
legend('Faders', 'interpolation');
ylim([0, timeMaxDisplayValue]);
ylabel('T');
title( "Characteristics T, E and P for experiment i = " + num2str(expeId));

subplot(3, 1, 2);
plot(synthsIds, expeResults(expeId).E(:,1), '+r');
hold on;
plot(synthsIds, expeResults(expeId).E(:,2), '*b');
ylabel('Final Error E');
ylim([0, errorYMax]);
xlim([0 (globalParams.synthsCount+1)]);
xticks([1:(globalParams.synthsCount+0)]);

subplot(3, 1, 3);
plot(synthsIds, expeResults(expeId).P(:,1), '+r');
hold on;
plot(synthsIds, expeResults(expeId).P(:,2), '*b');
ylabel('Performance index P');
ylim([0, perfIndexMaxDisplayValue]);
xlim([0 (globalParams.synthsCount+1)]);
xticks([1:(globalParams.synthsCount+0)]);
xlabel("j = Synth ID");

end

