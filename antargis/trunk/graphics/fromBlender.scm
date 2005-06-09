; copy this to your ~/gimp-2.2/scripts
(define (fromBlender filename output)
	(let* ((image (car (gimp-file-load RUN-NONINTERACTIVE filename filename)))
          (drawable (car (gimp-image-get-active-layer image))))
						; select background
						(gimp-by-color-select drawable (car (list (list 182 0 182))) 40 2 1 1 1 0)
						; clear background
						(gimp-edit-clear drawable)
						
						; select shadow
						(gimp-by-color-select drawable (car (list (list 115 0 115))) 3 2 1 1 1 0)
						; clear shadow
						(gimp-edit-clear drawable)
						; reset colors fg=black
						(gimp-context-set-default-colors)
						; fill shadow with (0,0,0,0x7f)
						(gimp-bucket-fill drawable 0 0 50 0 0 0 0)
						(gimp-selection-none image)
						;(gimp-drawable-transform-scale drawable 0 0 96 96 0 2 3 3 1)
						(gimp-image-scale image 96 96)
          	(gimp-file-save RUN-NONINTERACTIVE image drawable output output)))
(fromBlender "input.png" "output_neu.png")
