.syntax unified
.thumb
.text

@ Simbolos exportados
.global asm_svc
.global asm_sum
.global asm_zeros
.global asm_productoEscalar32
.global asm_productoEscalar16
.global asm_productoEscalar12
.global asm_filtroVentana10
.global asm_pack32to16
.global asm_max
.global asm_downsampleM
.global asm_invertir
.global asm_eco
.global asm_simd_eco


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_svc (void)
@
.thumb_func
    asm_svc:
        svc 0
        bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   uint32_t asmSum (uint32_t firstOperand, uint32_t secondOperand)
@
@ Parametros recibidos:
@   r0: firstOperand
@   r1: secondOperand
@
@ Valor de retorno:
@   r0: resultado de la suma de firstOperand y secondOperand
@
.thumb_func
    asm_sum:
        add r0, r1  @ r0 = r0 + r1
        bx lr       @ vuelve adonde fue llamada (especificamente, si "pc"
                    @ es la direccion, en el codigo del programa, de la
                    @ instruccion "bl asm_sum" usada para saltar a
                    @ esta funcion, entonces "lr" contiene ese "pc" + 4).
                    @ el valor de retorno debe quedar en el registro r0.
                    @ En este caso, r0 es el resultado de r0 + r1.

        @ Otras alternativas para el retorno
        @ mov pc,lr
        @ bx lr
        @ push {lr} ... pop {pc}


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_zeros(uint32_t * vector, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: direccion del vector
@   r1: longitud del vector
@
.thumb_func
    asm_zeros:
    	cbz r0, asm_zeros_exit // Chequeo r0 distinto de 0
    	cbz r1, asm_zeros_exit // Chequeo r1 distinto de 0
    	mov r2, #0	// Ponemos un cero en el registro 2
    asm_zeros_loop:
    	subs r1, #1 // Descremento la longitud (y modifica bit ZERO)
		str r2, [r0, r1, LSL #2] // Guardamos el cero del registro 2 en la posicion apuntada por el registro 0 (x 4)
		bne asm_zeros_loop // El flag Z queda seteado de la operacion subs
	asm_zeros_exit:
		bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar32(uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar)
@
@ Parametros recibidos:
@   r0: direccion del vector de entrada
@   r1: direccion del vector de salida
@   r2: longitud del vector
@	r3: numero a multiplicar
@
.thumb_func
    asm_productoEscalar32:
    	push {r4}
    asm_productoEscalar32_loop:
    	subs r2, #1
    	ldr r4, [r0, r2, LSL #2] // Cargo el valor a multiplicar (r4 = *(r0 + (r2 << 2)))
    	mul r4, r4, r3 // r4 = r4 * r3
		str r4, [r1, r2, LSL #2] // Guardamos el valor multiplicado *(r1 + (r2 << 2)) = r4
		bne asm_productoEscalar32_loop // El flag Z queda seteado de la operacion subs
		pop {r4}
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar16(uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar)
@
@ Parametros recibidos:
@   r0: direccion del vector de entrada
@   r1: direccion del vector de salida
@   r2: longitud del vector
@	r3: numero a multiplicar
@
.thumb_func
    asm_productoEscalar16:
		push {r4}
    asm_productoEscalar16_loop:
    	subs r2, #1
    	ldrh r4, [r0, r2, LSL #1] // Cargo el valor a multiplicar (r4 = *(r0 + (r2 << 1)))
    	mul r4, r4, r3 // r4 = r4 * r3
		strh r4, [r1, r2, LSL #1] // Guardamos el valor multiplicado *(r1 + (r2 << 1)) = r4
		bne asm_productoEscalar16_loop // El flag Z queda seteado de la operacion subs
		pop {r4}
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar12(uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar)
@
@	NOTA: Esta funcion usa aritmetica saturada
@
@ Parametros recibidos:
@   r0: direccion del vector de entrada
@   r1: direccion del vector de salida
@   r2: longitud del vector
@	r3: numero a multiplicar
@
.thumb_func
    asm_productoEscalar12:
    	push {r4}
    asm_productoEscalar12_loop:
    	subs r2, #1
    	ldrh r4, [r0, r2, LSL #1] // Cargo el valor a multiplicar (r4 = *(r0 + (r2 << 1)))
    	mul r4, r4, r3 // r4 = r4 * r3
    	usat r4, #12, r4 // Saturo el registro r4 a 12 bits y lo guardo en el registro r4
		strh r4, [r1, r2, LSL #1] // Guardamos el valor multiplicado *(r1 + (r2 << 1)) = r4
		bne asm_productoEscalar12 // El flag Z queda seteado de la operacion subs
		pop {r4}
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn)
@
@ Parametros recibidos:
@   r0: direccion del vector de entrada
@   r1: direccion del vector de salida
@   r2: longitud del vector
@
@ Registros usados temporalmente:
@	r3: iterador del vector
@	r4: posicion de inicio de ventana
@	r5: posicion de finalizacion de ventana
@	r6: promedio
@	r7: registro auxiliar donde se almacena el valor leido de memoria
@	r8: el tamaño del a ventana (11)
@
.thumb_func
	asm_filtroVentana10:
		push {r4-r8}
		mov r3, r2
		mov r8,#11
	asm_filtroVentana10_loop1:
		sub r3, #1 // Decremento el indice
		mov r6, #0 // Reseteo promedio

		// Inicializo ventana inicio
		mov r4, #0 // Inicializo por defecto en cero
		cmp r3, #4 // Comparo el iterador con 4
		it hi
		subhi r4, r3, #5 // Si era mayor que cuatro, entonces puedo restar 5 posiciones. De lo contrario, queda en cero.

		// Inicializo ventana fin
		add r5, r3, #6 // Sumo en 6 al iterador actual y me fijo si se paso del tamaño del vector.
		cmp r5, r2
		it hs // unsigned higher or same
		movhs r5, r2 // Si se paso, limito la ventana al final del vector'

		// En este punto tengo definido el inicio y el fin de la ventana.
		// Itero por las muestras contenidas en ella.
	asm_filtroVentana10_loop2:
		sub r5, #1 // Decremento el fin de la ventana
		ldrh r7, [r0, r5, LSL #1] // Cargo el valor a procesar
		add r6, r6, r7
		cmp r5, r4 // Me fijo si se completo el recorrido de la ventana
		bne asm_filtroVentana10_loop2

		// Termine de sumar todas las muestras. Hago la division por la cantidad de muestras en una ventana y guardo el valor en memoria.
		udiv r6, r6, r8
		strh r6, [r1, r3, LSL #1] // Guardo el promedio calculado

		cmp r3, #0 // Me fijo si ya procesamos todas las muestas del vector.
		bne asm_filtroVentana10_loop1
	asm_filtroVentana10_exit:
		pop {r4-r8}
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_pack32to16(int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: direccion del vector de entrada
@   r1: direccion del vector de salida
@   r2: longitud del vector
@
@ Registros usados temporalmente:
@	r3: almacena el valor a recortar
@
.thumb_func
	asm_pack32to16:
    	subs r2, #1
    	ldr r3, [r0, r2, LSL #2] // Cargo el valor recortar r3 = *(r0 + (r2 << 2))
    	lsr r3, r3, #16 // 16 bit shift a la derecha
		strh r3, [r1, r2, LSL #1] // Guardamos el valor recortado *(r1 + (r2 << 1)) = r3
		bne asm_pack32to16 // El flag Z queda seteado de la operacion subs
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   int32_t asm_max(int32_t * vectorIn, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: direccion del vector de entrada
@   r1: longitud del vector
@
@ Retorna: la posicion del valor maximo encontrado.
@
@ Registros usados temporalmente:
@ 	r2: almacena el valor provisoriamente leido de memoria
@	r3: almacena el valor maximo registrado
@	r4: almacena la posicion del valor maximo registrado
@
.thumb_func
	asm_max:
		push {r4}
		subs r1, #1 // Decremento la longitud
		ldr r3, [r0, r1, LSL #2] // Cargo el valor comparar r3 = *(r0 + (r1 << 2))
		mov r4, r1
		beq asm_max_exit // El flag Z queda seteado de la operacion subs
	asm_max_loop:
		sub r1, #1 // Decremento la longitud
		ldr r2, [r0, r1, LSL #2] // Cargo el valor comparar r2 = *(r0 + (r1 << 2))
		cmp r3,r2
		itt lt
		movlt r3,r2
		movlt r4,r1
		cmp r1,#0
		bne asm_max_loop
	asm_max_exit:
		mov r0, r4
		pop {r4}
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_downsampleM(int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N)
@
@ Parametros recibidos:
@   r0: direccion del vector de entrada
@   r1: direccion del vector de salida
@   r2: longitud del vector
@   r3: factor de diezmado
@
@
@ Registros usados temporalmente:
@ 	r4: iterador vector de entrada
@ 	r5: iterador vector de salida
@	r6: contador de muestras
@	r7: muestra temporal
@
.thumb_func
	asm_downsampleM:
		push {r4-r7}
		mov r4, #0 // Inicializo iterador vector de entrada
		mov r5, #0 // Inicializo iterador vector de salida
		mov r6, #0 // Inicializo contador de muestras
		sub r3, r3, #1 // Factor de diezmado - 1
	asm_downsampleM_loop:
		cmp r6, r3
		itttt eq
		ldreq r7, [r0, r4, LSL #2] // Cargo el valor comparar r7 = *(r0 + (r4 << 2))
		streq r7, [r1, r5, LSL #2] // Guardamos el valor recortado *(r1 + (r5 << 2)) = r7
		addeq r5, #1 // Incremento iterador vector de salida
		moveq r6, #0 // Reseteo contador de muestras
		it ne
		addne r6, r6, #1 // Incremento contador de muestras
		add r4, #1 // Incremento iterador vector de entrada
		cmp r4, r2 // Me fijo si llegamos al final del vector
		bne asm_downsampleM_loop
		pop {r4-r7}
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_invertir(uint16_t * vector, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: direccion del vector de entrada
@   r1: longitud del vector
@
@ Registros usados temporalmente:
@ 	r2: almacena el valor provisoriamente a intercambiar
@ 	r3: almacena el valor provisoriamente a intercambiar
@	r4: almacena el iterador que avanza desde el inicio del vector
@	r5: almacena el index que indica la finalizacion de la funcion
@
.thumb_func
	asm_invertir:
		push {r4-r5}
		lsr r5, r1, #1 // r5 = r1/2
		mov r4, #0
	asm_invertir_loop:
		sub r1, #1
		ldrh r2, [r0, r1, LSL #1] // Cargo el valor a intercambiar r2 = *(r0 + (r1 << 1))
		ldrh r3, [r0, r4, LSL #1] // Cargo el valor a intercambiar r3 = *(r0 + (r4 << 1))
		strh r3, [r0, r1, LSL #1] // Guardo el valor a intercambiar *(r0 + (r1 << 1)) = r3
		strh r2, [r0, r4, LSL #1] // Guardo el valor a intercambiar *(r0 + (r4 << 1)) = r2
		add r4, #1
		cmp r1, r5
		bne asm_invertir_loop
		pop {r4-r5}
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_eco(int16_t * vector, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: direccion del vector de entrada
@   r1: longitud del vector
@
@ Registros usados temporalmente:
@ 	r2: almacena el valor de la señal
@ 	r3: almacena el valor a sumar como eco
@	r4: almacena el index del eco
@	r5: almacena la constante de desfasaje del eco (882 en este caso)
@
.thumb_func
	asm_eco:
		push {r4-r5}
		mov r5, #882
	asm_eco_loop:
		sub r1, #1 // Decremento el index
		subs r4, r1, r5 // Calculo el index de la muestra a usar como eco
		ldrh r2, [r0, r1, LSL #1] // Cargo el valor de la señal r2 = *(r0 + (r1 << 1))
		ldrh r3, [r0, r4, LSL #1] // Cargo el valor del eco r3 = *(r0 + (r4 << 1))
		asr	r3, #1 // Divido por dos el eco (shift aritmetico para preservar el signo en complemento a2)
		add r2, r2, r3 // Calculo el nuevo valor de la señal con el eco sumado
		ssat r2, #16, r2 // Hago que la señal sature si se excede del maximo representable por int16
		strh r2, [r0, r1, LSL #1] // Guardo el nuevo valor de la señal con el eco sumado *(r0 + (r1 << 1)) = r2
		bne asm_eco_loop
		pop {r4-r5}
		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_simd_eco(int16_t * vector, uint32_t longitud)
@
@ NOTA: Idem funcion anterior pero haciendo uso de instrucciones SIMD
@
@ Parametros recibidos:
@   r0: direccion del vector de entrada
@   r1: longitud del vector
@
@ Registros usados temporalmente:
@ 	r2: almacena el valor de la señal
@ 	r3: almacena el valor a sumar como eco
@	r4: almacena el index del eco
@	r5: almacena la constante de desfasaje del eco (882 en este caso)
@   r6: valor 0 provisorio para lograr la division en SIMD
@
.thumb_func
	asm_simd_eco:
		push {r4-r6}
		lsr r1, #1 // Divido la longitud en 2 (el array siempre tiene que ser un numero par de muestras para poder usar esta funcion)
		mov r5, #441 // 882 dividido 2
		mov r6, #0
	asm_simd_eco_loop:
		sub r1, #1 // Decremento el index
		subs r4, r1, r5 // Calculo el index de la muestra a usar como eco
		ldr r2, [r0, r1, LSL #2] // Cargo DOS valores de la señal r2 = *(r0 + (r1 << 2))
		ldr r3, [r0, r4, LSL #2] // Cargo DOS valores de eco r3 = *(r0 + (r4 << 2))
		shadd16 r3, r3, r6 // r3[0]=(r3[0]+r6[0])/2 y r3[1]=(r3[1]+r6[1])/2 pero como r6=0 => r3[0]=r3[0]/2 y r3[1]=r3[1]/2 - Es decir, divido por dos las dos muestras de eco.
		qadd16 r2, r2, r3 // Sumo de a 16 bits y saturo el resultado signado.
		str r2, [r0, r1, LSL #2] // Guardo DOS nuevos valores de la señal con el eco sumado *(r0 + (r1 << 2)) = r2
		bne asm_simd_eco_loop
		pop {r4-r6}
		bx lr

