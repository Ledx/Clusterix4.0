function clusters=ConteoClusters(robots,N)
%Funcion que determina cuantos clusters existen y de que tamaño son
    clusters = zeros(N,1);
    for h=1:N
        tam = 0;
        for l=1:N
            if robots(l).clase==h
                tam = tam + 1;
            end
        end
        clusters(h)=tam/h;
    end   
	
end