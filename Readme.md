# <p align="center">Operating System Scheduling</p>

<p align=”justify”>Il simulatore implementato riproduce una versione semplificata del reale protocollo di scheduling utilizzato nella maggior parte dei sistemi moderni. L'algoritmo simulato è Round-Robin con un relativo quanto di tempo. </p>

<p align="center">
  <img src="https://github.com/AlebertoGuastalla/SIMMOD1920/blob/master/OSS.png"/>
</p>


## STAZIONI PRESENTI NEL MODELLO

<p align=”justify”>
<ul>
<li><b>Delay Station</b> (Infinite Server Station): questa stazione rappresenta appunto una stazione di ritardo per i vari jobs collegati al sistema. Possiede una distribuzione esponenziale negativa per quanto riguarda il tempo di servizio. </li>

<li><b>Reserve Station</b>: questa stazione serve per limitare il numero di jobs “attivi simultaneamente” nel sistema (si tratta dunque di una stazione in cui tempo di servizio è distribuito secondo una distribuzione di probabilità sconosciuta). </li>

<li><b>Swap-In Station</b>: una volta che il processo è ammesso nel sistema, questa stazione carica l’immagine del processo all’interno della memoria principale (il tempo di servizio è distribuito secondo la distribuzione esponenziale negativa). </li>

<li><b>CPU Station</b>: stazione che rappresenta il core del sistema e che processa i vari jobs. In questo caso il tempo di servizio è distribuito secondo la distribuzione iper-esponenziale. </li>

<li><b>IO1/IO2 Stations</b>: queste due stazioni rappresentano le stazioni di Input/Output il cui tempo di servizio è distribuito secondo la distribuzione esponenziale negativa. </li>

<li><b>Swap-Out Station</b>: questa stazione serve per rilasciare la memoria adibita al processo. In questo caso però, il tempo di servizio è trascurabile. </li>
</ul></p>

## EVENTI DEL SIMULATORE 

<ul>
<li><b>ARRIVAL_DELAY</b>: arrivo di un processo alla Delay Station. </li>

<li><b>ARRIVAL_RESERVE</b>: arrivo di un processo alla Reserve Station. </li>

<li><b>ARRIVAL_SWAP_IN</b>: arrivo di un processo alla Swap-in Station. </li>

<li><b>ARRIVAL_CPU</b>: arrivo di un processo alla Cpu Station. </li>

<li><b>ARRIVAL_IO1</b>: arrivo di un processo alla IO1 Station. </li>

<li><b>ARRIVAL_IO2</b>: arrivo di un processo alla IO2 Station. </li>

<li><b>ARRIVAL_SWAP_OUT</b>: arrivo di un processo alla Swap-Out Station. </li>

<li><b>DEPARTURE_DELAY</b>: partenza di un processo dalla Delay Station. </li>

<li><b>DEPARTURE_RESERVE</b>: partenza di un processo dalla Reserve Station. </li>

<li><b>DEPARTURE_SWAP_IN</b>: partenza di un processo dalla Swap-In Station. </li>

<li><b>DEPARTURE_CPU</b>: partenza di un processo dalla Cpu Station. </li>

<li><b>DEPARTURE_IO1</b>: partenza di un processo dalla IO1 Station. </li>

<li><b>DEPARTURE_IO2</b>: partenza di un processo dalla IO2 Station. </li>

<li><b>DEPARTURE_SWAP_OUT</b>:  partenza di un processo dalla Swap-Out Station. </li>
</ul>

## INIZIALIZZAZIONE DEL SIMULATORE 

<p align=”justify”>
Prima di tutto vengono inizializzati tutti i vari contatori utilizzati dal simulatore. 

Dopodichè, in base al parametro passato da riga di comando, viene lanciato il simulatore normale oppure una delle versioni semplificate dello stesso per scopi di validazione. 

La Feature Event List (FEL) del simulatore è inizializzata inserendo i vari eventi (uno per ogni terminale connesso) di arrivo alla Delay Station (il cui il tempo di occorrenza corrisponde al tempo di Start del simulatore). </p>

## GESTIONE DEI VARI EVENTI DEL SIMULATORE

<p align=”justify”>
<ul>
<li><b>ArrivalDelay</b>: gestisce semplicemente l’arrivo di un processo alla Delay Station, ritardando l’uscita dall’ultima del tempo di servizio attribuitogli. </li>

<li><b>DepartureDelay</b>: effettua l’inoltro del processo alla Reserve Station. </li>

<li><b>ArrivalReserve</b>: gestisce l’ammissione del processo al sistema. Nel caso in cui il grado di multiprogrammazione risulti essere uguale al numero di processi nel sistema, il job corrente è inserito (enqueue) nella relativa coda secondo la politica FCFS (First Come First Served). </li>

