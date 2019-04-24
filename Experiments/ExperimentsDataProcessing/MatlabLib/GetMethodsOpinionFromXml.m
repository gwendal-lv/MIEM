function [methodsOpinion] = GetMethodsOpinionFromXml(methodsOpinionNode)
%GETMETHODSOPINIONFROMXML Translates XML text answers into numeric values
% opinions sur les méthodes : comme d'habitude : 1 pour fader, 2 pour interpolation, 
% et la nouvelle : -1 si aucune méthode choisie
        

% on extrait les réponses attribut par attribut

if strcmpi(methodsOpinionNode.Attributes.fastest, 'fader')
    methodsOpinion.fastest = 1;
elseif strcmpi(methodsOpinionNode.Attributes.fastest, 'interpolation')
    methodsOpinion.fastest = 2;
else
    methodsOpinion.fastest = -1;
end

if strcmpi(methodsOpinionNode.Attributes.most_precise, 'fader')
    methodsOpinion.mostPrecise = 1;
elseif strcmpi(methodsOpinionNode.Attributes.most_precise, 'interpolation')
    methodsOpinion.mostPrecise = 2;
else
    methodsOpinion.mostPrecise = -1;
end

if strcmpi(methodsOpinionNode.Attributes.most_intuitive, 'fader')
    methodsOpinion.mostIntuitive = 1;
elseif strcmpi(methodsOpinionNode.Attributes.most_intuitive, 'interpolation')
    methodsOpinion.mostIntuitive = 2;
else
    methodsOpinion.mostIntuitive = -1;
end

if strcmpi(methodsOpinionNode.Attributes.preferred, 'fader')
    methodsOpinion.preferred = 1;
elseif strcmpi(methodsOpinionNode.Attributes.preferred, 'interpolation')
    methodsOpinion.preferred = 2;
else
    methodsOpinion.preferred = -1;
end


end

