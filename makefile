all:
	@echo Building...
	@gcc txtnorm.c -o txtnorm -std=c99 -Os -s
    
test: all
	@echo Testing...
	@txtnorm.exe < README.md > test_out.md
	@cmp test_out.md test_expected.md