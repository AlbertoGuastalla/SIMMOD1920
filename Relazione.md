# <p align="center">Operating System Scheduling</p>

<p align=”justify”>Il simulatore implementato riproduce una versione semplificata del reale protocollo di scheduling utilizzato nella maggior parte dei sistemi moderni. L'algoritmo simulato è Round-Robin con un relativo quanto di tempo. </p>

<p align="center">
  <img src="https://github.com/AlebertoGuastalla/SIMMOD1920/blob/master/OSS.png"/>
</p>

## DETTAGLI DEL MODELLO

- Tempo medio di ritardo Z = 5000 msec
- Tempo medio di servizio della stazione Swap-In S1 = 210 msec
- Tempo medio di servizio della stazione CPU S2 = 27 msec
- Lunghezza del time slice (quanto della CPU) δ = 3 msec
- Tempo medio di servizio della stazione I/O1 S4 = 40 msec
- Tempo medio di servizio della stazione I/O2 S5 = 180 msec
- CPU completion choice q3,4 = 0.65, q3,5 = 0.25, q3,6 = 0.1
- Swap-out choice q6,0 = 0.4, q6,1 = 0.6.
- Multi-Programming Degree MPD = 10

Tutte le stazioni presentano una distribuzione dei tempi di servizio esponenziale negativa a meno della CPU, la quale presenta un quanto di tempo costante e una distribuzione dei tempi di servizio iper-esponenziale a due stadi:
- &#402;X(x) = α ∗ 1/µ1 ∗ exp(−x/µ1) + β ∗ 1/µ2 ∗ exp(−x/µ2) <br>
di parametri: α = 0.8, β = 0.2, µ1 = 15 msec, and µ2 = 75 msec.

## STAZIONI PRESENTI NEL MODELLO

<p align=”justify”>
<ul>
<li><b>Delay Station</b> (Infinite Server Station): questa stazione rappresenta appunto una stazione di ritardo per i vari jobs collegati al sistema. Possiede una distribuzione esponenziale negativa per quanto riguarda il tempo di servizio. </li>

<li><b>Reserve Station</b>: questa stazione serve per limitare il numero di jobs “attivi simultaneamente” nel sistema (si tratta dunque di una stazione il cui tempo di servizio è distribuito secondo una distribuzione di probabilità sconosciuta). </li>

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
  
Vengono inizializzazati i valori delle variabili con le quali poi si definirà il punto di rigenerazione del sistema (vedi la sezione: PUNTO DI RIGENERAZIONE SCELTO).

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

<li><b>DepartureSwapOut</b>: gestisce la partenza dalla Swap-Out Station, inoltrando probabilisticamente il processo alla Reserve Station, se desidera ulteriore tempo di computazione oppure alla Delay Station viceversa. Se presente, viene estratto (dequeue) un processo dalla Reserve Queue. </li></ul></p>

## BREVE SIMULAZIONE
Seme utilizzato: 918273645

<img src="https://github.com/AlebertoGuastalla/SIMMOD1920/blob/master/sim1.png"/>

## PUNTO DI RIGENERAZIONE SCELTO

<p align=”justify”>
Data la natura della quantità da stimare (la media teorica del tempo di permanenza nel “sotto-sistema attivo” da parte di un processo), l’identificazione del punto di rigenerazione del sistema è stata effettuata mediante l'approccio del metodo del “tagged customer for passage times”. 

Il punto di rigenerazione da me scelto coincide quindi col verificarsi del seguente evento: 
<ul>
<li><b>Partenza del “tagged” job (n.0) dalla Swap-In station. </b></li>

Punto di entrata del sotto-sistema. 

<li><b>Numero di jobs alla Reserve Station = 0. </b></li>

Questa scelta è motivata dal fatto che la distribuzione dei tempi di servizio è sconosciuta, fortemente influenzata dal grado di multiprogrammazione scelto, dai tempi di attesa di ciascuna stazione e dai probability branch che caratterizzano il modello. Quindi in generale essa non gode della proprietà di assenza di memoria.

Va però precisato che, non essendo una stazione con un servitore, non vi è la possibilità di interrompere un cliente mentre esso è in servizio, inolte, come in parte già giustificato, un possibile evento di partenza dall'ultima è determinato dal resto della rete; quindi, sarebbe stato possibile fissare arbitrariamente un numero di jobs alla Reserve Station per caratterizzare un punto di rigenerazione valido.

<li><b>Numero di jobs alla Cpu Station = 0. </b></li>

Essendo che la Cpu Station presenta una distribuzione dei tempi di servizio iper-esponenziale, essa non gode della proprietà di assenza di memoria. 

Non avendo tempi di servizio distribuiti secondo l’esponenziale negativa (unica distribuzione di probabilità continua che gode della proprietà di assenza di memoria), una possibile situazione in cui, in almeno una delle due stazioni sia presente un job in esecuzione, non sarebbe un corretto punto di rigenerazione per il sistema poiché il tempo rimanente del medesimo non segue la stessa distribuzione esponenziale negativa.

<li><b>Numero di jobs alla Swap-Out Station = 0 </b></li>

<li><b>Numero di jobs alla Swap-In Station = CUSTOMER_IN_SWAP_IN_FOR_REGENERATION_COND </b></li>

<li><b>Numero di jobs alla IO1 Station = CUSTOMER_IN_IO1_FOR_REGENERATION_COND </b></li>

