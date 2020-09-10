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

for i = 1:length(grad_W)
   max_W(i) = max(max(abs(grad_W{i} - refGrad_W{i})));
end

for i = 1:length(grad_b)
   max_b(i) = max(max(abs(grad_b{i} - refGrad_b{i})));
end


end