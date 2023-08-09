# .

En donde tenga sentido todas los graficos tienen una linea vertical
punteada con coordenada $N = n \log n$ ya que teoricamente para ese
valor de $N$ y superiores los $n$ sets disjuntos iniciales formaran
un unico set y las siguientes llamadas a `Union(i,j)` solo afectaran
a la estructura solo a travez de una estrategia de compresion de
caminos.

## *Total path length* en funcion de $N$

## Analisis y comparacion entre las estrategias salvo NC

Las siguientes figuras muestran la evolucion del *total path length* (TPL) en funcion de la cantidad de
llamadas a `Union(i,j)` ($N$) para cada combinacion de estrategia de unión
y de compresion con excepcion de NC (estrategia sin compresion). Cada
fila corresponde a un numero inicial de sets disjuntos ($n$): en orden
$n=2^{10}$, $n=2^{12}$, $n=2^{13}$ y $n=2^{14}$.

![TPL over N - noNC - n10](n10_noNC__TPL_over_N__hue_C__col_U__loglog_scale.svg)
![TPL over N - noNC - n12](n12_noNC__TPL_over_N__hue_C__col_U__loglog_scale.svg)
![TPL over N - noNC - n13](n13_noNC__TPL_over_N__hue_C__col_U__loglog_scale.svg)
![TPL over N - noNC - n14](n14_noNC__TPL_over_N__hue_C__col_U__loglog_scale.svg)

En todos los casos para $N \ll n$ el TPL crece linealmente con $N$.
Inicialmente hay $n$ sets con un total de TLP 0 (cada set tiene un unico elemento).
Con cada llamada a `Union(i,j)`, dos sets se unen para formar uno con TPL de 1.
Esto se debe a que es mas probable hacer una unión entre 2 sets de 1 elemento
cada uno que entre sets mas grandes.

Para cuando $N \gg n$ el TPL se vuelve constante igual a 1.
Para $N \gg n$ habra un solo set de n elementos y cada llamada a `Union(i,j)` no hara
sino otra cosa comprimir la estructura (debido a las llamadas a find).
Eventualmente el unico set tendra una altura de 1 y por ende un TPL promedio de 1.

El analisis es mas interesante para $N \sim 2^{n}$ donde hay una transicion entre
el comportamiento  lineal y el constante del TPL. Las siguientes figuras son un acercamiento
a dicha transicion.

![TPL over N - noNC - n10 - zoomed](n10_noNC__TPL_over_N__hue_C__col_U__loglog_scale__zoomed.svg)
![TPL over N - noNC - n12 - zoomed](n12_noNC__TPL_over_N__hue_C__col_U__loglog_scale__zoomed.svg)
![TPL over N - noNC - n13 - zoomed](n13_noNC__TPL_over_N__hue_C__col_U__loglog_scale__zoomed.svg)
![TPL over N - noNC - n14 - zoomed](n14_noNC__TPL_over_N__hue_C__col_U__loglog_scale__zoomed.svg)

De los graficos puede verse que para la estrategia *quick union* (QU), las estrategias
de compresion se diferencian entre si con *full compression* (FC) siendo
la mejor y *reversal k=2* (R2) siendo la peor.

Para el resto de las estrategias de union, las estrategias de compresion
no muestran diferencias significativas, al menos para los valores de $n$
usados.

### Futura investigacion

Para $N \sim 2^{n}$ seria interesante usar mas puntos de muestreo. De los
graficos puede tener una idea de la forma de las curvas de TPL pero
lo correcto seria tener mas muestras y no especular. Tambien seria interesante
ver para que valor de $N$ se alcanza un maximo.

## Analisis y comparacion entre NC y FC

La estrategia de compresion nula (NC) requiere una analisis especial
puesto que como no comprime ningun camino es de esperarse que el TPL
promedio sea monotonamente creciente.

Notar como los valores de TPL para las estrategias anteriores alcanzan
un maximo entre $2^0$ y $2^3$ (1 y 8). En cambio para NC el maximo puede
llegar a $2^{10}$ haciendo que las pequeñas diferencias entre las otras
estrategias y NC se vean mas pequeñas en comparacion.

Aun asi, junto con NC se dibujo el TLP para FC como punto de referencia.
 Cada
fila corresponde a un numero inicial de sets disjuntos ($n$): en orden
$n=2^{10}$, $n=2^{12}$ y $n=2^{13}$.