<li><b>Numero di jobs alla IO2 Station = CUSTOMER_IN_IO2_FOR_REGENERATION_COND </b></li>
<br>
Queste tre ultime stazioni (tolta la Swap-Out station) presentano un numero variabile di clienti per poter incrementare la frequenza di occorrenza del punto di rigenerazione in base al modello del sistema scelto (modello originale o modello per validazione) e al numero di terminali connessi ad esso. <br>

### PUNTO DI RIGENERAZIONE (ORIGINAL MODEL)

Nel caso in cui si abbia scelto il modello originale, l’assegnazione dei valori a queste variabili è stata effettuata mediante l’utilizzo di MVA (risolvendo il primo modello di validazione), per appunto estrarre il numero medio di clienti per stazione (approssimato all’intero più vicino) come euristica per velocizzare la ricerca del punto rigenerazione scelto. 

Questa operazione è stata effettuata lanciando MVA su un processo figlio (mettendo quindi in attesa il simulatore per il tempo necessario affinchè quest'ultimo termini), il quale andrà a scrivere su file i vari valori medi per ciascuna stazione; dopodichè il padre riprendendo l'esecuzione, leggerà tali valori utilizzandoli come sopracitato.

Bisogna però tener presente che, questo approccio non interrompe, ne tantomeno invalida le prestazioni del simulatore poichè il tutto avviene nella fase di inizializzazione del medesimo.

Si utilizza questo approccio solo qualora il grado di multiprogrammazione sia maggiore o uguale al numero di terminali connessi al sistema (questo perché il modello sarebbe rilassato dal vincolo introdotto dalla Reserve Station), altrimenti, il valore assegnato alle variabili sopracitate diventa: 

<ul>
<li>CUSTOMER_IN_SWAP_OUT_FOR_REGENERATION_COND = 0 </li>  
  
<li>CUSTOMER_IN_SWAP_IN_FOR_REGENERATION_COND = 1 </li>

<li>CUSTOMER_IN_IO1_FOR_REGENERATION_COND = 1 </li>

<li>CUSTOMER_IN_IO2_FOR_REGENERATION_COND = 6 </li>
</ul>
<br>
Questa scelta è motivata nuovamente dal fatto che, si è alla ricerca di quell’assegnazione di valori che massimizza la probabilità di occorrere in una condizione di rigenerazione per abbattere quindi i tempi dovuti all’attesa dell’evento (sempre in accordo con le proprietà che ogni punto di rigenerazione deve possedere per essere considerato tale). <br>

### PUNTO DI RIGENERAZIONE (FIRST VALIDATION MODEL)
Nel caso in cui si abbia invece scelto il primo modello di validazione, l'assegnazione dei valori alle variabili che definiscono il punto di rigenerazione è guidata unicamente dai valori in output d MVA (risolvendo, come nel modello originale il primo modello di validazione).

### PUNTO DI RIGENERAZIONE (SECOND VALIDATION MODEL)
Nel caso in cui si abbia invece scelto il secondo modello di validazione, l'assegnazione dei valori alle variabili che definiscono il punto di rigenerazione è la medesima del primo modello di validazione (risolvendo però il secondo modello di validazione).

Infine, è garantito che ciascun ciclo di rigenerazione possieda almeno un numero di osservazioni sufficienti (almeno 30) per essere sicuri di avere una somma delle ultime (variabile aleatoria) approssimativamente normale come dimostrato dal teorema del limite centrale. Nel caso in cui ciò non avvenga, si provvede ad accorpare più cicli di rigenerazione in uno. 
</ul></p>

# VALIDATION MODELS

<p align="center">
  <img width="1000" height="400" src="https://github.com/AlebertoGuastalla/SIMMOD1920/blob/master/validation.png"/>
</p>

## FIRST VALIDATION STEP

### DETTAGLI DEL MODELLO

- Tempo medio di ritardo Z = 5000 msec
- Tempo medio di servizio della stazione Swap-In S1 = 210 msec
- Tempo medio di servizio della stazione CPU S2 = 2.7 msec
- Lunghezza del time slice (quanto della CPU) δ = DBL_MAX (massimo extended floating-point number con segno rappresentabile in 64 bit)
- Tempo medio di servizio della stazione I/O1 S4 = 40 msec
- Tempo medio di servizio della stazione I/O2 S5 = 180 msec
- CPU completion choice q33 = 0.9, q34 = 0.065, q35 = 0.025, q36 = 0.01
- Swap-out choice q6,0 = 0.4, q6,1 = 0.6.
- Multi-Programming Degree MPD = INT_MAX (massimo intero con segno in complemento a 2 rappresentabile in 32 bit)

Tutte le stazioni presentano una distribuzione dei tempi di servizio esponenziale negativa.

Utilizzando l’algoritmo Mean Value Analysis (MVA) si possono osservare le varie medie teoriche dei tempi di risposta del sistema e dei tempi di permanenza nel sotto-sistema “attivo” per ogni carico del sistema (da 1 a 30 jobs). Utilizzando la classica formula di Little in questa forma: R = N / X<SUB>0</SUB>(N) - Z (X<SUB>0</SUB>(N) letto direttamente dall’output di MVA per ogni livello di carico) è possibile ottenere i vari tempi medi di risposta del sistema: 
</p>

<table style="width:100%">
  <tr>
    <th>LIVELLO DI CARICO DEL SISTEMA</th>
    <th>RESPONSE TIME MEDIO</th>
    <th>LIVELLO DI CARICO DEL SISTEMA</th>
    <th>RESPONSE TIME MEDIO</th>
  </tr>
  <tr>
    <td>1</td>
    <td>2975.000000 </td>
    <td>16</td>
    <td>13233.567629 </td>
  </tr>
  <tr>
    <td>2</td>
    <td>3278.369906 </td>
    <td>17</td>
    <td>14290.239909 </td>
  </tr>
  <tr>
    <td>3</td>
    <td>3624.658248 </td>
    <td>18</td>
    <td>15365.471246 </td>
  </tr>
  <tr>
    <td>4</td>
    <td>4019.039128 </td>
    <td>19</td>
    <td>16454.787398 </td>
  </tr>
  <tr>
    <td>5</td>
    <td>4466.521677 </td>
    <td>20</td>
    <td>17554.567286 </td>
  </tr>
  <tr>
    <td>6</td>
    <td>4971.604695 </td>
    <td>21</td>
    <td>18661.972927 </td>
  </tr>
  <tr>
    <td>7</td>
    <td>5537.884825 </td>
    <td>22</td>
    <td>19774.841442 </td>
  </tr>
  <tr>
    <td>8</td>
    <td>6167.667693 </td>
    <td>23</td>
    <td>20891.564085 </td>
  </tr>
  <tr>
    <td>9</td>
    <td>6861.647033 </td>
    <td>24</td>
    <td>22010.969291 </td>
  </tr>
  <tr>
    <td>10</td>
    <td>7618.718170 </td>
    <td>25</td>
    <td>23132.219380 </td>
  </tr>
  <tr>
    <td>11</td>
    <td>8435.974603 </td>
    <td>26</td>
    <td>24254.724870 </td>
  </tr>
  <tr>
    <td>12</td>
    <td>9308.902088 </td>
    <td>27</td>
    <td>25378.076649 </td>
  </tr>
  <tr>
    <td>13</td>
    <td>10231.743119 </td>
    <td>28</td>
    <td>26501.994194 </td>
  </tr>
  <tr>
    <td>14</td>
    <td>11197.969502 </td>
    <td>29</td>
    <td>27626.287177 </td>
  </tr>
  <tr>
    <td>15</td>
    <td>12200.782963 </td>
    <td>30</td>
    <td>28750.827655 </td>
  </tr>
</table>

<p align=”justify”>
Mentre per quanto riguarda il tempo medio di permanenza nel sotto-sistema, esso è stato calcolato ricavando dapprima il throughput della stazione Swap-In e il numero medio di clienti nel sotto-sistema direttamente leggendo l'output prodotto da MVA. Dopodichè si sono sommati gli ultimi relativi a ciascuna stazione del sotto-sistema (poiché il valore atteso gode della proprietà di linearità i.e. E[n<SUB>CPU</SUB>] + E[n<SUB>IO1</SUB>] + E[n<SUB>IO2</SUB>] = E[n<SUB>CPU</SUB> + n<SUB>IO1</SUB> + n<SUB>IO2</SUB>]) dividendo il tutto per il flusso entrante in esso (throughput della stazione Swap-In).

In formule: <b>AverageActiveTime = E[n<SUB>CPU</SUB> + n<SUB>IO1</SUB> + n<SUB>IO2</SUB>] / X<SUB>SI</SUB></b> (ancora una volta la formula di Little). 
Qui sotto i vari tempi medi di permanenza nel sotto-sistema per ogni livello di carico: </p>

<table style="width:100%">
  <tr>
    <th>LIVELLO DI CARICO DEL SISTEMA</th>
    <th>TEMPO MEDIO DI PERMANENZA</th>
    <th>LIVELLO DI CARICO DEL SISTEMA</th>
    <th>TEMPO MEDIO DI PERMANENZA</th>
  </tr>
  <tr>
    <td>1</td>
    <td>980.000000 </td>
    <td>16</td>
    <td>4909.305363 </td>
  </tr>
  <tr>
    <td>2</td>
    <td>1087.523511 </td>
    <td>17</td>
    <td>5329.135412 </td>
  </tr>
  <tr>
    <td>3</td>
    <td>1211.474174 </td>
    <td>18</td>
    <td>5757.153767 </td>
  </tr>
  <tr>
    <td>4</td>
    <td>1354.081999 </td>
    <td>19</td>
    <td>6191.394795 </td>
  </tr>
  <tr>
    <td>5</td>
    <td>1517.575700 </td>
    <td>20</td>
    <td>6630.261907 </td>
  </tr>
  <tr>
    <td>6</td>
    <td>1704.040924 </td>
    <td>21</td>
    <td>7072.500922 </td>
  </tr>
  <tr>
    <td>7</td>
    <td>1915.249343 </td>
    <td>22</td>
    <td>7517.154779 </td>
  </tr>
  <tr>
    <td>8</td>
    <td>2152.477838 </td>
    <td>23</td>
    <td>7963.511253 </td>
  </tr>
  <tr>
    <td>9</td>
    <td>2416.346303 </td>
    <td>24</td>
    <td>8411.051703 </td>
  </tr>
  <tr>
    <td>10</td>
    <td>2706.706390 </td>
    <td>25</td>
    <td>8859.405489 </td>
  </tr>
  <tr>
    <td>11</td>
    <td>3022.608435 </td>
    <td>26</td>
    <td>9308.312014 </td>
  </tr>
  <tr>
    <td>12</td>
    <td>3362.359476 </td>
    <td>27</td>
    <td>9757.590621 </td>
  </tr>
  <tr>
    <td>13</td>
    <td>3723.665580 </td>
    <td>28</td>
    <td>10207.117598 </td>
  </tr>
  <tr>
    <td>14</td>
    <td>4103.833047 </td>
    <td>29</td>
    <td>10656.809130 </td>
  </tr>
  <tr>
    <td>15</td>
    <td>4499.991995 </td>
    <td>30</td>
    <td>11106.608965 </td>
  </tr>
</table>

Lanciando il simulatore con il parametro "-verbosity" specificato, è possibile visualizzare su riga di comando, il valore simulato del tempo medio di risposta e del tempo medio di permanenza nel sotto-sistema per ciascun cliente.
Si sono quindi dapprima calcolate le somme di entrambi i tempi:

- Tempo di risposta: Departure_Swap_Out_Time (nel caso venga scelto il ramo q60) - Departure_Delay_Time.
- Tempo di permanenza: Arrival_Swap_Out_Time - Departure_Swap_In_Time.

Infine si sono calcolate le due medie, dividendo rispettivamente entrambe le somme per il numero di passaggi alla Delay station e per il numero di passaggi al sotto-sistema.

### BREVE SIMULAZIONE
Seme utilizzato: 918273645

<img src="https://github.com/AlebertoGuastalla/SIMMOD1920/blob/master/sim2.png"/>

### EXTENSIVE VALIDATION 

<p align=”justify”>
Test effettuato su 100 simulazioni utilizzando semi iniziali differenti. <br>
Semi utilizzati: 100000000 ÷ 100000099. <br>
Numero di terminali connessi al sistema: 20. <br>
Valore teorico per il tempo medio di permanenza nel sotto-sistema per il dato livello di carico: 6630.261907 <br>
<img align=”center” src="https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/intervals.png"/> <br>
Teoricamente, con un livello di fiducia al 90%, ci si aspetterebbe che il 90% degli intervalli coprissero il valore teorico, mentre i restanti 10% viceversa (il 50% degli intervalli sia centrato su una stima più grande o uguale del valore teorico e l'altro 50% viceversa). Nell’esempio riportato, 91 intervalli contengono il valore teorico e 9 viceversa; 48 intervalli sono "sbilancianti" verso l'alto, mentre i rimanenti 52 verso il basso.

### BOTTLENECK ANALYSIS 

Una prima e approssimativa analisi delle strozzature si può facilmente vedere lanciando MVA e osservando che l’utilizzazione della IO2 Station arriva quasi ad essere 100% con un numero di terminali connessi al sistema pari a 20. 

<img src="https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/mva.png"/>

#0: Delay Station <br>
#1: Swap-In Station <br>
#2: CPU Station <br>
#3: IO1 Station <br>
#4: IO2 Station <br>

<p align=”justify”>
Con un’analisi più accurata invece si possono osservare i seguenti valori: 

<ul>
<li><b>Stazione bottleneck</b>: V<SUB>b</SUB> * S<SUB>b</SUB> = V<SUB>IO2</SUB> * S<SUB>IO2</SUB> = MAX {V<SUB>i</SUB> * S<SUB>i</SUB>} = max { 2.5 * 210, 250 * 2.7, 16.25 * 40, 6.25 * 180 } = 6.25 * 180 = 1125 (Quindi effettivamente la stazione IO2 risulta essere la stazione bottleneck del sistema). </li>

<li><b>Tempo medio di risposta del sistema con 1 cliente</b> = R<SUB>0</SUB>(1) = D = ∑V<SUB>i</SUB> * S<SUB>i</SUB> = V<SUB>SI</SUB> * S<SUB>SI</SUB> + V<SUB>CPU</SUB> * S<SUB>CPU</SUB> + V<SUB>IO1</SUB> * S<SUB>IO1</SUB> + V<SUB>IO2</SUB> * S<SUB>IO2</SUB> = 2.5 * 210 + 250 * 2.7 + 16.25 * 40 + 6.25 * 180 = 2975 msec </li> 
</ul>

Dove le varie visite alle stazioni sono state calcolate risolvendo il sistema lineare di 5 equazioni a 4 incognite V = VQ (poiché, essendo la Delay Station la stazione di riferimento, V<SUB>DS</SUB> risulta essere per definizione uguale ad 1). 

Il sistema di equazioni lineare è stato risolto attraverso il metodo di eliminazione di Gauss e l'unica soluzione risulta quindi essere: <b>V<SUB>DS</SUB> = 1, V<SUB>SI</SUB> = 2.5, V<SUB>CPU</SUB> = 250, V<SUB>IO1</SUB> = 16.25, V<SUB>IO2</SUB> = 6.25</b>.

La Reserve e la Swap-Out Station sono state omesse poichè irrilevanti nel calcolo degli indici di prestazione medi.
</p>
 
### ANALISI BOTTLENECK PER IL THROUGHPUT   

<ul>
<li><b>Tempo medio di ciclo del sistema con 1 cliente: </b> Y<SUB>0</SUB>(1) = R<SUB>0</SUB>(1) + Z = 2975 + 5000 = 7975 msec </li>
<li><b>Throughput del sistema con 1 cliente: </b> X<SUB>0</SUB>(1) = 1 / Y<SUB>0</SUB>(1) = 1 / 7975 = 0.00012539 jobs / msec </li>
</ul>

Da questo, l'asintoto obliquo per il throughput risulta essere: NX<SUB>0</SUB>(1) = N * 0.00012539 

Infine per quanto riguarda l’analisi bottleneck riguardante il throughput si possono ottenere facilmente gli asintoti orizzontali: 

<ul>
<li>1 / V<SUB>b</SUB> * S<SUB>b</SUB> = 1 / 1125 = 0.00088888888 (IO2) </li>
<li>1 / V<SUB>CPU</SUB> * S<SUB>CPU</SUB> = 1 / 675 = 0.00148148148 (CPU) </li>
<li>1 / V<SUB>IO1</SUB> * S<SUB>IO1</SUB> = 1 / 650 = 0.00153846153 (IO1) </li>
<li>1 / V<SUB>SI</SUB> * S<SUB>SI</SUB> = 1 / 525 = 0.0019047619 (Swap-In) </li>
</ul>

<img src="https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/throughput.png"/> 

Il valore di N* (il livello di carico superato il quale si `e certi che si formino code all’interno del sistema) è determinato dall'intersezione dell’asintoto orizzontale (1 / V<SUB>b</SUB> * S<SUB>b</SUB>) con l'asintoto obliquo (throughput del sistema qualora il tempo di ciclo rimanesse identico a quello in cui vi è un solo cliente nel sistema).

In formule: N* * 0.00012539 = 0.00088888888, che significa: N* = 7.0888

### ANALISI BOTTLENECK PER IL RESPONSE TIME 

L'asintoto orizzontale per il response time risulta essere: R<SUB>0</SUB>(1) = 2975 

Infine per quanto riguarda l’analisi bottleneck riguardante il response time si possono ottenere facilmente gli asintoti obliqui: 

<ul>
<li>N * V<SUB>b</SUB> * S<SUB>b</SUB> – Z = 1125 * N – 5000 (IO2) </li>
<li>N * V<SUB>CPU</SUB> * S<SUB>CPU</SUB> – Z = 675 * N – 5000 (CPU) </li>
<li>N * V<SUB>IO1</SUB> * S<SUB>IO1</SUB> – Z = 650 * N – 5000 (IO1) </li>
<li>N * V<SUB>SI</SUB> * S<SUB>SI</SUB> – Z = 525 * N – 5000 (Swap-In) </li>
</ul>

<img width="700" height="500" src="https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/responsetime.png"/> 

Il valore di N*, anche questa volta, è determinato dall’intersezione dell’asintoto orizzontale (R<SUB>0</SUB>(1)) con l’asintoto obliquo (response time del sistema qualora il numero di terminali tendesse ad infinito). 

In formule:  1125 * N* – 5000 = 2975, che significa: N* = 7.0888 <br>

## SECOND VALIDATION STEP

### DETTAGLI DEL MODELLO

- Tempo medio di ritardo Z = 5000 msec
- Tempo medio di servizio della stazione Swap-In S1 = 0.0 msec
- Tempo medio di servizio della stazione CPU S2 = 27 msec
- Lunghezza media del time slice (quanto della CPU) δ = 3 msec
- Tempo medio di servizio della stazione I/O1 S4 = 40 msec
- Tempo medio di servizio della stazione I/O2 S5 = 180 msec
- CPU completion choice q3,4 = 0.65, q3,5 = 0.25, q3,6 = 0.1
- Swap-out choice q6,0 = 0.4, q6,1 = 0.6.
- Multi-Programming Degree MPD = INT_MAX (massimo intero con segno in complemento a 2 rappresentabile in 32 bit)

Tutte le stazioni presentano una distribuzione dei tempi di servizio esponenziale negativa a meno della CPU, la quale presenta un quanto di tempo costante e una distribuzione dei tempi di servizio iper-esponenziale a due stadi:
- &#402;X(x) = α ∗ 1/µ1 ∗ exp(−x/µ1) + β ∗ 1/µ2 ∗ exp(−x/µ2) <br>
di parametri: α = 0.8, β = 0.2, µ1 = 15 msec, and µ2 = 75 msec.

### CATENA DI MARKOV A TEMPO CONTINUO (CTMC)

Per poter valutare il modello semplificato si è dovuto risolvere (trovare la distribuzione a regime) la catena di Markov a tempo continuo che descrive l'evoluzione dl sistema.
Dopo aver compilato la matrice Q che rappresenta il generatore infinitesimale della catena (https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/Markov%20Chain.xlsx), si è quindi risolto il seguente sistema di equazioni lineare (attraverso un solver apposito):
- <b>πQ</b> = <b>0</b>

trovando la seguente distribuzione limite (<b>π</b>):

<table>
  <tr>
    <td>π1 (3,0,0,0)</td>
    <td>0.2955257595</td>
    <td>π11 (1,(2,B),0,0)</td>
    <td>0.0033695517</td>
    <td>π21 (0,(2,B),1,0)</td>
    <td>0.0004338117</td>
  </tr>
  <tr>
    <td>π2 (0,(3,A),0,0)</td>
    <td>0.0010493937</td>
    <td>π12 (1,0,2,0)</td>
    <td>0.029980932</td>
    <td>π22 (0,(2,A),0,1)</td>
    <td>0.0025798483</td>
  </tr>
  <tr>
    <td>π3 (0,(3,B),0,0)</td>
    <td>0.0003091759</td>
    <td>π13 (1,0,0,2)</td>
    <td>0.0897702381</td>
    <td>π23 (0,(2,B),0,1)</td>
    <td>0.0007562691</td>
  </tr>
  <tr>
    <td>π4 (0,0,3,0)</td>
    <td>0.0038985165</td>
    <td>π14 (0,(1,A),2,0)</td>
    <td>0.0021275043</td>
    <td>π24 (0,(1,A),1,1)</td>
    <td>0.0036822808</td>
  </tr>
  <tr>
    <td>π5 (0,0,0,3)</td>
    <td>0.0201986299</td>
    <td>π15 (0,(1,B),2,0)</td>
    <td>0.0006081991</td>
    <td>π25 (0,(1,B),1,1)</td>
    <td>0.0010507244</td>
  </tr>
  <tr>
    <td>π6 (2,(1,A),0,0)</td>
    <td>0.0628669158</td>
    <td>π16 (0,(1,A),0,2)</td>
    <td>0.0063663591</td>
    <td>π26 (1,0,1,1)</td>
    <td>0.0518715347</td>
  </tr>
  <tr>
    <td>π7 (2,(1,B),0,0)</td>
    <td>0.0181319005</td>
    <td>π17 (0,(1,B),0,2)</td>
    <td>0.0018325876</td>
    <td>π27 (1,(1,A),0,1)</td>
    <td>0.0282945707</td>
  </tr>
  <tr>
    <td>π8 (2,0,1,0)</td>
    <td>0.115255928</td>
    <td>π18 (0,0,1,2)</td>
    <td>0.0116715845</td>
    <td>π28 (1,(1,B),0,1)</td>
    <td>0.0081441227</td>
  </tr>
  <tr>
    <td>π9 (2,0,0,1)</td>
    <td>0.19948052</td>
    <td>π19 (0,0,2,1)</td>
    <td>0.0067469732</td>
    <td>π29 (1,(1,A),1,0)</td>
    <td>0.0163612658</td>
  </tr>
  <tr>
    <td>π10 (1,(2,A),0,0)</td>
    <td>0.0114663278</td>
    <td>π20 (0,(2,A),1,0)</td>
    <td>0.0014916223</td>
    <td>π30 (1,(1,B),1,0)</td>
    <td>0.004676952</td>
  </tr>
</table>

<img src="https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/distribution1.png"/>

## CACLOLO DEGLI INDICI DI PRESTAZIONE MEDI

### NUMERO MEDIO DI JOBS

- Numero medio di jobs in Delay station E[n<SUB>DS</SUB>]: 1.921983303
- Numero medio di jobs in CPU station E[n<SUB>CPU</SUB>]: 0.1984139534
- Numero medio di jobs in IO1 station E[n<SUB>IO1</SUB>]: 0.2971184709
- Numero medio di jobs in IO2 station E[n<SUB>IO2</SUB>]: 0.5824842722

Calcolati attaverso: E[n] = &#8721; k<SUB>m</SUB> * π<SUB>i</SUB> con k<SUB>m</SUB> > 0, i = 1,...,30 e m = 0,...,3 (ottenuto attraverso la relativa distribuzione marginale di ciascuna delle M stazioni presenti nel modello).

### UTILIZZAZIONI

- Utilizzazione della Delay station U<SUB>DS</SUB>: 0.9351965193
- Utilizzazione della CPU station U<SUB>CPUS</SUB>: 0.1755993833
- Utilizzazione della IO1 station U<SUB>IO1</SUB>: 0.2498578293
- Utilizzazione della IO2 station U<SUB>IO2</SUB>: 0.4324462431

Calcolate attaverso: U = &#8721; π<SUB>i</SUB> con k<SUB>m</SUB> > 0, i = 1,...,30 e m = 0,...,3 (ottenuta attraverso la relativa distribuzione marginale di ciascuna delle M stazioni presenti nel modello).

### THROUGHPUTS

- Throughput della Delay station X<SUB>DS</SUB>: 0.0003843966605
- Throughput della CPU station X<SUB>CPU</SUB>: 0.06814304427
- Throughput della IO1 station X<SUB>IO1</SUB>: 0.006246445733
- Throughput della IO2 station X<SUB>IO2</SUB>: 0.002402479128

Calcolati attaverso: X = &#8721; π<SUB>i</SUB> * 1 / S(k<SUB>m</SUB>) con k<SUB>m</SUB> > 0, i = 1,...,30 e m = 0,...,3 (ottenuto attraverso la relativa distribuzione marginale di ciascuna delle M stazioni presenti nel modello).

In questo caso S(k<SUB>m</SUB>) = S<SUB>m</SUB> (per CPU, IO1 e IO2 Station essendo stazioni load independent), S(k<SUB>m</SUB>) = S<SUB>m</SUB> / k<SUB>DS</SUB> (per Delay Station essendo l'unica stazione load dependent)

### WAITING TIMES

- Waiting time della Delay station X<SUB>DS</SUB>: 5000
- Waiting time della CPU station X<SUB>CPU</SUB>: 3.050794736
- Waiting time della IO1 station X<SUB>IO1</SUB>: 47.56600531
- Waiting time della IO2 station X<SUB>IO2</SUB>: 242.4513351

Calcolati attaverso: W = E[n] / X (formula di Little per CPU, IO1 e IO2)

### RESPONSE TIME
- Response Time = (N / X<SUB>DS</SUB>) - Z = (3 / 0.0003843966605) - 5000 = 2804.438249

### AVERAGE ACTIVE TIME

Dato che essendo il sistema in equilibrio operazionale, denotando il flusso in entrata al sotto-sistema con λ e quello in uscita con μ, deve per forza valere che λ = μ.

Ma dato che, λ = X<SUB>DS</SUB> + q61 * μ = μ, allora μ = X<SUB>DS</SUB> / 0.4 = 0.0009609916513.

Sempre considerando il fatto che l'operatore valore atteso gode della proprietà di linearità, il tempo medio di permanenza del sotto-sistema attivo è quindi:
<b>AverageActiveTime = E[n<SUB>CPU</SUB> + n<SUB>IO1</SUB> + n<SUB>IO2</SUB>] / μ</b> = 1121.775299.

### BREVE SIMULAZIONE
Seme utilizzato: 123456789

<img src="https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/simulation2.png"/>

### CONFRONTO CON MVA

Imponendo entrambi gli stadi esponenziali della CPU a media η = 27 msec, l'iper-esponenziale si riduce ad una singola esponenziale negativa di media η = 27 msec rendendo cosi identiche l'analisi effettuata con la catena di Markov da quella effettuata con MVA durante la prima validazione (con l'unica differenza che il tempo medio di servizio della Swap-In è 0.0).

### DISTRIBUZIONE LIMITE

<table>
   <tr>
      <td>π1 (3,0,0,0)</td>
      <td>0.2685755224</td>
      <td>π2 (0,(3,A),0,0)</td>
      <td>0.0031718232</td>
      <td>π3 (0,(3,B),0,0)</td>
      <td>0.0007929558</td>
   </tr>
   <tr>
      <td>π4 (0,0,3,0)</td>
      <td>0.0035403625</td>
      <td>π5 (0,0,0,3)</td>
      <td>0.0183554584</td>
      <td>π6 (2,(1,A),0,0)</td>
      <td>0.0870184693</td>
   </tr>
   <tr>
      <td>π7 (2,(1,B),0,0)</td>
      <td>0.0217546173</td>
      <td>π8 (2,0,1,0)</td>
      <td>0.1047444537</td>
      <td>π9 (2,0,0,1)</td>
      <td>0.1812884776</td>
   </tr>
   <tr>
      <td>π10 (1,(2,A),0,0)</td>
      <td>0.0114663278</td>
      <td>π11 (1,(2,B),0,0)</td>
      <td>0.0033695517</td>
      <td>π12 (1,0,2,0)</td>
      <td>0.029980932</td>
   </tr>
   <tr>
      <td>π13 (1,0,0,2)</td>
      <td>0.0234949867</td>
      <td>π14 (0,(1,A),2,0)</td>
      <td>0.0058737467</td>
      <td>π15 (0,(1,B),2,0)</td>
      <td>0.0058737467</td>
   </tr>
   <tr>
      <td>π16 (0,(1,A),0,2)</td>
      <td>0.0063663591</td>
      <td>π17 (0,(1,B),0,2)</td>
      <td>0.0018325876</td>
      <td>π18 (0,0,1,2)</td>
      <td>0.0116715845</td>
   </tr>
   <tr>
      <td>π19 (0,0,2,1)</td>
      <td>0.0067469732</td>
      <td>π20 (0,(2,A),1,0)</td>
      <td>0.0014916223</td>
      <td>π21 (0,(2,B),1,0)</td>
      <td>0.0004338117</td>
   </tr>
   <tr>
      <td>π22 (0,(2,A),0,1)</td>
      <td>0.0025798483</td>
      <td>π23 (0,(2,B),0,1)</td>
      <td>0.0007562691</td>
      <td>π24 (0,(1,A),1,1)</td>
      <td>0.0036822808</td>
   </tr>
   <tr>
      <td>π25 (0,(1,B),1,1)</td>
      <td>0.0010507244</td>
      <td>π26 (1,0,1,1)</td>
      <td>0.0518715347</td>
      <td>π27 (1,(1,A),0,1)</td>
      <td>0.0282945707</td>
   </tr>
   <tr>
      <td>π28 (1,(1,B),0,1)</td>
      <td>0.0081441227</td>
      <td>π29 (1,(1,A),1,0)</td>
      <td>0.0163612658</td>
      <td>π30 (1,(1,B),1,0)</td>
      <td>0.004676952</td>
   </tr>
</table>

<img src="https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/distribution2.png"/>

### INDICI DI PRESTAZIONE MEDI DERIVATI DALLA DISTRIBUZIONE LIMITE

### NUMERO MEDIO DI JOBS

- Numero medio di jobs in Delay station E[n<SUB>DS</SUB>]: 1.857884605
- Numero medio di jobs in CPU station E[n<SUB>CPU</SUB>]: 0.2985386137
- Numero medio di jobs in IO1 station E[n<SUB>IO1</SUB>]: 0.2856433626
- Numero medio di jobs in IO2 station E[n<SUB>IO2</SUB>]: 0.5579334187

### UTILIZZAZIONI

- Utilizzazione della Delay station U<SUB>DS</SUB>: 0.9259275423
- Utilizzazione della CPU station U<SUB>CPU</SUB>: 0.2508144219
- Utilizzazione della IO1 station U<SUB>IO1</SUB>: 0.2415249987
- Utilizzazione della IO2 station U<SUB>IO2</SUB>: 0.4180240361

### THROUGHPUTS

- Throughput della Delay station X<SUB>DS</SUB>: 0.000371576921
- Throughput della CPU station X<SUB>CPU</SUB>: 0.09289423033
- Throughput della IO1 station X<SUB>IO1</SUB>: 0.006038124968
- Throughput della IO2 station X<SUB>IO2</SUB>: 0.002322355756

### WAITING TIMES

- Waiting time della Delay station X<SUB>DS</SUB>: 5000
- Waiting time della CPU station X<SUB>CPU</SUB>: 3.213747642
- Waiting time della IO1 station X<SUB>IO1</SUB>: 47.30663312
- Waiting time della IO2 station X<SUB>IO2</SUB>: 240.2445953

### RESPONSE TIME
- Response Time = 3073.698419

### AVERAGE ACTIVE TIME

μ = X<SUB>DS</SUB> / 0.4 = 0.0009289423026. <br>
<b>AverageActiveTime = E[n<SUB>CPU</SUB> + n<SUB>IO1</SUB> + n<SUB>IO2</SUB>] / μ</b> = 1229,479368.

In entrambe le versioni della seconda validazione, va precisato però che, per il calcolo degli indici di prestazioni riguardanti la stazione CPU, si sono utilizzati dei tempi medi di servizio pari a 1 / (μ<SUB>i</SUB> + δ) (rispettivamente per entrambi gli stadi della CPU), dove μ<SUB>i</SUB> rappresenta il tasso di servizio dello stadio i della CPU e δ il tasso dell'esponenziale quanto di tempo. 1 / (μ<SUB>i</SUB> + δ) rappresenta quindi il valore atteso dell'esponenziale minima fra le due sopracitate, nonchè il tempo medio di servizio tenendo conto della presenza di un quanto di tempo aleatorio.

### BREVE SIMULAZIONE
Seme utilizzato: 123456789

<img src="https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/simulation1.png"/>

### INDICI DI PRESTAZIONE MEDI DERIVATI DERIVATI DA MVA
<img src="https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/mva2.png"/>

#0: Delay Station <br>
#1: Swap-In Station <br>
#2: CPU Station <br>
#3: IO1 Station <br>
#4: IO2 Station <br>

### EXTENSIVE VALIDATION 

<p align=”justify”>
Test effettuato su 100 simulazioni utilizzando semi iniziali differenti. <br>
Semi utilizzati: 10000000 ÷ 10000099. <br>
Numero di terminali connessi al sistema: 3. <br>
Valore teorico per il tempo medio di permanenza nel sotto-sistema per il dato livello di carico: 1121.775299 <br> <br>
<img align=”center” src="https://github.com/AlbertoGuastalla/SIMMOD1920/blob/master/intervals2.png"/> <br>
Teoricamente, con un livello di fiducia al 90%, ci si aspetterebbe che il 90% degli intervalli coprissero il valore teorico, mentre i restanti 10% viceversa (il 50% degli intervalli sia centrato su una stima più grande o uguale del valore teorico e l'altro 50% viceversa). Nell’esempio riportato, 89 intervalli contengono il valore teorico e 11 viceversa; 50 intervalli sono "sbilancianti" verso l'alto, mentre i rimanenti 50 verso il basso.

Questo è dovuto al fatto che 100 simulazioni differenti offrono una buona approssimazione ma per potersi avvicinare maggiormente ai risultati teorici, bisognerebbe effettuarne un numero molto più grande.

## QUATTRO CASI DI TEST (MODELLO ORIGINALE)
Seme utilizzato: 918273645

<ol type="1">
  <li><b>δ = 3; CPU ~ Hyper-Exp (α = 0.8, β = 0.2, µ1 = 15, µ2 = 75) </b> <br>
  Sample Average Active Time = 4193.819712 (point-estimate) <br>
  Left Extreme = 4177.677693 <br>
  Right Extreme = 4209.961731 <br>
  Larghezza dell’intervallo: 32.284038 </li>
  <li><b>δ = 3; CPU ~ Exp (1/27) </b> <br>
  Sample Average Active Time = 4198.465349 (point-estimate) <br>
  Left Extreme = 4182.213559 <br>
  Right Extreme = 4214.717138 <br>
  Larghezza dell’intervallo: 32.503579 </li>
  <li><b>δ = 3000; CPU ~ Hyper-Exp (α = 0.8, β = 0.2, µ1 = 15, µ2 = 75) </b> <br>
  Sample Average Active Time = 4245.031203 (point-estimate) <br>
  Left Extreme = 4233.128181 <br>
  Right Extreme = 4256.934225 <br>
  Larghezza dell’intervallo: 23.806044 </li>
  <li><b>δ = 3000; CPU ~ Exp (1/27)  </b> <br>
  Sample Average Active Time = 4201.508537 (point-estimate) <br>
  Left Extreme = 4187.308633 <br>
  Right Extreme = 4215.708442 <br>
  Larghezza dell’intervallo: 28.399809 </li>
</ol>

Ciò che si può osservare è che la dimensione dell'intervallo nel caso in cui il quanto di tempo sia uguale 3000 msec risulta essere leggermente più piccola rispetto a quella in cui lo stesso vale 3 msec (indipendentemente dalla scelta della distribuzione dei tempi di servizio della CPU).

## AUTORE

* **Alberto Guastalla** - [AlbertoGuastalla](https://github.com/AlbertoGuastalla)
