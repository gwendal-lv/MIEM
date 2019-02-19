function [ outputStr ] = BuildDataFileName( experimentUID )
%BUILDDATAFILENAME Builds the name of the CSV data file for an experiment
%   Detailed explanation goes here


outputStr = 'Exp';

if experimentUID < 10
    outputStr = strcat(outputStr, '0');
    outputStr = strcat(outputStr, '0');
    outputStr = strcat(outputStr, '0');
elseif experimentUID < 100
    outputStr = strcat(outputStr, '0');
    outputStr = strcat(outputStr, '0');
elseif experimentUID < 1000
    outputStr = strcat(outputStr, '0');
else
end

outputStr = strcat(outputStr, num2str(experimentUID));
outputStr = strcat(outputStr, '_data.csv');


end

