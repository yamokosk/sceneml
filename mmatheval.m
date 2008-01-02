function val = mmatheval(str)
if ischar(str)
    evalstr = strcat('[', str, ']');
    val = eval(evalstr);
else
    warning('Input was not a string! Disregarding.');
    val = 0.0;
end