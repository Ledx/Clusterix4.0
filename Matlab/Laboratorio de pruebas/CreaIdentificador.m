function identificador=CreaIdentificador(longitud) 
%Funcion que realiza la creacion de un identificador aleatorio de archivo 
    caracteres=['a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 'u' 'v' 'w' 'x' 'z' '0' '1' '2' '3' '4' '5' '6' '7' '8' '9'];
    identificador = ''; 
    for i=1:longitud-1
        identificador =strcat(identificador,caracteres(randi(length(caracteres)))); 
    end
end