<li><b>DepartureReserve</b>: effettua l’inoltro del processo alla Reserve Station. </li>

<li><b>ArrivalSwapIn</b>: gestisce l’arrivo di un processo alla Swap-in Station, ritardando l’uscita dall’ultima del tempo di servizio attribuitogli. Nel caso in cui la stazione sia occupata, inserisce il processo nella relativa coda secondo la politica FCFS (First Come First Served). </li>

<li><b>DepartureSwapIn</b>: effettua l’inoltro del processo alla Cpu Station. Se presente, viene estratto (dequeue) un processo dalla relativa coda ritardando l’ultimo del tempo di servizio attribuitogli. </li>

<li><b>ArrivalCpu</b>: gestisce l’arrivo di un processo alla Cpu Station. Vengono quindi aggiornati i tempi di servizio rimasto e il tempo di servizio corrente in base alla grandezza del quanto di tempo utilizzato dall’algoritmo di scheduling Round-Robin. Dopodichè, nel caso la stazione sia libera, viene ritardato il processo del tempo di servizio attribuitogli, viceversa viene inserito (enqueue) nella relativa coda secondo la politica FCFS (First Come First Served). </li>

<li><b>DepartureCpu</b>: gestisce la partenza dalla Cpu Station. Nel caso in cui il processo ha finito il suo tempo di computazione (CPU Burst), esso può essere inoltrato probabilisticamente ad una delle due stazioni di I/O oppure direttamente alla Swap-Out Station; viceversa, viene rimandato alla Cpu Station per terminare il suo tempo di servizio attribuitogli. Se presente, viene estratto (dequeue) un processo dalla relativa coda ritardando l’ultimo del tempo di servizio rimanente. </li>

<li><b>ArrivalIO1</b>: gestisce l’arrivo di un processo alla IO1 Station, ritardando l’uscita dall’ultima del tempo di servizio attribuitogli. Nel caso in cui la stazione sia occupata, inserisce il processo nella relativa coda secondo la politica FCFS (First Come First Served). </li>

<li><b>DepartureIO1</b>: effettua l’inoltro del processo alla Cpu Station. Se presente, viene estratto (dequeue) un processo dalla relativa coda ritardando l’ultimo del tempo di servizio attribuitogli. </li>

<li><b>ArrivalIO2</b>: gestisce l’arrivo di un processo alla IO2 Station, ritardando l’uscita dall’ultima del tempo di servizio attribuitogli. Nel caso in cui la stazione sia occupata, inserisce il processo nella relativa coda secondo la politica FCFS (First Come First Served). </li>

<li><b>DepartureIO2</b>: effettua l’inoltro del processo alla Cpu Station. Se presente, viene estratto (dequeue) un processo dalla relativa coda ritardando l’ultimo del tempo di servizio attribuitogli. </li>

<li><b>ArrivalSwapOut</b>: gestisce l’arrivo di un processo alla Swap-Out Station. </li>

<li><b>DepartureSwapOut</b>: gestisce la partenza dalla Swap-Out Station, inoltrando probabilisticamente il processo alla Delay Station, se desidera ulteriore tempo di computazione oppure alla Reserve Station viceversa. Se presente, viene estratto (dequeue) un processo dalla Reserve Queue. </li></ul></p>

## PUNTO DI RIGENERAZIONE SCELTO

<p align=”justify”>
Data la natura della quantità da stimare (la media teorica del tempo di permanenza nel “sotto-sistema attivo” da parte di un processo), l’identificazione del punto di rigenerazione del sistema è stata effettuata mediante l'approccio del metodo del “tagged customer for passage times”. 

Il punto di rigenerazione da me scelto coincide quindi col verificarsi del seguente evento: 
<ul>
<li><b>Arrivo del “tagged” job (n.0) job alla Cpu Station. </b></li>

Punto di entrata del sotto-sistema. 

<li><b>Il “tagged customer”DEVE aver già attraversato il sotto-sistema. </b></li>

Questo per evitare di aggregare inutili regeneration points (nel caso in cui il “tagged customer” non abbia attraversato almeno una volta il sotto-sistema, esso non accumulerebbe nessun valore). 

<li><b>Numero di jobs alla Reserve Station = 0. </b></li>

Questa scelta è motivata dal fatto che la distribuzione dei tempi di servizio è sconosciuta, fortemente influenzata dal grado di multiprogrammazione scelto e in parte anche dai	 tempi di attesa in ciascuna stazione dei terminali connessi al sistema. Quindi in generale essa non gode della proprietà di assenza di memoria.  

<li><b>Numero di jobs alla Cpu Station = 0. </b></li>

Essendo che la Cpu Station presenta una distribuzione dei tempi di servizio iper-esponenziale, essa non gode della proprietà di assenza di memoria. 

