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
    	mov r2, #0	// Ponemos un cero en el registro 2
    asm_zeros_loop:
    	sub r1, #1 // Descremento la longitud
		str r2, [r0, r1, LSL #2] // Guardamos el cero del registro 2 en la posicion apuntada por el registro 0 (x 4)
		cmp r1, #0 // Comparo con cero la longitud
		bne asm_zeros_loop
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
    	sub r2, #1
    	ldr r4, [r0, r2, LSL #2] // Cargo el valor a multiplicar (r4 = *(r0 + (r2 << 2)))
    	mul r4, r4, r3 // r4 = r4 * r3
		str r4, [r1, r2, LSL #2] // Guardamos el valor multiplicado *(r1 + (r2 << 2)) = r4
		cmp r2, #0 // Comparo con cero la longitud
		bne asm_productoEscalar32
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
    	sub r2, #1
    	ldrh r4, [r0, r2, LSL #1] // Cargo el valor a multiplicar (r4 = *(r0 + (r2 << 1)))
    	mul r4, r4, r3 // r4 = r4 * r3
		strh r4, [r1, r2, LSL #1] // Guardamos el valor multiplicado *(r1 + (r2 << 1)) = r4
		cmp r2, #0 // Comparo con cero la longitud
		bne asm_productoEscalar16
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
    	sub r2, #1
    	ldrh r4, [r0, r2, LSL 1] // Cargo el valor a multiplicar (r4 = *(r0 + (r2 << 1)))
    	mul r4, r4, r3 // r4 = r4 * r3
    	usat r4, #12, r4 // Saturo el registro r4 a 12 bits y lo guardo en el registro r4
		strh r4, [r1, r2, LSL 1] // Guardamos el valor multiplicado *(r1 + (r2 << 1)) = r4
		cmp r2, #0 // Comparo con cero la longitud
		bne asm_productoEscalar12
		bx lr
