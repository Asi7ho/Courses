function [max_W, max_b] = CompareGradients(grad_W, grad_b, refGrad_W, refGrad_b)

% for i = 1:length(grad_W)
%    diff = abs(grad_W{i} - refGrad_W{i});
%    relativeDiff = diff/(max(eps, abs(grad_W{i}) + abs(refGrad_W{i})));
%    
%    max_W(i) = max(max(relativeDiff));
% end
% 
% for i = 1:length(grad_b)
%    diff = abs(grad_b{i} - refGrad_b{i});
%    relativeDiff = diff/(max(eps, abs(grad_b{i}) + abs(refGrad_b{i})));
%    
%    max_b(i) = max(max(relativeDiff));
% end

max_W = max(max(abs(grad_W - refGrad_W)));

max_b = max(abs(grad_b - refGrad_b));

end