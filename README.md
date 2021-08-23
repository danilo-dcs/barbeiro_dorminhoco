# barbeiro_dorminhoco
Problema do barbeiro dorminhoco implementado na linguagem C, utilizando os conceitos de threads e mutex.


## Descrição do problema

<ul>
    <li> Cada barbeiro trabalha de forma independente </li>
    <li> Existe uma cadeira de corte para cada barbeiro </li>
    <li> 
        Fluxo de trabalho dos barbeiros:
        <br>
        <ol>
            <li> O barbeiro dorme em sua cadeira quando termina de atender um cliente, e não há clientes na fila </li>
            <li> Quando um cliente chega, o barbeiro que está dormindo há mais tempo é acordado para atendê-lo </li>
            <li> Quando desperto, o barbeiro ocupa a sua cadeira de corte com o cliente </li>
            <li> Após o corte, o cliente está libre para sair </li>
            <li> Se houver cliente na fila, o próximo cliente se senta na cadeira imediatamente após ela ser liberada </li>
            <li> Se não h  ouver cliente na fila, o barbeiro volda a dormir </li>
        </ol>
    </li>
    <li>
        Ações do cliente:
        <br>
            <ol>
                <li> Qunado o cliente chega na barbearia e há cadeiras/barbeiros disponíveis, ele é atendido imediatamente</li>
                <li> Se não houver cadeira/barbeiro disponível, o cliente espera na fila de atendimento (máx 20 pessoas) </li>
                <li> Caso a fila esteja cheia, o cliente vai embora </li>
                <li> Se pelo menos um barbeiro estiver dormindo, o cliente dispara o sinal que acorda o barbeiro há mais tempo </li>
                <li> O cliente respeita a ordem de chegada </li>
            </ol>
        </li>
</ul>