![TPL over N - onlyNC - n10](n10_onlyNC__TPL_over_N__hue_FC__col_U__loglog_scale.svg)
![TPL over N - onlyNC - n12](n12_onlyNC__TPL_over_N__hue_FC__col_U__loglog_scale.svg)
![TPL over N - onlyNC - n13](n13_onlyNC__TPL_over_N__hue_FC__col_U__loglog_scale.svg)

Para $N \ll n$ el TPL crece linealmente con $N$, tal como sucede con el
resto de las estrategias. Es para $N \gg n$ donde hay un cambio.

Con la estrategia QU, `Union(i,j)` une el set `i` con `j` sin tomar en
consideracion si el set resultante seria mas alto de lo necesario --
y por ende, mas lento. Teoricamente el peor caso es $O(n)$ donde
`UnionFind` consiste es un solo set cuya estructura es una lista.

Con pares $(i, j)$ elegidos al azar este peor caso es improbable. Aun
asi el TPL es mas de $2^{5}$ veces mas grande que para FC y el resto
de las estrategias de compresion.

La situacion cambia cuando la union es *by weight* (UW) o *by rank*
(UR). `Union(i,j)` evita crear estructuras altas lo que termina
en TLP promedios mucho mas bajos aun sin comprimir.

### Futura investigacion

Para las estrategias UW y UR tiene sentido comparar NC con todas las
demas estrategias de compresion y para $n$ incluso mas altos y ver
cuan grande se vuelve la diferencia entre los TPL.


## *Total pointer update* en funcion de $N$

*Total pointer update* (TPU) es la cantidad de updates a los punteros
que se harian si se hicieran una compresion de caminos. Es un estimador
del costo de escritura en memoria de cada `Find(i)`. Las siguientes
figuras muestran el TPU promedio en funcion de $N$; cada
fila corresponde a un numero inicial de sets disjuntos ($n$): en orden
$n=2^{10}$, $n=2^{12}$, $n=2^{13}$ y $n=2^{14}$.

![TPU over N - noNC - n10](n10_noNC__TPU_over_N__hue_C__col_U__loglog_scale.svg)
![TPU over N - noNC - n12](n12_noNC__TPU_over_N__hue_C__col_U__loglog_scale.svg)
![TPU over N - noNC - n13](n13_noNC__TPU_over_N__hue_C__col_U__loglog_scale.svg)
![TPU over N - noNC - n14](n14_noNC__TPU_over_N__hue_C__col_U__loglog_scale.svg)

Como puede verse el TPU de la compresion *path halving* (PH) es siempre
la mitad que sus alternativas. Esta estrategia actualiza el puntero
*parent* de solo la mitad de los nodos en el camino.

Al igual de lo que pasa con TPL, el TPU tiene un comportamiento lineal
para  $N \ll n$ con el detalle que las mediciones muestran una mayor
dispersion de valores (en la grafica se ven las *error bars* mas
alargadas).

Para $N \sim n \log n$, el TPU promedio se desploma. Esto puede
entenderse mejor viendo al TPU como una funcion que depende del TPL:

$$
\textrm{TPU}_{\textrm{FC}}=\textrm{TPL}_{\textrm{FC}}-\sum_{\forall r\,\textrm{root}}r\textrm{'s\,children\,count}
$$

Para el resto de las estrategias de compresion estan las siguientes
equivalencias:
$\textrm{TPU}_{\textrm{FC}}=\textrm{TPU}_{\textrm{PS}}=\textrm{TPU}_{\textrm{R2}}$,
$\textrm{TPU}_{\textrm{FC}}=2\,\textrm{TPU}_{\textrm{PH}}$ y
$\textrm{TPU}_{\textrm{NC}}=0$.

Como se vio anteriormente el TPL promedio llega a 1 para $N \sim n \log n$
lo que implica que los sets tienen altura 1 y por lo tanto muchos mas
nodos como hijos directo del nodo raiz lo que cancela el TPL y da un TPU
promedio cercano a cero.

Las siguientes figuras muestran un acercamiento para $N < n \log n$:

![TPU over N - noNC - n10 - zoomed](n10_noNC__TPU_over_N__hue_C__col_U__loglog_scale__zoomed.svg)
![TPU over N - noNC - n12 - zoomed](n12_noNC__TPU_over_N__hue_C__col_U__loglog_scale__zoomed.svg)
![TPU over N - noNC - n13 - zoomed](n13_noNC__TPU_over_N__hue_C__col_U__loglog_scale__zoomed.svg)
![TPU over N - noNC - n14 - zoomed](n14_noNC__TPU_over_N__hue_C__col_U__loglog_scale__zoomed.svg)

## Cantidad de *roots' children* y sets en funcion de $N$

Las siguientes figuras muestran la cantidad de nodos que son hijos
directo de un nodo raiz, *roots' children*, en lineas contiguas
y la cantidad de nodos raiz o sets disjuntos que hay en funcion de $N$,
en una linea de guiones curva. Cada
fila corresponde a un numero inicial de sets disjuntos ($n$): en orden
$n=2^{10}$, $n=2^{12}$, $n=2^{13}$ y $n=2^{14}$.

![Cnt over N - noNC - n10](n10_noNC__Set_and_RChild_over_N__hue_C__style__Set_or_RChild__col_U__log_xscale.svg)
![Cnt over N - noNC - n12](n12_noNC__Set_and_RChild_over_N__hue_C__style__Set_or_RChild__col_U__log_xscale.svg)
![Cnt over N - noNC - n13](n13_noNC__Set_and_RChild_over_N__hue_C__style__Set_or_RChild__col_U__log_xscale.svg)
![Cnt over N - noNC - n14](n14_noNC__Set_and_RChild_over_N__hue_C__style__Set_or_RChild__col_U__log_xscale.svg)

La linea de guiones curva es facil de interpretar: comienza en $n$, pues
al principio hay inicialmente $n$ sets disjuntos, y decae a 1 para
$N \sim n \log n$ pues para ese momento todos los nodos pertenecen a un
mismo y unico set. Es un comportamiento independiente de las estrategias
usadas.

La curvas con lineas solidas son la cantidad de hijos directos: comienza
en 0, como es de esperarse, y crece rapidamente ya que en cada llamada a
`Union(i,j)` es muy probable que se traten de sets disjuntos $i$ y $j$
y la union estableza un nuevo hijo directo a la vez que se reduce
en uno la cantidad de set disjuntos. Por eso la curva es un reflejo de
la curva de guiones, tanto para $N$ chicos como para $N \gg n$.

Para QU se muestra un ascenso lento, un quiebre, un descenso y luego una
subida repentina. Este comportamiento se entiende mejor comparando una
estrategia de compresion como FC con la compresion nula NC como lo
muestran las siguientes figuras (para $n=2^{10}$, $n=2^{12}$ y
$n=2^{13}$):

![Cnt over N - onlyNC - n10](n10_onlyNC__Set_and_RChild_over_N__hue_C__style__Set_or_RChild__col_U__log_xscale.svg)
![Cnt over N - onlyNC - n12](n12_onlyNC__Set_and_RChild_over_N__hue_C__style__Set_or_RChild__col_U__log_xscale.svg)
![Cnt over N - onlyNC - n13](n13_onlyNC__Set_and_RChild_over_N__hue_C__style__Set_or_RChild__col_U__log_xscale.svg)

Para UW y UR, la cantidad de hijos directos se estanca muy por debajo
de $n$. Para $N$ grandes, `Union(i,j)` une 2 sets que ya forman parte
de un mismo set y debido a que no hay compresion, la estructura no
cambia y la cantidad de hijos directos queda fija. En cambio, para otras
estrategias de compresion como FC, la llamada a `Union(i,j)` tiene como
efecto secundario 2 llamadas a `Find(.)` que permiten la compresion de
los caminos y el agregado de multiples nodos como nuevos hijo directos.

Para QU la cantidad de hijos directos se desploma a
valores muy chicos. Esto se explica si se tienen sets con pocos hijos
directos que se unen entre si: el set resultante tendra pocos hijos
directos pero con una mayor altura.

Es interesante ver para QU las curvas de NC y FC en simultaneo: para
$N$ chicos ambas coinciden indicando que quien domina la funcion es
la estrategia de union QU. Pero luego hay un quiebre y las curvas se
separan: mientras la curva de NC se va a 0 la de FC sube rapidamente
lo que indica que es la estrategia de compresion la que tracciona y
domina la cantidad de hijos directos.

### Futura investigacion

Con solo 20 experimentos (20 *seeds*) no queda tan claro que tan tipico
es el comportamiento visto en QU. Ademas, se requieren mas puntos de
medicion para ver con exactitud la curva durante el quiebre, descenso
y posterior ascenso (salvo para NC).

## Tiempo de ejecucion empirico en funcion de $N$

El enunciado propone modelar el costo de una estrategia como $2\,\textrm{TPL} + \epsilon\,\textrm{TPU}$
pero se prefirio tener una medicion empirica para $n=2^{10}$ y para
todas las estrategias salvo NC. La razon es que TLP y TPU
son abstracciones que no permiten ver la interaccion con el hardware,
en especial con la memoria y la cache.

La primer figura es el grafico completo, la segunda es un acercamiento
para $N \gg n$.

![Exec over N - noNC - n10](n10_noNC__exectime_over_N__hue_C__col_U__log_xscale.svg)
![Exec over N - noNC - n10 - zoomed](n10_noNC__exectime_over_N__hue_C__col_U__log_xscale__zoomed.svg)

En QU puede verse como FC tiene un costo de performance superior al del
resto de las estrategias debido a que FC recorre 2 veces el camino al
nodo raiz. Para UW y UR este costo es despreciable por que los caminos
en si son cortos ya que tanto UW como UR fomentan la construccion de
arboles de baja altura.


### Futura investigacion

Salvo para QU, no es posible ver que estrategia tiene una mejora
visible con respecto a la otra. Hay varios puntos a mejorar:

- analizar $n$ mas grandes: ver si hay o no una tendencia
- incrementar el numero de rondas: para mayor precision
- reducir el impacto / *overhead* de la instrumentacion

El ultimo punto es importante: se midio el tiempo de ejecucion
de cada llamada a `Union(i,j)` lo que tambien tiene un mayor overhead.
Se deberia poder medir $m$ llamadas seguidas a `Union(i,j)` con un
overhead por la medicion amortizado de $1/m$.


## Metodos

Definiremos como configuracion del experimento los siguientes parametros
que han de quedar fijos para toda la ejecucion del mismo:

 - $n$ es la cantidad inicial de sets en la estructura `UnionFind`
 - $seed$ es la semilla que inicializa el generador de numeros aleatorios y por
   ende define deterministicamente la lista de pares (ver mas adelante).
 - $U$ y $C$ las estrategias de unión y compresion de camino respectivamente.

Para una configuracion dada se construye una estructura `UnionFind` de $n$ elementos
y se ejecuta `Union(i,j)` para todo par no ordenado $(i, j)$ con $i=/j$ elegidos al azar
y sin repetir de los $\binom{n}{2}$ posibles pares. Con un $seed$ fijo,
los pares son generados deterministicamente con el algoritmo `shuffle` y el generador `MT19937`.

En cada llamada a `Union(i,j)` se midio el tiempo de ejecucion tomando recaudo que
el compilador no reordenara las instracciones y pueda distorcionar la medicion.
Para tener menos ruido en la medicion debido a factores externos como otros procesos
compitiendo por la CPU, el OS manejando una interrupcion, etc,
se configuro un ambiente lo mas tranquilo posible:

 - se deshabilito hyperthreading
 - se desactivo el CPU throttling
 - se desactivo la interfaz grafica y la red (wifi)
 - se removio del scheduler el CPU numero 3 para su uso exclusivo en los tests.

Aun asi no todo el ruido puede ser removido. Para minimizarlo se ejecuto el mismo
test $R$ veces. Dado que en cada *ronda* la configuracion del experimento es la misma
(mismo $n$, misma lista de pares, mismas estrategias), la performance de `Union(i,j)`
se espera deterministica salvo por un ruido aditivo positivo.

Cada $\Delta_{i}$ pares procesados, se tomo el de ejecucion acumulado hasta
el momento y se lo comparo con el tiempo de la ronda anterior para el mismo
$\Delta_{i}$ quedandonse con el minimo de ambos. Asi tras $R$ rondas se obtuvo
el minimo de ejecucion para cada momento $\Delta_{i}$, reduciendo asi el ruido.

Ademas, cada $\Delta_{i}$ pares procesados se midio el *total path length* (TPL),
*total pointer update* (TPU), la cantidad de sets disjuntos y la cantidad de nodos
que son hijos directo de un nodo raiz. Para una misma configuracion del experimento,
todos estos valores son deterministicos independiente de en que ronda se midan.
Por simplicidad se midieron en la ultima.

En el enunciado se propone usar una frecuencia de medicion equiespaciada,
lease multiplos de $\Delta_{0}$ ($\Delta_{0}$, $2\Delta_{0}$, $3\Delta_{0}$, $4\Delta_{0}$).
Sin embargo se opto por una frecuencia que se duplica en cada iteracion con un $\Delta_{0}$ inicial de 16
($\Delta_{0}=16$, $2\Delta_{0}=32$, $4\Delta_{0}=64$, $8\Delta_{0}=128$).

El experimento se repitio para $S$ *seeds distintos* lo que se obtiene $S$
resultados para cada medicion $\Delta_{i}$. Es a partir de estos $S$ valores que
se calculan los valores medios y variancia de TPL, TPU, tiempo de ejecucion y demas.

A su vez, todo lo mencionado se ejecuto para los distintas combinaciones
de $n$, $U$ y $C$. Sin embargo, debido a que la cantidad de pares $(i,j)$
a procesar por experimento crece como $\binom{n}{2}$ algunas configuraciones no fueron ejecutadas.

 - Todos los valores salvo el tiempo de ejecucion fueron medidos con 1
   sola ronda:
    - para $n=2^{10}$, $n=2^{12}$ y $n=2^{13}$, todas las combinaciones de $U$ y $C$.
    - para $n=2^{14}$ , todas las combinaciones de $U$ y $C$ salvo NC.

 - Los tiempos de ejecucion fueron medidos con 35 rondas solo para
   $n=2^{10}$ y todas las combinaciones de $U$ y $C$ salvo NC.


La decision de descartar NC con $n = 14$ se debio a que en dichos
escenarios los sets no son comprimidos y los tiempos eran mas largos que los
que teniamos asignado. A su vez, la performance teorica de dicha estrategia
es considerablemente peor que la del resto y creemos que en el resto de los escenarios,
con $n$ chicos, esto ya sera visible. Lo mismo sucedio con la medicion del tiempo
de ejecucion donde no hay ningun escenario con NC.
Creemos que con el analisis de TPL sera suficiente para llegar a conclusiones razonables.

## Documentacion de referencia

Shuffle: https://en.cppreference.com/w/cpp/algorithm/random_shuffle
MT19937: https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
`DoNotOptimize` (post del autor): https://stackoverflow.com/questions/37786547/enforcing-statement-order-in-c
`DoNotOptimize` (repositorio de google): https://github.com/google/benchmark/blob/02a354f3f323ae8256948e1dc77ddcb1dfc297da/include/benchmark/benchmark.h#L468

## Apendice - Instrumentacion (assembly)

## Apendice - Instrumentacion (assembly)

(Una inspeccion del assembly muestra exactamente esto pero no hay 100% de garantias que el microprocesador no decida hacer un reordenamiento en el pipeline)



```
3590:       4c 8b 34 ca             mov    (%rdx,%rcx,8),%r14
                auto begin = std::chrono::steady_clock::now();
3594:       e8 97 eb ff ff          callq  2130 <steady_clock now>
3599:       49 89 c7                mov    %rax,%r15
    asm volatile("" : "+m,r"(value) : : "memory");
359c:       4c 89 f2                mov    %r14,%rdx
359f:       48 8b 7d a0             mov    -0x60(%rbp),%rdi
35a3:       89 d0                   mov    %edx,%eax
35a5:       48 c1 ea 20             shr    $0x20,%rdx
    unsigned Union(unsigned i, unsigned j) {
35a9:       0f 1f 80 00 00 00 00    nopl   0x0(%rax)
        return sets[i].parent == i;
35b0:       89 c6                   mov    %eax,%esi
35b2:       48 8d 34 76             lea    (%rsi,%rsi,2),%rsi
35b6:       4c 8d 04 b7             lea    (%rdi,%rsi,4),%r8
35ba:       89 c6                   mov    %eax,%esi
35bc:       41 8b 00                mov    (%r8),%eax
        while (not is_root(i)) {
35bf:       39 c6                   cmp    %eax,%esi
35c1:       75 ed                   jne    35b0 <vector pair>
35c3:       0f 1f 44 00 00          nopl   0x0(%rax,%rax,1)
    bool is_root(unsigned i) const {
35c8:       89 d2                   mov    %edx,%edx
35ca:       48 89 d0                mov    %rdx,%rax
        return sets[i].parent == i;
35cd:       48 8d 14 52             lea    (%rdx,%rdx,2),%rdx
35d1:       8b 14 97                mov    (%rdi,%rdx,4),%edx
        while (not is_root(i)) {
35d4:       39 d0                   cmp    %edx,%eax
35d6:       75 f0                   jne    35c8 <vector pair>
        if (i != j) {
35d8:       39 d6                   cmp    %edx,%esi
35da:       74 03                   je     35df <vector pair>
            sets[i].parent = j;
35dc:       41 89 10                mov    %edx,(%r8)
                elapsed += std::chrono::steady_clock::now() - begin;
35df:       e8 4c eb ff ff          callq  2130 <steady_clock now>
          static_cast<_CR>(__d.count()) / static_cast<_CR>(_CF::den)));
```

Notar como `Union(i,j)` esta *inline* en el codigo final.