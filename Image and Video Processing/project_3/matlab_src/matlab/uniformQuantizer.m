function output = uniformQuantizer(input, q)

% i = input
% q = quantizer step-size
    output = round(input/q)*q;
end