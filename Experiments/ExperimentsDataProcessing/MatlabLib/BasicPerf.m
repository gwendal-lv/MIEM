function [ P ] = BasicPerf(E, T, maxTime)
%BASICPERF Summary of this function goes here
%   Detailed explanation goes here


% On peut ajouter les composantes de perf de précision et de rapidité
% indépendemment,
% Puis multiplier par un coefficient général de précision (car les
% outliers / résultats très faux doivent dégager le mieux possible...)

precisionPerf = (1.0 - 2.0*E);
timePerf = (1 - T/(maxTime*1.0));
P = 0.6 * (precisionPerf + timePerf);
% clamp négatif
P = max(0, P);

P = P * (1.0 - 1.2*E) ;


% clamp
P = min(max(0, P), 1);

end

