function [ P ] = BasicPerf(E, T, maxTime)
%BASICPERF Summary of this function goes here
%   Detailed explanation goes here


% On peut ajouter les composantes de perf de précision et de rapidité
% indépendemment,
% Puis multiplier par un coefficient général de précision (car les
% outliers / résultats très faux doivent dégager le mieux possible...)

precisionPerf = 1.0 - 2.0*E;
timePerf = 1 - T/(maxTime*1.5);

% performance qui somme et pondère les perfs de temps et de précision
P = precisionPerf + 0.70 * timePerf;
% clamp négatif
P = max(0, P);

% mise en facteur de l'erreur (qui reste la plus importante...)
P = 0.65 * P * (1.0 - 1.3*E) ;



% clamp
P = min(max(0, P), 1);

end

