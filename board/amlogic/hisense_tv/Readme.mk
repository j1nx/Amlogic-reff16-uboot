hisense_tv_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson hisense_tv amlogic m2
	
hisense_tv_config_help:
	@echo =======================================================================
	@echo The mark in board is "HISENSE_M2C_V1.0_DDR3_4PC_SI2176 20111101"
	@echo config command: \"make $(@:%_help=%)\"
	
help:hisense_tv_config_help
