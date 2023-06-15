# Arquitectura de Microprocesadores
Repositorio de la asignatura Arquitectura de Microprocesadores de la especializacion en sistemas en embebidos de la UBA

## Preguntas Orientadoras:
### 1. Describa brevemente los diferentes perfiles de familias de microprocesadores/microcontroladores de ARM. Explique alguna de sus diferencias características.

ARM tiene tres familias de microprocesadores: Cortex A, Cortex R y Cortex M. Las principales diferencias son:
- **Cortex A**: Esta orientado a correr sistemas operativos de propósito general donde es más importante poder trabajar con muchas tareas en simultáneo que tener un tiempo de respuesta determinado en las mismas. Poseen memoria CACHE para mejorar su performance y poder tener memorias más grandes. Un ejemplo de esto es un celular donde se prioriza poder correr muchas aplicaciones al mismo tiempo y no tanto el tiempo de respuesta de cada aplicacion en concreto. 
- **Cortex R**: Esta orientado a ejecutar RTOS. Es similar a los Cortex A pero sin ninguna funcionalidad que pueda afectar el determinismo del mismo (por ejemplo memoria CACHE). A diferencia de los OS de proposito general, los RTOS estan enfocados en garantizar un tiempo máximo de respuesta y no tanto a ejecutar muchas tareas en simultaneo de manera óptima. Un Cortex R se podría usar para algúna aplicacion donde el tiempo de respuesta es crítico, como por ejemplo un controlador de estabilidad de un auto.
- **Cortex M**: Los Cortex M son similares en concepto a los Cortex R pero mucho más pequeños y por ende, mucho mas baratos. Se usan en sistemas que necesitan ejecutar tareas de tiempo real pero que no requieren de un nivel de procesamiento que justifique usar un Cortex R o en aplicaciones de proposito general que no necesitan tiempo real. Son los microcontroladores más económicos de ARM y hoy en día reemplazaron en gran medida a todos los microcontroladores históricos de 8 bits.

## Cortex M:
### 1. Describa brevemente las diferencias entre las familias de procesadores Cortex M0, M3 y M4.

Existen tres familias de Cortex M. Los Cortex M0 estan diseñados para se de bajo consumo y muy bajo costo. En cambio los M3 y M4 estan diseñados para tener una mejor performance e incluyen funciones extra para el procesamiento de señales, etc. A su vez, los M4 son superiores a los M3 porque incluyen funciones de DSP y FPU (aunque esto último es opcional).

### 2. ¿Por qué se dice que el set de instrucciones Thumb permite mayor densidad de código? Explique
El set de instrucciones Thumb permite una mayor densidad de código porque es un set de instrucciones de 16 bits (a diferencia del ARM que es de 32 bits). Eso significa que para la misma memoria flash usando el set de instrucciones Thumb, se pueden almacenar muchas más instrucciones (programas más grandes), por ende, se obtiene una mayor densidad de código.

### 3. ¿Qué entiende por arquitectura load-store? ¿Qué tipo de instrucciones no posee este tipo de arquitectura?
Una arquitectura Load-Store se destaca porque las unicas instrucciones que acceden a la memoria son las de carga y almacenamiento (load y store). Estas instrucciones trabajan desde los registros hacia la memoria y viceversa. En cambio, el resto de las intrucciones de la ISA trabajan únicamente con los registros (no tienen acceso a la memoria). Entonces podemos decir, que este tipo de arquitecturas no tienen instrucciones que traigan datos de memoria y hagan operaciones con esos valores (o hagan operaciones y guarden en memoria). 

### 4. ¿Cómo es el mapa de memoria de la familia?
El mapa de memoria es un continuo de direcciones donde todos los perifericos del microcontrolador estan mapeados. Como máximo puede direccionar 4G direcciones. 

### 5. ¿Qué ventajas presenta el uso de los “shadowed pointers” del PSP y el MSP?
El MSP es el Main Stack Pointer y es usado durante la ejecución del programa en modo privilegiado. En cambio el PSP es el Processor Stack Pointer y es usado por las tareas cuando se ejecutan en modo no privilegiado. Este es el tipico uso de los Stack Pointers cuando se ejecuta un sistema operativo. Si el programa es simple y no tiene S.O., entonces en general unicamente se usa el MSP. Con respecto al uso de “shadowed pointers” se refiere a que el PSP o el MSP se leen directamente desde el registro SP. Dependiendo de como este seteado un bit determinado de control (SPSEL), cuando el procesador lee el registro SP, esta leyendo el MSP o el PSP. 

