#Operating System Scheduling

Il simulatore implementato riproduce una versione semplificata del reale protocollo di scheduling utilizzato nella maggior parte dei sistemi moderni. L'algoritmo simulato è Round-Robin con un relativo quanto di tempo.

<p align="center">
  <img src="https://github.com/AlebertoGuastalla/SIMMOD1920/blob/master/OSS.png"/>
</p>


## STAZIONI PRESENTI NEL MODELLO

Delay Station (Infinite Server Station): questa stazione rappresenta appunto una stazione di ritardo per i vari jobs collegati al sistema. Possiede una distribuzione esponenziale negativa per quanto riguarda il tempo di servizio. 

Reserve Station: questa stazione serve per limitare il numero di jobs “attivi simultaneamente” nel sistema (si tratta dunque di una stazione in cui tempo di servizio è distribuito secondo una distribuzione di probabilità sconosciuta). 

Swap-In Station: una volta che il processo è ammesso nel sistema, questa stazione carica l’immagine del processo all’interno della memoria principale (il tempo di servizio è distribuito secondo la distribuzione esponenziale negativa). 

CPU: stazione che rappresenta il core del sistema e che processa i vari jobs. In questo caso il tempo di servizio è distribuito secondo la distribuzione iper-esponenziale. 

IO1/IO2: queste due stazioni rappresentano le stazioni di Input/Output il cui tempo di servizio è distribuito secondo la distribuzione esponenziale negativa. 

Swap-Out Station: questa stazione serve per rilasciare la memoria adibita al processo. 

In questo caso però, il tempo di servizio è trascurabile.



Those 32 teams have been then combined, according to the contraints in 8 rounds on two halves season: `girone(a;b;c;d;e;f;g;h).` with 4 teams each.
The final calendar follows the constraints here below:
- max 1 team per nation in the same round;
- in every round, every team plays against the other 3 only once, both for first and second half season;
- matches are grouped in 3 days per half season, all the 32 teams play each day;
- two teams from the same town can't both play home match during the same day;
- a team can't play more than 2 consecutive matches home or more than 2 away.

The data provided already meet the constraints about at least 2 teams from the same town and at least 4 nations with 4 teams each.

We tried to constrain some teams on a specific round, in order to obtain an improvement over the execution time, but the perfomances didn't show improvements.
The overall time to find an answer set is about 50 seconds.

## Calendar
The following calendar is the clingo answer set resulting from file `finale.lp`. 
<p align="center">
  <img src="https://github.com/VittorioParagallo/IALAB_2019-2020/blob/master/ASP/img/calendario finale.png"/>
</p>


## Authors

* **Alberto Guastalla** - [AlebertoGuastalla](https://github.com/AlebertoGuastalla)
* **Tommaso Toscano**  - [DarkRaider95](https://github.com/DarkRaider95)
* **Vittorio Paragallo**  - [VittorioParagallo](https://github.com/VittorioParagallo)
