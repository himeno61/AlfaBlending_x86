section .text	;obszar programu w ktorym mamy funkcje
global transfer	;sprawiamy, ze f jest globalne



; rsi - file 2
; rdi - file 1
; rax
; rbx
; ecx szerokosc
; edx wysokosc

; r8d k 
; r9  file 3
; r10 ilość bajtów na wiersz
; r11 - licznik kolumnowy
; r12 - wskaznik na docelowy obrazek(ostatni wiersz)
; r13 
; r14
; r15
; [rbp + 16] - x wej
; [rbp + 24] - y wej

; xmm0 pkt względem którego obliczamy odległość x 
; xmm1 pkt względem którego obliczamy odległość y
; xmm15 - ustalona 1.0
; xmm2 (x)
; xmm3 (y) - wartosc aktualnych wspolrzenych
; xmm14 == 2.0
; r13,r14,15 będą aktualizowane po skończeniu wiersza 
 
transfer:
_transfer:
	push 	rbp
	mov		rbp, rsp
	
	
	sub		rsp, 8
	
	mov		r10,rcx	
	imul 	r10,3
	add 	r10,3
	sar		r10,2
	sal		r10,2	; ilośc bajtów w wierszu
	

	finit                                       ;inicjacja koprocesora x87
	

	cvtsi2sd 	xmm0, dword [rbp + 16]
	cvtsi2sd 	xmm1, dword [rbp + 24]
	
	
	mov			rax, 1
	cvtsi2sd	xmm15, rax
	
	inc			rax
	cvtsi2sd	xmm14, rax
	
	cvtsi2sd	xmm13, r8d	; xmm13 := (duouble)k
	
	pxor		xmm3, xmm3						;y = 0
	
	
wierszowo:
	;ustawienie nowego wiersza
	pxor		xmm2, xmm2						;x = 0
	
	mov        r13, rdi	; r13 - obrazek 1
	mov        r14, rsi	; r14 - obrazek 2
	mov        r15, r9		; r15 - obrazek 3
		
	
	mov			r11d, ecx
kolumnowo:
			
	
	movsd 		xmm4,xmm2
	subsd 		xmm4,xmm0	; pod xmm4 mamy różnicę X
	
	movsd 		xmm5,xmm3
	subsd 		xmm5,xmm1	; pod xmm4 mamy różnicę Y
	
	mulsd 		xmm4,xmm4	;podnosimy do kwadratu
	mulsd 		xmm5,xmm5
	
	addsd 		xmm4,xmm5 	; pod xmm4 (dx)^2+(dy)^2	
	
	sqrtsd		xmm4, xmm4	; pod xmm4 - odleglosc
	
	;mulsd		xmm4, xmm13 ; xmm4 == odl * k
	divsd       xmm4, xmm13
	
	movq		qword [rbp - 8], xmm4
	fld			qword [rbp - 8]
	fsin
	fstp		qword [rbp - 8]
	movq		xmm4, qword [rbp - 8]
	
	

	
	
	
	
	
	addsd 		xmm4,xmm15
	divsd		xmm4,xmm14	; pod xmm4 mamy alfe
	
	mov			rbx, 3
suma:
	xor 		rax, rax
	mov			al, byte[r13]	
	
	
	cvtsi2sd	xmm5, rax	; xmm5  - kanal z pierwszego 
	
	
	mov			al, byte[r14]
	cvtsi2sd	xmm6, rax 	; xmm6 - kanał z drugiego
	
	mulsd		xmm5, xmm4 
	
	movsd		xmm7, xmm15
	subsd		xmm7, xmm4
	mulsd		xmm6, xmm7
	
	addsd		xmm5, xmm6 ; pod xmm5 mamy p1*alfa + p2*(1-alfa)	
	
	
	cvtsd2si	rax, xmm5 ; rzutowanie do rax :=(int)xmm5 	
	
	
	mov			byte[r15], al	

	
	inc			r13
	inc			r14
	inc			r15
	dec			rbx
	jnz			suma
	
	addsd		xmm2, xmm15					;x_aktualne += 1
	
	dec			r11d
	jnz			kolumnowo
	
	
	add			rdi, r10
	add			rsi, r10
	add			r9, r10
	
	addsd		xmm3, xmm15
	
	
	dec			edx
	jnz			wierszowo
	
	
	
end:
	mov     rsp, rbp
	pop     rbp
	ret
