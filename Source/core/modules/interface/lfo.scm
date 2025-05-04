(load "descriptor.scm")

(define namespace-lfo
  (list
    (cons 'ctl '(octave delta amp form options))
    (cons 'cvi '(fm am))
    (cons 'cvo '(a b))

    (cons 'cc (length (cdr (assoc 'ctl namespace-lfo))))  ; N Controls
    (cons 'ic (length (cdr (assoc 'cvi namespace-lfo))))  ; N CVinputs
    (cons 'oc (length (cdr (assoc 'cvo namespace-lfo))))  ; N CVoutputs

    (cons 'prefix "lfo")

    (cons 'set-i
        (list
            (list 'socket       '(12.00   66.00  16.00   16.00)  "fm"       0.000 1.0 0.00 0.5 0.000 0x00 #f 0x00000000)
            (list 'socket       '(50.00   66.00  16.00   16.00)  "am"       0.000 1.0 0.00 0.5 0.000 0x00 #f 0x00000000)
        )
    )

    (cons 'set-o
        (lis'
            'list 'socket       '(30.00    6.00  16.00   16.00)  "a"        0.000 1.0 0.00 0.5 0.000 0x00 #f 0x00000000)
            (list 'socket       '(30.00   36.00  16.00   16.00)  "b"        0.000 1.0 0.00 0.5 0.000 0x00 #f 0x00000000)
        )
    )

    (cons 'set-c
        (list
            (list 'parameter    '(0.00     0.00   0.00    0.00)  "octave"   0.000 1.0 0.00 0.5 0.000 0x00 #f 0x00000000)
            (list 'slider       '(22.00  106.00  32.00   32.00)  "delta"    0.001 1.0 0.01 0.2 0.001 0x00 #f 0x0000000B)
            (list 'slider       '(22.00  166.00  32.00   32.00)  "amp"      0.000 1.0 0.00 0.2 0.001 0x00 #f 0x0000000B)
            (list 'parameter    '(0.00     0.00   0.00    0.00)  "form"     0.000 2.0 0.00 0.5 0.000 0x00 #f 0x00000000)
            (list 'radio        '(60.00  228.00  12.00   12.00)  "options"  0.000 1.0 0.00 0.5 0.000 #xFF #f 0x00000000)
        )
    )

    ;; Rectangle constraint
    (cons 'constrain '(0.0 0.0 76.0 244.0))

    ;; Descriptor
    (cons 'descriptor
            (list
            'module-lfo
            (length (cdr (assoc 'cvi namespace-lfo)))  ; IC count from cvi list
            (length (cdr (assoc 'cvo namespace-lfo)))  ; OC count from cvo list
            (length (cdr (assoc 'ctl namespace-lfo)))  ; CC count from ctl list
            "lfo"                                      ; Prefix
            (list
                (list 'socket       '(12.00  66.00 16.00 16.00) "fm"       0.0 1.0 0.0 0.5 0.000     0 #f 0x00000000)
                (list 'socket       '(50.00  66.00 16.00 16.00) "am"       0.0 1.0 0.0 0.5 0.000     0 #f 0x00000000)) ; Inputs
            (list
                (list 'socket       '(30.00   6.00 16.00 16.00) "a"        0.0 1.0 0.00 0.5 0.000    0 #f 0x00000000)
                (list 'socket       '(30.00  36.00 16.00 16.00) "b"        0.0 1.0 0.00 0.5 0.000    0 #f 0x00000000)) ; Outputs
            (list
                (list 'parameter    '( 0.00   0.00  0.00  0.00) "octave"   0.0 1.0 0.00 0.5 0.000    0 #f 0x00000000)
                (list 'slider       '(22.00 106.00 32.00 32.00) "delta"  0.001 1.0 0.01 0.2 0.001    0 #f 0x0000000B)
                (list 'slider       '(22.00 166.00 32.00 32.00) "amp"      0.0 1.0 0.00 0.2 0.001    0 #f 0x0000000B)
                (list 'parameter    '( 0.00   0.00  0.00  0.00) "form"     0.0 2.0 0.00 0.5 0.000    0 #f 0x00000000)
                (list 'radio        '(60.00 228.00 12.00 12.00) "options"  0.0 1.0 0.00 0.5 0.000 #xFF #f 0x00000000)) ; Controls
           '(0.0 0.0 76.0 244.0)))))                  ; Rectangle constraint
