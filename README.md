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



