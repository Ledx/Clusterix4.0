function exito=Exito(robots,N)
%Revisa si se logro que todos los robots pertenezcan a un cluster
    
    exito = 0;
    for h=1:N
        if robots(h).clustered ~= 1
            exito = 1;
        end
        
    end
    	
end