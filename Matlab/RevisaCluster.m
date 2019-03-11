function mayor = RevisaCluster(robots,N,robot_actual)
%Funcion encargada de revisar la distribucion de clusters formados


    mayor = 0;
    clusters=ConteoClusters(robots,N);
    for i=1:N
        if clusters(i) > 0 && robots(robot_actual).clase < i
            mayor = 1;
        end
    end
    
end