### 6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo privilegiado a no priviligiado y nuevamente a privilegiado.
El modo privilegiado de ejecución tiene acceso a todos los recursos del sistema mientras que el modo no privilegiado tiene el acceso restringido. Para conmutar de modo privilegiado a modo no privilegiado se realiza mediante software (seteando el bit de control correspondiente). En cambio para pasar de modo no privilegiado a modo privilegiado si o si se necesita que ocurra una excepción para que el procesador entre en modo Handler y desde dicho modo es posible setear el bit de control para que la ejecución del programa se eleve a modo privilegiado.
Un ejemplo de esto es cuando un sistema operativo ejecuta un task. Mientras el scheduler esta activo, se ejecuta en modo privilegiado pero antes de pasarle el control a un task, cambia a modo no privilegiado para restringir el acceso al sistema por parte del task. Cuando el Task finaliza su ejecución, se puede generar una excepción para que entre el modo Handler y luego se llame al Scheduler nuevamente pero de forma privilegiada. Esto es útil cuando se usa en conjunto con el MPU para restringir el acceso a memoria por parte de los Tasks. 

### 7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo
Significa que cualquier instruccion de assembly puede operar con cualquier registro. En arquitecturas antiguas muchas veces una operacion siempre ponia el resultado en un registro determinado. En la arquitectura de ARM cualquier operacion puede trabajar con cualquier registro de origen y destino.

### 8. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un ejemplo
Esto permite generar bloques del tipo IF THEN, IF THEN THEN, IF THEN ELSE, etc. Es decir, estos bloques nos permiten ejecutar condicionalmente hasta 4 instrucciones de assembly. Si por ejemplo luego de una comparacion quisieramos modificar dos registros, podriamos ejecutar una instruccion de cmp y luego en un bloque ITT, podríamos ejecutar instrucciones que sean condicionales sobre esos dos registros (movne, addeq, subgt, etc).

### 9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).
La excepción de reset es la que se produce cuando hay un reset del procesador. Es la mas prioritaria de todas. 
La excepción de NMI es una excepción producida por una interrupcion no enmascarable. 
La excepción de Hardfault agrupa otras excepciones como MemManage, BusFault y UsageFault cuando estas no tienen un handler definido especifico para cada una de ellas. En general se produce por un problema del cual es dificil recuperarse.

### 10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado a funciones y su retorno?
La funcion del stack es guardar el contexto de una funcion. Es decir, si el procesador esta ejecutando una funcion y luego tiene un llamado a una funcion anidada, todas las variables en uso (que normalmente estaran en los registros del procesador), se guardan en la pila mediante PUSH para liberar los registros y recien ahi se puede llamar a la otra funcion que a su vez tendrá sus propias variables (que se guardaran en los registros del procesador para su uso). Cuando se retorna de esta subfuncion, se traen las variables en uso por la funcion a la que se retorna haciendo POP de la pila. 

### 11. Describa la secuencia de reset del microprocesador.
Inmediatamente luego de un RESET, el procesador accede a la primera posición de memoria (0x00000000) en donde encuentra el valor inicial del MSP, es decir la dirección en donde comienza el Stack (siempre recordando que el stack crece "hacia atrás"). Inmediatamente, lee la primera posición del vector de interrupciones que es donde se encuentra la dirección de la rutina de reset y salta a esa posición. Una vez ejecutando la rutina de reset, el programador ya es libre de hacer lo que desea, en general, ejecutar la funcion "main" del programa. 

### 12. ¿Qué entiende por “core peripherals”? ¿Qué diferencia existe entre estos y el resto de los periféricos?
Los "CORE PERIPHERALS" son los perifericos que vienen con la I.P. de ARM y en general son los siguientes: 
- Nested Vectored Interrupt Controller
- System Control Block
- System timer
- Memory Protection Unit
En cambio el resto de los perifericos son los que implementa el fabricante y ya no estan incluidos como parte de la CMSIS de ARM sino que es necesario usar las capas de abstracción provistas por el fabricante. 

### 13. ¿Cómo se implementan las prioridades de las interrupciones? Dé un ejemplo

### 14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?
El CMSIS es un paquete de software en C provisto por ARM que nos permiten hacer uso de todas las funcionalidad estandar que tiene un Cortex. La ventaja de programar usando el CMSIS es que es muy facil portar el software de un procesador a otro, siempre y cuando ambos sean Cortex. 

### 15. Cuando ocurre una interrupción, asumiendo que está habilitada ¿Cómo opera el microprocesador para atender a la subrutina correspondiente? Explique con un ejemplo

### 16. ¿Cómo cambia la operación de stacking al utilizar la unidad de punto flotante?

### 17. Explique las características avanzadas de atención a interrupciones: tail chaining y late arrival.

### 18. ¿Qué es el systick? ¿Por qué puede afirmarse que su implementación favorece la portabilidad de los sistemas operativos embebidos?
El Systick es el periferico que lleva la base de tiempo para los sistemas operativos. La ventaja de que este implementado como parte de la IP de ARM hace que los sistemas operativos que hacen uso del mismo sean facilmente portables entre procesadores Cortex (ej. FreeRTOS).

### 19. ¿Qué funciones cumple la unidad de protección de memoria (MPU)?
La funcion del MPU es definir permisos y atributos de memoria. Por ejemplo, sirve para proteger la memoria de un Task de un RTOS del acceso desde otro Task que lo pueda corromper. Tambien sirve para proteger regiones de memoria del acceso desde tasks sin privilegio. Permite marcar sectores de memoria como non-executable para protegernos de ataques de inyección de codigo en la SRAM.

