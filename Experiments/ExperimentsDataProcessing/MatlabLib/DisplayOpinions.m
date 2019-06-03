function [] = DisplayOpinions(globalParams, opinionResults)
%DISPLAYPREFERENCES Display the opinion (preferences) of all participants, 
% in a single plot

fig = figure;

% construction des abscisses et des légendes/étiquettes associées
%xBar = [1:12]';
xCategories = categorical({'fastest', 'most precise' ,'most intuitive' , 'one you preferred'});
% construction des valeurs de l'histogramme
% dans l'ordre demandé dans l'expé : fastest, most precise, most intuitive,
% preferred overall
% (4 groupes de 3)
yBar = [opinionResults.fader.fastestTotal opinionResults.none.fastestTotal opinionResults.interpolation.fastestTotal ;
        opinionResults.fader.preciseTotal opinionResults.none.preciseTotal opinionResults.interpolation.preciseTotal ;
        opinionResults.fader.intuitiveTotal opinionResults.none.intuitiveTotal opinionResults.interpolation.intuitiveTotal ;
        opinionResults.fader.preferredTotal opinionResults.none.preferredTotal opinionResults.interpolation.preferredTotal  ];

    % Utiliser FACECOLOR FLAT à combiner avec une colormap qui donne du 
    % bleu pour interp, du vert pour fader et du gris (ou autre... au
    % milieu)
barPlot = bar(xCategories, yBar, 'FaceColor', 'flat');

legend('Fader', 'None', 'Interpolation');
xlabel('Characteristic asked');
ylabel('Amount of subjects');
title('Answers to the questions: which method was the [...] ?');

% couleurs perso (après avoir mis l'attribut FaceColor = flat
barPlot(1).CData = globalParams.colors(1, :);
barPlot(2).CData = [0.4 0.4 0.4];
barPlot(3).CData = globalParams.colors(2, :);


end

