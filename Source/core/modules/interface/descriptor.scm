(define parameter-off 0x00)
(define parameter-cc  0xCC)
(define parameter-ic  0xAA)
(define parameter-oc  0xEE)
(define parameter-count  4)

(define module-fuse 0x00)
(define module-vco  0xFF)
(define module-lfo  0xEE)
(define module-cso  0xDD)
(define module-vca  0xCC)
(define module-vcd  0xBB)
(define module-vcf  0xAA)
(define module-snh  0x99)
(define module-sum  0x88)
(define module-pdt  0x77)
(define module-rtr  0x66)
(define module-mix  0x55)
(define module-env  0x44)
(define module-mod  0x33)
(define module-com  0x22)
(define module-cro  0x11)
(define module-count  16)

;; Rectangle
(define (rectangle x y w h)
  (list x y w h)) ; Positional data: x, y, width, height

(define (rectangle-x rect) (list-ref rect 0))
(define (rectangle-y rect) (list-ref rect 1))
(define (rectangle-w rect) (list-ref rect 2))
(define (rectangle-h rect) (list-ref rect 3))

;; Control
(define (make-control type constrain postfix min max def skew step radio-id symmetric flag)
  (list type constrain postfix min max def skew step radio-id symmetric flag)) ;; Types

(define (control-type control)      (list-ref control 0))
(define (control-constrain control) (list-ref control 1))
(define (control-postfix control)   (list-ref control 2))
(define (control-min control)       (list-ref control 3))
(define (control-max control)       (list-ref control 4))
(define (control-default control)   (list-ref control 5))
(define (control-skew control)      (list-ref control 6))
(define (control-step control)      (list-ref control 7))
(define (control-radio-id control)  (list-ref control 8))
(define (control-symmetric control) (list-ref control 9))
(define (control-flag control)      (list-ref control 10))

;; Descriptor
(define (make-descriptor type ic oc cc prefix set-i set-o set-c constrain)
  (list type ic oc cc prefix set-i set-o set-c constrain)) ;; Types

(define (descriptor-type descriptor)        (list-ref descriptor 0))
(define (descriptor-ic descriptor)          (list-ref descriptor 1))
(define (descriptor-oc descriptor)          (list-ref descriptor 2))
(define (descriptor-cc descriptor)          (list-ref descriptor 3))
(define (descriptor-prefix descriptor)      (list-ref descriptor 4))
(define (descriptor-set-i descriptor)       (list-ref descriptor 5))
(define (descriptor-set-o descriptor)       (list-ref descriptor 6))
(define (descriptor-set-c descriptor)       (list-ref descriptor 7))
(define (descriptor-constrain descriptor)   (list-ref descriptor 8))

;; Example usage
; (define my-rectangle (make-rectangle 0.0 0.0 100.0 50.0)) ;; Define a rectangle
; (define my-control 
;   (make-control 'slider my-rectangle "volume" 0.0 1.0 0.5 0.1 0.01 1 #f 0)) ;; Define a control

; (define my-descriptor 
;   (make-descriptor 'module-fuse 
;                    '(1 2 3)  ;; Inputs
;                    '(4 5 6)  ;; Outputs
;                    '(7 8 9)  ;; Controls
;                    "ModulePrefix" 
;                    '(control1 control2 control3) ;; Set inputs
;                    '(control4 control5 control6) ;; Set outputs
;                    '(control7 control8 control9) ;; Set controls
;                    my-rectangle))                 ;; Rectangle constraint

; ;; Access and display some data
; (display "Descriptor Type: ") 
; (display (descriptor-type my-descriptor)) 
; (newline)

; (display "Rectangle Width: ") 
; (display (rectangle-w (descriptor-constrain my-descriptor))) 
; (newline)

; (display "Rectangle Height: ") 
; (display (rectangle-h (descriptor-constrain my-descriptor))) 
; (newline)
