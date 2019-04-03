function [] = DisplayExpeResults(globalParams, expeResults, expeId)
%DISPLAYEXPERESULTS Summary of this function goes here
%   Detailed explanation goes here

perfIndexMaxDisplayValue = 1.0;
errorYMax = 1; % valeurs entre -1 et +1..... erreur max en norme 1 ?
timeMaxDisplayValue = 30.0; % fin max de l'expérience


figure('Name', strcat('Duration, error and performance index of experiment #', num2str(expeId)));


synthsIds = 1:globalParams.synthsCount;

subplot(3, 1, 1);
plot(synthsIds, expeResults(expeId).T(:,1), '+r');
hold on;
plot(synthsIds, expeResults(expeId).T(:,2), '*b');
xlim([0 (globalParams.synthsCount+1)]);
xticks([1:(globalParams.synthsCount+0)]);
legend('Faders', 'interpolation', 'Location', 'southeast');
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