Non avendo tempi di servizio distribuiti secondo l’esponenziale negativa (unica distribuzione di probabilità continua che gode della proprietà di assenza di memoria), una possibile situazione in cui, in almeno una delle due stazioni sia presente un job in esecuzione, non sarebbe un corretto punto di rigenerazione per il sistema poiché il tempo di servizio trascorso da questo ipotetico job influenzerebbe il tempo rimanente del medesimo nel successivo ciclo di rigenerazione. Si andrebbe quindi a violare la condizione di inter-indipendenza fra due cicli di rigenerazione adiacenti.

<li><b>Numero di jobs alla Swap-Out Station = 0 </b></li>

<li><b>Numero di jobs alla Swap-In Station = CUSTOMER_IN_SWAP_IN_FOR_REGENERATION_COND </b></li>

<li><b>Numero di jobs alla IO1 Station = CUSTOMER_IN_IO1_FOR_REGENERATION_COND </b></li>

<li><b>Numero di jobs alla IO2 Station = CUSTOMER_IN_IO2_FOR_REGENERATION_COND </b></li>
<br>
Queste tre ultime stazioni (tolta la Swap-Out station) presentano un numero variabile di clienti per poter incrementare la frequenza di occorrenza del punto di rigenerazione in base al modello del sistema scelto (modello originale o modello per validazione) e al numero di terminali connessi ad esso. 

Nel caso in cui si abbia scelto il modello per la validazione, l’assegnazione dei valori a queste variabili è stata effettuata mediante l’utilizzo di MVA, per appunto estrarre il numero medio di clienti per stazione (approssimato all’intero più vicino) come euristica per velocizzare la ricerca del punto rigenerazione scelto. 

Viceversa nel caso in cui si abbia scelto il modello originale, si utilizza lo stesso approccio di assegnazione di valori alle variabili del caso precedente solo qualora il grado di multiprogrammazione sia maggiore o uguale al numero di terminali connessi al sistema (questo perché il modello sarebbe rilassato dal vincolo introdotto dalla Reserve Station proprio come nel caso precedente). 

Nel caso invece si lanci il simulatore scegliendo il modello originale (in cui il grado di multiprogrammazione è in generale minore del numero di terminali connessi al sistema), il valore assegnato alle variabili sopracitate diventa: 

<ul>
<li>CUSTOMER_IN_SWAP_OUT_FOR_REGENERATION_COND = 0 </li>  
  
<li>CUSTOMER_IN_SWAP_IN_FOR_REGENERATION_COND = 1 </li>

<li>CUSTOMER_IN_IO1_FOR_REGENERATION_COND = 1 </li>

<li>CUSTOMER_IN_IO2_FOR_REGENERATION_COND = 6 </li>
</ul>
<br>
Questa scelta è motivata nuovamente dal fatto che, si è alla ricerca di quell’assegnazione di valori che massimizza la probabilità di occorrere in una condizione di rigenerazione per abbattere quindi i tempi dovuti all’attesa dell’evento (sempre in accordo con le proprietà che ogni punto di rigenerazione deve possedere per essere considerato tale). 

Infine, è garantito che ciascun ciclo di rigenerazione possieda almeno un numero di osservazioni sufficienti (almeno 30) per essere sicuri di avere una somma delle ultime (variabile aleatoria) quasi-normale. Nel caso in cui ciò non avvenga, si provvede ad accorpare più cicli di rigenerazione in uno. 
</ul></p>

# FIRST STEP VALIDATION

<p align=”justify”>
Il primo modello di validazione semplifica il modello originale modificando: 

<ul>
<li>La distribuzione di tempi di servizio della CPU: Esponenziale negativa di media λ = 2.7 msec. </li>
<li>CPU completion choice: q3,3 = 0.9, q3,4 = 0.065, q3,5 = 0.025, q3,6 = 0.01. </li>
<li>MPD = INT_MAX (massimo intero con segno in complemento a 2 rappresentabile in 32 bit). </li>
<li>CPU_QUANTUM = DBL_MAX (massimo extended floating-point number con segno rappresentabile in 64 bit). </li>
</ul>

Utilizzando l’algoritmo Mean Value Analysis (MVA) si possono osservare le varie medie teoriche dei tempi di risposta del sistema e dei tempi di permanenza nel sotto-sistema “attivo” per ogni carico del sistema (da 1 a 30 jobs). Utilizzando la classica formula di Little in questa forma: R = N / X0(N) - Z (X0(N) letto direttamente dall’output di MVA per ogni livello di carico) è possibile ottenere i vari tempi medi di risposta del sistema: 
</p>

## Author

* **Alberto Guastalla** - [AlbertoGuastalla](https://github.com/AlbertoGuastalla)
