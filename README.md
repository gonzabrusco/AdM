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

### 6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo privilegiado a no priviligiado y nuevamente a privilegiado.

### 7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo

### 8. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un ejemplo

### 9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).

### 10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado a funciones y su retorno?

### 11. Describa la secuencia de reset del microprocesador.

### 12. ¿Qué entiende por “core peripherals”? ¿Qué diferencia existe entre estos y el resto de los periféricos?

### 13. ¿Cómo se implementan las prioridades de las interrupciones? Dé un ejemplo

### 14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?

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

## ISA
### 1. ¿Qué son los sufijos y para qué se los utiliza? Dé un ejemplo

### 2. ¿Para qué se utiliza el sufijo ‘s’? Dé un ejemplo

### 3. ¿Qué utilidad tiene la implementación de instrucciones de aritmética saturada? Dé un ejemplo con operaciones con datos de 8 bits.

### 4. Describa brevemente la interfaz entre assembler y C ¿Cómo se reciben los argumentos de las funciones? ¿Cómo se devuelve el resultado? ¿Qué registros deben guardarse en la pila antes de ser modificados?

### 5. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un ejemplo.



