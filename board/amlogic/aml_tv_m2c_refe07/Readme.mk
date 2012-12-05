aml_tv_m2c_refe07_config: unconfig
	$(MKCONFIG) $(@:_config=) arm aml_meson aml_tv_m2c_refe07 amlogic m2
	
aml_tv_m2c_refe07_config_help:
	@echo =======================================================================
	@echo The mark in board is "BOE_7366m3_DDR3_4PC_SI2176 20120210"
	@echo config command: \"make $(@:%_help=%)\"
	
help:aml_tv_m2c_refe07_config_help
