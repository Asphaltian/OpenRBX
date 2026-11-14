# source/demos/Makefile
#
# Make all demos or an individual demo
#
# Example: make
# Example: make VAR_Demo
#
# The compiled executable will be copied to source/demos

IC = ../../bin/icompile

DEMOS = ASM_Shader_Demo Cg_Shader_Demo Collision_Demo GLSL_Demo MD2Model_Demo Network_Demo VAR_Demo

all: $(DEMOS)

.PHONY: $(DEMOS)
$(DEMOS): 
	@echo "Building $@"; \
	cd $@; \
        chmod u+x $(IC); \
	$(IC) --opt; \
	cp -f 'build/install/$@' '../$@_exe'; \
	cd ../;

.PHONY: clean
clean:
	for dir in $(DEMOS); do \
	cd $$dir; \
	chmod u+x $(IC); \
	$(IC) --clean; \
	cd ../; \
	done;

