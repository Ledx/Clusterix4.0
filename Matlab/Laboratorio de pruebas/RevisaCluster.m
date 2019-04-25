function mayor = RevisaCluster(robots,N,robot_actual)
%Funcion encargada de revisar si el robot actual pertenece al cluster mas
%grande formado hasta el momento


    mayor = 0;
    clusters=ConteoClusters(robots,N);
    for i=1:N
        if clusters(i) > 0 && robots(robot_actual).clase < i
            mayor = 1;
        end
    end
    
end