### 20. ¿Cuántas regiones pueden configurarse como máximo? ¿Qué ocurre en caso de haber solapamientos de las regiones? ¿Qué ocurre con las zonas de memoria no cubiertas por las regiones definidas?
Permite configurar hasta 8 regiones como máximo. Si hay solapamiento de regiones, en la zona solapada los permisos que rigen son los de la región con mayor numero. Por ejemplo si la región 1 y región 4 se solapan, entonces en el espacio de memoria que se produce el solapamiento, los atributos y permisos de la región 4 aplican a esa zona. Si se intenta acceder a una zona de memoria no definida en el MPU, esa transferencia es bloqueada y se produce una exceptión.

### 21. ¿Para qué se suele utilizar la excepción PendSV? ¿Cómo se relaciona su uso con el resto de las excepciones? Dé un ejemplo

### 22. ¿Para qué se suele utilizar la excepción SVC? Expliquelo dentro de un marco de un sistema operativo embebido.
En general se utiliza para que el programa pueda retornar a modo privilegiado. En el marco de un sistema operativo, el SVC se puede utilizar para forzar un llamado del Scheduler. 

## ISA
### 1. ¿Qué son los sufijos y para qué se los utiliza? Dé un ejemplo
Lo sufijos sirven para modificar el comportamiento de la instruccion. Por ejemplo, pueden hacer que una instruccion se ejecute o no de forma condicional (sufijos ne, eq, etc). O tambien pueden especificar si deben modificar los flags Z, N, V, C; o no.

### 2. ¿Para qué se utiliza el sufijo ‘s’? Dé un ejemplo
El sufijo S sirve para especificar si una instruccion debe modificar los flags N,Z,V,C o no. Por ejemplo, una instruccion del tipo "sub" no modificaría ninguno de estos registros, pero si le agregamos el sufijo "s" transformandola en "subs" cuando se ejecute la instrucción modificará los flags en base al resultado.

### 3. ¿Qué utilidad tiene la implementación de instrucciones de aritmética saturada? Dé un ejemplo con operaciones con datos de 8 bits.
Las instrucciones de aritmética saturada son de mucha utilidad en procesamiento de señales por ejemplo. Si nosotros estuvieramos procesando una señal que excede el rango de representación de la arquitectura con la que estamos trabajando (8 bits o 32 bits), al ocurrir on overflow, aparecería una discontinuidad en la señal porque los valores pasarían de estar en el rango alto de la representación, al rango bajo. 
Por ejemplo, si la arquitectura fuera de 8 bits, un numero signado iría de -128 to 127. Si estuvieramos aplicando a amplificación x2 a una senoidal que tenía un valor pico pico de 140, nos quedaría una señal pico pico de 280. Pero ya sabemos que el rango de representacion de 8 bits es de 255 en total. Por ende, cuando se produzca el overflow para valores superiores a 127, esos valores se trasladaran a -128, -127, -125, etc. Es decir, lo que eran los valores mas altos de la senoidal ahora aparecerían como valores muy negativos. Lo mismo pasaría en la otra mitad del periodo pero al reves (los numeros que sean mas negativos que -128, aparecerán como 127, 126, 125, etc).

### 4. Describa brevemente la interfaz entre assembler y C ¿Cómo se reciben los argumentos de las funciones? ¿Cómo se devuelve el resultado? ¿Qué registros deben guardarse en la pila antes de ser modificados?
Los argumentos se reciben en los registros r0, r1, r2 y r3, dependiendo de la cantidad de parametros pasados. Si tuviera más argumentos, los mismos se pasarían por stack. En cambio el valor de retorno siempre se devuelve por r0.
Todos los registros del r4 inclusive en adelante deben ser guardados en el stack antes de ser utilizados. Toda esta informacion de pasaje de parametros se puede encontrar en el ARM calling convention.

### 5. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un ejemplo.
Una instrucción SIMD es aquella que puede procesar multiples datos en simultaneo dentro de la misma operación. Conceptualmente lo que hacen es compartimentar los registros de 32bits en dos partes de 16bits o dos partes de 8bits y cuando operan, operan sobre estos "compartimentos" permitiendoles trabajar con dos o cuatro numeros en simultaneo.
La ventaja principal es que en el mismo tiempo de ejecucion de una instruccion, se pueden realizar dos o cuatro operaciones más, dependiendo del tipo de dato contenido en el registro (16 o 8 bits).
Un ejemplo de esto es la instruccion SHADD16 que toma las dos mitades de dos registros, hace una suma entre ellas y luego las divide por dos. Es decir, hace un promedio con cada mitad de los registros. A continuación se puede ver con mejor detalle lo que realiza:

res[15:0]  = (val1[15:0]  + val2[15:0]) / 2
res[31:16] = (val1[31:16] + val2[31:16]) / 2




