all: ImageMaker

ImageMaker:
	@echo
	@echo ============ Build ImageMaker ============
	@echo

	make -C 00.ImageMaker
	mv 00.ImageMaker/ImageMaker.bin ../

	@echo
	@echo ============ Build Complete ============
	@echo

clean:
	make -C 00.ImageMaker clean
