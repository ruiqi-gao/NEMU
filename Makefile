-include nemu/Makefile.git

defalut:
	@echo "Please run 'make' under any subprojects to compile."
 
clean:
	-$(MAKE) -C nemu clean
	-$(MAKE) -C nexus-am clean
	-$(MAKE) -C nanos-lite clean
	-$(MAKE) -C navy-apps clean

submit: clean
	git gc
	cd .. && tar cj $(shell basename `pwd`) > $(STU_ID).tar.bz2
count:
	find /home/1210077937/ics2017/nemu/ -name *.c -or -name *.h | xargs  cat | wc -l
.PHONY: default clean submit
