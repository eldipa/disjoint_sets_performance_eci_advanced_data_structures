La estructura del repositorio es la siguiente:

- `data_collected`: tiene las mediciones recolectadas durante la
    experimentación y las que son reflejadas por las figuras del
    presente trabajo.
- `plots`: las figuras y los scripts necesarios para recrearlas
- `submission`: este informe
- `disjoint_set.h`: la implementación de `UnionFind` incluyendo todas
    las estrategias de unión y compresión.
- `demo.cpp`: una mini aplicación para probar la estructura y renderizar
    el árbol resultando con `dot`
- `perf.cpp`: la implementación del setup de prueba y ejecución de
    experimentos.
- `Makefile`: instrucciones de compilación


Para compilar ejecutar `make`.

El programa `demo` imprime por `stdout` un diagrama en formato `dot` de
una estructura de `n` elementos a la que se le hicieron ciertas
operaciones de `Union(i,j)` específicas. Puede ejecutarse como:

```
./demo N | dot -Tpng > result.png
```

El program `perf` es quien realiza los experimentos y mediciones. Todas
las ejecuciones del programa guardan los resultados al final del archivo
`results.csv` ( *append* ).

Para modificar los parámetros de ejecución se debe modificar el código
fuente:

 - `ROUNDS` es la cantidad de veces que se ejecuta un
     experimento con el final de tener un tiempo de ejecución con un
     mínimo de ruido. Valores razonables son 30. Si no interesa el
     tiempo de ejecución, un valor de 1 es mucho más rápido e igualmente
     correcto para el resto de las métricas.
 - `ELEM_N_S` es un array con los valores `n` a usar. La actual
     implementación de los tests (no de la estructura a testear)
     requiere de mucha memoria para valores de `n` grandes.
 - `SEEDS` es un array con los *seeds* que inicializan el generador de
     números aleatorios. Cuantos más *seeds* haya más experimentos se
     realizaran. El código actualmente tiene 20 *seeds* sacados de los
     dígitos del número PI.

Al final de la función `main` están las llamadas a la ejecución
de los experimentos, una llamada por cada estrategia de compresión.
Estas líneas pueden ser comentadas si se desea probar unas estrategias
y no otras.

Si se quiere medir tiempos de ejecución deben configurarse un entorno
lo más silencioso posible (ver
[Quiescent-Environment](https://book-of-gehn.github.io/articles/2021/03/07/Quiescent-Environment.html) )
Suponiendo que la CPU 3 esta para uso exclusivo para la experimentación,
el programa `perf` puede ejecutarse como:

```
taskset --cpu-list 3 ./perf
```
