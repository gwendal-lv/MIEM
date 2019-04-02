function [P] = AlternativePerf(E,T, maxTime)
%DISPLAYEDPERF Summary of this function goes here
%   Detailed explanation goes here



errorFactor = 1 - 2.0 * E;
timeFactor = 1 - T / maxTime;
P = errorFactor * (0.80 + 0.20*timeFactor);

% clamp
P = min(max(0, P), 1);


end

